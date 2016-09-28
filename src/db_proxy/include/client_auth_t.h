#pragma once
namespace ychat
{
	namespace db
	{
		struct auth_req_t;
	}
	class client_auth_t
	{
	public:
		client_auth_t();
		bool auth(db::auth_req_t *req);

	};
}