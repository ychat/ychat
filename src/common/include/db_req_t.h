#pragma once
#include <string>
#include <stdint.h>
#include <assert.h>
namespace acl
{
class string;
}
namespace ychat
{
namespace db
{
	enum type_t
	{
		e_ping,
		e_pong,

		e_auth_req,
		e_auth_resp,

		e_req_add_friend_req,
		e_req_add_friend_resp,

		e_add_friend_req,
		e_add_friend_resp,

		e_update_add_friend_result_req,
		e_update_add_friend_result_resp,

		e_del_friend_req,
		e_del_friend_resp,

		e_update_friend_laber_req,
		e_update_friend_laber_resp,

		e_update_join_group_result_req,
		e_update_join_group_result_resp,

		e_group_add_user_req,
		e_group_add_user_resp,

		e_group_del_user_req,
		e_group_del_user_resp,
	};
	enum status_t
	{
		e_ok,
		e_failed,
		e_user_no_exist,
	};
	const char *type_to_str (type_t type);
	const char *status_to_str (status_t status);
	struct req_t
	{
		virtual ~req_t(){}
		int64_t req_id_;
		type_t type_;
	};


	struct resp_t
	{
		
		virtual ~resp_t ()
		{}
		int64_t req_id_;
		type_t  type_;
		status_t status_;
		virtual acl::string to_json ();
		
	};
	struct ping_t :req_t
	{
		ping_t ();
		acl::string to_json ();
	};
	
	struct pong_t:resp_t
	{
		pong_t (int64_t req_id, bool result);
	};

	struct auth_req_t:req_t
	{
		auth_req_t ();
		std::string username_;
		std::string password_;
		acl::string to_json ();
	};

	struct req_add_friend_req_t:req_t
	{
		std::string user_id_;
		std::string from_;
		std::string to_;
		std::string text_msg_;
		acl::string to_json ();
	};
	struct add_friend_req_t : req_t
	{
		add_friend_req_t ();
		std::string user_id_;
		std::string friend_id_;
		std::string laber_;
		acl::string to_json ();
	};


	struct update_add_friend_result_req_t:req_t
	{
		update_add_friend_result_req_t ();
		int64_t add_friend_req_id_;
		bool result_;
		std::string user_id_;
		acl::string to_json ();
	};

	struct del_friend_req_t:req_t
	{
		del_friend_req_t ();
		std::string user_id_;
		std::string friend_id_;
		acl::string to_json ();
	};

	struct update_friend_laber_req_t:req_t
	{
		update_friend_laber_req_t ();
		std::string user_id_;
		std::string friend_id_;
		std::string label_;
		acl::string to_json ();

	};

	struct update_join_group_result_req_t:req_t
	{
		update_join_group_result_req_t ();
		bool result_;
		int64_t join_group_req_id_;
		std::string user_id_;
		acl::string to_json ();
	};

	struct group_add_user_req_t:req_t
	{
		group_add_user_req_t ();
		std::string group_id_;
		std::string user_id_;
		std::string laber_;
		acl::string to_json ();
	};

	struct group_del_user_req_t:req_t
	{
		group_del_user_req_t ();
		std::string group_id_;
		std::string user_id_;
		acl::string to_json ();
	};

	struct auth_resp_t:resp_t
	{
		auth_resp_t (int64_t req_id, bool result);
	};
	struct add_friend_resp_t:resp_t
	{
		add_friend_resp_t (int64_t req_id,bool result);
	};

	struct set_add_friend_result_resp_t :resp_t
	{
		set_add_friend_result_resp_t (int64_t req_id, bool result);
	};

	struct del_friend_resp_t:resp_t
	{
		del_friend_resp_t (int64_t req_id, bool result_);
	};

	struct update_friend_laber_resp_t:resp_t
	{
		update_friend_laber_resp_t (int64_t req_id, bool result);
	};
		
	struct update_join_group_result_resp_t:resp_t
	{
		update_join_group_result_resp_t (int64_t req_id, bool result);
	};
		
	struct group_add_user_resp_t:resp_t
	{
		group_add_user_resp_t (int64_t req_id, bool result);
	};

	struct group_del_user_resp_t:resp_t
	{
		group_del_user_resp_t (int64_t req_id, bool result);
	};

	struct req_add_friend_resp_t:resp_t
	{
		req_add_friend_resp_t (int64_t req_id, bool result);
	};

	req_t *to_req (const acl::string &json);

	resp_t *to_resp (const acl::string &json);

	void free_req (req_t *req);
}
	
}