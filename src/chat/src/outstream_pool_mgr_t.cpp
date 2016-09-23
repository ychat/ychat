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

	outstream_pool_t * outstream_pool_mgr_t::get(const std::string &addr_)
	{
		outstream_pool_t *pool;
		outstream_pools_itr_t itr = outstream_pools_.find(addr_);
		if(itr == outstream_pools_.end()) 
		{
			if (outstream_addrs_.find(addr_) != outstream_addrs_.end())
			{
				pool = new outstream_pool_t (addr_.c_str (), 0);
				pool->inc_ref ();
				outstream_pools_[addr_] = pool;
				return pool;
			}
			//the outstream server is down 
			return NULL;
		}
		return itr->second;
	}

	void outstream_pool_mgr_t::del (const std::string &addr)
	{
		if (outstream_addrs_.find(addr) != outstream_addrs_.end())
		{
			outstream_addrs_.erase (outstream_addrs_.find (addr));
			if (outstream_pools_.find (addr) != outstream_pools_.end ())
			{
				//decrease ref count 
				outstream_pools_[addr]->dec_ref ();
				outstream_pools_.erase (outstream_pools_.find (addr));
			}
		}
		else
		{
			//why this?
			logger_warn ("addr no in outstream_addr list:%s",addr.c_str());
		}
	}

	void outstream_pool_mgr_t::add_addr (const std::string &addr)
	{
		outstream_addrs_.insert (addr);
	}

}