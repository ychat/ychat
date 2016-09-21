#pragma once

namespace ychat
{
	class connector_t;
	class connector_mgr_t
	{
	public:
		static connector_mgr_t& get_instance();
	private:
		std::map<uint32_t, connector_t&> connectors_;
	};
}