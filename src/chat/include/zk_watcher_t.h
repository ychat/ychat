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

	struct msg_queue_slots_change_t
	{
		std::vector<uint32_t> queue_id_;
	};

	struct config_init_t
	{
		std::string redis_addr_;
		std::string mongodb_addr_;
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
		virtual void on_event(const start_t &start) = 0;
		virtual void on_event(const stop_t &stop) = 0;
		virtual void on_event(const msg_queue_slots_change_t &change) = 0;
		virtual void on_event(const config_init_t &change) = 0;
		virtual void on_event(const outstream_info_update_t &update) = 0;
	};

	class zk_watcher_t : public zk_client_t::zk_callback_t
	{
	public:
		zk_watcher_t();

		void watch(event_callback_t *callback);

		void del(event_callback_t *callback);

		//eg:/service/ychat/instance-
		void set_chat_path_prefix(const std::string &path);

		void set_outstream_path (const std::string &path);

		//ip:port
		void  set_chat_service_addr(const std::string &addr);

		void set_redis_addr_path (const std::string &path);

		void set_mongodb_addr_path (const std::string &path);

		void set_zk_client(zk_client_t *zk);


	private:
		void get_config();

		void get_outstream_instance_info ();

		void handle_queue_slots(const std::string &buffer);

		int recursive_create(const std::string& path,
							 const std::string& value,
							 std::string real_path,
							 int flag);

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

		void update_queue_slots_data ();
		
		typedef std::set<event_callback_t *> callback_set_t;
		typedef callback_set_t::iterator callback_set_iter_t;

		callback_set_t callbacks_;

		class zk_client_t *zk_client_;
		//chat servcie path
		std::string chat_path;
		//chat service path prefix
		std::string chat_path_prefix_;
		
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