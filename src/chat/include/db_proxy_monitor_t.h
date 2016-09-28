#pragma once
namespace ychat
{
	class db_proxy_monitor_t:public acl::connect_monitor
	{

		struct pingpong_t:acl::aio_callback
		{
			virtual void close_callback ();

			virtual bool timeout_callback ();

			virtual bool read_callback (char* data, int len);

			void ping ();
			acl::check_client* checker_;
			acl::aio_socket_stream* conn_;
		};

		virtual void nio_check (acl::check_client& checker,
								acl::aio_socket_stream& conn);

	};
}