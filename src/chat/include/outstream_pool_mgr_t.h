#pragma once
namespace ychat
{
	class outstream_pool_t;
	class acl::connect_pool;
	class outstream_pool_mgr_t :public acl::connect_manager
	{
	public:
		outstream_pool_mgr_t();
		~outstream_pool_mgr_t();
		virtual acl::connect_pool* create_pool (const char* addr, 
												size_t count, size_t idx) ;
	};
}