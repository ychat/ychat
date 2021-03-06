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
		//default 120 (seconds)
		int  io_timeout_;
		//default 120 (seconds)
		int  zk_recv_timeout_;
		// default "/ychat/msg_queue/{queue_id}"
		char *msg_queue_prefix_;
		// default "/ychat/friends/{user_id}"
		char *friend_map_prefix_;
		// default "/ychat/user/{user_id}"
		char *user_id_prefix_;
		//group info in redis. default /yhat/group/
		char *group_map_prefix_;
		// default "/ychat/uuid"
		char *msg_uuid_;

		//mongo db name default "ychat"
		char *mongo_db_name_;

		//default users_
		char *mongodb_user_collection_name_;

		char *mongodb_chat_log_collection_name_;
		//for acl
		acl::master_str_tbl *str_tab_;
		acl::master_bool_tbl *bool_tab_;
		acl::master_int_tbl *int_tab_;
		acl::master_int64_tbl *int64_tab_;

	};
	extern config_t g_config;
}