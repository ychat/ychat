#include "stdafx.h"
#include "db_proxy_client_t.h"
#include "db_proxy_client_pool_t.h"
namespace ychat
{
	db_proxy_client_pool_t::db_proxy_client_pool_t
		(const char* addr, size_t max, size_t idx /*= 0*/)
		:acl::connect_pool (addr, max, idx)
	{
	}

	acl::connect_client* db_proxy_client_pool_t::create_connect ()
	{
		db_proxy_client_t *client = new db_proxy_client_t;
		client->set_addr (addr_);
		client->set_timeout (conn_timeout_, rw_timeout_);
		return client;
	}



}