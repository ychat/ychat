#pragma once
#include <stdint.h>
#include <vector>
#include <set>
#include "zk_client_t.h"
namespace ychat
{
	/*
		event_watcher watch zookeeper,and notify to others 
	*/

	struct start_t
	{

	};
	
	struct stop_t
	{

	};

	struct msg_queue_change_t
	{
		std::vector<uint32_t> queue_id_;
	};

	struct event_callback_t
	{
		virtual void on_event (start_t &start) = 0;
		virtual void on_event (stop_t &stop) = 0;
		virtual void on_event (msg_queue_change_t &msg_queue_change) = 0;
	};

	class event_watcher_t:zk_client_t::zk_callback_t
	{
	public:
		event_watcher_t ();
		
		void watch (event_callback_t *callback);
		
		void del (event_callback_t *callback);

		//
		void init (const std::string &zookeep_host);
		
		//eg:/service/ychat/instance-
		void set_chat_path_prefix (const std::string &path);
		
		//ip:port
		void set_chat_service_addr (const std::string &addr);
	private:
		void handle_queue_slots (const std::string &buffer);

		int recursive_create (const std::string& path,
							  const std::string& value,
							  std::string real_path,
							  int flag);

		virtual void on_connected () override;

		virtual void on_connecting () override;

		virtual void on_auth_failed () override;

		virtual void on_associating () override;

		virtual void on_session_expired () override;

		virtual void on_created (const char* path) override;

		virtual void on_deleted (const char* path) override;

		virtual void on_changed (const char* path) override;

		virtual void on_child_changed (const char* path) override;

		virtual void on_not_watching (const char* path) override;

		typedef std::set<event_callback_t *> callback_set_t;
		typedef callback_set_t::iterator callback_set_iter_t;

		callback_set_t callbacks_;

		class zk_client_t *zk_client_;

		std::string ychat_path;
		std::string ychat_path_prefix_;
		std::string addr_;
		std::vector<uint32_t> queue_slots_;
	};
}