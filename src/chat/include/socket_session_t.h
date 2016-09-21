namespace ychat
{
	class socket_session_t
	{
	public:
		socket_session_t(acl::socket_stream& conn);
		bool doRun ();
	private:
		bool is_auth_;
		acl::socket_stream conn_;
	};
}