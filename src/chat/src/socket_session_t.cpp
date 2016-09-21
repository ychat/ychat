#include "stdafx.h"
#include "socket_session_t.h"
#include "auth_t.h"

namespace ychat
{

	socket_session_t::socket_session_t (acl::socket_stream& conn)
		:conn_ (conn),
		is_auth_ (false)
	{

	}

	bool socket_session_t::doRun ()
	{
		int msg_len = 0;
		acl::string buffer;
		if (conn_.read (msg_len) == false)
			return false;
		if (conn_.read (buffer, msg_len, true) == false)
			return false;
		//first msg must be auth msg
		if (auth_t ().check (buffer) == false)
			return false;
	}

}

