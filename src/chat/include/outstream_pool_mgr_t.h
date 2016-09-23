#pragma once
namespace ychat
{
	class outstream_pool_t;
	class outstream_pool_mgr_t
	{
	public:
		outstream_pool_mgr_t();
		~outstream_pool_mgr_t();
		outstream_pool_t *get(const std::string &addr_);
		void del (const std::string &addr);
		void add_addr (const std::string &addr);
	private:
		typedef std::map<std::string, outstream_pool_t*> outstream_pools_t;
		typedef outstream_pools_t::iterator outstream_pools_itr_t;
		outstream_pools_t outstream_pools_;
		std::set<std::string > outstream_addrs_;
	};
}