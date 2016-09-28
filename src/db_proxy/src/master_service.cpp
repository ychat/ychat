#include "stdafx.h"
#include "http_servlet.h"
#include "master_service.h"
#include "db_proxy_t.h"

static char *var_cfg_str;

acl::master_str_tbl var_conf_str_tab[] = {
	{ "str", "test_msg", &var_cfg_str },

	{ 0, 0, 0 }
};

static int  var_cfg_debug_enable;

acl::master_bool_tbl var_conf_bool_tab[] = {
	{ "debug_enable", 1, &var_cfg_debug_enable },

	{ 0, 0, 0 }
};

static int  var_cfg_io_timeout;

acl::master_int_tbl var_conf_int_tab[] = {
	{ "io_timeout", 120, &var_cfg_io_timeout, 0, 0 },

	{ 0, 0 , 0 , 0, 0 }
};

acl::master_int64_tbl var_conf_int64_tab[] = {
	{ 0, 0 , 0 , 0, 0 }
};


//////////////////////////////////////////////////////////////////////////

master_service::master_service(void)
{
	db_proxy_ = new ychat::db_proxy_t;
}

master_service::~master_service(void)
{
	delete db_proxy_;
}

void master_service::on_accept(acl::socket_stream& conn)
{
	logger("connect from %s, fd %d", conn.get_peer(), conn.sock_handle());

	conn.set_rw_timeout(120);

	db_proxy_->handle_socket(conn);

	logger("disconnect from %s", conn.get_peer());
}

void master_service::proc_pre_jail(void)
{
	logger(">>>proc_pre_jail<<<");
}

void master_service::proc_on_init(void)
{
	logger(">>>proc_on_init<<<");
}

void master_service::proc_on_exit(void)
{
	logger(">>>proc_on_exit<<<");
}
