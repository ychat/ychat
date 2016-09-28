#include "stdafx.h"
#include "client_t.h"
#include "db_req_t.h"
#include "client_auth_t.h"
namespace ychat
{


	client_t::client_t(client_auth_t &client_auth, acl::socket_stream* stream)
		:client_auth_(client_auth),
		is_auth_(false),
		stream_(stream)
	{

	}

	client_t::~client_t()
	{
		uninit_mongo ();
	}

	void client_t::run()
	{
		if(init_mongo() == false)
		{
			logger_warn("init_mongo error");
			return;
		}

		do 
		{
			int len;
			if(stream_->read(len) != sizeof(int))
			{
				logger_warn("read len error");
				break;
			}
			char protocol_name[6];//ychat
			if (stream_->read(protocol_name,5) == false)
			{
				logger_warn("read protocol_name error");
				break;
			}
			if (strcmp(protocol_name,"ychat") != 0)
			{
				logger_warn("protocol_name %s error",protocol_name);
				break;
			}
			acl::string buffer;
			len = ntohl(len);
			if (stream_->read(buffer,len,true) != len)
			{
				logger_warn("read msg data error,len%d", len);
				break;
			}
			acl::string result;
			if (handle_req(buffer,result) == false)
			{
				logger_warn("handle_req error");
				break;
			}
			if(stream_->write(result) != result.size())
			{
				logger_warn("write resp error,len:%s",result.size());
				break;
			}
		} while (1);
	}

	bool client_t::handle_req(const acl::string &data, acl::string &result)
	{
		db::resp_t *resp;
		db::req_t *req = db::to_req(data);
		if(req == NULL) 
		{
			logger_warn("to_req error,%s", data.c_str());
			return false;
		}
		if(is_auth_ == false) 
		{
			if(req->type_ == db::e_ping) 
			{
				resp = handle_ping_req(req);

			}else if (req->type_ == db::e_auth_req)
			{
				resp = handle_auth_req(req);
				if(resp->status_ == false)
				{
					logger_warn("auth failed");
					delete req;
					delete resp;
					return false;
				}
			}
			else
			{
				logger_warn("client not auth close");
				return false;
			}
		}
		switch(req->type_)
		{
		case db::e_req_join_group_req:
			resp = handle_req_join_group_req(req);
			break;
		case db::type_t::e_req_add_friend_req:
			resp = handle_req_add_friend_req(req);
			break;
		case db::type_t::e_add_friend_req:
			resp = handle_add_req(req);
			break;
		case db::type_t::e_update_add_friend_result_req:
			resp = handle_update_add_friend_result_req(req);
			break;
		case db::type_t::e_del_friend_req:
			resp = handle_del_friend_req(req);
			break;
		case db::type_t::e_update_friend_laber_req:
			resp = handle_update_friend_laber_req(req);
			break;
		case db::type_t::e_update_join_group_result_req:
			resp = handle_update_join_group_result_req(req);
			break;
		case db::e_group_add_user_req:
			resp = handle_group_add_user_req(req);
			break;
		case db::e_group_del_user_req:
			resp = handle_group_del_user_req(req);
			break;
		default:
			break;
		}
		result = resp->to_json();
		delete resp;
		delete req;
		return true;
	}

	bool client_t::init_mongo()
	{
		mongo_client_ = mongoc_client_new (mongodb_addr_.c_str ());
		if (mongo_client_ == NULL)
		{
			logger_warn("mongoc_client_new error,addr:%s",
						  mongodb_addr_.c_str ());
			return false;
		}

		user_clt_ = mongoc_client_get_collection (
			mongo_client_,
			db_name_.c_str(),
			user_collection_name_.c_str());

		if (user_clt_ == NULL)
		{
			logger_warn("mongoc_client_get_collection error,"
						  "db_name:%s, collection:%s",
						  db_name_.c_str(),
						  user_collection_name_.c_str());
			return false;
		}
		chat_log_clt_ = mongoc_client_get_collection (
			mongo_client_,
			db_name_.c_str(),
			chat_log_collection_name_.c_str());

		if (chat_log_clt_ == NULL)
		{
			logger_warn("mongoc_client_get_collection error",
						  db_name_.c_str(),
						  chat_log_collection_name_.c_str());
			return false;
		}
		return true;
	}

	void client_t::uninit_mongo ()
	{
		if (user_clt_)
		{
			mongoc_collection_destroy (user_clt_);
		}
		if (chat_log_clt_)
		{
			mongoc_collection_destroy (chat_log_clt_);
		}
		if (group_clt_)
		{
			mongoc_collection_destroy (group_clt_);
		}
	}

	db::resp_t * client_t::handle_add_req (db::req_t * _req)
	{
		db::add_friend_req_t &req = *(db::add_friend_req_t*)_req;

		bool ret = false;
		bson_error_t error;
		bson_t *selector = BCON_NEW ("user_id_", 
									 BCON_UTF8 (req.user_id_.c_str ()));

		bson_t *update = BCON_NEW ("$addToSet", "{", "friends_",
								   "{",
								   "user_id_", 
								   BCON_UTF8 (req.friend_id_.c_str ()),
								   "laber_", 
								   BCON_UTF8 (req.laber_.c_str ()),
								   "}",
								   "}");

		if (!mongoc_collection_update (user_clt_, MONGOC_UPDATE_NONE,
			selector, update, NULL, &error))
		{
			char *update_json = bson_as_json (update, NULL);
			if (update_json)
			{
				logger_warn ("mongoc_collection_update "
							 "error:%s,update:%s",
							 error.message,
							 update_json);
				bson_free (update_json);
			}
		}
		else
			ret = true;
		if (selector)
			bson_destroy (selector);
		if (update)
			bson_destroy (update);
		
		return new db::add_friend_resp_t (req.req_id_, ret);
	}
	/*
	shell:
	db.user_.update( {"user_id_":"123","add_friend_reqs.msg_id_":"2"} ,
	{ $set:{"add_friend_reqs.$.result_":"yes"}});
	*/
	db::resp_t * client_t::handle_update_add_friend_result_req (
		db::req_t * _req)
	{
		db::update_add_friend_result_req_t *req = 
			(db::update_add_friend_result_req_t *)_req;

		bool ret = false;
		bson_error_t error;
		bson_t *selector = BCON_NEW ("user_id_",
									 BCON_UTF8 (req->user_id_.c_str ()),
									 "add_friend_reqs.req_id_",
									 BCON_INT64 (req->add_friend_req_id_));

		bson_t *update = BCON_NEW ("$set", "{",
								     "add_friend_reqs.$.result_", 
								      BCON_UTF8(req->result_?"yes":"no"), 
								   "}");

		if (!mongoc_collection_update (user_clt_, MONGOC_UPDATE_NONE,
			selector, update, NULL, &error))
		{
			char *update_json = bson_as_json (update, NULL);
			if (update_json)
			{
				logger_warn ("mongoc_collection_update "
							 "error,update json:%s",
							 update_json);
				bson_free (update_json);
			}
		}
		else
			ret = true;

		bson_destroy (selector);
		bson_destroy (update);

		return new db::set_add_friend_result_resp_t (req->req_id_, ret);
	}
	/*
	mongo shell
	db.user_.update(
	{"user_id_":"123"} ,
	{ $pull:{"friends_":{"user_id_":"192282783"}}});

	*/
	db::resp_t * client_t::handle_del_friend_req (db::req_t * _req)
	{
		db::del_friend_req_t *req = (db::del_friend_req_t*)_req;
		bool ret = false;
		bson_error_t error;

		bson_t *selector = BCON_NEW ("user_id_",
									 BCON_UTF8 (req->user_id_.c_str ()));

		bson_t *update = BCON_NEW ("$pull",
								   "{",
								   "friends_",
								   "{",
								   "user_id_",
								   BCON_UTF8 (req->friend_id_.c_str ()),
								   "}",
								   "}");

		if (!mongoc_collection_update (user_clt_, MONGOC_UPDATE_NONE,
			selector, update, NULL, &error))
		{
			char *update_json = bson_as_json (update, NULL);
			if (update_json)
			{
				logger_warn ("mongoc_collection_update "
							 "error:%s,update:%s",
							 error.message,
							 update_json);
				bson_free (update_json);
			}
		}
		else
			ret = true;
		if (selector)
			bson_destroy (selector);
		if (update)
			bson_destroy (update);

		return new db::del_friend_resp_t(req->req_id_,ret);
	}
	/*
	mongo shell
	db.user_.update( {"user_id_":"123","friends_.user_id_":"192282783"} ,
	{ $set:{"friends_.$.laber_":"world"}});

	*/
	db::resp_t * client_t::handle_update_friend_laber_req (db::req_t * _req)
	{
		db::update_friend_laber_req_t &req = 
			*(db::update_friend_laber_req_t*)_req;

		bool ret = false;
		bson_error_t error;

		bson_t *selector = BCON_NEW ("user_id_",
									 BCON_UTF8 (req.user_id_.c_str ()),
									 "friends_.user_id_",
									 BCON_UTF8 (req.friend_id_.c_str ()));
		bson_t *update = BCON_NEW ("$set", "{",
								   "friends_.$.laber_",
								   BCON_UTF8 (req.label_.c_str ()),
								   "}");

		if (!mongoc_collection_update (user_clt_, MONGOC_UPDATE_NONE,
			selector, update, NULL, &error))
		{
			char *update_json = bson_as_json (update, NULL);
			if (update_json)
			{
				logger_warn ("mongoc_collection_update error:%s,update:%s",
							 error.message,
							 update_json);
				bson_free (update_json);
			}
		}
		else
			ret = true;
		if (selector)
			bson_destroy (selector);
		if (update)
			bson_destroy (update);

		return new db::update_friend_laber_resp_t(req.req_id_,ret);
	}

	db::resp_t * client_t::handle_update_join_group_result_req (
		db::req_t * _req)
	{

		db::update_join_group_result_req_t &req = 
			*(db::update_join_group_result_req_t*)_req;

		const char *result = req.result_?"yes":"no";

		bson_t *selector = BCON_NEW ("user_id_",
									 BCON_UTF8 (req.user_id_.c_str ()),
									 "join_group_reqs_.req_id_",
									 BCON_INT64(req.join_group_req_id_));

		bson_t *update = BCON_NEW ("$set", "{",
								   "join_group_reqs_.$.result_",
								   BCON_UTF8 (result),
								   "}");
		bson_error_t error;
		bool ret = false;
		if (!mongoc_collection_update (user_clt_, MONGOC_UPDATE_NONE,
			selector, update, NULL, &error))
		{
			char *update_json = bson_as_json (update, NULL);
			if (update_json)
			{
				logger_warn ("mongoc_collection_update error:%s,update:%s",
							 error.message,
							 update_json);
				bson_free (update_json);
			}
		}
		else
			ret = true;
		if (selector)
			bson_destroy (selector);
		if (update)
			bson_destroy (update);

		return new db::set_add_friend_result_resp_t(req.req_id_,ret);
	}

	db::resp_t * client_t::handle_group_add_user_req (db::req_t * _req)
	{
		db::group_add_user_req_t &req = *(db::group_add_user_req_t*)_req;

		bool ret = false;
		bson_error_t error;

		bson_t *selector = BCON_NEW ("group_id_",
									 BCON_UTF8 (req.group_id_.c_str ()));
		bson_t *update = BCON_NEW ("$push",
								   "{",
								   "members_",
								   "{",
								   "user_id_", 
								   BCON_UTF8 (req.user_id_.c_str ()),
								   "label_", 
								   BCON_UTF8 (req.laber_.c_str ()),
								   "}",
								   "}");

		if (!mongoc_collection_update (group_clt_, MONGOC_UPDATE_NONE,
			selector, update, NULL, &error))
		{
			char *update_json = bson_as_json (update, NULL);
			if (update_json)
			{
				logger_warn ("mongoc_collection_update error:%s,update:%s",
							 error.message,
							 update_json);
				bson_free (update_json);
			}
		}
		else
			ret = true;
		if (selector)
			bson_destroy (selector);
		if (update)
			bson_destroy (update);
		return new db::group_add_user_resp_t (req.req_id_, ret);
	}

	db::resp_t * client_t::handle_group_del_user_req (db::req_t * _req)
	{
		db::group_del_user_req_t &req = *(db::group_del_user_req_t*)_req;
		bool ret = false;
		bson_error_t error;

		bson_t *selector = BCON_NEW ("group_id_", 
									 BCON_UTF8 (req.group_id_.c_str ()));

		bson_t *update = BCON_NEW ("$pull",
								   "{",
								   "members_",
								   "{",
								   "user_id_",
								   BCON_UTF8 (req.user_id_.c_str ()),
								   "}",
								   "}");

		if (!mongoc_collection_update (group_clt_, MONGOC_UPDATE_NONE,
			selector, update, NULL, &error))
		{
			char *update_json = bson_as_json (update, NULL);
			if (update_json)
			{
				logger_warn ("mongoc_collection_update error:%s,update:%s",
							 error.message,
							 update_json);
				bson_free (update_json);
			}
		}
		else
			ret = true;
		if (selector)
			bson_destroy (selector);
		if (update)
			bson_destroy (update);

		return new db::group_del_user_resp_t(req.req_id_, ret);
	}

	db::resp_t * client_t::handle_req_add_friend_req (db::req_t * _req)
	{
		db::req_add_friend_req_t &req = *(db::req_add_friend_req_t*)_req;

		bool result = false;
		bson_t *selector = BCON_NEW ("user_id_", 
									 BCON_UTF8 (req.user_id_.c_str ()));
		bson_t *update =
			BCON_NEW ("$push",
					  "{",
					  "add_friend_reqs",
					  "{",
						  "req_id_", BCON_INT64 (req.req_id_),
						  "from_", BCON_UTF8 (req.from_.c_str()),
						  "to_", BCON_UTF8 (req.to_.c_str()),
						  "time_", BCON_DATE_TIME (time(NULL)),
						  "text_msg_", BCON_UTF8 (req.text_msg_.c_str ()),
						  "result_", "_",// yes, no _
					  "}",
					  "}");

		bson_error_t error;

		if (!mongoc_collection_update (user_clt_, MONGOC_UPDATE_NONE,
			selector, update, NULL, &error))
		{
			char *update_json = bson_as_json (update, NULL);
			if (update_json)
			{
				logger_warn ("mongoc_collection_update"
							 " error%s,update json:%s",
							 error.message,
							 update_json);
				bson_free (update_json);
			}
		}
		else
			result = true;

		bson_destroy (update);
		bson_destroy (selector);
		return new db::req_add_friend_resp_t (req.req_id_, result);
	}

	db::resp_t * client_t::handle_req_join_group_req(db::req_t * _req)
	{
		db::req_join_group_req_t *req = (db::req_join_group_req_t*)_req;
		bool ret = false;

		bson_t *selector = BCON_NEW("user_id_", 
									BCON_UTF8(req->user_id_.c_str()));

		bson_t *update = BCON_NEW("$push",
								  "{",
								  "join_group_reqs_",
								  "{",
								  "req_id_",
								  BCON_INT64(req->req_id_),
								  "group_id_",
								  BCON_UTF8(req->group_id_.c_str()),
								  "from_"
								  , BCON_UTF8(req->from_.c_str()),
								  "to_",
								  BCON_UTF8(req->to_.c_str()),
								  "text_msg_",
								  BCON_UTF8(req->text_msg_.c_str()),
								  "result_",
								  "_",//for init
								  "time_",
								  BCON_DATE_TIME(time(NULL)),
								  "}",
								  "}");
		bson_error_t error;
		if(!mongoc_collection_update(user_clt_, MONGOC_UPDATE_NONE,
		   selector, update, NULL, &error)) {
			char *update_json = bson_as_json(update, NULL);
			if(update_json) {
				logger_warn("mongoc_collection_update error:%s,update:%s",
							error.message,
							update_json);
				bson_free(update_json);
			}
		}
		else
			ret = true;

		if(selector)
			bson_destroy(selector);
		if(update)
			bson_destroy(update);

		return new db::req_join_group_resp_t(req->req_id_, ret);
	}

	db::resp_t * client_t::handle_ping_req(db::req_t * _req)
	{
		db::ping_t *req = (db::ping_t*)_req;
		return new db::pong_t(req->req_id_, true);
	}

	db::resp_t * client_t::handle_auth_req(db::req_t * _req)
	{
		db::auth_req_t *req = (db::auth_req_t*)_req;
		if(client_auth_.auth(req) == false)
		{
			return new db::auth_resp_t(req->req_id_, false);
		}
		return new db::auth_resp_t(req->req_id_, true);
	}

	

}