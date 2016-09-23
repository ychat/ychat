namespace ychat
{
	class client_session_t
	{
	public:
		client_session_t(acl::socket_stream& conn);
		bool run ();
		bool check_auth ();
	private:
		bool is_auth_;
		acl::socket_stream conn_;
	};
}