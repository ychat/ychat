#include "stdafx.h"
#include "client_session_t.h"
#include "chat_service.h"
#include "zk_client_t.h"
#include "zk_watcher_t.h"
#include "dispatcher_mgr_t.h"
#include "config.h"


namespace ychat
{
	chat_service::chat_service (void)
	{}

	chat_service::~chat_service (void)
	{}

	void chat_service::on_accept (acl::socket_stream& conn)
	{
		logger ("connect from %s, fd %d",
				conn.get_peer (), 
				conn.sock_handle ());

		conn.set_rw_timeout (0);

		client_session_t socket_sess (conn);
		client_sessions_.insert (&socket_sess);
		//here is in a fiber.
		socket_sess.run ();
		if (client_sessions_.find(&socket_sess) != client_sessions_.end())
		{
			client_sessions_.erase (client_sessions_.find (&socket_sess));
		}

		logger ("disconnect from %s", conn.get_peer ());
	}

	void chat_service::proc_pre_jail (void)
	{
		logger (">>>proc_pre_jail<<<");
	}

	void chat_service::proc_on_init (void)
	{
		logger (">>>proc_on_init<<<");
		dispatcher_mgr_ = new dispatcher_mgr_t;
		zk_client_ = new zk_client_t;

		zk_watcher_ = new zk_watcher_t;
		zk_watcher_->set_chat_service_path(g_config.chat_path_prefix_);
		zk_watcher_->set_zk_client(zk_client_);
		zk_watcher_->watch (dispatcher_mgr_);

		int ret = zk_client_->init(g_config.zk_addr_, 
				g_config.zk_recv_timeout_, zk_watcher_);
		if (ret == false)
		{
			logger_fatal("zk_client init error, addr:%s", 
						 g_config.zk_addr_);
		}

	}
	
	void chat_service::on_event (const zk_status_update_t &status)
	{
		switch (status.status_)
		{
			case zk_status_update_t::e_associating:
				logger ("zk_status associating");
			case zk_status_update_t::e_connecting:
				logger ("zk_status connecting");
				break;
			case zk_status_update_t::e_connected:
				init ();
				zk_watcher_->do_watch ();
			default:
				break;
		}
	
	}

	void chat_service::proc_on_exit (void)
	{
		logger (">>>proc_on_exit<<<");
	}

	void chat_service::init ()
	{
		//get redis addr
		int ret = zk_client_->get (g_config.redis_addr_path_, true,
								   redis_addr_);
		if (ret != ZOK)
		{
			logger_fatal ("zk get error,path:%s, code:%s",
						  g_config.redis_addr_path_,
						  zk_client_->get_error_str (ret));
		}

		//get mongodb addr
		ret = zk_client_->get (g_config.mongodb_addr_path_,
							   true,
							   mongodb_addr_);
		if (ret != ZOK)
		{
			logger_fatal ("zk get error,path:%s, code:%s",
						  g_config.mongodb_addr_path_,
						  zk_client_->get_error_str (ret));
		}
		redis_cluster_ = new acl::redis_client_cluster ();
		redis_cluster_->set (redis_addr_.c_str (), 0);


		//
		//sequence instance-,result will install-0,install-1,install-3,and so on
		int ret = zk_client_->recursive_create (g_config.chat_path_prefix_,
												std::string (),
												chat_service_path_, 
												ZOO_SEQUENCE);
		if (ret != ZOK)
		{
			logger_fatal ("zk_create %s error;[%s]",
						  g_config.chat_path_prefix_,
						  zk_client_->get_error_str (ret));
		}
		//create ///addr path
		std::string real_path;
		std::string path = chat_service_path_ + "/addr";
		ret = zk_client_->create (path, g_config.service_addr_, real_path, 
								  ZOO_EPHEMERAL);
		if (ret != ZOK)
		{
			logger_fatal ("zk_create %s error;[%s]",
						  g_config.chat_path_prefix_,
						  zk_client_->get_error_str (ret));
		}
		acl_assert (real_path == path);
	}
	

}