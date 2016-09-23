#include "stdafx.h"
#include "zk_watcher_t.h"
#include "zk_client_t.h"
namespace ychat
{

	void zk_watcher_t::watch (event_callback_t *callback)
	{
		callbacks_.insert (callback);
	}

	void zk_watcher_t::del (event_callback_t *callback)
	{
		callback_set_iter_t itr = callbacks_.find (callback);
		if (itr != callbacks_.end())
		{
			callbacks_.erase (itr);
		}
	}

	zk_watcher_t::zk_watcher_t ()
		:zk_client_ (NULL)
	{

	}

	void zk_watcher_t::do_watch ()
	{
		//watch queue_slots
		queue_slot_path_ = chat_service_path_ + "/queue_slots";
		Stat stat;
		int ret = zk_client_->exists (queue_slot_path_, true, stat);
		if (ret == ZOK)
		{
			if (stat.dataLength)
			{
				update_queue_slots_data ();
			}
		}
	}

	void zk_watcher_t::on_session_expired ()
	{
		logger (__FUNCTION__);

		for (callback_set_iter_t it = callbacks_.begin ();
		it != callbacks_.end ();
			++it)
		{
			//notify 
			(*it)->on_event (stop_t());
		}
	}

	void zk_watcher_t::on_created (const char* path)
	{
		logger ("function:%s,path:%s", __FUNCTION__, path);
		
		if (queue_slot_path_ == path)
		{
			update_queue_slots_data ();
		}
	}

	void zk_watcher_t::on_deleted (const char* path)
	{
		logger ("function:%s,path:%s", __FUNCTION__, path);
		if (queue_slot_path_ == path)
		{
			update_queue_slots_data ();
		}
	}

	void zk_watcher_t::on_changed (const char* path)
	{
		logger ("function:%s, path:%s",__FUNCTION__,path);
		if (queue_slot_path_ == path)
		{
			update_queue_slots_data ();
		}
	}

	void zk_watcher_t::on_child_changed (const char* path)
	{
		logger ("function:%s,path:%s", __FUNCTION__, path);
		if (queue_slot_path_ == path)
		{
			update_queue_slots_data ();
		}
	}

	void zk_watcher_t::on_not_watching (const char* path)
	{
		logger ("function:%s,path:%s",__FUNCTION__,path);
	}



	void zk_watcher_t::set_chat_service_path (const std::string &path)
	{
		chat_service_path_ = path;
	}

	void zk_watcher_t::handle_queue_slots_update (const std::string &buffer)
	{
		if (buffer.size () == 0)
		{
			for (callback_set_iter_t it = callbacks_.begin (); 
				it != callbacks_.end(); 
				++it)
			{
				//notify clear
				(*it)->on_event (msg_queue_slots_change_t());
			}
			return;
		}

		acl::json json;
		json.update (buffer.c_str());

		std::vector<acl::json_node*> slots = json.getElementsByTagName ("slots");
		if (slots.empty())
		{
			logger_warn ("get slots error,data:%s",buffer.c_str());
			return;
		}

		for (std::vector<acl::json_node*>::iterator it = slots.begin ();
			it != slots.end();
			it++)
		{
			const long long *slot = (*it)->get_int64 ();
			if (slot == NULL)
			{
				logger_warn ("get_init64 error");
				return;
			}

		}

	}

	void zk_watcher_t::set_zk_client(zk_client_t *zk)
	{
		zk_client_ = zk;
	}

	void zk_watcher_t::get_outstream_instance_info ()
	{
		std::vector<std::string> result;
		int ret = zk_client_->get_children (outstream_path_, true, result);
		if (ret!= ZOK){
			logger_error ("zk_client get_children  error \
						  ,path:%s error_str:%s",
						  outstream_path_.c_str(),
						  zk_client_->get_error_str(ret));
			return;
		}
		if (result.empty ())
			return;
		outstream_info_update_t new_info;
		for (std::vector<std::string>::iterator itr = result.begin ();
			itr != result.end();
			++itr)
		{

			std::string status;
			//get address ip:port string
			std::string status_path = (*itr) + "/status";
			ret = zk_client_->get (status_path, 0, status);
			if (ret != ZOK)
			{
				logger_error ("zk_get error,path:%s,error_str:%s",
							  status_path.c_str (),
							  zk_client_->get_error_str (ret));
			}
			//this instance not set addr ?
			if (status.size () == 0)
				continue;

			std::string addr;
			//get address ip:port string
			std::string addr_path = (*itr) + "/addr";
			ret = zk_client_->get (addr_path, 0, addr);
			if (ret != ZOK)
			{
				logger_error ("zk_get error,path:%s,error_str:%s", 
							  addr_path.c_str(), 
							  zk_client_->get_error_str(ret));
			}
			//this instance not set addr ?
			if(addr.size() == 0)
				continue;

			outstream_info_t info;
			info.addr_ = addr;
			info.status_ = (strcasecmp (status.c_str (), "up") == 0)?
				outstream_info_t::e_up: outstream_info_t::e_down;

			new_info.infos_.insert(std::make_pair(addr, info));
		}
		for (callback_set_iter_t it = callbacks_.begin ();
		it != callbacks_.end ();
				++it)
		{
			(*it)->on_event (new_info);
		}

	}

	void zk_watcher_t::update_queue_slots_data ()
	{
		std::string buffer;
		int ret = zk_client_->get (queue_slot_path_, true, buffer);
		if (ret != ZOK)
		{
			logger_fatal ("zk get %s error;[%s]",
						  queue_slot_path_.c_str (),
						  zk_client_->get_error_str (ret));
		}
		//maybe nothing change.
		if (buffer != last_queue_slot_data_)
		{
			last_queue_slot_data_ = buffer;
			handle_queue_slots_update (buffer);
		}
	}

	void zk_watcher_t::set_outstream_path (const std::string &path)
	{
		outstream_path_ = path;
	}



}


