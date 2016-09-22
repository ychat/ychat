#include "stdafx.h"
#include "dispatcher_t.h"

namespace ychat
{

	dispatcher_t::dispatcher_t (uint32_t msg_queue_id)
		:msg_queue_id_(msg_queue_id)
	{

	}
	
	dispatcher_t::~dispatcher_t ()
	{

	}


	void dispatcher_t::init ()
	{

	}

	uint32_t dispatcher_t::get_msg_queue_id ()
	{
		return msg_queue_id_;
	}

	void dispatcher_t::run (void)
	{
		//this block can't not block the fiber forever
		init_mongodb();
		do 
		{

		} while (is_stop_ == false);
		char _;
		unint_mongodb ();
		wait_for_stop_ << _;
	}

	void dispatcher_t::stop (bool)
	{
		is_stop_ = true;
		char _;
		wait_for_stop_.pop (_);
	}

	void dispatcher_t::set_redis_cluster (acl::redis_client_cluster *cluster)
	{
		redis_cluster_ = cluster;
	}

	void dispatcher_t::set_mongodb_addr (const std::string &addr)
	{
		mongodb_addr_ = addr;
	}

	void dispatcher_t::init_mongodb ()
	{
		
	}

	void dispatcher_t::unint_mongodb ()
	{

	}

}

