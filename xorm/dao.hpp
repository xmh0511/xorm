#pragma once
#include "simple_pool.hpp"
#include <atomic>
#include <utility>
#include "dbconfig.hpp"
namespace xorm {

	class dao_message {
	public:
		static dao_message& get() {
			static dao_message instance;
			return instance;
		}
	private:
		dao_message() {
			error_callback_ = [](std::string const& message) {
				std::cout << message << std::endl;
			};
		}
	public:
		void set_error_callback(std::function<void(std::string const&)> const& callback) {
			if (callback != nullptr) {
				error_callback_ = callback;
			}
		}
		void trigger_error(std::string const& message) {
			if (error_callback_ != nullptr) {
				error_callback_(message);
			}
		}
	private:
		std::function<void(std::string const&)> error_callback_;
	};

	template<typename T>
	struct Pool {
		Pool(std::size_t size, dataBaseConfig const& config):pool_(size){
			pool_.init_pool([&config](std::shared_ptr<T>& iter) {
				iter = std::make_shared<T>(config, [](std::string const& message) {
					dao_message::get().trigger_error(message);
				});
			});
		}
		simple_pool<T> pool_;
	};
	inline dataBaseConfig& init_database_config(dataBaseConfig const& v = {}) {
		static dataBaseConfig config{ v };
		return config;
	}

	template<typename DataBaseType>
	class dao_t {
	public:
		static simple_pool<DataBaseType>& get_conn_pool() {
			auto& config = init_database_config();
			static Pool<DataBaseType> pool{ config.conn_number,config };
			return pool.pool_;
		}
	public:
		dao_t() {
			simple_pool<DataBaseType>& pool = get_conn_pool();
			conn_ = pool.takeout();
			//conn_->set_error_callback([this](std::string const& message) {
			//	dao_message::get().trigger_error(message);
			//});
			if (!conn_->ping()) {
				auto& config = init_database_config();
				conn_->reconnect(config);
			}
		}
	public:
		/*return type   
		  first:effective row number
		  second: insert data's id(key)
		*/
		template<typename T> 
		std::pair<std::int64_t, std::int64_t> insert(T&& t) {
			if (!conn_->is_connect()) {
				return {0,0};
			}
			return conn_->insert(std::forward<T>(t));
		}
		template<typename T>
		bool del(std::string const& condition) {
			if (!conn_->is_connect()) {
				return false;
			}
			return conn_->template del<T>(condition);
		}

		template<typename T>
		bool update(T&& v) {
			if (!conn_->is_connect()) {
				return false ;
			}
			return conn_->update(std::forward<T>(v));
		}

		template<typename T>
		std::pair<std::int64_t, std::int64_t> update(T&& v, std::string const& condition) {
			if (!conn_->is_connect()) {
				return {0,0};
			}
			return conn_->update(std::forward<T>(v), condition);
		}

		template<typename T>
		std::pair<bool, std::vector<T>> query(std::string const& condition = "") {
			if (!conn_->is_connect()) {
				return { false,{} };
			}
			return conn_->template query<T>(condition);
		}
		bool begin() {
			return conn_->begin();
		}

		bool commit() {
			bool b =  conn_->commit();
			start_transaction_ = !b;
			return b;
		}

		bool rollback() {
			start_transaction_ = false;
			return conn_->rollback();
		}

		bool execute(std::string const& sql) {
			return conn_->execute(sql);
		}

		template<typename T>
		bool execute(std::string const& sql,std::function<void(T)> const& callback) {
			return conn_->execute(sql, callback);
		}

		void start_transaction() {
			begin();
			start_transaction_ = true;
		}

		bool is_open() {
			return conn_->is_connect();
		}
	public:
		~dao_t() {
			if (start_transaction_) {
				commit();
			}
			simple_pool<DataBaseType>& pool = get_conn_pool();
			pool.revert(conn_);
		}
	private:
		std::shared_ptr<DataBaseType> conn_;
		bool start_transaction_ = false;
	};
}