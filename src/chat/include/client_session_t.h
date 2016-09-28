#include <bson.h>
#include <bcon.h>
#include <mongoc.h>

namespace ychat
{
	struct msg_t;
	class client_session_t
	{
	public:
		client_session_t(acl::socket_stream& conn);
		void set_redis_cluster(acl::redis_client_cluster *redis_cluster);
		void run ();
		bool check_auth ();
		void stop();
	private:
		void init_mongodb ();

		void uninit_mongodb ();
		
		bool handle_msg (const char *data, int len);

		bool handle_chat_msg (msg_t * msg);
		//handle msg
		bool handle_chat_ack_msg (msg_t * msg);
		
		bool handle_add_friend_result (msg_t * msg);
		
		bool handle_add_friend (msg_t * msg);
		
		bool handle_del_friend (msg_t * msg);

		bool handle_label_friend (msg_t * msg);

		bool handle_group_chat (msg_t * msg);

		bool handle_group_chat_ack (msg_t * msg);

		bool handle_join_group (msg_t * msg);

		bool handle_join_group_result(msg_t * msg);

		bool handle_leave_group(msg_t * msg);

		bool handle_group_del_menber(msg_t * msg);

		long long get_uuid ();

		bool get_group_creator (const std::string & group_id_, 
								std::string &creator);
		
		bool add_group_to_user(const std::string &user_id,
							   const std::string &username,
							   const std::string& group_id);

		// redis interface.
		bool to_redis_queue (msg_t *msg);

		bool get_username_from_redis (const std::string& user_id, 
									  std::string &username);

		bool check_friend_from_redis (const std::string &client_id,
						   const std::string &friend_id);

		bool check_group_member(const std::string &group_id,
								const std::string &user_id);

		bool add_friend_to_redis (std::string user_id, std::string friend_id,
								  const std::string &friend_name);

		bool del_friend_from_redis (const std::string &user_id, 
									const std::string &friend_id);

		bool update_friend_laber_to_redis(const std::string &user_id,
											 const std::string &friend_id,
											 const std::string &label);

		bool get_group_info_from_redis (const std::string &group_id, 
						std::map<acl::string, acl::string> &result);

		bool group_add_user_to_redis(const std::string &user_id,
									 const std::string &username, 
									 const std::string& group_id);

		bool del_user_from_redis(const std::string &user_id, 
								 const std::string &group_id);

		bool check_group_member_from_redis (const std::string & group_id, 
											const std::string &user_id);
		//

		//mongo interface
		bool get_username_from_mongo (const std::string& user_id, 
									  std::string &username);

		uint32_t get_msg_queue_id (const std::string &client_id);

		bool push_add_friend_to_db (msg_t * msg, std::string client_id);
		
		bool update_add_friend_result_to_db (msg_t * msg, std::string client_id);
		
		bool add_friend_to_db (std::string client_id, std::string friend_id,
							   const std::string &friend_name);

		bool del_friend_from_db (const std::string &user_id, 
									  const std::string &friend_id);

		bool update_friend_laber_to_db (const std::string &user_id,
											 const std::string &friend_id,
											 const std::string &label);

		bool update_join_group_result_to_db(const std::string &user_id,
												 const std::string &req_id,
												 const char *result);

		bool group_add_user_to_db(const std::string &user_id,
									 const std::string &username,
									 const std::string& group_id);

		bool del_user_from_db(const std::string &user_id,
								 const std::string &group_id);

		//helper function
		
		bool get_username (const std::string& user_id,
						   std::string &username);

		void notify_group_add_member(const std::string &user_id, 
									 const std::string &username, 
									 const std::string &group_id);


		void notify_group_leave_member(const std::string &user_id,
									   const std::string &username, 
									   const std::string &group_id);
		
		bool is_stop_;
		bool is_auth_;
		acl::redis_client_cluster *redis_cluster_;
		acl::socket_stream conn_;
		int msg_queues_;
		std::string mongodb_addr_;
		//mongodb
		mongoc_client_t      *mongo_client_;
		mongoc_database_t    *mongo_db_;
		mongoc_collection_t  *user_clt_;
		mongoc_collection_t  *chat_log_clt_;
		mongoc_collection_t  *group_clt_;
	};
}