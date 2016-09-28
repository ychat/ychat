#pragma once
#include "stdafx.h"
#include "db_proxy_pool_manager_t.h"
#include "db_proxy_client_pool_t.h"
namespace ychat
{
	acl::connect_pool* db_proxy_pool_manager_t::create_pool (const char* addr,
															 size_t count,
															 size_t idx)
	{
		return new db_proxy_client_pool_t (addr, count, idx);
	}
}