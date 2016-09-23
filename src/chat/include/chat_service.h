#pragma once
#include "zk_watcher_t.h"



//////////////////////////////////////////////////////////////////////////
namespace ychat
{
	

	class zk_client_t;
	class zk_watcher_t;
	class dispatcher_mgr_t;
	class client_session_t;

	class chat_service: public acl::master_fiber,event_callback_t
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

		virtual void on_event (const zk_status_update_t &status);
		void init ();
	private:
		std::string redis_addr_;
		std::string mongodb_addr_;
		std::string chat_service_path_;
		zk_client_t *zk_client_;
		zk_watcher_t *zk_watcher_;
		dispatcher_mgr_t *dispatcher_mgr_;
		std::set<client_session_t *> client_sessions_;
		acl::redis_client_cluster *redis_cluster_;

	};

}
