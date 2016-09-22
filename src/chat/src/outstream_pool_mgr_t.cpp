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
		for(outstream_pools_itr_t itr = outstream_pools_.begin();
			itr != outstream_pools_.end();
			itr++) 
		{
			delete itr->second;
		}
	}

	outstream_pool_t & outstream_pool_mgr_t::get(const std::string &addr_)
	{
		outstream_pool_t *pool;
		outstream_pools_itr_t itr = outstream_pools_.find(addr_);
		if(itr == outstream_pools_.end()) 
		{
			pool = new outstream_pool_t(addr_.c_str(), 0);
			outstream_pools_[addr_] = pool;
			return *pool;
		}
		return *itr->second;
	}

}