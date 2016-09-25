#include "stdafx.h"
#include "client_session_t.h"
#include "auth_t.h"

namespace ychat
{

	client_session_t::client_session_t (acl::socket_stream& conn)
		:conn_ (conn),
		is_auth_ (false),
		is_stop_(false)
	{

	}

	void client_session_t::set_redis_cluster(acl::redis_client_cluster
											 *redis_cluster)
	{
		redis_cluster_ = redis_cluster;
	}

	void client_session_t::run()
	{
		if (check_auth () == false)
			return ;
		do 
		{
			char type = 0;
			int len = 0;
			acl::string buffer;
			if (conn_.read(type) == false)
			{
				logger_warn("read char error");
				break;
			}
			if(conn_.read(len) == false) 
			{
				logger_warn("read int error");
				break;
			}
			len = ntohl(len);
			if(conn_.read(buffer, len, true) == false) 
			{
				logger_warn("read %d error", len);
				break;
			}
			if(handle_msg(type, len, buffer.c_str()) == false) {
				logger("handle msg error, type:%d,msg_len%d,buffer:%s",
					   type, len, buffer);
				break;
			}

		} while (is_stop_ == false);
	}

	bool client_session_t::check_auth ()
	{
		int msg_len = 0;
		acl::string buffer;
		if (conn_.read (msg_len) == false)
			return false;
		msg_len = ntohl(msg_len);

		if (conn_.read (buffer, msg_len, true) == false)
			return false;
		//first msg must be auth msg
		if (auth_t ().check (buffer) == false)
			return false;
	}

	void client_session_t::stop()
	{
		is_stop_ = true;
	}

	bool client_session_t::handle_msg(char type,
									  int len, 
									  const char *data)
	{

	}

}

