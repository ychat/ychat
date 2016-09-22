#include "stdafx.h"
#include "chat_service.h"
#include "config.h"

int main(int argc, char *argv[])
{
	acl::acl_cpp_init();
	ychat::chat_service& ms = 
		acl::singleton2<ychat::chat_service>::get_instance();

	// 设置配置参数表
	ms.set_cfg_int(ychat::g_config.int_tab_);
	ms.set_cfg_int64(ychat::g_config.int64_tab_);
	ms.set_cfg_str(ychat::g_config.str_tab_);
	ms.set_cfg_bool(ychat::g_config.bool_tab_);

	if (argc >= 2 && strcasecmp(argv[1], "help") == 0)
		printf("usage: %s alone configure addr\r\n", argv[0]);
	else if (argc >= 2 && strcasecmp(argv[1], "alone") == 0)
	{
		const char* addr = "0.0.0.0:9001";

		acl::log::stdout_open(true);

		if (argc >= 4)
			addr = argv[3];

		printf("listen: %s\r\n", addr);
		ychat::g_config.service_addr_ = (char *)addr;
		ms.run_alone(addr, argc >= 3 ? argv[2] : NULL, 0);
	}
	else
		ms.run_daemon(argc, argv);

	return 0;
}
