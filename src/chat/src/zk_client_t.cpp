#include "stdafx.h"
#include "zk_client_t.h"
#include <string.h>

namespace ychat
{

	zk_client_t::zk_client_t ()
		:zhandle (NULL),
		event_callback_ (NULL)
	{

	}

	zk_client_t::~zk_client_t ()
	{
		if (zhandle)
		{
			int ret = zookeeper_close (zhandle);
			if (ret != ZOK)
			{
				const char *str = zerror (ret);
				if (str)
					logger_warn ("zookeeper_close error;[%s]", str);
			}
		}
	}

	bool zk_client_t::init (const std::string &hosts, int recv_timeout, 
								   zk_callback_t *callback)
	{
		event_callback_ = callback;
		zhandle = zookeeper_init (hosts.c_str (), watcher_callback, 
								  recv_timeout, NULL, this, 0);
		if (zhandle == NULL)
		{
			logger_error("zookeeper_init  error:[%s]",strerror(errno));
			return false;
		}
		return true;
	}

	int zk_client_t::create (const std::string &path, 
							 const std::string &value, 
							 std::string &real_path, 
							 int flag )
	{
		const char *cvalue = NULL;
		int value_len = -1;
		if (value.size ())
		{
			cvalue = value.data ();
			value_len = value.length ();
		}
		std::string real_path;
		real_path.reserve (path.length () + 64);
		return zoo_create (zhandle, path.c_str (), cvalue, value_len,
						   &ZOO_OPEN_ACL_UNSAFE, flag,
						   (char*)real_path.data (),
						   real_path.capacity ());
	}

	int zk_client_t::exists (const std::string &path, bool watch, Stat &stat)
	{
		acl_assert (zhandle);
		return zoo_exists (zhandle, path.c_str (), watch, &stat);
	}

	int zk_client_t::get (const std::string&path,
						  bool watch, 
						  std::string &buffer)
	{
		acl_assert (zhandle);
		Stat stat;
		int ret = exists (path, watch, stat);
		if (ret != ZOK)
			return ret;

		buffer.reserve (stat.dataLength * 2 < 64 * 1014?
						64 * 1024:stat.dataLength * 2);

		int buffer_len = buffer.capacity ();
		ret = zoo_get (zhandle, path.c_str (), watch, (char*)buffer.data (),
					   &buffer_len, &stat);
		if (ret != ZOK)
			return ret;
		if (buffer_len > buffer.capacity ())
		{
			//todo :process error.
		}
		buffer.resize (buffer_len);
		return ret;
	}

	int zk_client_t::set (const std::string &path, bool watch, 
						  const std::string &buffer, int version /*= -1*/)
	{
		acl_assert (zhandle);
		return zoo_set (zhandle, path.c_str (), 
						   buffer.data (), 
						   buffer.size (), version);
		
	}

	int zk_client_t::del (const std::string &path, int version /*= -1*/)
	{
		acl_assert (zhandle);
		return zoo_delete (zhandle, path.c_str (), version);
	}

	bool zk_client_t::get_children (const std::string &path,
									bool watch,
									std::vector<std::string> &result)
	{
		acl_assert (zhandle);
		String_vector str_vec;
		int ret = zoo_get_children (zhandle, path.c_str (), watch, &str_vec);
		if (ret != ZOK)
			return ret;
		for (int i = 0; i < str_vec.count; i++)
		{
			result.emplace_back (str_vec.data[i]);
		}
		return ZOK;
	}


	void zk_client_t::watcher_callback (zhandle_t *zh,
											   int type,
											   int state, 
											   const char *path, 
											   void *watcherCtx)
	{
		acl_assert (watcherCtx);
		zk_client_t *self = 
			(zk_client_t *)watcherCtx;
		if (self->event_callback_ == NULL){
			return;
		}

		if (type == ZOO_SESSION_EVENT)
		{
			if (state == ZOO_EXPIRED_SESSION_STATE)
			{
				self->event_callback_->on_session_expired();
			}
			else if (state == ZOO_CONNECTED_STATE)
			{
				self->event_callback_->on_connected();
			}
			else if (state == ZOO_CONNECTING_STATE)
			{
				self->event_callback_->on_connecting ();
			}
			else if(state == ZOO_AUTH_FAILED_STATE)
			{
				self->event_callback_->on_auth_failed ();
			}
			else if(state == ZOO_ASSOCIATING_STATE)
			{
				self->event_callback_->on_associating ();
			}
		}
		else if (type == ZOO_CREATED_EVENT)
		{
			self->event_callback_->on_created (path);
		}
		else if (type == ZOO_DELETED_EVENT)
		{
			self->event_callback_->on_deleted (path);
		}
		else if (type == ZOO_CHANGED_EVENT)
		{
			self->event_callback_->on_changed (path);
		}
		else if (type == ZOO_CHILD_EVENT)
		{
			self->event_callback_->on_child_changed (path);
		}
		else if (type == ZOO_NOTWATCHING_EVENT)
		{
			self->event_callback_->on_not_watching (path);
		}
	}
	
	const char* zk_client_t::get_error_str (int code)
	{
		const char *str = zerror (code);
		if (str == NULL)
			return "";
		return str;
	}


}

