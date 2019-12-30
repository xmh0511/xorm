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
#include "dbconfig.hpp"
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
		typename std::enable_if<is_fundamention_type<typename std::remove_reference<T>::type>::value || is_date_type<typename std::remove_reference<T>::type>::value>::type bind_value(T& t, MYSQL_BIND& bind, bool get = false) {
			using type = typename std::remove_reference<T>::type;
			bind.buffer_type = type::field_type;
			bind.buffer = t.buffer();
			bind.is_null = t.null_buffer();
			bind.length = 0;
			//bind.buffer_length = 0;
		}
		template<typename T>
		typename std::enable_if<std::is_same<typename std::remove_reference<T>::type, std::string>::value>::type bind_value(T& t, MYSQL_BIND& bind, bool get = false) {
			if (get) {
				t.resize(string_max_size_);
			}
			bind.buffer_type = MYSQL_TYPE_STRING;
			bind.buffer = &(t[0]);
			bind.is_null = 0;
			bind.length = 0;
			bind.buffer_length = (unsigned long)t.size();
		}

		template<typename T,typename U>
		typename std::enable_if<std::is_same<typename std::remove_reference<T>::type, std::string>::value>::type clear_field(T& t,U& v) {
			v = std::string(&v[0], strlen(v.data()));
			memset(&t[0], 0, t.size());
		}

		template<typename T, typename U>
		typename std::enable_if<!std::is_same<typename std::remove_reference<T>::type, std::string>::value>::type clear_field(T& t,U& v) {
			t.clear();
		}
	public:
		mysql() = default;
		mysql(dataBaseConfig const& config) {
			connect(config);
		}
	public:
		void connect(dataBaseConfig const& config) {
			bool is_success = false;
			if (0 == mysql_server_init(0, nullptr, nullptr)) {
				is_success = true;
			}
			conn_ = mysql_init(nullptr);
			if (nullptr != conn_) {
				is_success = true;
				mysql_options(conn_, MYSQL_OPT_RECONNECT, &config.reconnect_number);
				mysql_options(conn_, MYSQL_SET_CHARSET_NAME, config.character_encoding.data());
			}

			if (config.timeout > 0) {
				if (mysql_options(conn_, MYSQL_OPT_CONNECT_TIMEOUT, &config.timeout) != 0) {
					is_success = false;
				}
				else {
					is_success = true;
				}
			}

			if (is_success && nullptr != mysql_real_connect(conn_, config.host.c_str(), config.user.c_str(), config.password.c_str(), config.dbname.c_str(), config.port, nullptr, 0)) {
				is_connect_ = true;
			}
		}
		void reconnect(dataBaseConfig const& config) {
			disconnect();
			connect(config);
		}

		void disconnect() {
			if (conn_ != nullptr) {
				mysql_close(conn_);
				conn_ = nullptr;
				is_connect_ = false;
			}
		}
		bool is_connect() {
			return conn_!=nullptr && is_connect_;
		}

		bool ping() {
			if (conn_ == nullptr) {
				is_connect_ = false;
				return false;
			}
			bool r =  mysql_ping(conn_) == 0;
			if (!r) {
				is_connect_ = false;
			}
			return r;
		}
	public:
		template<typename T>
		typename std::enable_if<reflector::is_reflect_class<typename std::remove_reference<T>::type>::value, std::pair<std::int64_t, std::int64_t>>::type insert(T&& t) {
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
			return stmt_execute(ss.str(), bind);
		}

		template<typename T>
		typename std::enable_if<reflector::is_reflect_class<typename std::remove_reference<T>::type>::value, bool>::type del(std::string const& condition) {
			auto meta = meta_info_reflect(T{});
			std::string sql = "DELETE FROM " + meta.get_class_name() + " " + condition;
			return execute(sql);
		}

		template<typename T>
		typename std::enable_if<reflector::is_reflect_class<typename std::remove_reference<T>::type>::value, bool>::type update(T&& v, std::string const& condition = "") {
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
			auto rpr = stmt_execute(ss.str(), bind);
			if (condition.empty()) {
				return rpr.first == 2 ? true : false;
			}
			else {
				return rpr.first != 0 ? true : false;
			}
		}

		template<typename T>
		typename std::enable_if<reflector::is_reflect_class<typename std::remove_reference<T>::type>::value, std::pair<bool, std::vector<T>>>::type query(std::string const& condition="" ) {
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
			pStmt = mysql_stmt_init(conn_);
			std::vector<T> result;
			if (pStmt != nullptr) {
				auto sqlStr = ss.str();
				int iRet = mysql_stmt_prepare(pStmt, sqlStr.c_str(), (unsigned long)sqlStr.size());
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
								T copy_v = tmp;
								reflector::each_object(tmp, [this,&copy_v](auto&& obj, auto name, auto field) {
									this->clear_field((obj.*field),(copy_v.*field));
								});
								result.push_back(std::move(copy_v));
							}
							return { true,result };
						}
					}
				}
				else {
					std::cout << mysql_error(conn_) << std::endl;
				}
			}
			return { false,result };
		}

		template<typename T>
		typename std::enable_if<xorm::is_tuple_type_v<T>, std::pair<bool, std::vector<T>>>::type query(std::string const& sqlStr) {
			constexpr std::size_t tuple_size = std::tuple_size<T>::value;
			MYSQL_BIND bind[tuple_size];
			memset(bind, 0, sizeof(bind));
			MYSQL_STMT* pStmt = nullptr;
			pStmt = mysql_stmt_init(conn_);
			std::vector<T> result;
			if (pStmt != nullptr) {
				int iRet = mysql_stmt_prepare(pStmt, sqlStr.c_str(), (unsigned long)sqlStr.size());
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
								T copy_v = tmp;
								each_tuple<0, tuple_size>::each2(tmp, copy_v,[&bind,this](auto& v,auto& u) {
									this->clear_field(v,u);
								});
								result.push_back(copy_v);
							}
							return { true,result };
						}
					}
				}
				else {
					std::cout << mysql_error(conn_) << std::endl;
				}
			}
			return { false,result };
		}

		bool execute(std::string const& sql) {
			auto iRet = mysql_query(conn_, sql.c_str());
			bool r = iRet != 0 ? false : true;
			if (r) {
				auto pRes = mysql_use_result(conn_);
				mysql_free_result(pRes);
			}
			else {
				std::cout << mysql_error(conn_) << std::endl;
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
		std::pair<std::uint64_t, std::uint64_t> stmt_execute(std::string const& sqlStr, MYSQL_BIND* bind) {
			MYSQL_STMT* pStmt = nullptr;
			pStmt = mysql_stmt_init(conn_);
			if (pStmt != nullptr) {
				begin();
				int iRet = mysql_stmt_prepare(pStmt, sqlStr.data(), (unsigned long)sqlStr.size());
				if (iRet == 0) {
					iRet = mysql_stmt_bind_param(pStmt, bind);
					if (iRet == 0) {
						iRet = mysql_stmt_execute(pStmt);
						if (iRet == 0) {
							auto rows = mysql_stmt_affected_rows(pStmt);
							if (rows != 0) {
								bool cr = commit();
								auto pr = query<std::tuple<mysql::Integer>>("SELECT LAST_INSERT_ID();");
								if (cr && pr.first) {
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
		MYSQL* conn_;
		bool is_connect_ = false;
		std::size_t string_max_size_ = 1024 * 1024;
	};
}