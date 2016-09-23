#pragma once
#include <zookeeper.h>
namespace ychat
{


	class zk_client_t
	{
	public:
		class zk_callback_t
		{
		public:
			virtual ~zk_callback_t ()
			{}

			virtual void on_connected ()
			{}
			virtual void on_connecting ()
			{}
			virtual void on_auth_failed ()
			{}
			virtual void on_associating ()
			{}
			virtual void on_session_expired ()
			{}
			virtual void on_created (const char* path)
			{}
			virtual void on_deleted (const char* path)
			{}
			virtual void on_changed (const char* path)
			{}
			virtual void on_child_changed (const char* path)
			{}
			virtual void on_not_watching (const char* path)
			{}
		};

		zk_client_t ();
		~zk_client_t ();

		//param host comma separated host:port pairs, each corresponding to a zk
		//*   server.e.g. "127.0.0.1:3000,127.0.0.1:3001,127.0.0.1:3002"
		bool init (const std::string &hosts, int recv_timeout,
				   zk_callback_t *callback);

		//param flags this parameter can be set to 0 for normal create or 
		//an OR ZOO_EPHEMERAL, ZOO_SEQUENCE
		//of the Create Flags
		int create (const std::string &path, const std::string &value,
					std::string &real_path, int flag);

		int exists (const std::string &path, bool watch, Stat &stat);

		int get (const std::string&path, bool watch, std::string &buffer);

		int set (const std::string &path, bool watch,
				 const std::string &buffer, int version = -1);

		int del (const std::string &path, int version = -1);

		bool get_children (const std::string &path,
						   bool watch,
						   std::vector<std::string> &result);

		int recursive_create (const std::string& path,
							  const std::string& value,
							  std::string& real_path,
							  int flag);

		const char* get_error_str (int code);

	private:
		struct event_t
		{
			event_t (int type, int state, const std::string &path);
			bool stop_;
			int type_;
			int state_;
			std::string path_;
		};
		static void watcher_callback (zhandle_t *zh, int type, int state,
									  const char *path, void *watcherCtx);

		void event_callback (event_t *event);

		zhandle_t *zhandle;
		zk_callback_t *event_callback_;
		struct tranfor_event_fiber_t:acl::fiber
		{
			tranfor_event_fiber_t (ACL_MBOX *event_mbox,
								   zk_client_t &zk_cli)
				: event_mbox_(event_mbox),
				zk_client_(zk_cli)
			{

			}
			ACL_MBOX *event_mbox_;
			zk_client_t &zk_client_;
			virtual void run (void);
			void stop (bool);
			acl::channel<bool> wait_for_stop_;
		};
		friend struct tranfor_event_fiber_t;
		tranfor_event_fiber_t *tranfor_event_fiber_;
		ACL_MBOX *event_mbox_;
	};
}