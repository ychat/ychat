#include "stdafx.h"
#include "db_proxy_monitor_t.h"
namespace ychat
{
	void db_proxy_monitor_t::pingpong_t::close_callback ()
	{
		delete this;
	}

	bool db_proxy_monitor_t::pingpong_t::timeout_callback ()
	{
		checker_->set_alive (false);
		return false;
	}

	bool db_proxy_monitor_t::pingpong_t::read_callback (char* data, int len)
	{
		checker_->set_alive (true);
		return false;
	}

	void db_proxy_monitor_t::pingpong_t::ping ()
	{

	}

	void db_proxy_monitor_t::nio_check (acl::check_client& checker,
										acl::aio_socket_stream& conn)
	{
		pingpong_t *pingpong = new pingpong_t;
		pingpong->checker_ = &checker;
		pingpong->conn_ = &conn;

		conn.add_close_callback (pingpong);
		conn.add_read_callback (pingpong);
		conn.add_timeout_callback (pingpong);
		pingpong->ping ();
	}
}