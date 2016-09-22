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

	void zk_watcher_t::on_connected ()
	{
		logger (__FUNCTION__);
		get_config();

		//sequence instance-,result will install-0,install-1,install-3,and so on
		int ret = recursive_create (ychat_path_prefix_, std::string(), 
										ychat_path, ZOO_SEQUENCE);
		if (ret != ZOK)
		{
			logger_fatal ("zk_create %s error;[%s]",
						  ychat_path_prefix_.c_str(), 
						  zk_client_->get_error_str(ret));
		}
		//create ///addr path
		std::string real_path;
		std::string path = ychat_path + "/addr";
		ret = zk_client_->create(path, addr_, real_path, ZOO_EPHEMERAL);
		if(ret != ZOK) {
			logger_fatal("zk_create %s error;[%s]",
						 ychat_path_prefix_.c_str(),
						 zk_client_->get_error_str(ret));
		}
		else
			acl_assert(real_path == path);
		
		//watch queue_slots
		path = ychat_path + "/queue_slots";
		Stat stat;
		ret = zk_client_->exists (path, true, stat);
		if (ret == ZOK)
		{
			if (stat.dataLength)
			{
				update_queue_slots_data (); 
			}
		}
	}

	void zk_watcher_t::on_connecting ()
	{
		logger (__FUNCTION__);
	}

	void zk_watcher_t::on_auth_failed ()
	{
		logger (__FUNCTION__);
	}

	void zk_watcher_t::on_associating ()
	{
		logger (__FUNCTION__);
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

	int zk_watcher_t::recursive_create (const std::string& path, 
										   const std::string& value, 
										   std::string real_path, 
										   int flag)
	{
		std::string::size_type pos = 0;
		int ret = ZOK;
		do
		{
			pos = path.find ('/', pos + 1);
			if (pos == std::string::npos)
			{
				break;
			}
			real_path.clear ();
			ret = zk_client_->create (path.substr (0, pos), 
									  std::string (), 
									  real_path, 
									  0);
			if (ret != ZOK || ret != ZNODEEXISTS)
				return ret;
			pos = pos + 1;
		} while (true);

		return zk_client_->create (path.substr (0, pos), value, real_path, 0);
	}

	void zk_watcher_t::set_chat_path_prefix (const std::string &path)
	{
		ychat_path_prefix_ = path;
	}

	void zk_watcher_t::set_chat_service_addr (const std::string &addr)
	{
		addr_ = addr;
	}

	void zk_watcher_t::handle_queue_slots (const std::string &buffer)
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
		}

	}

	void zk_watcher_t::set_zk_client(zk_client_t *zk)
	{
		zk_client_ = zk;
	}

	void zk_watcher_t::get_config()
	{
		config_init_t config;
		//get redis addr
		int ret = zk_client_->get(redis_addr_path_, true, config.redis_addr_);
		if (ret != ZOK)
		{
			logger_fatal("zk get error,path:%s, code:%s",
						 redis_addr_path_.c_str(),
						 zk_client_->get_error_str(ret));
		}

		//get mongodb addr
		
		ret = zk_client_->get(mongodb_addr_path_, true, config.mongodb_addr_);
		if(ret != ZOK) 
		{
			logger_fatal("zk get error,path:%s, code:%s",
						 redis_addr_path_.c_str(),
						 zk_client_->get_error_str(ret));
		}
		for(callback_set_iter_t it = callbacks_.begin();
			it != callbacks_.end();
			++it) {
			//notify clear
			(*it)->on_event(config);
		}
	}

	void zk_watcher_t::set_redis_addr_path (const std::string &path)
	{
		redis_addr_path_ = path;
	}

	void zk_watcher_t::set_mongodb_addr_path (const std::string &path)
	{
		mongodb_addr_path_ = path;
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
			handle_queue_slots (buffer);
		}
	}

}

