#pragma once
#ifdef  XORM_ENABLE_SQLITE
#include <sqlite3.h>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <memory>
#include <cstring>
#include <tuple>
#include <functional>
#include "reflect/reflector.hpp"
#include "dbconfig.hpp"
#include "meta_utility.hpp"
#include "db_utils.hpp"
namespace xorm {

	template<typename Type>
	class SqliteFundamentionType final {
		friend std::ostream& operator <<(std::ostream& out, SqliteFundamentionType const& v) {
			out << v.value();
			return out;
		}
	public:
		using value_type = Type;
	public:
		SqliteFundamentionType() :data_(0), is_null_(true) {

		}
		SqliteFundamentionType(Type v) :data_(v), is_null_(false) {

		}
		SqliteFundamentionType& operator=(Type v) {
			data_ = v;
			is_null_ = false;
			return *this;
		}

		bool is_null() const {
			return is_null_;
		}

		void clear() {
			data_ = 0;
			is_null_ = true;
		}
		Type value() const {
			return data_;
		}
	private:
		Type data_;
		bool is_null_;
	};
	template<typename T>
	struct is_sqlitefundametion_type {
		constexpr static bool value = false;
	};

	template<typename T>
	struct is_sqlitefundametion_type<SqliteFundamentionType<T>> {
		constexpr static bool value = true;
	};

	//template<typename T>
	//typename std::enable_if<is_sqlitefundametion_type<typename std::remove_reference<T>::type>::value, bool>::type is_null_value_for_field(T&& t) {
	//	return t.is_null();
	//}

	//template<typename T>
	//typename std::enable_if<!is_sqlitefundametion_type<typename std::remove_reference<T>::type>::value, bool>::type is_null_value_for_field(T&& t) {
	//	return t.empty();
	//}

	class sqlite final {
	public:
		using Integer = SqliteFundamentionType<int>;
		using Double = SqliteFundamentionType<double>;
		using Int64 = SqliteFundamentionType<sqlite3_int64>;
		using Blob = std::vector<char>;
		struct SQLITE_RES {
			int argu1;
			char** argu2;
			char** argu3;
		};
	protected:
		template<typename T>
		class stmt_guard {
		public:
			template<typename U>
			stmt_guard(U&& resource) :resource_(std::forward<U>(resource)) {

			}
		public:
			~stmt_guard() {
				if (resource_ != nullptr) {
					sqlite3_finalize(resource_);
				}
			}
		private:
			T* resource_ = nullptr;
		};

		template<typename T0>
		struct auto_params_lambda0 {
			auto_params_lambda0(std::stringstream& ss_, int& index_, std::size_t size_, std::string& value_place_, T0* that) :ss(ss_), index(index_), size(size_), value_place(value_place_),this_(that) {

			}
			template<typename T, typename U, typename Y>
			void operator()(T&& obj, U&& name, Y&& field) {

				if (index < (size - 1)) {
					ss << "'" << name << "' " << ",";
					value_place.append("?,");
				}
				else if (index == (size - 1)) {
					ss << "'" << name << "' ";
					value_place.append("?");
				}
				++index;
			}
			std::stringstream& ss;
			int& index;
			std::size_t size;
			std::string& value_place;
			T0* this_;
		};

		template<typename T0>
		struct auto_params_lambda1 {
			auto_params_lambda1(std::stringstream& ss_, int& index_, std::size_t size_, T0* that) :ss(ss_), index(index_), size(size_), this_(that) {

			}
			template<typename T, typename U, typename Y>
			void operator()(T&& obj, U&& name, Y&& field) {

				if (index < (size - 1)) {
					ss << "`" << name << "` " << ",";
				}
				else if (index == (size - 1)) {
					ss << "`" << name << "` ";
				}
				++index;
			}
			std::stringstream& ss;
			int& index;
			std::size_t size;
			T0* this_;
		};
	private:
		template<typename T>
		typename std::enable_if<is_sqlitefundametion_type<typename std::remove_reference<T>::type>::value && std::is_same<typename std::remove_reference<T>::type::value_type,int>::value,int>::type bind_value(sqlite3_stmt* stmt,T&& v,std::size_t index,bool get = false) {
			if (get) {
				 v = sqlite3_column_int(stmt, (int)index);
				 return 0;
			}
			if (v.is_null()) {
				return sqlite3_bind_null(stmt, (int)index);
			}
			return  sqlite3_bind_int(stmt, (int)index, v.value());
		}

		template<typename T>
		typename std::enable_if<is_sqlitefundametion_type<typename std::remove_reference<T>::type>::value&& std::is_same<typename std::remove_reference<T>::type::value_type, double>::value, int>::type bind_value(sqlite3_stmt* stmt, T&& v, std::size_t index, bool get = false) {
			if (get) {
				v = sqlite3_column_double(stmt, (int)index);
				return 0;
			}
			if (v.is_null()) {
				return sqlite3_bind_null(stmt, (int)index);
			}
			return  sqlite3_bind_double(stmt, (int)index, v.value());
		}

		template<typename T>
		typename std::enable_if<is_sqlitefundametion_type<typename std::remove_reference<T>::type>::value&& std::is_same<typename std::remove_reference<T>::type::value_type, sqlite3_int64>::value, int>::type bind_value(sqlite3_stmt* stmt, T&& v, std::size_t index, bool get = false) {
			if (get) {
				v = sqlite3_column_int64(stmt, (int)index);
				return 0;
			}
			if (v.is_null()) {
				return sqlite3_bind_null(stmt, (int)index);
			}
			return sqlite3_bind_int64(stmt, (int)index, v.value());
		}

		template<typename T>
		typename std::enable_if<!is_sqlitefundametion_type<typename std::remove_reference<T>::type>::value&& std::is_same<typename std::remove_reference<T>::type, std::string>::value, int>::type bind_value(sqlite3_stmt* stmt, T&& v, std::size_t index, bool get = false) {
			if (get) {
				auto size = (std::size_t)sqlite3_column_bytes(stmt, (int)index);
				v = std::string((char*)sqlite3_column_text(stmt, (int)index), size);
				return 0;
			}
			return sqlite3_bind_text(stmt, (int)index, v.data(),(int)v.size(),nullptr);
		}

		template<typename T>
		typename std::enable_if<!is_sqlitefundametion_type<typename std::remove_reference<T>::type>::value&& std::is_same<typename std::remove_reference<T>::type, Blob>::value, int>::type bind_value(sqlite3_stmt* stmt, T&& v, std::size_t index, bool get = false) {
			if (get) {
				auto size = (std::size_t)sqlite3_column_bytes(stmt, (int)index);
				auto start = (char*)sqlite3_column_blob(stmt, (int)index);
				v = Blob(start, start + size);
				return 0;
			}
			return sqlite3_bind_blob(stmt, (int)index, v.data(), (int)v.size(), nullptr);
		}

		template<typename...T>
		int void_parameter_content(T...args) {
			auto range = sizeof...(T);
			int arr[sizeof...(T)+1] = { args... };
			int r = SQLITE_OK;
			for (auto i = 0; i < range; i++) {
				r = r | arr[i];
			}
			return r;
		}

		template<typename T,typename Tuple,std::size_t...Indexs>
		int bind_params_with_obj(sqlite3_stmt* stmt,T&& obj, Tuple&& tup, xorm_utils::index_package<Indexs...>) {
			return void_parameter_content(bind_value(stmt, (obj.*(std::get<Indexs>(tup))), Indexs+1)...);
		}

		template<typename Tuple, std::size_t...Indexs>
		int bind_params_without_obj(sqlite3_stmt* stmt,Tuple&& tup, xorm_utils::index_package<Indexs...>) {
			return void_parameter_content(bind_value(stmt, std::get<Indexs>(tup), Indexs + 1)...);
		}

		template<typename Tuple, std::size_t...Indexs>
		int get_row_data_for_tuple(sqlite3_stmt* stmt, Tuple&& tup, xorm_utils::index_package<Indexs...>, bool get = false) {
			return void_parameter_content(bind_value(stmt, std::get<Indexs>(tup), Indexs, get)...);
		}

		template<typename T,typename Tuple, std::size_t...Indexs>
		int get_row_data_for_obj(sqlite3_stmt* stmt,T&& obj,Tuple&& tup, xorm_utils::index_package<Indexs...>) {
			return void_parameter_content(bind_value(stmt, (obj.*(std::get<Indexs>(tup))), Indexs,true)...);
		}
	private:
		void trigger_error(std::string const& msg) {
			if (error_callback_ != nullptr) {
				error_callback_("db_index: " + db_index_key_ + " , error: " + msg);
			}
		}
		void set_error_callback(std::function<void(std::string const&)> const& callback) {
			if (callback != nullptr) {
				error_callback_ = callback;
			}
		}
		void init_error_default_callback() {
			error_callback_ = [](std::string const& message) {
				std::cout << message << "\n";
			};
		}
	public:
		sqlite() {
			init_error_default_callback();
		}
		sqlite(dataBaseConfig const& config, std::function<void(std::string const&)> const& error_callback = nullptr) {
			if (error_callback == nullptr) {
				init_error_default_callback();
			}
			else {
				error_callback_ = error_callback;
			}
			connect(config);
		}
		~sqlite() {
			disconnect();
		}
		bool connect(dataBaseConfig const& config) {
			db_index_key_ = config.index_key;
			auto r = sqlite3_open(config.host.c_str(), &sqlite_handler_);
			if (r == SQLITE_OK) {  //表示连接成功
#ifdef SQLITE_HAS_CODEC
				if (!config.password.empty()) {
					sqlite3_key(sqlite_handler_, config.password.c_str(), (int)config.password.size());
				}
#endif
				is_connect_ = true;
				return true;
			}
			return false;
		}
		void reconnect(dataBaseConfig const& config) {
			disconnect();
			connect(config);
		}
		void disconnect() {
			if (sqlite_handler_ != nullptr) {
				sqlite3_close(sqlite_handler_);
				sqlite_handler_ = nullptr;
				is_connect_ = false;
			}
		}
		bool ping() {
			return is_connect_;
		}
		bool is_connect() {
			return sqlite_handler_ != nullptr && is_connect_;
		}
	public:
		template<typename T,typename  = std::enable_if<reflector::is_reflect_class<typename std::remove_reference<T>::type>::value>>
		db_result<void> insert(T&& obj) {
			auto meta = meta_info_reflect(obj);
			std::string tablename = meta.get_class_name();
			std::stringstream ss;
			ss << "INSERT INTO '" << tablename << "' (";
			auto size = meta.element_size();
			int index = 0;
			std::string value_place = "";
			sqlite3_stmt* stmt = nullptr;
			auto_params_lambda0<sqlite> lambda{ ss ,index ,size ,value_place ,this };
			reflector::each_object(std::forward<T>(obj), lambda);
			ss << ")" << " VALUES(" << value_place << ")";
			auto sqlStr = ss.str();
			return stmt_execute_with_obj(sqlStr, std::forward<T>(obj), meta.get_element_meta_protype());
		}
		template<typename T, typename = std::enable_if<reflector::is_reflect_class<typename std::remove_reference<T>::type>::value>,typename...Params>
		db_result<void> del(std::string const& condition, Params&&...params) {
			auto meta = meta_info_reflect(T{});
			std::stringstream ss;
			ss << "DELETE FROM '" << meta.get_class_name() << "' "<< condition;
			return stmt_execute(ss.str(), std::forward<Params>(params)...);
		}

		template<typename T, typename = std::enable_if<reflector::is_reflect_class<typename std::remove_reference<T>::type>::value>>
		db_result<void> update(T&& obj) {
			auto meta = meta_info_reflect(obj);
			std::stringstream ss;
			ss << "replace into '" << meta.get_class_name() << "' (";
			int index = 0;
			auto size = meta.element_size();
			std::string value_place = "";
			auto_params_lambda0<sqlite> lambda{ ss ,index ,size ,value_place ,this };
			reflector::each_object(std::forward<T>(obj), lambda);
			ss << ")" << " VALUES(" << value_place << ")";
			auto sqlStr = ss.str();
			return  stmt_execute_with_obj(sqlStr, std::forward<T>(obj), meta.get_element_meta_protype());
		}

		template<typename...Params>
		db_result<void> update(std::string const& sqlStr ,Params&& ...params) {
			return stmt_execute(sqlStr, std::forward<Params>(params)...);
		}

		template<typename T, typename...Params>
		typename std::enable_if <reflector::is_reflect_class<T>::value, db_result<T>>::type query(std::string const& condition, Params&& ...params) {
			auto meta = meta_info_reflect(T{});
			std::stringstream ss;
			ss << "SELECT ";
			int index = 0;
			auto size = meta.element_size();
			auto_params_lambda1<sqlite> lambda{ ss,index ,size ,this };
			reflector::each_object(T{}, lambda);
			ss << " FROM '" << meta.get_class_name() << "' "<< condition;
			return get_obj_stmt_execute<T>(ss.str(), std::forward<Params>(params)...);
		}

		template<typename Type,typename...Params>
		typename std::enable_if<(!reflector::is_reflect_class<Type>::value && is_tuple_type<Type>::value), db_result<Type> >::type query(std::string const& sqlStr, Params&& ...params) {
			return get_tuple_stmt_execute<Type>(sqlStr, std::forward<Params>(params)...);
		}

		db_result<void> execute(const std::string& sql) {
			db_result<void> dbresult;
			if (sqlite3_exec(sqlite_handler_, sql.data(), nullptr, nullptr, nullptr) != SQLITE_OK) {
				std::string message = sqlite3_errmsg(sqlite_handler_);
				trigger_error(message);
				dbresult.success = false;
				dbresult.error.is_error_ = true;
				dbresult.error.message_ = message;
				return dbresult;
			}
			dbresult.success = true;
			return dbresult;
		}
		db_result<void> execute(std::string const& sql, std::function<void(SQLITE_RES*)> const& data_callback) {
			db_result<void> dbresult;
			auto callback = [](void* a0, int a1, char** a2, char** a3)->int {
				SQLITE_RES r{ a1,a2,a3 };
				auto fun = static_cast<std::function<void(SQLITE_RES*)>*>(a0);
				(*fun)(&r);
				return 0;
			};
			if (sqlite3_exec(sqlite_handler_, sql.data(), callback, const_cast<std::function<void(SQLITE_RES*)>*>(&data_callback), nullptr) != SQLITE_OK) {
				std::string message = sqlite3_errmsg(sqlite_handler_);
				trigger_error(message);
				dbresult.success = false;
				dbresult.error.is_error_ = true;
				dbresult.error.message_ = message;
				return dbresult;
			}
			dbresult.success = true;
			return dbresult;
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
		std::uint64_t get_affected_rows() {
			return sqlite3_changes(sqlite_handler_);
		}
	private:
		template<typename...Params>
		db_result<void> stmt_execute(std::string const& sqlStr, Params&& ...params) {
			sqlite3_stmt* stmt = nullptr;
			auto r = sqlite3_prepare_v2(sqlite_handler_, sqlStr.c_str(), (int)sqlStr.size(), &stmt, nullptr);
			stmt_guard<sqlite3_stmt> guard{ stmt };
			db_result<void> dbresult;
			if (r == SQLITE_OK) {
				auto params_tuple = std::make_tuple(params...);
				auto result = bind_params_without_obj(stmt, params_tuple, xorm_utils::make_index_package<sizeof...(params)>{});
				if (result == SQLITE_OK) {
					result = sqlite3_step(stmt);
					if (result == SQLITE_DONE) {
						auto effect_rows = get_affected_rows();
						dbresult.affect_rows = effect_rows;
						dbresult.success = true;
						dbresult.unique_id = sqlite3_last_insert_rowid(sqlite_handler_);
						return dbresult;
					}
				}
			}
			std::string error_message = sqlite3_errmsg(sqlite_handler_);
			trigger_error(error_message);
			dbresult.success = false;
			dbresult.error.is_error_ = true;
			dbresult.error.message_ = error_message;
			return dbresult;
		}

		template<typename T, typename = std::enable_if<reflector::is_reflect_class<typename std::remove_reference<T>::type>::value>,typename Tuple>
		db_result<void> stmt_execute_with_obj(std::string const& sqlStr, T&& obj, Tuple&& tup) {
			sqlite3_stmt* stmt = nullptr;
			auto r = sqlite3_prepare_v2(sqlite_handler_, sqlStr.c_str(), (int)sqlStr.size(), &stmt, nullptr);
			stmt_guard<sqlite3_stmt> guard{ stmt };
			db_result<void> dbresult;
			if (r == SQLITE_OK) {
				auto result = bind_params_with_obj(stmt,std::forward<T>(obj), std::forward<Tuple>(tup), xorm_utils::make_index_package<std::tuple_size<typename std::remove_reference<Tuple>::type>::value>{});
				if (result == SQLITE_OK) {
					result = sqlite3_step(stmt);
					if (result == SQLITE_DONE) {
						dbresult.affect_rows = get_affected_rows();
						dbresult.success = true;
						dbresult.unique_id = sqlite3_last_insert_rowid(sqlite_handler_);
						return dbresult;
					}
				}
			}
			std::string error_message = sqlite3_errmsg(sqlite_handler_);
			trigger_error(error_message);
			dbresult.success = false;
			dbresult.error.is_error_ = true;
			dbresult.error.message_ = error_message;
			return dbresult;
		}

	private:
		template<typename T,typename...Params>
		db_result<T> get_obj_stmt_execute(std::string const& sqlStr, Params&& ...params) {
			sqlite3_stmt* stmt = nullptr;
			auto r = sqlite3_prepare_v2(sqlite_handler_, sqlStr.c_str(), -1, &stmt, nullptr);
			stmt_guard<sqlite3_stmt> guard{ stmt };
			db_result<T> dbresult;
			if (r == SQLITE_OK) {
				auto params_tuple = std::make_tuple(params...);
				auto result = bind_params_without_obj(stmt, params_tuple, xorm_utils::make_index_package<sizeof...(params)>{});
				if (result == SQLITE_OK) {
					while (sqlite3_step(stmt)== SQLITE_ROW) {
						T tmp{};
						auto meta = meta_info_reflect(tmp);
						get_row_data_for_obj(stmt, tmp, meta.get_element_meta_protype(), xorm_utils::make_index_package<meta.element_size()>{});
						dbresult.results.emplace_back(tmp);
					}
					dbresult.success = true;
					return dbresult;
				}
				else {
					std::string error_message = sqlite3_errmsg(sqlite_handler_);
					trigger_error(error_message);
					dbresult.success = false;
					dbresult.error.is_error_ = true;
					dbresult.error.message_ = error_message;
					return dbresult;
				}
			}
			else {
				std::string error_message = sqlite3_errmsg(sqlite_handler_);
				trigger_error(error_message);
				dbresult.success = false;
				dbresult.error.is_error_ = true;
				dbresult.error.message_ = error_message;
				return dbresult;
			}
			return dbresult;
		}

		template<typename T, typename...Params>
		db_result<T> get_tuple_stmt_execute(std::string const& sqlStr, Params&& ...params) {
			sqlite3_stmt* stmt = nullptr;
			auto r = sqlite3_prepare_v2(sqlite_handler_, sqlStr.c_str(), -1, &stmt, nullptr);
			stmt_guard<sqlite3_stmt> guard{ stmt };
			db_result<T> dbresult;
			if (r == SQLITE_OK) {
				auto params_tuple = std::make_tuple(params...);
				auto result = bind_params_without_obj(stmt, params_tuple, xorm_utils::make_index_package<sizeof...(params)>{});
				if (result == SQLITE_OK) {
					while (sqlite3_step(stmt) == SQLITE_ROW) {
						T tmp{};
						get_row_data_for_tuple(stmt, tmp, xorm_utils::make_index_package<std::tuple_size<T>::value>{},true);
						dbresult.results.emplace_back(tmp);
					}
					dbresult.success = true;
					return dbresult;
				}
				else {
					std::string error_message = sqlite3_errmsg(sqlite_handler_);
					trigger_error(error_message);
					dbresult.success = false;
					dbresult.error.is_error_ = true;
					dbresult.error.message_ = error_message;
					return dbresult;
				}
			}
			else {
				std::string error_message = sqlite3_errmsg(sqlite_handler_);
				trigger_error(error_message);
				dbresult.success = false;
				dbresult.error.is_error_ = true;
				dbresult.error.message_ = error_message;
				return dbresult;
			}
			return dbresult;
		}
	public:
		sqlite3* get_raw_connetion() {
			return is_connect_ == true ? sqlite_handler_ : nullptr;
		}
	private:
		bool is_connect_ = false;
		sqlite3* sqlite_handler_ = nullptr;
		std::function<void(std::string const&)> error_callback_;
		std::string db_index_key_;
	};
}
#endif //  ENABLE_SQLITE