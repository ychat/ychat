#pragma once
namespace ychat
{
	class msg_t;
	class json_helper_t
	{
	public:
		static msg_t *to_msg (const char *data);
		static bool to_json (const msg_t *msg, std::string &json);
	};
}