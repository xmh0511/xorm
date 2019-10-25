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

	init_database_config({ "127.0.0.1","root","root","xorm",3306,2 });
	auto t0 = std::thread([]() {
		for (auto i = 0; i < 5; i++) {
			dao<mysql> t;
			test data;
			data.id = 0;
			data.a = i;
			data.b = "t0"+ std::to_string(i);
			data.time = "2019-10-25 18:05:01";
			data.date = "2019-10-09";
			data.tm = "18:19:01";
			t.insert(data);
			std::this_thread::sleep_for(std::chrono::milliseconds(800));
		}
	});
	auto t1 = std::thread([]() {
		for (auto i = 0; i < 5; i++) {
			dao<mysql> t;
			test data;
			data.id = 0;
			data.a = i;
			data.b = "t1" + std::to_string(i);
			data.time = "2019-10-25 18:05:01";
			data.date = "2019-10-09";
			data.tm = "18:19:01";
			t.insert(data);
			std::this_thread::sleep_for(std::chrono::milliseconds(802));
		}
	});
	auto t2 = std::thread([]() {
		for (auto i = 0; i < 5; i++) {
			dao<mysql> t;
			test data;
			data.id = 0;
			data.a = i;
			data.b = "t2" + std::to_string(i);
			data.time = "2019-10-25 18:05:01";
			data.date = "2019-10-09";
			data.tm = "18:19:01";
			t.insert(data);
			std::this_thread::sleep_for(std::chrono::milliseconds(803));
		}
	});
	auto t3 = std::thread([]() {
		for (auto i = 0; i < 5; i++) {
			dao<mysql> t;
			test data;
			data.id = 0;
			data.a = i;
			data.b = "t3" + std::to_string(i);
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