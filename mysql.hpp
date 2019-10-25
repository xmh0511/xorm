#pragma once
#ifdef  _WIN32
#include <mysql.h>
#else
#include <mysql/mysql.h>
#endif // __LINUX__
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <memory>
#include <cstring>
#include <tuple>
#include "reflect/reflector.hpp"
#include "data_type.hpp"
#include "meta_utility.hpp"
namespace xorm {
	class mysql final {
	public:
		using Integer = FundamentionType<int, MYSQL_TYPE_LONG>;
		using Float = FundamentionType<float, MYSQL_TYPE_FLOAT>;
		using Double = FundamentionType<double, MYSQL_TYPE_DOUBLE>;
		using BigInt =  FundamentionType<std::int64_t, MYSQL_TYPE_LONGLONG>;
		using SmallInt =  FundamentionType<short, MYSQL_TYPE_SHORT>;
		using TinyInt = FundamentionType<char, MYSQL_TYPE_TINY>;
		using MysqlDate = TimeDate<MYSQL_TYPE_DATE>;
		using MysqlDateTime = TimeDate<MYSQL_TYPE_DATETIME>;
		using MysqlTime = TimeDate<MYSQL_TYPE_TIME>;
	private:
		template<typename T>
		std::enable_if_t<is_fundamention_type<typename std::remove_reference<T>::type>::value || is_date_type<typename std::remove_reference<T>::type>::value> bind_value(T& t, MYSQL_BIND& bind, bool get = false) {
			using type = typename std::remove_reference<T>::type;
			bind.buffer_type = type::field_type;
			bind.buffer = t.buffer();
			bind.is_null = t.null_buffer();
			bind.length = 0;
			//bind.buffer_length = 0;
		}
		template<typename T>
		std::enable_if_t<std::is_same_v<typename std::remove_reference<T>::type, std::string>> bind_value(T& t, MYSQL_BIND& bind, bool get = false) {
			if (get) {
				t.resize(string_max_size_);
			}
			bind.buffer_type = MYSQL_TYPE_STRING;
			bind.buffer = &(t[0]);
			bind.is_null = 0;
			bind.length = 0;
			bind.buffer_length = t.size();
		}

		template<typename T>
		std::enable_if_t<std::is_same_v<typename std::remove_reference<T>::type, std::string>> clear_field(T& t) {
			memset(&t[0], 0, t.size());
		}

		template<typename T>
		std::enable_if_t<!std::is_same_v<typename std::remove_reference<T>::type, std::string>> clear_field(T& t) {
			t.clear();
		}
	public:
		mysql() = default;
		mysql(std::string const& host, std::string const& user, std::string const& password, std::string const& dbname, unsigned int port = 3306) {
			connect(host, user, password, dbname, port);
		}
	public:
		void connect(std::string const& host, std::string const& user, std::string const& password, std::string const& dbname, unsigned int port) {
			bool is_success = false;
			if (0 == mysql_server_init(0, nullptr, nullptr)) {
				is_success = true;
			}

			if (nullptr != mysql_init(&mydata)) {
				is_success = true;
			}

			if (is_success && nullptr != mysql_real_connect(&mydata, host.c_str(), user.c_str(), password.c_str(), dbname.c_str(), port, nullptr, 0)) {
				is_connect_ = true;
			}
		}
		bool is_connect() {
			return is_connect_;
		}
	public:
		template<typename T>
		std::enable_if_t<reflector::is_reflect_class<typename std::remove_reference<T>::type>::value, std::pair<std::int64_t, std::int64_t>> insert(T&& t) {
			auto meta = meta_info_reflect(t);
			std::string tablename = meta.get_class_name();
			std::stringstream ss;
			ss << "INSERT INTO " << tablename << " (";
			auto size = meta.element_size();
			int index = 0;
			std::string value_place = "";
			MYSQL_BIND bind[meta.element_size()];
			memset(bind, 0, sizeof(bind));
			reflector::each_object(std::forward<T>(t), [&ss, &index, size, &value_place, &bind, this](auto&& obj, auto name, auto field) {
				this->bind_value((obj.*field), bind[index]);
				if (index < (size - 1)) {
					ss << name << ",";
					value_place.append("?,");
				}
				else if (index == (size - 1)) {
					ss << name;
					value_place.append("?");
				}
				++index;
			});
			ss << ")" << " VALUES(" << value_place << ")";
			return stmt_excute(ss.str(), bind);
		}

		template<typename T>
		std::enable_if_t<reflector::is_reflect_class<typename std::remove_reference<T>::type>::value, bool> del(std::string const& condition) {
			auto meta = meta_info_reflect(T{});
			std::string sql = "DELETE FROM " + meta.get_class_name() + " " + condition;
			return execute(sql);
		}

		template<typename T>
		std::enable_if_t<reflector::is_reflect_class<typename std::remove_reference<T>::type>::value, bool> update(T&& v, std::string const& condition = "") {
			auto meta = meta_info_reflect(v);
			std::string tablename = meta.get_class_name();
			std::stringstream ss;
			MYSQL_BIND bind[meta.element_size()];
			memset(bind, 0, sizeof(bind));
			if (condition.empty()) {
				ss << "replace  into " << tablename << " (";
				auto size = meta.element_size();
				int index = 0;
				std::string value_place = "";
				reflector::each_object(std::forward<T>(v), [&ss, &index, size, &value_place, &bind, this](auto&& obj, auto name, auto field) {
					this->bind_value((obj.*field), bind[index]);
					if (index < (size - 1)) {
						ss << name << ",";
						value_place.append("?,");
					}
					else if (index == (size - 1)) {
						ss << name;
						value_place.append("?");
					}
					++index;
				});
				ss << ")" << " VALUES(" << value_place << ")";
			}
			else {
				ss << "UPDATE " << tablename << " SET ";
				auto size = meta.element_size();
				int index = 0;
				reflector::each_object(std::forward<T>(v), [&ss, &index, size, &bind, this](auto&& obj, auto name, auto field) {
					this->bind_value((obj.*field), bind[index]);
					if (index < (size - 1)) {
						ss << name << "=?" << ",";
					}
					else if (index == (size - 1)) {
						ss << name << "=?";
					}
					++index;
				});
				ss << " " << condition;
			}
			auto rpr = stmt_excute(ss.str(), bind);
			if (condition.empty()) {
				return rpr.first == 2 ? true : false;
			}
			else {
				return rpr.first != 0 ? true : false;
			}
		}

		template<typename T>
		std::enable_if_t<reflector::is_reflect_class<typename std::remove_reference<T>::type>::value, std::pair<bool, std::vector<T>>> query(std::string const& condition="" ) {
			auto meta = meta_info_reflect(T{});
			std::string tablename = meta.get_class_name();
			std::stringstream ss;
			MYSQL_BIND bind[meta.element_size()];
			memset(bind, 0, sizeof(bind));
			auto name_arr = meta.get_element_names();
			ss << "SELECT ";
			auto size = name_arr.size();
			for (auto index = 0; index < size; index++) {
				ss << name_arr[index];
				if (index < (size - 1)) {
					ss << ",";
				}
			}
			ss << " FROM " << tablename<<" "<< condition;
			MYSQL_STMT* pStmt = nullptr;
			pStmt = mysql_stmt_init(&mydata);
			std::vector<T> result;
			if (pStmt != nullptr) {
				auto sqlStr = ss.str();
				int iRet = mysql_stmt_prepare(pStmt, sqlStr.c_str(), sqlStr.size());
				if (iRet == 0) {
					T tmp{};
					int index = 0;
					reflector::each_object(tmp, [&index, &bind, this](auto&& obj, auto name, auto field) {
						this->bind_value((obj.*field), bind[index], true);
						++index;
					});
					bool r = mysql_stmt_bind_result(pStmt, bind);
					if (!r) {
						r = mysql_stmt_execute(pStmt);
						if (!r) {
							while (mysql_stmt_fetch(pStmt) == 0) {
								result.push_back(tmp);
								reflector::each_object(tmp, [this](auto&& obj, auto name, auto field) {
									this->clear_field((obj.*field));
								});
							}
							return { true,result };
						}
					}
				}
			}
			return { false,result };
		}

		template<typename T>
		std::enable_if_t<xorm::is_tuple_type_v<T>, std::pair<bool, std::vector<T>>> query(std::string const& sqlStr) {
			MYSQL_BIND bind[std::tuple_size_v<T>];
			memset(bind, 0, sizeof(bind));
			MYSQL_STMT* pStmt = nullptr;
			pStmt = mysql_stmt_init(&mydata);
			std::vector<T> result;
			constexpr std::size_t tuple_size = std::tuple_size_v<T>;
			if (pStmt != nullptr) {
				int iRet = mysql_stmt_prepare(pStmt, sqlStr.c_str(), sqlStr.size());
				if (iRet == 0) {
					T tmp{};
					int index = 0;
					each_tuple<0, tuple_size>::each(tmp, [&bind,this,&index](auto& v) {
						this->bind_value(v, bind[index], true);
						++index;
					});
					bool r = mysql_stmt_bind_result(pStmt, bind);
					if (!r) {
						r = mysql_stmt_execute(pStmt);
						if (!r) {
							while (mysql_stmt_fetch(pStmt) == 0) {
								result.push_back(tmp);
								each_tuple<0, tuple_size>::each(tmp, [&bind,this](auto& v) {
									this->clear_field(v);
								});
							}
							return { true,result };
						}
					}
				}
			}
			return { false,result };
		}

		bool execute(std::string const& sql) {
			auto iRet = mysql_query(&mydata, sql.c_str());
			bool r = iRet != 0 ? false : true;
			if (r) {
				auto pRes = mysql_use_result(&mydata);
				mysql_free_result(pRes);
			}
			else {
				std::cout << mysql_error(&mydata) << std::endl;
			}
			return r;
		}
		bool begin() {
			return execute("BEGIN");
		}
		bool commit() {
			return execute("COMMIT");
		}
		bool rollback() {
			return execute("ROLLBACK");
		}
		void set_max_string_size(std::size_t size) {
			string_max_size_ = size;
		}
		std::size_t get_max_string_size() {
			return string_max_size_;
		}
	private:
		std::pair<std::uint64_t, std::uint64_t> stmt_excute(std::string const& sqlStr, MYSQL_BIND* bind) {
			MYSQL_STMT* pStmt = nullptr;
			pStmt = mysql_stmt_init(&mydata);
			if (pStmt != nullptr) {
				begin();
				int iRet = mysql_stmt_prepare(pStmt, sqlStr.data(), sqlStr.size());
				if (iRet == 0) {
					iRet = mysql_stmt_bind_param(pStmt, bind);
					if (iRet == 0) {
						iRet = mysql_stmt_execute(pStmt);
						if (iRet == 0) {
							auto rows = mysql_stmt_affected_rows(pStmt);
							if (rows != 0) {
								commit();
								auto pr = query<std::tuple<mysql::Integer>>("SELECT LAST_INSERT_ID();");
								if (pr.first) {
									auto& id_arr = pr.second;
									if (!id_arr.empty()) {
										auto id = std::get<0>((id_arr[0]));
										return { rows ,id.value() };
									}
								}
								return { rows,0 };
							}
						}
					}
				}
				rollback();
			}
			return { 0 ,0};
		}
	private:
		MYSQL mydata;
		bool is_connect_ = false;
		std::size_t string_max_size_ = 1024 * 1024;
	};
}