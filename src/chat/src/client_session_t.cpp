#include "stdafx.h"
#include "client_session_t.h"
#include "auth_t.h"

namespace ychat
{

	client_session_t::client_session_t (acl::socket_stream& conn)
		:conn_ (conn),
		is_auth_ (false)
	{

	}

	bool client_session_t::run ()
	{
		if (check_auth () == false)
			return false;
	
	}

	bool client_session_t::check_auth ()
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

