#include "stdafx.h"
#include "config.h"

namespace ychat
{
	config_t g_config;

	config_t::config_t ()
	{
		str_tab_ = new acl::master_str_tbl[10] 
		{
			{"str", "127.0.0.1:2181", &zk_addr_},
			{"chat_path_prefix", "/ychat/service/chat/instance-", &chat_path_prefix_},
			{"service_addr", "0.0.0.0:9001", &service_addr_},
			{"redis_addr","/ychat/config/redis/addr", &redis_addr_path_},
			{"mongodb_addr", "/ychat/config/mongodb/addr", &mongodb_addr_path_},
			{"msg_queue_prefix","/ychat/msg_queue/",&msg_queue_prefix_},
			{"friend_map_prefix","/ychat/friends/",&friend_map_prefix_},
			{"mongo_db_name","ychat",&mongo_db_name_},
			{"mongodb_user_collection_name", "user_", &mongodb_user_collection_name_},
			{"mongodb_chat_log_collection_name","chat_log", &mongodb_chat_log_collection_name_}
			{0, 0, 0}
		};

		bool_tab_ = new acl::master_bool_tbl[2]
		{
			{"debug_enable", 1, &g_config.debug_enable_},
			{0, 0, 0}
		};

		int_tab_ = new acl::master_int_tbl[3]
		{
			{"io_timeout", 120, &io_timeout_, 0, 0},
			{"zk_recv_timeout", 120, &zk_recv_timeout_, 0, 0},
			{0, 0, 0, 0, 0}
		};
		acl::master_int64_tbl var_conf_int64_tab[] = {
			{0, 0, 0, 0, 0}
		};
	}

}