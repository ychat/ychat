#pragma once
namespace ychat
{
	class outstream_pool_t :
		public acl::connect_pool
	{
	public:
		outstream_pool_t(const char* addr, size_t max = 0);
		virtual acl::connect_client* create_connect();
	};
}