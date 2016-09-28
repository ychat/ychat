#pragma once
namespace ychat
{
	class client_t
	{
	public:
		client_t();
		~client_t ();
		void set_socket_stream (acl::socket_stream* stream)
		{
			stream_ = stream;
		}
		void run ()
		{

		}
	private:
		bool handle_req (const acl::string &data, acl::string &result)
		{
			db::resp_t *resp;
			db::req_t *req = db::to_req(data);
			if (req == NULL)
			{
				logger_warn ("to_req error,%s",data.c_str());
				return false;
			}
			switch (req->type_)
			{
				case db::type_t::e_req_add_friend_req:
					resp = handle_req_add_friend_req (req);
					break;
				case db::type_t::e_add_friend_req:
					resp = handle_add_req (req);
					break;
				case db::type_t::e_update_add_friend_result_req:
					resp = handle_update_add_friend_result_req (req);
					break;
				case db::type_t::e_del_friend_req:
					resp = handle_del_friend_req (req);
					break;
				case db::type_t::e_update_friend_laber_req:
					resp = handle_update_friend_laber_req (req);
					break;
				case db::type_t::e_update_join_group_result_req:
					resp = handle_update_join_group_result_req (req);
					break;
				case db::e_group_add_user_req:
					resp = handle_group_add_user_req (req);
					break;
				case db::e_group_del_user_req:
					resp = handle_group_del_user_req (req);
					break;
				default:
					break;
			}
			result = resp->to_json ();
			delete resp;
			delete req;
			return true;
		}
		void init_mongo ();
		void uninit_mongo ();
		//req handle
		db::resp_t * handle_add_req (db::req_t * req);
		db::resp_t * handle_update_add_friend_result_req (db::req_t * _req);
		db::resp_t * handle_del_friend_req (db::req_t * _req);
		db::resp_t * handle_update_friend_laber_req (db::req_t * _req);
		db::resp_t * handle_update_join_group_result_req (db::req_t * _req);
		db::resp_t * handle_group_add_user_req (db::req_t * _req);
		db::resp_t * handle_group_del_user_req (db::req_t * _req);
		db::resp_t * handle_req_add_friend_req (db::req_t * req);
		//
		acl::socket_stream* stream_;
		std::string mongodb_addr_;
		//mongodb
		mongoc_client_t      *mongo_client_;
		mongoc_collection_t  *user_clt_;
		mongoc_collection_t  *chat_log_clt_;
		mongoc_collection_t  *group_clt_;

		//config
		std::string db_name_;
		std::string user_collection_name_;
		std::string chat_log_collection_name_;
	};
}