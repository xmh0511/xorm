#include <iostream>
#include "mysql.hpp"
#include "dao.hpp"
using namespace xorm;
struct test {
	mysql::Integer id;
	mysql::Integer a;
	std::string b;
	mysql::MysqlDateTime time;
	mysql::MysqlDate date;
	mysql::MysqlTime tm;
};
REFLECTION(test, id, a, b, time, date, tm)
int main() {
	dataBaseConfig config;
	config.character_encoding = "utf8";
	config.conn_number = 2;
	config.dbname = "xorm";
	config.host = "127.0.0.1";
	config.password = "root";
	config.user = "root";
	init_database_config(config);

	dao_t<mysql> dao;
	test data;
	data.id = 0;
	data.a.clear();
	data.b = "t0 中国好" + std::to_string(5);
	data.time = "2019-10-25 18:05:01";
	data.date = "2019-10-09";
	data.tm.format_timestamp(std::time(nullptr));
	dao.insert(data);

	auto t0 = std::thread([]() {
		for (auto i = 0; i < 5; i++) {
			dao_t<mysql> dao;
			test data;
			data.id = 0;
			data.a = i;
			data.b = "t0 中国好"+ std::to_string(i);
			data.time = "2019-10-25 18:05:01";
			data.date = "2019-10-09";
			data.tm = "18:19:01";
			dao.insert(data);
			std::this_thread::sleep_for(std::chrono::milliseconds(800));
		}
	});
	auto t1 = std::thread([]() {
		dao_t<mysql> t;
		for (auto i = 0; i < 5; i++) {
			test data;
			data.id = 0;
			data.a = i;
			data.b = "t1 中国好" + std::to_string(i);
			data.time = "2019-10-25 18:05:01";
			data.date = "2019-10-09";
			data.tm = "18:19:01";
			t.insert(data);
			std::this_thread::sleep_for(std::chrono::milliseconds(802));
		}
	});
	auto t2 = std::thread([]() {
		dao_t<mysql> t;
		for (auto i = 0; i < 5; i++) {
			test data;
			data.id = 0;
			data.a = i;
			data.b = "t2 中国好" + std::to_string(i);
			data.time = "2019-10-25 18:05:01";
			data.date = "2019-10-09";
			data.tm = "18:19:01";
			t.insert(data);
			std::this_thread::sleep_for(std::chrono::milliseconds(803));
		}
	});
	auto t3 = std::thread([]() {
		dao_t<mysql> t;
		for (auto i = 0; i < 5; i++) {
			test data;
			data.id = 0;
			data.a = i;
			data.b = "t3 中国好" + std::to_string(i);
			data.time = "2019-10-25 18:05:01";
			data.date = "2019-10-09";
			data.tm = "18:19:01";
			t.insert(data);
			std::this_thread::sleep_for(std::chrono::milliseconds(804));
		}
	});
	t0.join();
	t1.join();
	t2.join();
	t3.join();
	std::cout << "end" << std::endl;
}