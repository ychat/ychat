#include "stdafx.h"
namespace ychat
{

	class outstream_checker_t:public acl::aio_callback
	{
	public:
		outstream_checker_t (acl::check_client &checker)
			:checker_(checker)
		{

		}

		virtual void close_callback () override
		{
			delete this;
		}

		virtual bool timeout_callback () override
		{
			checker_.set_blocked (false);
			return false;
		}

		virtual bool read_callback (char* data, int len) override
		{
			// outstream server is not dead.
			checker_.set_alive (true);
			checker_.close ();
			return true;
		}
	private:
		acl::check_client &checker_;
	};
	class outstream_monitor_t :public acl::connect_monitor
	{
	public:
		outstream_monitor_t (acl::connect_manager& manager)
			:acl::connect_monitor(manager),
			is_run_(false)
		{

		}
		bool is_run ()
		{
			return is_run_;
		}
		void set_run (bool value)
		{
			is_run_ = value;
		}
		virtual void nio_check (acl::check_client& checker, 
								acl::aio_socket_stream& conn) 
		{
			//send ping to server.
			// 创建异步检测对象
			outstream_checker_t* outstream = new outstream_checker_t (checker);

			// 注册非阻塞 IO 处理过程的回调过程
			conn.add_close_callback (outstream);
			conn.add_read_callback (outstream);
			conn.add_timeout_callback (outstream);

			//read with timeout 10seconds
			conn.read (0, 10);
		}
	private:
		bool is_run_;
	};
}
