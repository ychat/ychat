#include <bson.h>
#include <bcon.h>
#include <mongoc.h>
#include "msg.hpp"
namespace ychat
{
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
		bool check_friend (const std::string &client_id,const std::string &friend_id);
		bool to_msg_queue (msg_t *msg);

		uint32_t get_msg_queue_id (const std::string &client_id);
		bool handle_chat_ack_msg (msg_t * msg);
		long long get_uuid ();
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