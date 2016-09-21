#pragma once
namespace ychat
{
	//peek msg from redis queue,and dispatch to connector.
	class dispatcher_t
	{
	public:
		dispatcher_t (uint32_t msg_queue_id);
		void do_run ();
		void stop ();
		uint32_t get_msg_queue_id ();
	private:
		void init ();
		bool is_stop_;
		uint32_t msg_queue_id_;
	};
}