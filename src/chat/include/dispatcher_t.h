#pragma once
namespace ychat
{
	//peek msg from redis queue,and dispatch to connector.
	class dispatcher_t : public acl::fiber
	{
	public:
		dispatcher_t (uint32_t msg_queue_id);
		
		~dispatcher_t ();
		
		uint32_t get_msg_queue_id ();

		void set_redis_cluster (acl::redis_client_cluster *cluster);
		//
		void set_mongodb_addr (const std::string &addr);
		//stop the fiber.this function will yield current fiber.
		void stop (bool );
	private:
		void init ();
		virtual void run (void);
		void init_mongodb ();
		void unint_mongodb ();
		uint32_t msg_queue_id_;
		bool is_stop_;
		acl::channel<char> wait_for_stop_;
		std::string mongodb_addr_;
		acl::redis_client_cluster *redis_cluster_;
	};
}