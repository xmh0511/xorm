#pragma once
#include "simple_pool.hpp"
#include <atomic>
#include <utility>
namespace xorm {
	struct dataBaseConfig {
		std::string host;
		std::string user;
		std::string password;
		std::string dbname;
		unsigned int port;
		std::size_t conn_number;
	};

	template<typename T>
	struct Pool {
		Pool(std::size_t size, dataBaseConfig const& config):pool_(size){
			pool_.init_pool([config](auto& iter) {
				iter = std::make_shared<T>(config.host, config.user, config.password, config.dbname, config.port);
			});
		}
		simple_pool<T> pool_;
	};
	inline dataBaseConfig& init_database_config(dataBaseConfig const& v = {}) {
		static dataBaseConfig config{ v };
		return config;
	}
	template<typename DataBaseType>
	class dao {
	private:
		static simple_pool<DataBaseType>& get_conn_pool() {
			auto& config = init_database_config();
			static Pool<DataBaseType> pool{ config.conn_number,config };
			return pool.pool_;
		}
	public:
		dao() {
			simple_pool<DataBaseType>& pool = get_conn_pool();
			conn_ = pool.takeout();
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
		bool update(T&& v, std::string const& condition = "") {
			if (!conn_->is_connect()) {
				return false ;
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
			return conn_->commit();
		}

		bool rollback() {
			return conn_->rollback();
		}

		bool execute(std::string const& sql) {
			return conn_->execute(sql);
		}

		void start_transaction() {
			begin();
			start_transaction_ = true;
		}

	public:
		~dao() {
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