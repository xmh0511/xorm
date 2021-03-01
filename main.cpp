#include <iostream>
#include <xorm.hpp>
using namespace xorm;

#ifdef XORM_ENABLE_MYSQL
struct test {
	mysql::Integer id;
	mysql::Integer a;
	std::string b;
	mysql::MysqlDateTime time;
	mysql::MysqlDate date;
	mysql::MysqlTime tm;
	mysql::Double money;
	std::string d;
};
REFLECTION(test, id, a, b, time, date, tm, money, d)

struct default_tb {
	mysql::Integer id;
	std::string text;
};
REFLECTION(default_tb,id,text)
#endif 


#ifdef  XORM_ENABLE_SQLITE
struct test1 {
	sqlite::Integer a;
	sqlite::Double b;
	sqlite::Integer id;
	std::string text;
};
REFLECTION(test1,a,b,id, text)
#endif 

int main() {
#ifdef XORM_ENABLE_MYSQL
	dataBaseConfig config;
	config.index_key = "xorm";
	config.character_encoding = "utf8";
	config.conn_number = 2;
	config.dbname = "xorm";
	config.host = "127.0.0.1";
	config.password = "root";
	config.user = "root";
	dao_t<mysql>::init_conn_pool(config);



	dataBaseConfig config2;
	config2.index_key = "xorm2";
	config2.character_encoding = "utf8";
	config2.conn_number = 4;
	config2.dbname = "xorm2";
	config2.host = "127.0.0.1";
	config2.password = "root";
	config2.user = "root";
	dao_t<mysql>::init_conn_pool(config2);

	dao_message::get().set_error_callback([](std::string const& msg) {
		std::cout << msg << "\n";
	});

	auto t0 = std::thread([]() {
		for (auto i = 0; i < 5; i++) {
			dao_t<mysql> dao{"xorm"};
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
		dao_t<mysql> t{ "xorm" };
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
		dao_t<mysql> t{ "xorm" };
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
		dao_t<mysql> t{ "xorm" };
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
		dao_t<mysql> dao_query{ "xorm" };
		dao_query.start_transaction();
		auto r = dao_query.query<test>(" for update");
		std::cout << r.success << "  " << r.results.size() << std::endl;
		if (!r.results.empty()) {
			auto& info = r.results[0];
			auto rr = dao_query.update(info);
			std::cout << rr.affect_rows << std::endl;
		}
	}
	t0.join();
	t1.join();
	t2.join();
	t3.join();

	dao_t<mysql> dao_query{ "xorm" };
	auto r0 = dao_query.update("update test set a=? where a=?", mysql::Integer{ 2048 }, mysql::Integer{ 0 });
	auto r1 = dao_query.del<test>("where id=?", mysql::Integer{1});
	auto r2 = dao_query.query<test>(" where id=?", mysql::Integer{ 2 });
	if (!r2.results.empty()) {
		auto& info = r2.results[0];
		info.a = 6666;
		auto r5 = dao_query.update(info);
	}
	auto r3 = dao_query.query<test>("");
	auto r4 = dao_query.query<std::tuple<mysql::Integer>>("select a from test where id=?", mysql::Integer{ 2 });
	auto rrr = dao_query.del<test>("");

	//dao_t<mysql> dao_query;
	//auto r = dao_query.query<std::tuple<mysql::Integer,std::string, std::string>>("select id, b,d from test");
	//int c = 0;

	//auto r0 = dao_query.query<test>("");
	//int d = 0;

	dao_t<mysql> dao_query2{ "xorm2" };
	auto xorm2r = dao_query2.query<default_tb>(" where id=?", mysql::Integer{ 1 });
	bool state = false;
	dao_query2.get_connection()->get_raw_connetion(state);

#endif 

#ifdef  XORM_ENABLE_SQLITE
		dataBaseConfig config;
		config.index_key = "xorm";
		config.character_encoding = "utf8";
		config.host = "./test1.db";
		config.conn_number = 5;
		dao_message::get().set_error_callback([](std::string const& msg) {
		   std::cout << msg << "\n";
		});
		dao_t<sqlite>::init_conn_pool(config);
		dao_t<sqlite> dao{ "xorm" };
		auto r = dao.query<test1>("");
		test1 tt;
		tt.a = 56;
		tt.b = 10.4;
		tt.id = 1;
		tt.text = "abccdd";
		auto r0 = dao.insert(tt);
		auto r1 = dao.query<std::tuple<sqlite::Integer>>("select a from test1 where id=?", sqlite::Integer{ 6 });
		auto r11 = dao.query<test1>("");
		auto r12 = dao.query<test1>("where id=?", sqlite::Integer{ 6 });
		tt.b = 1024.1024;
		auto r2 = dao.update(tt);
		auto r3 = dao.del<test1>("");
#endif //  XORM_ENABLE_SQLITE
		std::getchar();
}