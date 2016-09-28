#pragma once

namespace ychat
{
	class db_proxy_client_t:public acl::connect_client
	{
	public:
		db_proxy_client_t ();
		~db_proxy_client_t ();
		virtual bool open () override;
		void set_addr (const std::string &addr);
	private:
		std::string addr_;
		acl::socket_stream stream_;
	};

	

	
}