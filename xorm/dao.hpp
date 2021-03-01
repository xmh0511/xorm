#pragma once
#include <iostream>
#include "simple_pool.hpp"
#include <atomic>
#include <utility>
#include <functional>
#include <map>
#include "reflect/reflector.hpp"
#include "db_utils.hpp"
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

	struct pool_impl {
		pool_impl() = default;
		virtual ~pool_impl(){}
	};

	template<typename T>
	struct Pool:pool_impl {
		Pool(std::size_t size, dataBaseConfig const& config):pool_(size){
			pool_.init_pool([&config](std::shared_ptr<T>& iter) {
				iter = std::make_shared<T>(config, [](std::string const& message) {
					dao_message::get().trigger_error(message);
				});
			});
		}
		simple_pool<T> pool_;
	};


	struct data_base {
		std::shared_ptr<dataBaseConfig> config_ = nullptr;
		std::shared_ptr<pool_impl> pool_ = nullptr;
	};

	inline std::unordered_map<std::string,std::shared_ptr<data_base>>& get_database_configs() {
		static std::unordered_map<std::string, std::shared_ptr<data_base>> instance_;
		return instance_;
	}

	template<typename DataBaseType>
	class dao_t {
	public:
		static void init_conn_pool(dataBaseConfig const& config) {
			auto data_base_ptr = std::shared_ptr<data_base>(new data_base{});
			data_base_ptr->config_ = std::make_shared<dataBaseConfig>(config);
			auto pool = std::shared_ptr<Pool<DataBaseType>>{ new Pool<DataBaseType>{data_base_ptr->config_->conn_number,*(data_base_ptr->config_)} };
			data_base_ptr->pool_ = pool;
			auto&& configs_map = get_database_configs();
			configs_map.insert(std::make_pair(config.index_key, data_base_ptr));
		}

		static std::shared_ptr<data_base>  get_data_base(std::string const& indexKey) {
			auto&& configs_map = get_database_configs();
			auto iter = configs_map.find(indexKey);
			if (iter != configs_map.end()) {
				return iter->second;
			}
			return nullptr;
		}

	public:
		dao_t(std::string const& indexKey){
			std::shared_ptr<data_base> data_base_ptr = get_data_base(indexKey);
			if (data_base_ptr != nullptr) {
				data_base_ = data_base_ptr;
				auto current_pool = std::dynamic_pointer_cast<Pool<DataBaseType>>(data_base_ptr->pool_);
				if (current_pool != nullptr) {
					conn_ = current_pool->pool_.takeout();
					if (!conn_->ping()) {
						conn_->reconnect(*(data_base_ptr->config_));
					}
				}
				else {
					conn_ = nullptr;
				}
			}
			else {
				conn_ = nullptr;
			}

		}
	public:
		/*return type   
		  first:effective row number
		  second: insert data's id(key)
		*/
		template<typename T> 
		db_result<void> insert(T&& t) {
			if (conn_ ==nullptr || !conn_->is_connect()) {
				return {};
			}
			return conn_->insert(std::forward<T>(t));
		}
		template<typename T,typename...U>
		db_result<void> del(std::string const& condition,U&&...args) {
			if (conn_ == nullptr || !conn_->is_connect()) {
				return {};
			}
			return conn_->template del<T>(condition,std::forward<U>(args)...);
		}

		template<typename T,typename  = typename std::enable_if<reflector::is_reflect_class<typename std::remove_reference<T>::type>::value>::type>
		db_result<void> update(T&& v) {
			if (conn_ == nullptr ||  !conn_->is_connect()) {
				return {};
			}
			return conn_->update(std::forward<T>(v));
		}

		template<typename...T>
		db_result<void> update(std::string const& condition,T&&...args) {
			if (conn_ == nullptr || !conn_->is_connect()) {
				return {};
			}
			return conn_->update(condition,std::forward<T>(args)...);
		}

		//template<typename T>
		//std::pair<bool, std::vector<T>> query(std::string const& condition = "") {
		//	if (!conn_->is_connect()) {
		//		return { false,{} };
		//	}
		//	return conn_->template query<T>(condition);
		//}

		template<typename T,typename...Params>
		db_result<T> query(std::string const& condition, Params&&...params) {
			if (conn_ == nullptr || !conn_->is_connect()) {
				return {};
			}
			return conn_->template query<T>(condition,std::forward<Params>(params)...);
		}

		db_result<void> begin() {
			if (conn_ == nullptr) {
				return {};
			}
			return conn_->begin();
		}

		db_result<void> commit() {
			if (conn_ == nullptr) {
				start_transaction_ = false;
				return {};
			}
			auto r =  conn_->commit();
			start_transaction_ = !r.success;
			return r;
		}

		db_result<void> rollback() {
			start_transaction_ = false;
			if (conn_ == nullptr) {
				return {};
			}
			return conn_->rollback();
		}

		db_result<void> execute(std::string const& sql) {
			if (conn_ == nullptr) {
				return {};
			}
			return conn_->execute(sql);
		}

		template<typename T>
		db_result<void> execute(std::string const& sql,std::function<void(T)> const& callback) {
			if (conn_ == nullptr) {
				return {};
			}
			return conn_->execute(sql, callback);
		}

		std::uint64_t get_affected_rows() {
			if (conn_ == nullptr) {
				return 0;
			}
			return conn_->get_affected_rows();
		}

		void start_transaction() {
			begin();
			start_transaction_ = true;
		}

		bool is_open() {
			if (conn_ == nullptr) {
				return false;
			}
			return conn_->is_connect();
		}
		std::shared_ptr<DataBaseType> const& get_connection() {
			return conn_;
		}
	public:
		~dao_t() {
			if (start_transaction_) {
				commit();
			}
			auto data_base_ptr = data_base_.lock();
			if (data_base_ptr != nullptr) {
				auto current_pool = std::dynamic_pointer_cast<Pool<DataBaseType>>(data_base_ptr->pool_);
				if (current_pool != nullptr) {
					current_pool->pool_.revert(conn_);
				}
			}
		}
	private:
		std::weak_ptr<data_base> data_base_;
		std::shared_ptr<DataBaseType> conn_ = nullptr;
		bool start_transaction_ = false;
	};
}