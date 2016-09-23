#pragma once
namespace ychat
{
	class json_helper
	{
	public:
		bool to_object (const char *data, std::vector<int> &object);
	};
}