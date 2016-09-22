#pragma once

namespace ychat
{
	//outstream connect to session layer. outstream write msg to session layer.
	//and then session layer will send msg to client.
	class outstream :public acl::connect_client
	{
	public:
		outstream(const std::string &addr);

		~outstream();

		virtual bool open();

		int send(void *data, int len);

		void set_id(const std::string &id);

		std::string get_id();

	private:
		std::string id_;
		std::string addr_;
		acl::socket_stream stream_;
	};
}