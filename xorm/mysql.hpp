#pragma once
#ifdef XORM_ENABLE_MYSQL
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
#include <functional>
#include "reflect/reflector.hpp"
#include "data_type.hpp"
#include "meta_utility.hpp"
#include "dbconfig.hpp"
#include "db_utils.hpp"
namespace xorm {

	template<typename T0>
	struct auto_params_lambda0 {
		auto_params_lambda0(std::stringstream& ss_, int& index_, std::size_t size_, std::string& value_place_, MYSQL_BIND* bind_, T0* that) :ss(ss_), index(index_), size(size_), value_place(value_place_), bind(bind_), this_(that) {

		}
		template<typename T, typename U, typename Y>
		void operator()(T&& obj, U&& name, Y&& field) {
			this_->bind_value((obj.*field), bind[index]);
			if (index < (size - 1)) {
				ss << "`" << name << "` " << ",";
				value_place.append("?,");
			}
			else if (index == (size - 1)) {
				ss << "`" << name << "` ";
				value_place.append("?");
			}
			++index;
		}
		std::stringstream& ss;
		int& index;
		std::size_t size;
		std::string& value_place;
		MYSQL_BIND* bind;
		T0* this_;
	};

	template<typename T0>
	struct auto_params_lambda1 {
		auto_params_lambda1(std::stringstream& ss_, int& index_, std::size_t size_, MYSQL_BIND* bind_, T0* that) :ss(ss_), index(index_), size(size_), bind(bind_), this_(that) {

		}
		template<typename T, typename U, typename Y>
		void operator()(T&& obj, U&& name, Y&& field) {
			this_->bind_value((obj.*field), bind[index]);
			if (index < (size - 1)) {
				ss << "`" << name << "`" << "=?" << ",";
			}
			else if (index == (size - 1)) {
				ss << "`" << name << "`" << "=?";
			}
			++index;
		}
		std::stringstream& ss;
		int& index;
		std::size_t size;
		MYSQL_BIND* bind;
		T0* this_;
	};
	template<typename T0>
	struct auto_params_lambda2 {
		auto_params_lambda2(int& index_, MYSQL_BIND* bind_, T0* that) :index(index_), bind(bind_), this_(that) {

		}
		template<typename T, typename U, typename Y>
		void operator()(T&& obj, U&& name, Y&& field) {
			this_->bind_value((obj.*field), bind[index], true);
			++index;
		}
		int& index;
		MYSQL_BIND* bind;
		T0* this_;
	};
	template<typename Object, typename T0>
	struct auto_params_lambda3 {
		auto_params_lambda3(T0* that, Object& copy_v_) : this_(that), copy_v(copy_v_) {

		}
		template<typename T, typename U, typename Y>
		void operator()(T&& obj, U&& name, Y&& field) {
			this_->clear_field((obj.*field), (copy_v.*field), index_, index_blob_);
		}
		T0* this_;
		Object& copy_v;
		std::size_t index_ = 0;
		std::size_t index_blob_ = 0;
	};
	template<typename T0>
	struct auto_params_lambda4 {
		auto_params_lambda4(int& index_, MYSQL_BIND* bind_, T0* that) :index(index_), bind(bind_), this_(that) {

		}
		template<typename T>
		void operator()(T& v) {
			this_->bind_value(v, bind[index], true);
			++index;
		}
		int& index;
		MYSQL_BIND* bind;
		T0* this_;
	};
	template<typename T0>
	struct auto_params_lambda5 {
		auto_params_lambda5(MYSQL_BIND* bind_, T0* that) : bind(bind_), this_(that) {

		}
		template<typename T, typename U>
		void operator()(T& v, U& u) {
			this_->clear_field(v, u, index_, index_blob_);
		}
		MYSQL_BIND* bind;
		T0* this_;
		std::size_t index_ = 0;
		std::size_t index_blob_ = 0;
	};
	template<typename T>
	class stmt_guard {
	public:
		template<typename U>
		stmt_guard(U&& resource) :resource_(std::forward<U>(resource)) {

		}
	public:
		~stmt_guard() {
			if (resource_ != nullptr) {
				mysql_stmt_close(resource_);
			}
		}
	private:
		T* resource_ = nullptr;
	};
	class string_size_vec_clear {
	public:
		string_size_vec_clear(std::vector<unsigned long>& vec):vec_(vec){
			vec_.clear();
		}
		~string_size_vec_clear() {
			vec_.clear();
		}
	private:
		std::vector<unsigned long>& vec_;
	};
	class mysql final {
		template<typename T0>
		friend struct auto_params_lambda0;
		template<typename T0>
		friend struct auto_params_lambda1;
		template<typename T0>
		friend struct auto_params_lambda2;
		template<typename Object, typename T0>
		friend struct auto_params_lambda3;
		template<typename T0>
		friend struct auto_params_lambda4;
		template<typename T0>
		friend struct auto_params_lambda5;
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
		using Blob = std::vector<char>;
	private:
		template<typename T>
		typename std::enable_if<is_fundamention_type<typename std::remove_reference<T>::type>::value || is_date_type<typename std::remove_reference<T>::type>::value, bool>::type bind_value(T & t, MYSQL_BIND & bind, bool get = false) {
			using type = typename std::remove_reference<T>::type;
			bind.buffer_type = type::field_type;
			bind.buffer = t.buffer();
			bind.is_null = t.null_buffer();
			bind.length = 0;
			//bind.buffer_length = 0;
			return true;
		}
		template<typename T>
		typename std::enable_if<std::is_same<typename std::remove_reference<T>::type, std::string>::value,bool>::type bind_value(T& t, MYSQL_BIND& bind, bool get = false) {
			if (get) {
				record_string_size_.push_back(0);
				auto& data = record_string_size_.back();
				bind.length = &data;
				t.resize(string_max_size_);
			}
			else {
				bind.length = nullptr;
			}
			bind.buffer_type = MYSQL_TYPE_STRING;
			bind.buffer = &(t[0]);
			bind.is_null = 0;
			bind.buffer_length = (unsigned long)t.size();
			return true;
		}

		template<typename T>  //blob
		typename std::enable_if<std::is_same<typename std::remove_reference<T>::type, Blob>::value, bool>::type bind_value(T& t, MYSQL_BIND& bind, bool get = false) {
			if (get) {
				record_blob_size_.push_back(0);
				auto& data = record_blob_size_.back();
				bind.length = &data;
				t.resize(blob_max_size_);
			}
			else {
				bind.length = nullptr;
			}
			bind.buffer_type = MYSQL_TYPE_LONG_BLOB;
			bind.buffer = t.empty()? nullptr:&(t[0]);
			bind.is_null = 0;
			bind.buffer_length = (unsigned long)t.size();
			return true;
		}

		template<typename T, typename U>
		typename std::enable_if<std::is_same<typename std::remove_reference<T>::type, std::string>::value>::type clear_field(T& t, U& v,std::size_t& index, std::size_t& index_blob) {
			auto& size = record_string_size_[index];
			index++;
			if (size != 0) {
				v = std::string(&t[0], size);
			}
			else {
				v = std::string();
			}
			size = 0;
			//memset(&t[0], 0, t.size()); no use, optimized by record_string_size_
		}

		template<typename T, typename U>  //blob
		typename std::enable_if<std::is_same<typename std::remove_reference<T>::type, Blob>::value>::type clear_field(T& t, U& v, std::size_t& index, std::size_t& index_blob) {
			auto& size = record_blob_size_[index_blob];
			index_blob++;
			if (size != 0) {
				auto begin = &t[0];
				v = Blob(begin, begin + size);
			}
			else {
				v = Blob();
			}
			size = 0;
			//memset(&t[0], 0, t.size()); no use, optimized by record_string_size_
		}

		template<typename T, typename U>
		typename std::enable_if<!std::is_same<typename std::remove_reference<T>::type, std::string>::value && !std::is_same<typename std::remove_reference<T>::type, Blob>::value>::type clear_field(T& t, U& v, std::size_t& index, std::size_t& index_blob) {
			v = t;
			t.clear();
		}
	public:
		mysql() {
			init_error_default_callback();
		}
		mysql(dataBaseConfig const& config, std::function<void(std::string const&)> const& error_callback = nullptr) {
			if (error_callback == nullptr) {
				init_error_default_callback();
			}
			else {
				error_callback_ = error_callback;
			}
			connect(config);
		}
		~mysql() {
			disconnect();
		}
	private:
		void init_error_default_callback() {
			error_callback_ = [](std::string const& message) {
				std::cout << message << "\n";
			};
		}
	public:
		void connect(dataBaseConfig const& config) {
			db_index_key_ = config.index_key;
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
			else {
				trigger_error(mysql_error(conn_));
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
			return conn_ != nullptr && is_connect_;
		}

		bool ping() {
			if (conn_ == nullptr) {
				is_connect_ = false;
				return false;
			}
			bool r = mysql_ping(conn_) == 0;
			if (!r) {
				is_connect_ = false;
			}
			return r;
		}
	public:
		template<typename T>
		typename std::enable_if<reflector::is_reflect_class<typename std::remove_reference<T>::type>::value, db_result<void>>::type insert(T&& t) {
			auto meta = meta_info_reflect(t);
			std::string tablename = meta.get_class_name();
			std::stringstream ss;
			ss << "INSERT INTO `" << tablename << "` (";
			auto size = meta.element_size();
			int index = 0;
			std::string value_place = "";
			MYSQL_BIND bind[meta.element_size()];
			memset(bind, 0, sizeof(bind));
			auto_params_lambda0<mysql> lambda{ ss ,index ,size ,value_place ,bind,this };
			reflector::each_object(std::forward<T>(t), lambda);
			ss << ")" << " VALUES(" << value_place << ")";
			return stmt_execute(ss.str(), bind);
		}

		template<typename T,typename...U>
		typename std::enable_if<reflector::is_reflect_class<typename std::remove_reference<T>::type>::value, db_result<void>>::type del(std::string const& condition,U&&...args) {
			//static_assert((sizeof...(args)) != 0, "require at least one argument!!!");
			auto meta = meta_info_reflect(T{});
			MYSQL_BIND bind[sizeof...(args)+1];  //0 argument will be ill-formed
			std::string sql = "DELETE FROM `" + meta.get_class_name() + "` " + condition;
			auto tp = std::make_tuple(std::forward<U>(args)...);
			expand_bind_value(bind, tp, xorm_utils::make_index_package<sizeof...(args)>{});
			return stmt_execute(sql, bind);
		}

		template<typename T>
		typename std::enable_if<reflector::is_reflect_class<typename std::remove_reference<T>::type>::value, db_result<void>>::type update(T&& v) {
			auto meta = meta_info_reflect(v);
			std::string tablename = meta.get_class_name();
			std::stringstream ss;
			MYSQL_BIND bind[meta.element_size()];
			memset(bind, 0, sizeof(bind));
			ss << "replace  into `" << tablename << "` (";
			auto size = meta.element_size();
			int index = 0;
			std::string value_place = "";
			auto_params_lambda0<mysql> lambda{ ss ,index ,size ,value_place ,bind,this };
			reflector::each_object(std::forward<T>(v), lambda);
			ss << ")" << " VALUES(" << value_place << ")";
			return stmt_execute(ss.str(), bind);
		}

		void void_parameter_content(...) {

		}

		template<typename Bind,typename...T,std::size_t...Indexs>
		void expand_bind_value(Bind&& bind,std::tuple<T...>& tp, xorm_utils::index_package<Indexs...>) {
			void_parameter_content(bind_value(std::get<Indexs>(tp), bind[Indexs])...);
		}

		template<typename Bind, std::size_t...Indexs>
		void expand_bind_value(Bind&& bind, std::tuple<>& tp, xorm_utils::index_package<Indexs...>) {

		}

		template<typename...T>
		db_result<void> update(std::string const& sql,T&&...args) {
			//static_assert((sizeof...(args)) != 0, "require at least one argument!!!");
			MYSQL_BIND bind[sizeof...(args)+1];  //0 argument will be ill-formed
			auto tp = std::make_tuple(std::forward<T>(args)...);
			expand_bind_value(bind, tp, xorm_utils::make_index_package<sizeof...(args)>{});
			return stmt_execute(sql, bind);
		}

		//template<typename T>
		//typename std::enable_if<reflector::is_reflect_class<typename std::remove_reference<T>::type>::value, std::pair<bool, std::uint64_t>>::type update(T&& v, std::string const& condition) {
		//	auto meta = meta_info_reflect(v);
		//	std::string tablename = meta.get_class_name();
		//	std::stringstream ss;
		//	MYSQL_BIND bind[meta.element_size()];
		//	memset(bind, 0, sizeof(bind));
		//	ss << "UPDATE `" << tablename << "` SET ";
		//	auto size = meta.element_size();
		//	int index = 0;
		//	auto_params_lambda1<mysql> lambda1{ ss ,index ,size ,bind ,this };
		//	reflector::each_object(std::forward<T>(v), lambda1);
		//	ss << " " << condition;
		//	auto r =  stmt_execute(ss.str(), bind);
		//	return { r.first != 0,r.first };
		//}

		template<typename T,typename...Params>
		typename std::enable_if<reflector::is_reflect_class<typename std::remove_reference<T>::type>::value, db_result<T>>::type query(std::string const& condition , Params&&...params) {
			auto meta = meta_info_reflect(T{});
			std::string tablename = meta.get_class_name();
			std::stringstream ss;
			MYSQL_BIND bind[meta.element_size()];
			MYSQL_BIND bind_params[sizeof...(params) + 1];  //0 will ill-formed
			auto tp = std::make_tuple(std::forward<Params>(params)...);
			expand_bind_value(bind_params, tp, xorm_utils::make_index_package<sizeof...(params)>{});
			memset(bind, 0, sizeof(bind));
			auto name_arr = meta.get_element_names();
			ss << "SELECT ";
			auto size = name_arr.size();
			for (std::size_t index = 0; index < size; index++) {
				ss << "`" << name_arr[index] << "` ";
				if (index < (size - 1)) {
					ss << ",";
				}
			}
			ss << " FROM `" << tablename << "` " << condition;
			MYSQL_STMT* pStmt = mysql_stmt_init(conn_);
			stmt_guard<MYSQL_STMT> guard(pStmt);
			string_size_vec_clear string_guard{ record_string_size_ };
			string_size_vec_clear blob_guard{ record_blob_size_ };
			record_string_size_.reserve(meta.element_size()*2);
			record_blob_size_.reserve(meta.element_size() * 2);
			db_result<T> dbresult;
			if (pStmt != nullptr) {
				auto sqlStr = ss.str();
				int iRet = mysql_stmt_prepare(pStmt, sqlStr.c_str(), (unsigned long)sqlStr.size());
				if (iRet == 0) {
					iRet = mysql_stmt_bind_param(pStmt, bind_params);
					if (iRet != 0) {
						std::string error_message = mysql_error(conn_);
						trigger_error(error_message);
						dbresult.success = false;
						dbresult.error.is_error_ = true;
						dbresult.error.message_ = error_message;
						return dbresult;
					}
					T tmp{};
					int index = 0;
					auto_params_lambda2<mysql> lambda{ index ,bind ,this };
					reflector::each_object(tmp, lambda);
					bool r = mysql_stmt_bind_result(pStmt, bind);
					if (!r) {
						r = mysql_stmt_execute(pStmt);
						r |= mysql_stmt_store_result(pStmt) != 0;
						if (!r) {
							while (mysql_stmt_fetch(pStmt) == 0) {
								T copy_v{};
								auto_params_lambda3<T, mysql> lambda3{ this,copy_v };
								reflector::each_object(tmp, lambda3);
								dbresult.results.emplace_back(std::move(copy_v));
							}
							dbresult.success = true;
							return dbresult;
						}
						else {
							std::string error_message = mysql_error(conn_);
							trigger_error(error_message);
							dbresult.success = false;
							dbresult.error.is_error_ = true;
							dbresult.error.message_ = error_message;
							return dbresult;
						}
					}
					else {
						std::string error_message = mysql_error(conn_);
						trigger_error(error_message);
						dbresult.success = false;
						dbresult.error.is_error_ = true;
						dbresult.error.message_ = error_message;
						return dbresult;
					}
				}
				else {
					std::string error_message = mysql_error(conn_);
					trigger_error(error_message);
					dbresult.success = false;
					dbresult.error.is_error_ = true;
					dbresult.error.message_ = error_message;
					return dbresult;
				}
			}
			dbresult.success = false;
			return dbresult;
		}

		template<typename T, typename...Params>
		typename std::enable_if<xorm::is_tuple_type<T>::value, db_result<T>>::type query(std::string const& sqlStr ,Params&& ...params) {
			constexpr std::size_t tuple_size = std::tuple_size<T>::value;
			MYSQL_BIND bind[tuple_size];
			MYSQL_BIND bind_params[sizeof...(params) + 1];  //0 will ill-formed
			auto tp = std::make_tuple(std::forward<Params>(params)...);
			expand_bind_value(bind_params, tp, xorm_utils::make_index_package<sizeof...(params)>{});
			memset(bind, 0, sizeof(bind));
			MYSQL_STMT* pStmt = mysql_stmt_init(conn_);
			stmt_guard<MYSQL_STMT> guard(pStmt);
			string_size_vec_clear string_guard{ record_string_size_ };
			string_size_vec_clear blob_guard{ record_blob_size_ };
			record_string_size_.reserve(tuple_size*2);
			record_blob_size_.reserve(tuple_size*2);
			db_result<T> dbresult;
			if (pStmt != nullptr) {
				int iRet = mysql_stmt_prepare(pStmt, sqlStr.c_str(), (unsigned long)sqlStr.size());
				if (iRet == 0) {
					iRet = mysql_stmt_bind_param(pStmt, bind_params);
					if (iRet != 0) {
						std::string error_message = mysql_error(conn_);
						trigger_error(error_message);
						dbresult.success = false;
						dbresult.error.is_error_ = true;
						dbresult.error.message_ = error_message;
						return dbresult;
					}
					T tmp{};
					int index = 0;
					auto_params_lambda4<mysql> lambda4{ index ,bind,this };
					each_tuple<0, tuple_size>::each(tmp, lambda4);
					bool r = mysql_stmt_bind_result(pStmt, bind);
					if (!r) {
						r = mysql_stmt_execute(pStmt);
						r |= mysql_stmt_store_result(pStmt) != 0;
						if (!r) {
							while (mysql_stmt_fetch(pStmt) == 0) {
								T copy_v{};
								auto_params_lambda5<mysql> lambda5{ bind ,this };
								each_tuple<0, tuple_size>::each2(tmp, copy_v, lambda5);
								dbresult.results.emplace_back(std::move(copy_v));
							}
							dbresult.success = true;
							return dbresult;
						}
						else {
							std::string error_message = mysql_error(conn_);
							trigger_error(error_message);
							dbresult.success = false;
							dbresult.error.is_error_ = true;
							dbresult.error.message_ = error_message;
							return dbresult;
						}
					}
					else {
						std::string error_message = mysql_error(conn_);
						trigger_error(error_message);
						dbresult.success = false;
						dbresult.error.is_error_ = true;
						dbresult.error.message_ = error_message;
						return dbresult;
					}
				}
				else {
					std::string error_message = mysql_error(conn_);
					trigger_error(error_message);
					dbresult.success = false;
					dbresult.error.is_error_ = true;
					dbresult.error.message_ = error_message;
					return dbresult;
				}
			}
			dbresult.success = false;
			return dbresult;
		}

		db_result<void> execute(std::string const& sql) {
			db_result<void> dbresult;
			auto iRet = mysql_query(conn_, sql.c_str());
			bool r = iRet != 0 ? false : true;
			if (r) {
				dbresult.success = true;
				auto pRes = mysql_use_result(conn_);
				mysql_free_result(pRes);
			}
			else {
				std::string message = mysql_error(conn_);
				trigger_error(message);
				dbresult.success = false;
				dbresult.error.is_error_ = true;
				dbresult.error.message_ = message;
			}
			return dbresult;
		}

		db_result<void> execute(std::string const& sql, std::function<void(MYSQL_RES*)> const& data_callback) {
			auto iRet = mysql_query(conn_, sql.c_str());
			db_result<void> dbresult;
			bool r = iRet != 0 ? false : true;
			if (r) {
				dbresult.success = true;
				auto pRes = mysql_store_result(conn_);
				data_callback(pRes);
				mysql_free_result(pRes);
			}
			else {
				std::string message = mysql_error(conn_);
				trigger_error(message);
				data_callback(nullptr);
				dbresult.success = false;
				dbresult.error.is_error_ = true;
				dbresult.error.message_ = message;
			}
			return dbresult;
		}

		std::uint64_t get_affected_rows() {
			return mysql_affected_rows(conn_);
		}

		db_result<void> begin() {
			return execute("BEGIN");
		}
		db_result<void> commit() {
			return execute("COMMIT");
		}
		db_result<void> rollback() {
			return execute("ROLLBACK");
		}
		void set_max_string_size(std::size_t size) {
			string_max_size_ = size;
		}
		std::size_t get_max_string_size() {
			return string_max_size_;
		}
		void set_max_blob_size(std::size_t size) {
			blob_max_size_ = size;
		}
		std::size_t get_max_blob_size() {
			return blob_max_size_;
		}
	private:
		db_result<void> stmt_execute(std::string const& sqlStr, MYSQL_BIND* bind) {
			MYSQL_STMT* pStmt = mysql_stmt_init(conn_);
			stmt_guard<MYSQL_STMT> guard(pStmt);
			db_result<void> result;
			if (pStmt != nullptr) {
				//begin();
				int iRet = mysql_stmt_prepare(pStmt, sqlStr.data(), (unsigned long)sqlStr.size());
				if (iRet == 0) {
					iRet = mysql_stmt_bind_param(pStmt, bind);
					if (iRet == 0) {
						iRet = mysql_stmt_execute(pStmt);
						if (iRet == 0) {
							auto rows = mysql_stmt_affected_rows(pStmt);
								//bool cr = commit();
							auto key_id = mysql_insert_id(conn_);
							result.success = true;
							result.affect_rows = rows;
							result.unique_id = key_id;
							return result;
						}
						else {
							std::string error_message = mysql_error(conn_);
							trigger_error(error_message);
							result.affect_rows = 0;
							result.success = false;
							result.unique_id = 0;
							result.error.is_error_ = true;
							result.error.message_ = error_message;
							return result;
						}
					}
					else {
						std::string error_message = mysql_error(conn_);
						trigger_error(error_message);
						result.affect_rows = 0;
						result.success = false;
						result.unique_id = 0;
						result.error.is_error_ = true;
						result.error.message_ = error_message;
						return result;
					}
				}
				else {
					std::string error_message = mysql_error(conn_);
					trigger_error(error_message);
					result.affect_rows = 0;
					result.success = false;
					result.unique_id = 0;
					result.error.is_error_ = true;
					result.error.message_ = error_message;
					return result;
				}
				//rollback();
			}
			return result;
		}
	public:
		void set_error_callback(std::function<void(std::string const&)> const& callback) {
			if (callback != nullptr) {
				error_callback_ = callback;
			}
		}
		void trigger_error(std::string const& message) {
			if (error_callback_ != nullptr) {
				error_callback_("db_index: "+ db_index_key_+ " , error: "+ message);
			}
		}
		MYSQL* get_raw_connetion() {
			return is_connect_ == true ? conn_ : nullptr;
		}
	private:
		MYSQL* conn_ = nullptr;
		bool is_connect_ = false;
		std::size_t string_max_size_ = 1024 * 1024;
		std::size_t blob_max_size_ = 3* 1024 * 1024;
		std::function<void(std::string const&)> error_callback_;
		std::vector<unsigned long> record_string_size_;
		std::vector<unsigned long> record_blob_size_;
		std::string db_index_key_;
	};
}
#endif // ENABLE_MYSQL