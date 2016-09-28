#pragma once
#include <list>
namespace ychat
{
	class db_proxy_t
	{
	public:
		db_proxy_t();
		~db_proxy_t();
		void handle_socket(acl::socket_stream& conn);
	public:
		//mongo config
		std::string mongodb_addr_;
		std::string db_name_;
		std::string user_collection_name_;
		std::string chat_log_collection_name_;
	private:
		std::list<class client_t&> clients_;
		class  client_auth_t *client_auth_;
	};
}