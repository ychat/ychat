#include "stdafx.h"
#include "client_t.h"
#include "db_req_t.h"
namespace ychat
{

	client_t::client_t ()
	{

	}

	client_t::~client_t ()
	{
		uninit_mongo ();
	}

	void client_t::init_mongo ()
	{
		mongo_client_ = mongoc_client_new (mongodb_addr_.c_str ());
		if (mongo_client_ == NULL)
		{
			logger_error ("mongoc_client_new error,addr:%s",
						  mongodb_addr_.c_str ());

		}

		user_clt_ = mongoc_client_get_collection (mongo_client_,
												  db_name_.c_str(),
												  user_collection_name_.c_str());
		if (user_clt_ == NULL)
		{
			logger_error ("mongoc_client_get_collection error,"
						  "db_name:%s, collection:%s",
						  db_name_.c_str(),
						  user_collection_name_.c_str());
		}
		chat_log_clt_ = mongoc_client_get_collection (mongo_client_,
													  db_name_.c_str(),
													  chat_log_collection_name_.c_str());
		if (chat_log_clt_ == NULL)
		{
			logger_error ("mongoc_client_get_collection error",
						  db_name_.c_str(),
						  chat_log_collection_name_.c_str());
		}
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
								   "user_id_", BCON_UTF8 (req.friend_id_.c_str ()),
								   "laber_", BCON_UTF8 (req.laber_.c_str ()),
								   "}",
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
		
		return new db::add_friend_resp_t (req.req_id_, ret);
	}

	db::resp_t * client_t::handle_update_add_friend_result_req (db::req_t * _req)
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
				logger_warn ("mongoc_collection_update error,update json:%s",
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

		return new db::del_friend_resp_t(req->req_id_,ret);
	}

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

	db::resp_t * client_t::handle_update_join_group_result_req (db::req_t * _req)
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

		bson_t *selector = BCON_NEW ("group_id_", BCON_UTF8 (req.group_id_.c_str ()));
		bson_t *update = BCON_NEW ("$push",
								   "{",
								   "members_",
								   "{",
								   "user_id_", BCON_UTF8 (req.user_id_.c_str ()),
								   "label_", BCON_UTF8 (req.laber_.c_str ()),
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
				logger_warn ("mongoc_collection_update error%s,update json:%s",
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

}