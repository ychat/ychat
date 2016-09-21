#pragma once
namespace ychat
{
	class auth_t
	{
	public:
		auth_t ()
		{

		}
		bool check (acl::string &buffer);
	};
}