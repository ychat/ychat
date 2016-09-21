#include <string>

namespace ychat
{
	enum msg_type_t
	{
		/*one to one chat*/
		e_chat = 1,
		e_chat_ack,

		/*add friend*/
		e_add_friend,
		e_add_friend_result,
		e_add_friend_result_ack,

		/*del friend*/
		e_del_friend,
		e_del_friend_notify, //notify another that lost a friend.
		e_del_friend_notify_ack, //ack the del_friend_notify msg.

		/*make the friend a label,eg:best friend ,classmake....*/
		e_label_friend,

		/*group chat*/
		e_group_chat,
		e_group_chat_ack,

		/*join group*/
		e_join_group,
		e_join_group_result,
		
		e_group_add_member_notify,

		/*leave group*/
		e_leave_group,

		e_group_del_member_notify,

	};
	typedef uint64_t msg_id_t;
	typedef uint64_t msg_time_t;
	typedef std::string user_id_t;
	typedef std::string group_id_t;

	struct msg_t
	{
		msg_type_t msg_type_;
		msg_id_t msg_id_;//uuid 
		std::string source_id_;//who send the msg.
		std::string dst_id_;//destination id,if msg_type != e_gchat ,dst_id = client_id.
							//if msg_type = e_gchat,dst_id= group_id.
		msg_time_t time_;//msg receive time
	};

	struct chat_msg_t :msg_t
	{
		msg_time_t ack_time_;// client ack msg timepoint
		std::string data_;// msg data
	};

	struct chat_msg_ack_t :msg_t
	{
		msg_time_t ack_time_;
		msg_id_t dst_msg_id_;//dstination chat_msg'id.
	};

	struct add_friend :msg_t
	{
		user_id_t user_id_;
		std::string text_msg_;//text messgae to someone,eg:"hello ,I am A."
	};

	struct add_friend_result_t : msg_t
	{
		user_id_t user_id_;
		msg_id_t dst_msg_id_;//dst_msg_id_ eq add_friend' msg_id_
		bool result_;//true,for "yes",false for otherwise
		std::string text_msg_;
	};

	struct add_friend_result_ack_t : msg_t
	{
		msg_id_t dst_msg_id_;
		msg_time_t ack_time_;
	};

	struct del_friend :msg_t
	{
		user_id_t user_id_;
		msg_time_t time_;
		std::string result_;//'ok','no friendship'or'system error'
	};

	struct del_friend_notify_t : msg_t
	{
		user_id_t user_id_;//the id of the friend just lost
		msg_time_t time_;
	};

	struct del_friend_notify_ack_t : msg_t
	{
		msg_id_t dst_msg_id_;
		msg_time_t time_;
	};

	struct label_friend :msg_t
	{
		user_id_t user_id_;
		std::string label_;
		std::string result_;// 'ok','no friendship'or'system error'
	};

	struct group_chat :msg_t
	{
		group_id_t group_id_;
		msg_time_t send_time_;//
		msg_time_t ack_time_;//if ack_time == 0 then this msg is not readed
		std::string data_;
	};

	struct group_chat_ack :msg_t
	{
		msg_time_t ack_time_;
		msg_id_t dst_msg_id_;//dstination group_chat'msg_id_.
	};

	struct join_group: msg_t
	{
		group_id_t group_id_;
	};

	struct join_group_result :msg_t
	{
		group_id_t group_id_;
		std::string result_; //'ok','full','no find group','already joined','system error'
	};

	struct group_add_member_notify: msg_t
	{
		group_id_t group_id_;
		user_id_t member_id_;
		std::string username_;
		std::string user_icon_;
	};

	struct leave_group: msg_t
	{
		group_id_t group_id_;
	};

	struct group_del_member_notify: msg_t
	{
		group_id_t group_id_;
		user_id_t user_id_;
	};
}