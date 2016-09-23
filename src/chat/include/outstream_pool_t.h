#pragma once
namespace ychat
{
	class outstream_pool_t :
		public acl::connect_pool
	{
	public:
		outstream_pool_t(const char* addr, size_t max = 0);

		virtual acl::connect_client* create_connect();

		//why not std::shared_ptr ?. 
		void dec_ref ()
		{
			--ref_;
			if (ref_ <= 0)
			{
				delete this;
			}
		}
		void inc_ref ()
		{
			++ref_;
		}
	private:
		int ref_;
	};
}