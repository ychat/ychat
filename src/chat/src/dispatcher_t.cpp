#include "stdafx.h"
#include "dispatcher_t.h"

namespace ychat
{

	dispatcher_t::dispatcher_t (uint32_t msg_queue_id)
		:msg_queue_id_(msg_queue_id)
	{

	}

	void dispatcher_t::do_run ()
	{

	}

	void dispatcher_t::stop ()
	{
		is_stop_ = true;
	}

	void dispatcher_t::init ()
	{

	}

	uint32_t dispatcher_t::get_msg_queue_id ()
	{
		return msg_queue_id_;
	}

}

