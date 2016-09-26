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
	};
}