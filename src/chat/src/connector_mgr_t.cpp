#include "stdafx.h"
#include "connector_mgr_t.h"

namespace ychat
{

	connector_mgr_t& connector_mgr_t::get_instance ()
	{
		static connector_mgr_t inst;
		return inst;
	}

}

