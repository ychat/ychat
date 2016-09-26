#include "stdafx.h"
#include "dispatcher_t.h"
#include "msg.hpp"
#include "json_helper_t.h"
#include "outstream_pool_mgr_t.h"
#include "outstream.h"

namespace ychat
{

	dispatcher_t::dispatcher_t (uint32_t msg_queue_id)
		:msg_queue_id_(msg_queue_id)
	{

	}
	
	dispatcher_t::~dispatcher_t ()
	{
		unint_mongodb ();

	}


	void dispatcher_t::init ()
	{
		init_mongodb ();
	}

	uint32_t dispatcher_t::get_msg_queue_id ()
	{
		return msg_queue_id_;
	}

	void dispatcher_t::run (void)
	{
		//this block can't not block the fiber forever
		acl::string queue_slot_id;
		queue_slot_id.format_append("/ychat/msg_queue/%u",msg_queue_id_);
		do 
		{

			acl::redis cmd (redis_cluster_);
			acl::string data;
			int ret = cmd.lindex (queue_slot_id.c_str(), 0, data);
			if(ret == false){
				//fiber sleep don't block current thread.
				acl_fiber_sleep (1);
				if (is_stop_)
					break;
				continue;
			}
			msg_t *msg = json_helper_t::to_msg(data);
			if (msg == NULL)
			{
				logger_warn ("to_msg error,data%s",data.c_str());
				continue;
			}

			if (push_to_mongodb (msg) == false)
			{
				logger_error ("push_to_mongodb error,data:%s",data.c_str());
				delete msg;
				break;
			}

			if (push_to_outstream (data, msg->to_.c_str()) == false)
			{
				delete msg;
				continue;
			}
			delete msg;

		} while (is_stop_ == false);
		char _;
		wait_for_stop_ << _;
	}

	void dispatcher_t::stop ()
	{
		is_stop_ = true;
		char _;
		wait_for_stop_.pop (_);
	}

	void dispatcher_t::set_redis_cluster (acl::redis_client_cluster *cluster)
	{
		redis_cluster_ = cluster;
	}

	void dispatcher_t::set_mongodb_addr (const std::string &addr)
	{
		mongodb_addr_ = addr;
	}

	void dispatcher_t::init_mongodb ()
	{
		
	}

	void dispatcher_t::unint_mongodb ()
	{

	}

	void dispatcher_t::set_outstream_pool_mgr (outstream_pool_mgr_t *mgr)
	{
		outstream_pool_mgr_ = mgr;
	}

	bool dispatcher_t::push_to_mongodb (msg_t * msg)
	{
		add_friend_t *add_friend = (add_friend_t*)msg;
		return true;
	}

	bool dispatcher_t::push_to_outstream (acl::string & data,
										  const char *client_id )
	{
		acl::string location;
		acl::redis cmd;
		if (cmd.hget ("/ychat/client/location/",
			client_id,
			location) == false)
		{
			logger_warn ("redis hget /ycaht/client/location/%s error", 
						 client_id);
			false;
		}

		acl::connect_pool *outstream_pool =
			outstream_pool_mgr_->get (location.c_str ());
		if (outstream_pool == NULL)
		{
			logger_warn ("get %s outstream_pool error,", location.c_str ());
			false;
		}

		outstream *out = (outstream*)outstream_pool->peek ();
		if (out == NULL)
		{
			logger_warn ("peek outstream error");
			false;
		}

		int nbytes = out->send (data.c_str (), data.size ());
		if (nbytes != data.size ())
		{
			logger_warn ("outstream send error, ret = %d", nbytes);
			return false;
		}
		return true;
	}

}
