#include "stdafx.h"
#include "outstream_pool_t.h"
#include "outstream.h"


namespace ychat
{
	outstream_pool_t::outstream_pool_t(const char* addr, size_t max /*= 0*/)
		:connect_pool(addr, max, 0)
	{

	}

	acl::connect_client* outstream_pool_t::create_connect()
	{
		return new outstream(addr_);
	}

}
