#pragma once
#include "simple_pool.hpp"
#include <atomic>
namespace xorm {
	template<typename T>
	struct Pool {
		simple_pool<T> pool_;
		std::atomic_bool is_init = false;
	};
	struct dataBaseConfig {
		std::string host;
		std::string user;
		std::string password;
		std::string dbname;
		unsigned int port;
		std::atomic_bool is_init = false;
	};
	template<typename DataBaseType>
	class dao {
	public:
		static dataBaseConfig& init_config(dataBaseConfig const& v = {}) {
			static dataBaseConfig config;
			if (!config.is_init) {
				config = v;
				config.is_init = true;
			}
			return config;
		}
		static simple_pool<DataBaseType>& get_conn_pool(std::size_t size = 0) {
			static Pool<DataBaseType> pool;
			if (!pool.is_init) {
				pool.pool_.expand(size);
				auto& config = init_config();
				pool.pool_.init_pool([&config](auto& iter) {
					iter->connect(config.host, config.user, config.password,config.dbname, config.port);
				})
				pool.is_init = true;
			}
			return pool.pool_;
		}
	public:
		dao() {
			simple_pool<DataBaseType>& pool = get_conn_pool();
			conn_ = pool.takeout();
		}
	public:

	public:
		~dao() {
			simple_pool<DataBaseType>& pool = get_conn_pool();
			pool.revert(conn_);
		}
	private:
		std::shared_ptr<DataBaseType> conn_;
	};
}