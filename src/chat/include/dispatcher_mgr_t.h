#pragma once
#include "zk_watcher_t.h"
namespace ychat
{
	class dispatcher_t;
	class outstream_pool_mgr_t;

	class dispatcher_mgr_t :public event_callback_t
	{
	public:
		dispatcher_mgr_t ();

	private:
		void init ();

		virtual void on_event (start_t &start);

		virtual void on_event (stop_t &stop);

		virtual void on_event (msg_queue_slots_change_t &msg_queue_change);

		virtual void on_event(config_init_t &change);

		typedef std::map<uint32_t, dispatcher_t*> dispatchers_t;

		typedef dispatchers_t::iterator dispatchers_itr_t;
	
		dispatchers_t dispatchers_;

		bool is_stop_;
		std::string redis_addr_;
		std::string mongodb_addr_;
		acl::redis_client_cluster *redis_cluster_;
		outstream_pool_mgr_t *outstream_pool_mgr_;
	};
}