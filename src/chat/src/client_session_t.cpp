#include "stdafx.h"
#include "client_session_t.h"
#include "auth_t.h"
#include "json_helper_t.h"
#include "msg.hpp"
#include "config.h"

namespace ychat
{

	client_session_t::client_session_t (acl::socket_stream& conn)
		:conn_ (conn),
		is_auth_ (false),
		is_stop_(false),
		msg_queues_(20)
	{

	}

	void client_session_t::set_redis_cluster(acl::redis_client_cluster
											 *redis_cluster)
	{
		redis_cluster_ = redis_cluster;
	}

	void client_session_t::run()
	{
		if (check_auth () == false)
			return ;
		do 
		{
			char magic[6];
			int len = 0;
			acl::string buffer;
			/*
			massage binary format:
			|ychat|int|char[]|
			*/
			if (conn_.read(magic, 5) == false)
			{
				logger_warn ("read magic error");
				break;
			}
			magic[6] = 0;
			if (strcasecmp(magic,"ychat"))
			{
				logger ("magic error,%s",magic);
				break;;
			}
			if(conn_.read(len) == false) 
			{
				logger_warn("read int error");
				break;
			}
			len = ntohl(len);
			if(conn_.read(buffer, len, true) == false) 
			{
				logger_warn("read %d error", len);
				break;
			}
			if(handle_msg(buffer.c_str(),len) == false) {
				logger("handle msg error");
				break;
			}

		} while (is_stop_ == false);
	}

	void client_session_t::init_mongodb ()
	{
		mongo_client_ = mongoc_client_new (mongodb_addr_.c_str ());
		if (mongo_client_ == NULL)
		{
			logger_error ("mongoc_client_new error,addr:%s",
						  mongodb_addr_.c_str());

		}
		mongo_db_ = mongoc_client_get_database (mongo_client_,
												g_config.mongo_db_name_);
		if (mongo_db_ == NULL)
		{
			logger_error ("mongoc_client_get_database error,%s",
						  g_config.mongo_db_name_);
		}

		user_clt_ = mongoc_client_get_collection (mongo_client_, 
									g_config.mongo_db_name_, 
									g_config.mongodb_user_collection_name_);
		if (user_clt_ == NULL)
		{
			logger_error ("mongoc_client_get_collection error,
						  db_name:%s,collection:%s",
						  g_config.mongo_db_name_,
						  g_config.mongodb_user_collection_name_);
		}
		chat_log_clt_ = mongoc_client_get_collection (mongo_client_,
										  g_config.mongo_db_name_,
								  g_config.mongodb_chat_log_collection_name_);
		if (chat_log_clt_ == NULL)
		{
			logger_error ("mongoc_client_get_collection error",
						  g_config.mongo_db_name_,
						  g_config.mongodb_chat_log_collection_name_);
		}
	}

	void client_session_t::uninit_mongodb ()
	{

	}

	bool client_session_t::handle_msg (const char *data, int len)
	{
		msg_t *msg = json_helper_t::to_msg (data);
		if (msg == NULL)
		{
			logger_warn ("to_msg error,data:%s",data);
			return false;
		}
		bool ret = false;
		switch (msg->msg_type_)
		{
			case msg_t::e_chat:
				ret = handle_chat_msg (msg);
				break;
			case msg_t::e_chat_ack:
				ret = handle_chat_ack_msg (msg);
				break;
			case msg_t::e_add_friend:
				ret = handle_add_friend (msg);
				break;
			case msg_t::e_add_friend_result:
				ret = handle_add_friend_result (msg);

			default:
				break;
		}
		delete msg;
		return ret;
	}


	bool client_session_t::check_auth ()
	{
		int msg_len = 0;
		acl::string buffer;
		if (conn_.read (msg_len) == false)
			return false;
		msg_len = ntohl(msg_len);

		if (conn_.read (buffer, msg_len, true) == false)
			return false;
		//first msg must be auth msg
		if (auth_t ().check (buffer) == false)
			return false;
	}

	void client_session_t::stop()
	{
		is_stop_ = true;
	}

	bool client_session_t::handle_chat_msg (msg_t * msg)
	{
		//current time.
		msg->time_ = time (NULL);
		msg->msg_id_ = get_uuid ();
		if (check_friend (msg->from_, msg->to_) == false)
		{
			logger_warn ("%s,%s not friend",
						 msg->from_.c_str(),
						 msg->to_.c_str());
			return false;
		}
		if (to_msg_queue(msg))
		{
			logger ("to_msg_queue error");
			return false;
		}
		return true;
	}

	bool client_session_t::check_friend (const std::string &client_id, 
										 const std::string &friend_id)
	{
		acl::string client_key;
		client_key.format ("%s%d",g_config.friend_map_prefix_, client_id);
		acl::redis cmd(redis_cluster_);
		acl::string result;
		return cmd.hexists(client_key.c_str(), friend_id.c_str());
	}

	bool client_session_t::to_msg_queue (msg_t *msg)
	{
		uint32_t msg_queue_id = get_msg_queue_id (msg->to_);

		acl::string msg_queue;
		msg_queue.format ("%s%u", g_config.msg_queue_prefix_, msg_queue_id);
		acl::redis cmd(redis_cluster_);
		std::string json;
		if (json_helper_t::to_json (msg, json) == false)
		{
			logger_warn ("to_json error");
			return false;
		}

		if (cmd.rpush (msg_queue, json.c_str (), NULL) == false)
		{
			logger ("redis rpush error, msg_queue:%s", msg_queue.c_str ());
			return false;
		}
		return true;
	}

	bool client_session_t::get_username (const std::string& user_id,
										 std::string &username)
	{
		if (get_username_from_redis(user_id,username) == false)
		{
			return get_username_from_mongo (user_id, username);
		}
		return true;
	}

	bool client_session_t::get_username_from_redis (const std::string& user_id,
										 std::string &username)
	{
		acl::redis cmd (redis_cluster_);
		acl::string user_key;
		user_key.format ("%s%s", g_config.user_id_prefix_, user_id.c_str ());
		acl::string result_;
		if (cmd.hget (user_key.c_str, "username", result_) == false)
			return false;
		username = result_.c_str ();
		return true;
	}

	/*
		friend item format:
		{"user_id_": "192282783","laber_": "akzi"}
	*/

	bool client_session_t::add_friend_to_redis (std::string client_id,
												std::string friend_id, 
												const std::string &friend_name)
	{

		acl::json json;
		acl::json_node &root = json.get_root ();
		root.add_child (json.create_node ("user_id_", client_id.c_str ()));
		root.add_child (json.create_node ("laber_", friend_name.c_str ()));
		const acl::string &str = json.to_string ();

		acl::redis cmd(redis_cluster_);
		acl::string key;
		key.format ("%s%s", g_config.friend_map_prefix_, client_id.c_str ());
		if (cmd.hset (key.c_str (), friend_id.c_str (), str.c_str ()) == false)
		{
			logger ("redis hset error,map:%s key:%s value:%s",
					key.c_str(),friend_id.c_str(),str.c_str());
			return false;
		}
		return true;
	}

	bool client_session_t::get_username_from_mongo (const std::string& user_id,
													std::string &username)
	{
		bool res = false;
		bson_t *query = BCON_NEW ("user_id_",BCON_UTF8(username.c_str()));
		bson_t *fields = BCON_NEW("username_",BCON_INT32(1));
		mongoc_cursor_t *cursor = mongoc_collection_find (user_clt_, 
														  MONGOC_QUERY_NONE, 
														  0, 0, 0,
														  query, NULL, NULL);
		const bson_t *doc;
		if(mongoc_cursor_next(cursor,&doc) && doc)
		{
			bson_iter_t iter;
			if (bson_iter_init (&iter, doc) &&
				bson_iter_find (&iter, "username_"))
			{
				uint32_t len;
				const char * value = bson_iter_utf8 (&iter,&len);
				if (value)
				{
					username.append (value, len);
					res = true;
				}
			}
		}
		if(query)
			bson_destroy (query);
		if (fields)
			bson_destroy (fields);
		if(cursor)
			mongoc_cursor_destroy (cursor);

		return true;
	}

	uint32_t client_session_t::get_msg_queue_id (const std::string &client_id)
	{
		uint32_t hash = acl_hash_crc32 (client_id.c_str (), client_id.size ());
		return hash % msg_queues_;
	}

	bool client_session_t::handle_chat_ack_msg (msg_t * msg)
	{
		chat_msg_ack_t *ack = (chat_msg_ack_t*)msg;

		acl::string msg_id;
		bool result = false;
		msg_id.format ("%llu",msg->msg_id_);
		bson_error_t error;
		bson_t *selector = BCON_NEW ("msg_id_",BCON_UTF8(msg_id.c_str()));
		bson_t *update =  BCON_NEW ("$set",
									"{", 
										"ack_time_",
										BCON_DATE_TIME (time (NULL)),
									 "}");

		if (!mongoc_collection_update (chat_log_clt_, MONGOC_UPDATE_NONE,
			selector, update, NULL, &error))
		{
			char *selector_str = bson_as_json (selector,NULL);
			char *update_str = bson_as_json (update, NULL);
			if (selector_str && update_str)
			{
				logger_warn ("mongoc_collection_update error,query:%s,update:%s",
							 selector_str,
							 update_str);
			}
			if(selector_str)
				bson_free (selector_str);
			if(update_str)
				bson_free (update_str);
		}
		else {
			result = true;
		}
		bson_destroy (update);
		bson_destroy (selector);
		return result;
	}

	long long client_session_t::get_uuid ()
	{
		acl::redis cmd (redis_cluster_);
		long long result = 0;
		;
		if (cmd.incr (g_config.msg_uuid_, &result) == false)
		{
			logger_warn ("redis incr error");
			return -1;
		}
		return result;
	}

	bool client_session_t::handle_add_friend (msg_t * msg)
	{
		if (push_add_friend_to_db (msg, msg->from_) == false)
		{
			logger_warn ("");
			return false;
		}
		if (push_add_friend_to_db (msg, msg->to_) == false)
		{
			logger_warn ("");
			return false;
		}
		return to_msg_queue (msg);
	}

	bool client_session_t::push_add_friend_to_db (msg_t * msg, 
												  std::string client_id)
	{
		bool result = false;
		add_friend_t *addf = (add_friend_t *)msg;
		bson_t *selector = BCON_NEW("user_id_",BCON_UTF8(client_id.c_str()));
		bson_t *update = 
				BCON_NEW("$push",
						 "{",
						   "add_friend_reqs",
						    "{",
							 "msg_id_", BCON_INT64(addf->msg_id_),
							 "from_", BCON_UTF8(addf->from_.c_str()),
							 "to_", BCON_UTF8(addf->to_.c_str()),
							 "time_",BCON_DATE_TIME(addf->time_),
							 "text_msg_",BCON_UTF8(addf->text_msg_.c_str()),
							 "result_","_",// yes, no _
						 "}",
					"}");

		bson_error_t error;

		if(!mongoc_collection_update(user_clt_, MONGOC_UPDATE_NONE,
		   selector, update, NULL, &error)) 			   
		{
			char *update_json = bson_as_json(update, NULL);
			if (update_json)
			{
				logger_warn("mongoc_collection_update error%s,update json:%s",
							error.message,
							update_json);
				bson_free(update_json);
			}
		}else
			result = true;

		bson_destroy(update);
		bson_destroy(selector);
		return result;
	}
	/*
	shell:
	db.user_.update( {"user_id_":"123","add_friend_reqs.msg_id_":"2"} ,
					{ $set:{"add_friend_reqs.$.result_":"yes"}});

	*/
	bool client_session_t::update_add_friend_to_db (msg_t * msg, 
													std::string client_id)
	{
		add_friend_result_t *result = (add_friend_result_t*)msg;
		const char *res = result->result_?"yes":"no";
		bool ret = false;
		bson_error_t error;
		bson_t *selector = BCON_NEW ("user_id_",
									 BCON_UTF8 (client_id.c_str ()),
									 "add_friend_reqs.msg_id_",
									 BCON_INT64 (result->dst_msg_id_));

		bson_t *update = BCON_NEW ("$set","{",
								   "add_friend_reqs.$.result_", res,"}");

		if (!mongoc_collection_update (user_clt_, MONGOC_UPDATE_NONE,
			selector, update, NULL, &error))
		{
			char *update_json = bson_as_json (update, NULL);
			if (update_json)
			{
				logger_warn ("mongoc_collection_update error,update json:%s",
							 update_json);
				bson_free (update_json);
			}
		}
		else
			ret = true;
		bson_destroy (selector);
		bson_destroy (update);
		return res;
	}

	bool client_session_t::handle_add_friend_result (msg_t * msg)
	{
		add_friend_result_t *result = (add_friend_result_t*)msg;

		if (update_add_friend_to_db (msg, msg->from_) == false)
		{
			logger_warn ("update_add_friend_to_db error");
			return false;
		}
		if (update_add_friend_to_db (msg, msg->to_) == false)
		{
			logger_warn ("update_add_friend_to_db error");
			return false;
		}
		if (result->result_)
		{
			std::string from_name, to_name;
			if (get_username (msg->from_, from_name) == false)
			{
				logger_warn ("get_username failed");
				return false;
			}
			if (get_username (msg->to_, to_name) == false)
			{
				logger_warn ("get_username failed");
				return false;
			}
			if (add_friend_to_db (msg->from_, msg->to_, to_name) == false)
			{
				logger_warn ("add_friend_to_user error");
				return false;
			}
			if (add_friend_to_db (msg->to_, msg->from_,from_name) == false)
			{
				logger_warn ("add_friend_to_user error");
				return false;
			}

		}
		to_msg_queue (msg);
	}
	bool client_session_t::add_friend_to_db (std::string user_id,
											   std::string friend_id, 
											   const std::string &friend_name)
	{
		bool ret = false;
		bson_error_t error;

		bson_t *selector = BCON_NEW ("user_id_", BCON_UTF8 (user_id.c_str ()));

		bson_t *update = BCON_NEW ("$addToSet", "{", "friends_",
								   "{",
								   "user_id_", BCON_UTF8 (friend_id.c_str ()),
								   "laber_", BCON_UTF8 (friend_name.c_str ()),
								   "}",
								   "}");

		if (!mongoc_collection_update (user_clt_, MONGOC_UPDATE_NONE,
			selector, update, NULL, &error))
		{
			char *update_json = bson_as_json (update, NULL);
			if (update_json)
			{
				logger_warn ("mongoc_collection_update error:%s,update json:%s",
							 error.message,
							 update_json
							 );
				bson_free (update_json);
			}
		}
		else
			ret = true;
		if(selector)
			bson_destroy (selector);
		if(update)
			bson_destroy (update);
		return ret;
	}

}

