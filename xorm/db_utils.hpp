#pragma once
#include <string>
#include <vector>
namespace xorm {
	class db_error final {
		friend class mysql;
		friend class sqlite;
	public:
		db_error() = default;
	public:
		operator bool() {
			return is_error_;
		}
		std::string message() {
			return message_;
		}
	private:
		bool is_error_ = false;
		std::string message_;
	};

	template<typename DataType>
	struct db_result {  //for query 
		db_error error{};
		bool success = false;
		std::vector<DataType> results;
	};

	template<>
	struct db_result<void> {  //for insert, del, update, execute
		db_error error{};
		bool success = false;
		std::uint64_t affect_rows = 0;
		std::uint64_t unique_id = 0;
	};
}