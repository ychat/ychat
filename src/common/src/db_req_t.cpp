#include "db_req_t.h"
#include "acl_cpp/lib_acl.hpp"
#include "lib_acl.h"

namespace ychat
{
namespace db
{
	ping_t::ping_t ()
	{
		type_ = e_ping;
	}

	acl::string ping_t::to_json ()
	{
		acl::json json;
		json.get_root ().
			add_child (json.create_node ("req_id_", req_id_)).
			add_child (json.create_node ("type_", type_to_str (type_)));
		return json.to_string ();
	}

	auth_req_t::auth_req_t ()
	{
		type_ = e_auth_req;
	}

	acl::string auth_req_t::to_json ()
	{
		acl::json json;
		json.get_root ().
			add_child (json.create_node ("req_id_", req_id_)).
			add_child (json.create_node ("type_", type_to_str (type_))).
			add_child (json.create_node ("username_",username_.c_str ())).
			add_child (json.create_node ("password_", password_.c_str ()));

		return json.to_string ();
	}
	acl::string req_add_friend_req_t::to_json ()
	{
		acl::json json;
		json.get_root ().
			add_child (json.create_node ("req_id_", req_id_)).
			add_child (json.create_node ("type_", type_to_str (type_))).
			add_child (json.create_node ("user_id_", user_id_.c_str ())).
			add_child (json.create_node ("from_", from_.c_str ())).
			add_child (json.create_node ("to_", to_.c_str ())).
			add_child (json.create_node ("text_msg_", text_msg_.c_str ()));
		return json.to_string ();
	}


	add_friend_req_t::add_friend_req_t ()
	{
		type_ = e_add_friend_req;
	}

	acl::string add_friend_req_t::to_json ()
	{
		acl::json json;
		json.get_root ().
			add_child (json.create_node ("req_id_", req_id_)).
			add_child (json.create_node ("type_", type_to_str (type_))).
			add_child (json.create_node ("user_id_", user_id_.c_str ())).
			add_child (json.create_node ("friend_id_", friend_id_.c_str ())).
			add_child (json.create_node ("laber_", laber_.c_str ()));

		return json.to_string ();
	}

	update_add_friend_result_req_t::update_add_friend_result_req_t ()
	{
		type_ = e_update_add_friend_result_req;
	}

	acl::string update_add_friend_result_req_t::to_json ()
	{
		acl::json json;
		json.get_root ().
			add_child (json.create_node ("req_id_", req_id_)).
			add_child (json.create_node ("type_", type_to_str (type_))).
			add_child (json.create_node ("add_friend_req_id_", add_friend_req_id_)).
			add_child (json.create_node ("result_", result_)).
			add_child (json.create_node ("user_id_", user_id_.c_str ()));

		return json.to_string ();
	}

	del_friend_req_t::del_friend_req_t ()
	{
		type_ = e_del_friend_req;
	}

	acl::string del_friend_req_t::to_json ()
	{
		acl::json json;
		json.get_root ().
			add_child (json.create_node ("req_id_", req_id_)).
			add_child (json.create_node ("type_", type_to_str (type_))).
			add_child (json.create_node ("user_id_", user_id_.c_str ())).
			add_child (json.create_node ("friend_id_", friend_id_.c_str()));

		return json.to_string ();
	}

	update_friend_laber_req_t::update_friend_laber_req_t ()
	{
		type_ = e_update_friend_laber_req;
	}

	acl::string update_friend_laber_req_t::to_json ()
	{
		acl::json json;
		json.get_root ().
			add_child (json.create_node ("req_id_", req_id_)).
			add_child (json.create_node ("type_", type_to_str (type_))).
			add_child (json.create_node ("user_id_", user_id_.c_str ())).
			add_child (json.create_node ("friend_id_", friend_id_.c_str ())).
		add_child (json.create_node ("label_", label_.c_str ()));

		return json.to_string ();
	}

	update_join_group_result_req_t::update_join_group_result_req_t ()
	{
		type_ = e_update_join_group_result_req;
	}

	acl::string update_join_group_result_req_t::to_json ()
	{
		acl::json json;
		json.get_root ().
			add_child (json.create_node ("req_id_", req_id_)).
			add_child (json.create_node ("type_", type_to_str (type_))).
			add_child (json.create_node ("user_id_", user_id_.c_str ())).
			add_child (json.create_node ("result_", result_)).
			add_child (json.create_node ("join_group_req_id_",
					   join_group_req_id_));

		return json.to_string ();
	}

	req_join_group_req_t::req_join_group_req_t()
	{
		type_ = e_req_join_group_req;
	}

	acl::string req_join_group_req_t::to_json()
	{
		acl::json json;
		json.get_root().
			add_child(json.create_node("req_id_", req_id_)).
			add_child(json.create_node("type_", type_to_str(type_))).
			add_child(json.create_node("user_id_", user_id_.c_str())).
			add_child(json.create_node("group_id_", group_id_.c_str())).
			add_child(json.create_node("from_", from_.c_str())).
			add_child(json.create_node("to_", to_.c_str())).
			add_child(json.create_node("text_msg_",text_msg_.c_str()));

		return json.to_string();
	}



	group_add_user_req_t::group_add_user_req_t ()
	{
		type_ = e_group_add_user_req;
	}

	acl::string group_add_user_req_t::to_json ()
	{
		acl::json json;
		json.get_root ().
			add_child (json.create_node ("req_id_", req_id_)).
			add_child (json.create_node ("type_", type_to_str (type_))).
			add_child (json.create_node ("user_id_", user_id_.c_str ())).
			add_child (json.create_node ("group_id_", group_id_.c_str ())).
			add_child (json.create_node ("laber_", laber_.c_str ()));

		return json.to_string ();
	}

	group_del_user_req_t::group_del_user_req_t ()
	{
		type_ = e_group_del_user_req;
	}
	acl::string group_del_user_req_t::to_json ()
	{
		acl::json json;
		json.get_root ().
			add_child (json.create_node ("req_id_", req_id_)).
			add_child (json.create_node ("type_", type_to_str (type_))).
			add_child (json.create_node ("user_id_", user_id_.c_str ())).
			add_child (json.create_node ("group_id_", group_id_.c_str ()));

		return json.to_string ();
	}

	const char * status_to_str (status_t status)
	{
		switch (status)
		{
			case ychat::db::e_ok:
				return "ok";
			case ychat::db::e_failed:
				return "failed";
			case ychat::db::e_user_no_exist:
				return "user_no_exist";
			default:
				acl_assert (false);
				break;
		}
		return "undefined";
	}

	

	pong_t::pong_t (int64_t req_id, bool result)
	{
		type_ = e_pong;
		req_id_ = req_id;
		status_ = result ? e_ok : e_failed;
	}

	acl::string resp_t::to_json ()
	{
		acl::string json;
		json.format ("{"
					 "\"req_id\":%lld,"
					 "\"type\":\"%s\","
					 "\"status\":\"%s\""
					 "}",
					 type_to_str (type_),
					 req_id_,
					 status_to_str (status_));
		return json;
	}


	auth_resp_t::auth_resp_t (int64_t req_id, bool result)
	{
		type_ = e_auth_resp;
		req_id_ = req_id;
		status_ = result?e_ok : e_failed;
	}

	req_add_friend_resp_t::req_add_friend_resp_t (
		int64_t req_id, bool result)
	{
		req_id_ = req_id;
		type_ = e_req_add_friend_resp;
		status_ = result?e_ok:e_failed;
	}
	req_join_group_resp_t::req_join_group_resp_t(int64_t req_id, bool result)
	{
		req_id_ = req_id;
		type_ = e_req_join_group_resp_t;
		status_ = result ? e_ok : e_failed;
	}


	add_friend_resp_t::add_friend_resp_t (int64_t req_id, bool result)
	{
		req_id_ = req_id;
		type_ = e_add_friend_resp;
		status_ = result?e_ok:e_failed;
	}

	set_add_friend_result_resp_t::set_add_friend_result_resp_t 
		(int64_t req_id, bool result)
	{
		req_id_ = req_id;
		type_ = e_update_add_friend_result_resp;
		status_ = result?e_ok :e_failed;
	}

	del_friend_resp_t::del_friend_resp_t (
		int64_t req_id, bool result)
	{
		req_id_ = req_id;
		type_ = e_del_friend_resp;
		status_ = result?e_ok:e_failed;

	}

	update_friend_laber_resp_t::update_friend_laber_resp_t 
		(int64_t req_id, bool result)
	{
		req_id_ = req_id;
		type_ = e_update_friend_laber_resp;
		status_ = result?e_ok: e_failed;
	}

	update_join_group_result_resp_t::update_join_group_result_resp_t (
		int64_t req_id, bool result)
	{
		req_id_ = req_id;
		type_ = e_update_add_friend_result_req;
		status_ = result ? e_ok:e_failed;
	}
		

	group_add_user_resp_t::group_add_user_resp_t (int64_t req_id, bool result)
	{
		req_id_ = req_id;
		type_ = e_update_add_friend_result_req;
		status_ = result ? e_ok : e_failed;
	}

	group_del_user_resp_t::group_del_user_resp_t (int64_t req_id, bool result)
	{
		req_id_ = req_id;
		type_ = e_update_add_friend_result_req;
		status_ = result ? e_ok : e_failed;
	}


	const char * type_to_str (type_t type)
	{
		switch (type)
		{
			case ychat::db::e_auth_req:
				return "auth_req";
			case ychat::db::e_auth_resp:
				return "auth_resp";
			case ychat::db::e_ping:
				return "ping";
			case ychat::db::e_pong:
				return "pong";
			case ychat::db::e_req_add_friend_req:
				return "req_add_friend_req";
			case ychat::db::e_req_add_friend_resp:
				return "req_add_friend_resp";
			case ychat::db::e_add_friend_req:
				return "add_friend_req";
			case ychat::db::e_add_friend_resp:
				return "add_friend_resp";
			case ychat::db::e_update_add_friend_result_req:
				return "update_add_friend_result_req";
			case ychat::db::e_update_add_friend_result_resp:
				return "update_add_friend_result_resp";
			case ychat::db::e_del_friend_req:
				return "del_friend_req";
			case ychat::db::e_del_friend_resp:
				return "del_friend_resp";
			case ychat::db::e_update_friend_laber_req:
				return "update_friend_laber_req";
			case ychat::db::e_update_friend_laber_resp:
				return "update_friend_laber_resp";
			case ychat::db::e_update_join_group_result_req:
				return "update_join_group_result_req";
			case ychat::db::e_update_join_group_result_resp:
				return "update_join_group_result_resp";
			case ychat::db::e_group_add_user_req:
				return "group_add_user_req";
			case ychat::db::e_group_add_user_resp:
				return "group_add_user_resp";
			case ychat::db::e_group_del_user_req:
				return "group_del_user_req";
			case ychat::db::e_group_del_user_resp:
				return "group_del_user_resp";
		}
		return "error_type";
	}

	bool to_json (const ping_t *req, acl::string &buffer)
	{
		
		return true;
	}
	bool to_json (const req_t *_req, acl::string &buffer)
	{
		type_t type = _req->type_;
		switch (type)
		{
			case ychat::db::e_ping:
				{
					ping_t *ping = (ping_t*)_req;
					return to_json (ping, buffer);
				}
				break;
			case ychat::db::e_auth_req:
				{
					auth_req_t *req = (auth_req_t *)_req;
					json.get_root ()
						.add_child (
							json.create_node ("req_id_", req->req_id_)).
						add_child (
							json.create_node ("type_", type_to_str (e_auth_req))).
						add_child(
							json.create_node("service_name_",
							req->username_.c_str())).
						add_child (
							json.create_node ("password_", 
							req->password_.c_str ()));

				}
				break;
			case ychat::db::e_auth_resp:
				break;
			case ychat::db::e_req_add_friend_req:
				break;
			case ychat::db::e_req_add_friend_resp:
				break;
			case ychat::db::e_add_friend_req:
				break;
			case ychat::db::e_add_friend_resp:
				break;
			case ychat::db::e_update_add_friend_result_req:
				break;
			case ychat::db::e_update_add_friend_result_resp:
				break;
			case ychat::db::e_del_friend_req:
				break;
			case ychat::db::e_del_friend_resp:
				break;
			case ychat::db::e_update_friend_laber_req:
				break;
			case ychat::db::e_update_friend_laber_resp:
				break;
			case ychat::db::e_update_join_group_result_req:
				break;
			case ychat::db::e_update_join_group_result_resp:
				break;
			case ychat::db::e_group_add_user_req:
				break;
			case ychat::db::e_group_add_user_resp:
				break;
			case ychat::db::e_group_del_user_req:
				break;
			case ychat::db::e_group_del_user_resp:
				break;
			default:
				break;
		}
	}

	bool get_json_node (acl::json &json,
						const char *node_name,
						std::string &value)
	{
		acl::json_node* node =
			json.getFirstElementByTagName (node_name);
		if (node == NULL)
			return false;
		if (node->get_string () == NULL)
			return false;
		value = node->get_string ();
		return true;
	}
	bool get_json_node (acl::json &json,
						const char *node_name,
						int64_t &value)
	{
		acl::json_node* node =
			json.getFirstElementByTagName (node_name);
		if (node == NULL)
			return false;
		if (node->get_int64 () == NULL)
			return false;
		value = *node->get_int64 ();
		return true;
	}
	bool get_json_node (acl::json &json,
						const char *node_name,
						bool &value)
	{
		acl::json_node* node =
			json.getFirstElementByTagName (node_name);
		if (node == NULL)
			return false;
		if (node->get_bool() == NULL)
			return false;
		value = *node->get_bool ();
		return true;
	}
	req_t * to_req (const acl::string &data)
	{
		acl::json json;
		json.update (data.c_str ());
		std::string type;
		int64_t req_id;

		if (get_json_node (json, "type", type) == false)
			return NULL;
		if (get_json_node (json, "req_id", req_id) == false)
			return NULL;

		if (type == type_to_str (e_ping))
		{
			ping_t *req = new ping_t();
			req->req_id_ = req_id;
			return req;
		}
		else if (type == type_to_str (e_auth_req))
		{
			auth_req_t *req = new auth_req_t();
			get_json_node (json, "req_id_", req->req_id_);
			get_json_node (json, "username_", req->username_);
			get_json_node (json, "password_", req->password_);
			return req;
		}
		else if (type == type_to_str (e_req_add_friend_req))
		{
			req_add_friend_req_t *req = new req_add_friend_req_t;
			get_json_node (json, "req_id_", req->req_id_);
			get_json_node (json, "from_", req->from_);
			get_json_node (json, "text_msg_", req->text_msg_);
			get_json_node (json, "to_", req->to_);
			get_json_node (json, "user_id_", req->user_id_);
			return req;
		}
		else if (type == type_to_str (e_add_friend_req))
		{
			add_friend_req_t *req = new add_friend_req_t;
			get_json_node (json, "req_id_", req->req_id_);
			get_json_node (json, "friend_id_", req->friend_id_);
			get_json_node (json, "laber_", req->laber_);
			get_json_node (json, "user_id_", req->user_id_);
			return req;
		}
		else if (type == type_to_str (e_update_add_friend_result_req))
		{
			update_add_friend_result_req_t *req = 
				new update_add_friend_result_req_t;
			get_json_node (json, "req_id_", req->req_id_);
			get_json_node (json, "add_friend_req_id_", req->add_friend_req_id_);
			get_json_node (json, "result_", req->result_);
			get_json_node (json, "user_id_", req->user_id_);
			return req;
		}
		else if (type == type_to_str (e_del_friend_req))
		{
			del_friend_req_t *req = new del_friend_req_t;
			get_json_node (json, "req_id_", req->req_id_);
			get_json_node (json, "friend_id_", req->friend_id_);
			get_json_node (json, "user_id_", req->user_id_);
			return req;
		}
		else if (type == type_to_str (e_update_friend_laber_req))
		{
			update_friend_laber_req_t *req = new update_friend_laber_req_t;
			get_json_node (json, "req_id_", req->req_id_);
			get_json_node (json, "friend_id_", req->friend_id_);
			get_json_node (json, "label_", req->label_);
			get_json_node (json, "user_id_", req->user_id_);
			return req;
		}
		else if (type == type_to_str (e_update_join_group_result_req))
		{
			update_join_group_result_req_t *req 
				= new update_join_group_result_req_t;

			get_json_node (json, "req_id_", req->req_id_);
			get_json_node (json, "join_group_req_id_", req->join_group_req_id_);
			get_json_node (json, "result_", req->result_);
			get_json_node (json, "user_id_", req->user_id_);
			return req;
		}
		else if (type == type_to_str(e_req_join_group_req))
		{
			req_join_group_req_t *req = new req_join_group_req_t;
			get_json_node(json, "req_id_", req->req_id_);
			get_json_node(json, "group_id_", req->group_id_);
			get_json_node(json, "from_", req->from_);
			get_json_node(json, "to_", req->to_);
			get_json_node(json, "user_id_", req->user_id_);
			get_json_node(json, "text_msg_", req->text_msg_);
		}
		else if (type == type_to_str (e_group_add_user_req))
		{
			group_add_user_req_t *req = new group_add_user_req_t;
			get_json_node (json, "req_id_", req->req_id_);
			get_json_node (json, "group_id_", req->group_id_);
			get_json_node (json, "laber_", req->laber_);
			get_json_node (json, "user_id_", req->user_id_);
			return req;
		}
		else if (type == type_to_str (e_group_del_user_req))
		{
			group_del_user_req_t *req = new group_del_user_req_t;
			get_json_node (json, "req_id_", req->req_id_);
			get_json_node (json, "group_id_", req->group_id_);
			get_json_node (json, "user_id_", req->user_id_);
			return req;
		}
		return NULL;
	}

	resp_t * to_resp (const acl::string &data)
	{
		acl::json json;
		json.update (data.c_str ());
		std::string type;
		std::string status;
		int64_t req_id;
			
		if (get_json_node (json, "type", type) == false)
			return NULL;
		if (get_json_node (json, "req_id", req_id) == false)
			return NULL;
		if (get_json_node (json, "status", status) == false)
			return NULL;

		if (type == type_to_str (e_pong))
		{
			return new pong_t (req_id, status == "ok");
		}
		else if (type == type_to_str(e_auth_resp))
		{
			return new auth_resp_t (req_id, status == "ok");
		}
		else if (type == type_to_str (e_req_add_friend_resp))
		{
			return new req_add_friend_resp_t (req_id, status == "ok");
		}
		else if (type == type_to_str (e_add_friend_resp))
		{
			return new add_friend_resp_t (req_id, status == "ok");
		}
		else if (type == type_to_str (e_update_add_friend_result_resp))
		{
			return new set_add_friend_result_resp_t (req_id, status == "ok");
		}
		else if (type == type_to_str (e_del_friend_resp))
		{
			return new del_friend_resp_t (req_id, status == "ok");
		}
		else if (type == type_to_str (e_update_friend_laber_resp))
		{
			return new update_friend_laber_resp_t (req_id, status == "ok");
		}
		else if (type == type_to_str (e_update_join_group_result_resp))
		{
			return new update_join_group_result_resp_t (req_id, status == "ok");
		}
		else if (type == type_to_str (e_group_add_user_resp))
		{
			return new group_add_user_resp_t (req_id,status == "ok");
		}
		else if (type == type_to_str (e_group_del_user_resp))
		{
			return new group_del_user_resp_t (req_id,status == "ok");
		}
	}

	void free_req (req_t *req)
	{
		if (req)
		{
			delete req;
		}
	}

}
}