#pragma once
namespace ychat
{
	class msg_t;
	class json_helper
	{
	public:
		static bool to_msg (const char *data, std::vector<int> &object);
		static msg_t *to_msg (const char *data);
	};
}