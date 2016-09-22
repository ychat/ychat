#pragma once
namespace ychat
{
	struct config_t
	{
		config_t ();
		// chat_service addr ip:port
		char *service_addr_;
		// zookeeper server addr ip:port,ip:port,ip:port
		char *zk_addr_;
		// chat service instance prefix path in zk
		// default /ychat/service/chat/instance-
		char *chat_path_prefix_;
		//default /ychat/config/redis/addr
		char *redis_addr_path_;
		//default /ychat/config/mongodb/addr
		char *mongodb_addr_path_;
		// default false
		int  debug_enable_;
		// 120 (seconds)
		int  io_timeout_;
		// 120 (seconds)
		int  zk_recv_timeout_;

		//for acl
		acl::master_str_tbl *str_tab_;
		acl::master_bool_tbl *bool_tab_;
		acl::master_int_tbl *int_tab_;
		acl::master_int64_tbl *int64_tab_;

	};
	extern config_t g_config;
}