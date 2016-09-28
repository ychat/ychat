#pragma once
namespace ychat
{
	class db_proxy_pool_manager_t:public acl::connect_manager
	{
	public:
		db_proxy_pool_manager_t ()
		{}

		virtual acl::connect_pool* create_pool (const char* addr,
												size_t count,
												size_t idx);
	};
}