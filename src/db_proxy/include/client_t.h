#pragma once
namespace ychat
{
	class client_auth_t;
	class client_t
	{
	public:
		client_t(client_auth_t &client_auth, acl::socket_stream* stream);
		~client_t ();

		//config
		std::string mongodb_addr_;
		std::string db_name_;
		std::string user_collection_name_;
		std::string chat_log_collection_name_;

		void run ();
	private:
		bool handle_req (const acl::string &data, acl::string &result);
		bool init_mongo ();
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
		db::resp_t * handle_req_join_group_req(db::req_t * _req);
		db::resp_t * handle_ping_req(db::req_t * _req);
		db::resp_t * handle_auth_req(db::req_t * _req);
		//
		acl::socket_stream* stream_;
		//mongodb
		mongoc_client_t      *mongo_client_;
		mongoc_collection_t  *user_clt_;
		mongoc_collection_t  *chat_log_clt_;
		mongoc_collection_t  *group_clt_;
		// auth
		bool is_auth_;
		client_auth_t &client_auth_;
	};
}