// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#pragma once


//#include <iostream>
//#include <tchar.h>

// TODO: 在此处引用程序要求的附加头文件
#include <stdint.h>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <vector>

#include "lib_acl.h"
#include "acl_cpp/lib_acl.hpp"
#include "fiber/lib_fiber.h"
#include "fiber/lib_fiber.hpp"
#include "stdlib/acl_mbox.h"

#ifdef	WIN32
#define	snprintf _snprintf
#endif

