#include "stdafx.h"
#include "socket_session_t.h"
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
		logger ("connect from %s, fd %d", conn.get_peer (), conn.sock_handle ());

		conn.set_rw_timeout (0);

		socket_session_t socket_sess (conn);

		while (socket_sess.doRun ())
		{
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

		zk_watch_ = new zk_watcher_t;
		zk_watch_->set_chat_path_prefix(g_config.chat_path_prefix_);
		zk_watch_->set_chat_service_addr(g_config.service_addr_);
		zk_watch_->set_redis_addr_path (g_config.redis_addr_path_);
		zk_watch_->set_mongodb_addr_path (g_config.mongodb_addr_path_);
		zk_watch_->set_zk_client(zk_client_);
		zk_watch_->watch (dispatcher_mgr_);

		int ret = zk_client_->init(g_config.zk_addr_, 
				g_config.zk_recv_timeout_, zk_watch_);
		if (ret == false)
		{
			logger_fatal("zk_client init error, addr:%s", 
						 g_config.zk_addr_);
		}

	}

	void chat_service::proc_on_exit (void)
	{
		logger (">>>proc_on_exit<<<");
	}

}