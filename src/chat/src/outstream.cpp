#include "stdafx.h"
#include "outstream.h"
namespace ychat
{
	outstream::outstream(const std::string &addr)
		:addr_(addr)
	{

	}

	outstream::~outstream()
	{

	}

	bool outstream::open()
	{
		return stream_.open(addr_.c_str(), conn_timeout_, rw_timeout_);
	}

	int outstream::send(void *data, int len)
	{
		return stream_.write(data, len);
	}

	void outstream::set_id(const std::string &id)
	{
		id_ = id;
	}

	std::string outstream::get_id()
	{
		return id_;
	}

}