#include "stdafx.h"
#include "db_proxy_client_t.h"
#include "db_req_t.h"
namespace ychat
{

	db_proxy_client_t::db_proxy_client_t ()
	{

	}

	db_proxy_client_t::~db_proxy_client_t ()
	{
		if (stream_.alive ())
		{
			stream_.close ();
		}
	}

	bool db_proxy_client_t::open ()
	{
		return stream_.open (addr_.c_str (), conn_timeout_, rw_timeout_);
	}

	void db_proxy_client_t::set_addr (const std::string &addr)
	{
		addr_ = addr;
	}

}