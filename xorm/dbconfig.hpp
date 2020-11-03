#pragma once
#include <string>
struct dataBaseConfig {
	std::string index_key;
	std::string host;
	std::string user;
	std::string password;
	std::string dbname;
	std::string character_encoding;
	unsigned int port = 3306;
	std::size_t reconnect_number = 1;
	int timeout = -1;
	std::size_t conn_number;
};