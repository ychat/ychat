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
	struct zk_status_update_t 
	{
		enum status_t
		{
			e_connecting,
			e_connected,
			e_associating,
			e_session_expired,
		};
		status_t status_;

	};
	struct msg_queue_slots_change_t
	{
		std::vector<uint32_t> queue_id_;
	};

	struct outstream_info_t
	{
		//ip:port
		std::string addr_;
		//status up/down. true for up,false for down
		enum status_t
		{
			e_up,
			e_down,
		};
		status_t status_;
	};
	struct outstream_info_update_t
	{
		std::map<std::string ,outstream_info_t> infos_;
	};
	struct event_callback_t
	{
		virtual void on_event (const start_t &start)
		{

		}
		virtual void on_event (const stop_t &stop)
		{

		}
		virtual void on_event (const msg_queue_slots_change_t &change)
		{

		}
		virtual void on_event (const outstream_info_update_t &update)
		{

		}
		virtual void on_event (const zk_status_update_t &status)
		{

		}
	};

	class zk_watcher_t : public zk_client_t::zk_callback_t
	{
	public:
		zk_watcher_t();

		void watch(event_callback_t *callback);

		void del(event_callback_t *callback);

		//eg:/service/ychat/instance-
		void set_chat_service_path(const std::string &path);

		void set_outstream_path (const std::string &path);

		void set_zk_client(zk_client_t *zk);

		void do_watch ();
	private:

		void update_outstream_instance_info ();

		void handle_queue_slots_update(const std::string &buffer);

		virtual void on_connected() override;

		virtual void on_connecting() override;

		virtual void on_auth_failed() override;

		virtual void on_associating() override;

		virtual void on_session_expired() override;

		virtual void on_created(const char* path) override;

		virtual void on_deleted(const char* path) override;

		virtual void on_changed(const char* path) override;

		virtual void on_child_changed(const char* path) override;

		virtual void on_not_watching(const char* path) override;

		void update_queue_slots ();
		
		typedef std::set<event_callback_t *> callback_set_t;
		typedef callback_set_t::iterator callback_set_iter_t;

		callback_set_t callbacks_;

		class zk_client_t *zk_client_;
		//chat servcie path
		std::string chat_path;
		//chat service path prefix
		std::string chat_service_path_;
		
		//session layer 
		std::string outstream_path_;

		//ychat_path+"/queue_slots"
		std::string queue_slot_path_;
		std::string last_queue_slot_data_;
		std::string addr_;
		std::string redis_addr_path_;
		std::string mongodb_addr_path_;
	};
}