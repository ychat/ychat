#include "stdafx.h"
#include "db_proxy_t.h"
#include "client_t.h"
namespace ychat
{

	db_proxy_t::db_proxy_t()
	{

	}

	db_proxy_t::~db_proxy_t()
	{

	}

	void db_proxy_t::handle_socket(acl::socket_stream& conn)
	{
		client_t client(*client_auth_, &conn);
		std::list<client_t &>::iterator itr = 
			clients_.insert(clients_.begin(), client);
		
		//mongodb config
		client.mongodb_addr_ = mongodb_addr_;
		client.db_name_ = db_name_;
		client.chat_log_collection_name_ = chat_log_collection_name_;
		client.user_collection_name_ = user_collection_name_;

		client.run();
		clients_.erase(itr);
	}

}