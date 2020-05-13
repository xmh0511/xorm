#include <iostream>
#include <xorm.hpp>
using namespace xorm;
struct test {
	mysql::Integer id;
	mysql::Integer a;
	std::string b;
	mysql::MysqlDateTime time;
	mysql::MysqlDate date;
	mysql::MysqlTime tm;
	mysql::Double money;
};
REFLECTION(test, id, a, b, time, date, tm, money)
int main() {
	dataBaseConfig config;
	config.character_encoding = "utf8";
	config.conn_number = 2;
	config.dbname = "xorm";
	config.host = "127.0.0.1";
	config.password = "root";
	config.user = "root";
	init_database_config(config);

	dao_message::get().set_error_callback([](std::string const& msg) {
		std::cout << msg << "\n";
	});
	auto t0 = std::thread([]() {
		for (auto i = 0; i < 5; i++) {
			dao_t<mysql> dao;
			dao.start_transaction();
			test data;
			data.id = 0;
			data.a = i;
			data.b = "t0 中国好" + std::to_string(i);
			data.time.format_timestamp(std::time(nullptr));
			data.date = "2019-10-09";
			data.tm.format_timestamp(std::time(nullptr));
			data.money = 12.03;
			dao.insert(data);
			std::this_thread::sleep_for(std::chrono::milliseconds(800));
		}
		});
	auto t1 = std::thread([]() {
		dao_t<mysql> t;
		t.start_transaction();
		for (auto i = 0; i < 5; i++) {
			test data;
			data.id = 0;
			data.a = i;
			data.b = "t1 中国好" + std::to_string(i);
			data.time.format_timestamp(std::time(nullptr));
			data.date = "2019-10-09";
			data.tm.format_timestamp(std::time(nullptr));
			t.insert(data);
			std::this_thread::sleep_for(std::chrono::milliseconds(802));
		}
		});
	auto t2 = std::thread([]() {
		dao_t<mysql> t;
		t.start_transaction();
		for (auto i = 0; i < 5; i++) {
			test data;
			data.id = 0;
			data.a = i;
			data.b = "t2 中国好" + std::to_string(i);
			data.time.format_timestamp(std::time(nullptr));
			data.date = "2019-10-09";
			data.tm.format_timestamp(std::time(nullptr));
			t.insert(data);
			std::this_thread::sleep_for(std::chrono::milliseconds(803));
		}
		});
	auto t3 = std::thread([]() {
		dao_t<mysql> t;
		t.start_transaction();
		for (auto i = 0; i < 5; i++) {
			test data;
			data.id = 0;
			data.a = i;
			data.b = "t3 中国好" + std::to_string(i);
			data.time.format_timestamp(std::time(nullptr));
			data.date = "2019-10-09";
			data.tm.format_timestamp(std::time(nullptr));
			t.insert(data);
			std::this_thread::sleep_for(std::chrono::milliseconds(804));
		}
		});
	{
		dao_t<mysql> dao_query;
		dao_query.start_transaction();
		auto r = dao_query.query<test>(" for update");
		std::cout << r.first << "  " << r.second.size() << std::endl;
		if (!r.second.empty()) {
			auto& info = r.second[0];
			auto rr = dao_query.update(info);
			std::cout << rr << std::endl;
		}
	}
	t0.join();
	t1.join();
	t2.join();
	t3.join();

	dao_t<mysql> dao_query;
	auto r0 = dao_query.update("update test set a=? where a=?", mysql::Integer{ 2048 }, mysql::Integer{ 0 });
	auto r1 = dao_query.del<test>("where id=?", mysql::Integer{1});
	auto r2 = dao_query.query<test>(" where a=?", mysql::Integer{ 0 });
	auto r3 = dao_query.query<test>("");
	auto r4 = dao_query.query<std::tuple<mysql::Integer>>("select a from test where id=?", mysql::Integer{ 2 });
	std::cin.get();
}