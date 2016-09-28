#include "stdafx.h"
#include "client_auth_t.h"
#include "db_req_t.h"

namespace ychat
{

	client_auth_t::client_auth_t()
	{

	}

	bool client_auth_t::auth(db::auth_req_t *req)
	{
		return true;
	}

}