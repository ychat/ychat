#pragma once



//////////////////////////////////////////////////////////////////////////
namespace ychat
{
	extern acl::master_str_tbl var_conf_str_tab[];
	extern acl::master_bool_tbl var_conf_bool_tab[];
	extern acl::master_int_tbl var_conf_int_tab[];
	extern acl::master_int64_tbl var_conf_int64_tab[];
	extern std::string service_addr;

	class zk_client_t;
	class zk_watcher_t;
	class dispatcher_mgr_t;

	class chat_service: public acl::master_fiber
	{
	public:
		chat_service (void);
		~chat_service (void);

	protected:
		// @override
		void on_accept (acl::socket_stream& conn);

		// @override
		void proc_pre_jail (void);

		// @override
		void proc_on_init (void);

		// @override
		void proc_on_exit (void);

	private:

		zk_client_t *zk_client_;
		zk_watcher_t *zk_watch_;
		dispatcher_mgr_t *dispatcher_mgr_;
	};

}
