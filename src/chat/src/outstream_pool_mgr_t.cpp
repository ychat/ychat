#include "stdafx.h"
#include "outstream_pool_mgr_t.h"
#include "outstream_pool_t.h"
namespace ychat
{

	outstream_pool_mgr_t::outstream_pool_mgr_t()
	{

	}

	outstream_pool_mgr_t::~outstream_pool_mgr_t()
	{

	}

	acl::connect_pool* outstream_pool_mgr_t::create_pool (const char* addr, 
														  size_t count, 
														  size_t idx)
	{
		return  new outstream_pool_t (addr, 0);
	}

}