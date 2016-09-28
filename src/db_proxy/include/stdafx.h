// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#pragma once


//#include <iostream>
//#include <tchar.h>

// TODO: 在此处引用程序要求的附加头文件

#include <bson.h>
#include <bcon.h>
#include <mongoc.h>
#include <list>
#include <stdint.h>

#include "lib_acl.h"
#include "acl_cpp/lib_acl.hpp"
#include "fiber/lib_fiber.h"
#include "fiber/lib_fiber.hpp"


//common
#include "db_req_t.h"


#ifdef	WIN32
#define	snprintf _snprintf
#endif

