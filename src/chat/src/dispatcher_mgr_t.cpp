#include "stdafx.h"
#include "dispatcher_mgr_t.h"
#include "zk_watcher_t.h"
#include "dispatcher_t.h"

namespace ychat
{
	

	dispatcher_mgr_t::dispatcher_mgr_t ()
	{

	}

	void dispatcher_mgr_t::init()
	{

	}

	void dispatcher_mgr_t::on_event (start_t &start)
	{
		(void)start;
	}

	void dispatcher_mgr_t::on_event (stop_t &stop)
	{
		(void)stop;
	}

	void dispatcher_mgr_t::on_event (msg_queue_slots_change_t &msg_queue_change)
	{
		if (msg_queue_change.queue_id_.size() == 0)
		{
			if (dispatchers_.size() == 0)
			{
				return;
			}

			for (dispatchers_itr_t itr = dispatchers_.begin (); 
				itr != dispatchers_.end();
				++itr)
			{
				(itr)->second->stop ();
			}
			return;
		}

		for (std::vector<uint32_t>::iterator queue_id_itr =
			 msg_queue_change.queue_id_.begin ();
			 queue_id_itr != msg_queue_change.queue_id_.end ();
			 queue_id_itr++)
		{
			dispatchers_itr_t itr = dispatchers_.find (*queue_id_itr);
			if (itr != dispatchers_.end ())
			{
				//todo ...
			}
		}
	}

	void dispatcher_mgr_t::on_event(redis_addr_change_t &change)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void dispatcher_mgr_t::on_event(mongodb_addr_change_t &change)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

}

