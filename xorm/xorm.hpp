#pragma once
#include "dao.hpp"
#ifdef _ENABLE_XORM_
#ifdef XORM_ENABLE_MYSQL 
#include "mysql.hpp"
#endif // ENABLE_MYSQL 
#ifdef XORM_ENABLE_SQLITE
#include "sqlite3.hpp"
#endif // ENABLE_MYSQL 
#endif // _ENABLE_XORM_
