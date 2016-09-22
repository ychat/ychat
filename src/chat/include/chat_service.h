#pragma once



//////////////////////////////////////////////////////////////////////////
namespace ychat
{
	

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
