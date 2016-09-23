#include "stdafx.h"
#include "dispatcher_mgr_t.h"
#include "zk_watcher_t.h"
#include "dispatcher_t.h"
#include "lock_guard.hpp"
#include "outstream_pool_mgr_t.h"

namespace ychat
{
	

	dispatcher_mgr_t::dispatcher_mgr_t ()
	{

	}

	void dispatcher_mgr_t::init()
	{
		outstream_pool_mgr_ = new outstream_pool_mgr_t();
	}

	void dispatcher_mgr_t::on_event (const start_t &start)
	{
		(void)start;
	}

	void dispatcher_mgr_t::on_event (const stop_t &stop)
	{
		(void)stop;
	}

	void dispatcher_mgr_t::on_event (const msg_queue_slots_change_t 
									 &msg_queue_change)
	{
		//clear all dispathers
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
				(itr)->second->stop (true);
			}
			return;
		}

		//
		dispatchers_t tmp;
		for (std::vector<uint32_t>::const_iterator queue_id_itr =
			 msg_queue_change.queue_id_.begin ();
			 queue_id_itr != msg_queue_change.queue_id_.end ();
			 queue_id_itr++)
		{
			dispatchers_itr_t itr = dispatchers_.find (*queue_id_itr);
			int queue_slot = *queue_id_itr;

			if (itr != dispatchers_.end ())
			{
				dispatcher_t *dispatcher = new dispatcher_t(queue_slot);
				tmp.insert (std::make_pair (queue_slot, dispatcher));
			}
			else
			{
				tmp.insert (std::make_pair(queue_slot,itr->second));
				dispatchers_.erase (itr);
			}
		}
		//stop remain dispather
		tmp.swap (dispatchers_);
		for (dispatchers_itr_t itr = tmp.begin ();
			itr != tmp.end();
				++itr)
		{
			itr->second->stop (true);
			delete itr->second;
		}
	}

	void dispatcher_mgr_t::on_event(const config_init_t &change)
	{
		//reset dispathers;
		for (dispatchers_itr_t itr = dispatchers_.begin ();
			itr != dispatchers_.end ();
			++itr)
		{
			itr->second->stop (true);
			delete itr->second;
		}
		redis_addr_ = change.redis_addr_;
		if (redis_cluster_)
			delete redis_cluster_;
		redis_cluster_ = new acl::redis_client_cluster;
		//2 seconds
		redis_cluster_->set (redis_addr_.c_str (), 0, 2, 2);

		//remake dispather
		dispatchers_t tmp;
		for (dispatchers_itr_t itr = dispatchers_.begin ();
		itr != dispatchers_.end ();
			++itr)
		{
			dispatcher_t *dispatcher = new dispatcher_t (itr->first);
			dispatcher->set_redis_cluster (redis_cluster_);
			tmp[itr->first] = dispatcher;
		}
		tmp.swap (dispatchers_);

	}

	void dispatcher_mgr_t::on_event (const outstream_info_update_t &update)
	{
		if (last_outstream_status_.infos_ == update.infos_)
			return;

		std::vector<std::string> news, down;

		for (std::map<std::string, outstream_info_t>::const_iterator it 
			 = update.infos_.begin ();
			it != update.infos_.end();
			++it)
		{
			std::map<std::string, outstream_info_t>::iterator itr =
				last_outstream_status_.infos_.find (it->first);
			if (itr == last_outstream_status_.infos_.end())
			{
				news.push_back (it->first);
				continue;
			}
			//
			if (it->second.status_ == outstream_info_t::e_up &&
				itr->second.status_ == outstream_info_t::e_down)//down.
			{

				down.push_back(it->first);
			}

		}
		// clear the outstream when it'status is down

		for (std::vector<std::string>::const_iterator itr = down.begin ();
			itr != down.end();
			++itr)
		{
			//remove 
			outstream_pool_mgr_->del (*itr);
		}

		for (std::vector<std::string>::const_iterator itr = news.begin ();
			itr != news.end ();
			++itr)
		{
			//remove 
			outstream_pool_mgr_->add_addr(*itr);
		}
	}

}

