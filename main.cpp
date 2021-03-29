#include <iostream>
#include <xorm.hpp>
using namespace xorm;
#include <fstream>
#include <sstream>
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
	mysql::Blob binary1;
	mysql::Blob binary2;
};
REFLECTION(test, id, a, b, time, date, tm, money, d, binary1, binary2)

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
	sqlite::Blob binary;
};
REFLECTION(test1,a,b,id, text, binary)
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

	//auto t0 = std::thread([]() {
	//	for (auto i = 0; i < 5; i++) {
	//		dao_t<mysql> dao{"xorm"};
	//		dao.start_transaction();
	//		test data;
	//		data.id = 0;
	//		data.a = i;
	//		data.b = "t0 中国好" + std::to_string(i);
	//		data.time.format_timestamp(std::time(nullptr));
	//		data.date = "2019-10-09";
	//		data.tm.format_timestamp(std::time(nullptr));
	//		data.money = 12.03;
	//		dao.insert(data);
	//		std::this_thread::sleep_for(std::chrono::milliseconds(800));
	//	}
	//	});
	//auto t1 = std::thread([]() {
	//	dao_t<mysql> t{ "xorm" };
	//	t.start_transaction();
	//	for (auto i = 0; i < 5; i++) {
	//		test data;
	//		data.id = 0;
	//		data.a = i;
	//		data.b = "t1 中国好" + std::to_string(i);
	//		data.time.format_timestamp(std::time(nullptr));
	//		data.date = "2019-10-09";
	//		data.tm.format_timestamp(std::time(nullptr));
	//		t.insert(data);
	//		std::this_thread::sleep_for(std::chrono::milliseconds(802));
	//	}
	//	});
	//auto t2 = std::thread([]() {
	//	dao_t<mysql> t{ "xorm" };
	//	t.start_transaction();
	//	for (auto i = 0; i < 5; i++) {
	//		test data;
	//		data.id = 0;
	//		data.a = i;
	//		data.b = "t2 中国好" + std::to_string(i);
	//		data.time.format_timestamp(std::time(nullptr));
	//		data.date = "2019-10-09";
	//		data.tm.format_timestamp(std::time(nullptr));
	//		t.insert(data);
	//		std::this_thread::sleep_for(std::chrono::milliseconds(803));
	//	}
	//	});
	//auto t3 = std::thread([]() {
	//	dao_t<mysql> t{ "xorm" };
	//	t.start_transaction();
	//	for (auto i = 0; i < 5; i++) {
	//		test data;
	//		data.id = 0;
	//		data.a = i;
	//		data.b = "t3 中国好" + std::to_string(i);
	//		data.time.format_timestamp(std::time(nullptr));
	//		data.date = "2019-10-09";
	//		data.tm.format_timestamp(std::time(nullptr));
	//		t.insert(data);
	//		std::this_thread::sleep_for(std::chrono::milliseconds(804));
	//	}
	//	});
	//{
	//	dao_t<mysql> dao_query{ "xorm" };
	//	dao_query.start_transaction();
	//	auto r = dao_query.query<test>(" for update");
	//	std::cout << r.success << "  " << r.results.size() << std::endl;
	//	if (!r.results.empty()) {
	//		auto& info = r.results[0];
	//		auto rr = dao_query.update(info);
	//		std::cout << rr.affect_rows << std::endl;
	//	}
	//}
	//t0.join();
	//t1.join();
	//t2.join();
	//t3.join();

	//dao_t<mysql> dao_query{ "xorm" };
	//auto r0 = dao_query.update("update test set a=? where a=?", mysql::Integer{ 2048 }, mysql::Integer{ 0 });
	//auto r1 = dao_query.del<test>("where id=?", mysql::Integer{1});
	//auto r2 = dao_query.query<test>(" where id=?", mysql::Integer{ 2 });
	//if (!r2.results.empty()) {
	//	auto& info = r2.results[0];
	//	info.a = 6666;
	//	auto r5 = dao_query.update(info);
	//}
	//auto r3 = dao_query.query<test>("");
	//auto r4 = dao_query.query<std::tuple<mysql::Integer>>("select a from test where id=?", mysql::Integer{ 2 });
	//auto rrr = dao_query.del<test>("");

	//dao_t<mysql> dao_query;
	//auto r = dao_query.query<std::tuple<mysql::Integer,std::string, std::string>>("select id, b,d from test");
	//int c = 0;

	//auto r0 = dao_query.query<test>("");
	//int d = 0;

	//dao_t<mysql> dao_query2{ "xorm2" };
	//auto xorm2r = dao_query2.query<default_tb>(" where id=?", mysql::Integer{ 1 });
	//dao_query2.get_connection()->get_raw_connetion();

     dao_t<mysql> dao{ "xorm" };
	 test tb_info;
	 tb_info.id = 0;
	 tb_info.a = 0;
	 //tb_info.b = "hello";
	 tb_info.time.format_timestamp( std::time(nullptr));
	 tb_info.date.format_timestamp(std::time(nullptr));
	 tb_info.tm.format_timestamp(std::time(nullptr));
	 tb_info.money = 100.05;
	 tb_info.d = "world";
	 std::ifstream file1("./time.jpg", std::ios::binary);
	 std::stringstream ss1;
	 ss1 << file1.rdbuf();
	 auto content0 = ss1.str();

	 std::ifstream file2("./time2.jpg", std::ios::binary);
	 std::stringstream ss2;
	 ss2 << file2.rdbuf();
	 auto content2 = ss2.str();

	 tb_info.binary1 = mysql::Blob(content0.begin(), content0.end());
	 //tb_info.binary2 = mysql::Blob(content2.begin(), content2.end());

	auto r10 =  dao.insert(tb_info);
	tb_info.id = r10.unique_id;
	auto r11 = dao.query<test>("");

	//if (!r11.results.empty()) {
	//	auto&& data = r11.results[0];
	//	std::ofstream out0("./record0.jpg",std::ios::binary);
	//	out0.write(data.binary1.data(), data.binary1.size());

	//	std::ofstream out1("./record1.jpg", std::ios::binary);
	//	out1.write(data.binary2.data(), data.binary2.size());
	//}

	//auto r12 = dao.query<std::tuple<mysql::Blob>>("select binary1 from test");
	//tb_info.binary2 = tb_info.binary1;
	//auto r13 = dao.update(tb_info);
	auto r14 = dao.del<test>("");

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
		auto rs0 = dao.query<test1>("");
		auto r = dao.query<test1>("");
		test1 tt;
		tt.a = 56;
		tt.b = 10.4;
		tt.id = 1;
		tt.text = "abccdd";
		std::ifstream file("./time.jpg", std::ios::binary);
		std::stringstream ss;
		ss << file.rdbuf();
		auto content = ss.str();
		auto data_begin = content.data();
		tt.binary = sqlite::Blob(data_begin, data_begin + content.size());
		auto r0 = dao.insert(tt);
		auto r1 = dao.query<std::tuple<sqlite::Integer>>("select a from test1 where id=?", sqlite::Integer{ 1 });
		auto r11 = dao.query<test1>("");
		if (!r11.results.empty()) {
			auto&& first = r11.results[0];
			std::ofstream output("./readout.jpg", std::ios::binary);
			output.write((char const*)first.binary.data(), first.binary.size());
		}
		auto r12 = dao.query<test1>("where id=?", sqlite::Integer{ 1 });
		tt.b = 1024.1024;
		auto r2 = dao.update(tt);
		auto r90 = dao.update("update test1 set a=? where id=?", sqlite::Integer{ 100 }, sqlite::Integer{ 1 });
		auto rs = dao.query<std::tuple<sqlite::Integer>>("select id from test1 where text=?", std::string("abccdd"));
		auto r3 = dao.del<test1>("");
#endif //  XORM_ENABLE_SQLITE
		std::getchar();
}