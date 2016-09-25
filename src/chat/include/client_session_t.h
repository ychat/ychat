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
		bool handle_msg(char type, int len, const char *data);
	private:
		bool is_stop_;
		bool is_auth_;
		acl::redis_client_cluster *redis_cluster_;
		acl::socket_stream conn_;
	};
}