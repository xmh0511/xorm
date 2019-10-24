#include <iostream>
#include "mysql.hpp"
using namespace xorm;
struct test {
	mysql::Integer id;
	mysql::Integer a;
	std::string b;
	mysql::MysqlDateTime time;
	mysql::MysqlDate date;
	mysql::MysqlTime tm;
};
REFLECTION(test, id,a,b, time, date,tm)
int main() {
	xorm::mysql t("127.0.0.1","root","root","xorm",3306);
	//test data;
	//data.id = 0;
	//data.a = 4096;
	//data.b = "hello,world1024";
	//data.time = "2019-10-24 18:05:01";
	//data.date = "2019-10-09";
	//data.tm = "18:19:01";
	//auto r = t.insert(data);
	//t.update(data,"where id = 21");

	//auto pr = t.query<test>("where id = 3");
	//if (pr.first) {
	//	auto& vec = pr.second;
	//	for (auto& iter : vec) {
	//		reflector::each_object(iter, [](auto&& t,auto name,auto field) {
	//			std::cout << name << ": " << (t.*field)<<std::endl;
	//		});
	//		std::cout <<"----------------------"<< std::endl;
	//	}
	//}
	auto r = t.del<test>("where id = 22");
	std::cout << "end" << std::endl;
}