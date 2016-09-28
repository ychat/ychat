#pragma once
namespace ychat
{
	class db_proxy_client_pool_t:public acl::connect_pool
	{

	public:
		/*
		addr formart ip:port(domain:port)
		max  max connections ,0 for unlimited£¬
		idx index of pool.default 0
		*/

		db_proxy_client_pool_t (const char* addr, size_t max, size_t idx = 0);

		virtual acl::connect_client* create_connect ();
	};
}