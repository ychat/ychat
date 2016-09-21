#include "stdafx.h"
#include "socket_session_t.h"
#include "chat_service.h"
#include "zk_client_t.h"
#include "zk_watcher_t.h"
#include "dispatcher_mgr_t.h"


namespace ychat
{

	static char *var_zk_add_str;
	static char *var_chat_path_prefix_str;

	acl::master_str_tbl var_conf_str_tab[] = {
		{ "str", "127.0.0.1:2181", &var_zk_add_str },
		{ "chat_path_prefix", "/service/chat/instance-",
								&var_chat_path_prefix_str },

		{ 0, 0, 0 }
	};

	static int  var_cfg_debug_enable;

	acl::master_bool_tbl var_conf_bool_tab[] = {
		{ "debug_enable", 1, &var_cfg_debug_enable },

		{ 0, 0, 0 }
	};

	static int  var_cfg_io_timeout;
	static int  var_cfg_zk_recv_timeout;

	acl::master_int_tbl var_conf_int_tab[] = {
		{ "io_timeout", 120, &var_cfg_io_timeout, 0, 0 },
		{ "zk_recv_timeout", 120, &var_cfg_zk_recv_timeout, 0, 0 },
		{ 0, 0, 0, 0, 0 }
	};
	std::string service_addr;

	acl::master_int64_tbl var_conf_int64_tab[] = {
		{ 0, 0, 0, 0, 0 }
	};

	//////////////////////////////////////////////////////////////////////////

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
		zk_watch_->set_chat_path_prefix(var_chat_path_prefix_str);
		zk_watch_->set_chat_service_addr(service_addr);
		zk_watch_->set_zk_client(zk_client_);

		int ret = zk_client_->init(var_zk_add_str, 
				var_cfg_zk_recv_timeout, zk_watch_);
		if (ret == false)
		{
			logger_fatal("zk_client init error, addr:%s", var_zk_add_str);
		}


	}

	void chat_service::proc_on_exit (void)
	{
		logger (">>>proc_on_exit<<<");
	}

}