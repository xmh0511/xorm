#pragma once
#include <iostream>
#ifdef  _WIN32
#include <mysql.h>
#else
#include <mysql/mysql.h>
#endif // __LINUX__
#include <sstream>
#include <string>
#include <ctime>
namespace xorm {
	template<typename Type, enum_field_types FieldType>
	class FundamentionType final {
		friend std::ostream& operator <<(std::ostream& out, FundamentionType const& v) {
			if (v.is_null()) {
				out << "NULL";
			}
			else {
				out << v.value();
			}
			return out;
		}
	public:
		using value_type = Type;
	public:
		FundamentionType() :data_(0), is_null_(true) {

		}
		FundamentionType(Type v) :data_(v), is_null_(false) {

		}
		FundamentionType& operator=(Type v) {
			data_ = v;
			is_null_ = false;
			return *this;
		}
		bool is_null() const {
			return is_null_;
		}
		char* null_buffer() {
			return &is_null_;
		}
		char* buffer() {
			return (char*)(&data_);
		}
		void clear() {
			data_ = 0;
			is_null_ = true;
		}
		Type value() const {
			return data_;
		}
	public:
		static const enum_field_types field_type = FieldType;
	private:
		Type data_;
		char is_null_;
	};

	template<typename T>
	struct is_fundamention_type :std::false_type {

	};

	template<typename T, enum_field_types FieldType>
	struct is_fundamention_type<FundamentionType<T, FieldType>> :std::true_type {

	};

	template<enum_field_types FieldType>
	class TimeDate {
		friend std::ostream& operator <<(std::ostream& out, TimeDate const& v) {
			if (v.is_null()) {
				out << "NULL";
			}
			else {
				out << v.value();
			}
			return out;
		}
	public:
		using value_type = MYSQL_TIME;
	public:
		static const enum_field_types field_type = FieldType;
	public:
		TimeDate() :is_null_(true) {

		}
		TimeDate(std::string const& date) :is_null_(false) {
			decode_time(date);
		}
		TimeDate& operator=(std::string const& date) {
			decode_time(date);
			return *this;
		}
		void clear() {
			data_.day = 0;
			data_.hour = 0;
			data_.minute = 0;
			data_.month = 0;
			data_.second = 0;
			data_.second_part = 0;
			data_.year = 0;
			data_.neg = 0;
			is_null_ = true;
		}
		bool is_null() const {
			return is_null_;
		}
		char* null_buffer() {
			return &is_null_;
		}
		char* buffer() {
			return (char*)(&data_);
		}
		std::string value() const {
			return encode_time();
		}
		void format_timestamp(std::time_t timestamp) {
			struct tm* ttime = localtime(&timestamp);
			auto year = ttime->tm_year + 1900;
			auto month = ttime->tm_mon + 1;
			auto day = ttime->tm_mday;
			auto hour = ttime->tm_hour;
			auto minute = ttime->tm_min;
			auto second = ttime->tm_sec;
			if (FieldType == MYSQL_TYPE_DATETIME) {
				data_.year = year;
				data_.month = month;
				data_.day = day;
				data_.hour = hour;
				data_.minute = minute;
				data_.second = second;
				data_.second_part = 0;
			}
			else if (FieldType == MYSQL_TYPE_DATE) {
				data_.year = year;
				data_.month = month;
				data_.day = day;
				data_.hour = 0;
				data_.minute = 0;
				data_.second = 0;
				data_.second_part = 0;
			}
			else if (FieldType == MYSQL_TYPE_TIME) {
				data_.year = 0;
				data_.month = 0;
				data_.day = 0;
				data_.hour = hour;
				data_.minute = minute;
				data_.second = second;
				data_.second_part = 0;
				data_.neg = 0;
			}
			is_null_ = false;
		}
	private:
		void decode_time(std::string const& date) {
			char const* cptr = date.data();
			if (FieldType == MYSQL_TYPE_DATETIME) {
				data_.year = atoi(cptr);
				data_.month = atoi(cptr + 5);
				data_.day = atoi(cptr + 8);
				data_.hour = atoi(cptr + 11);
				data_.minute = atoi(cptr + 14);
				data_.second = atoi(cptr + 17);
				data_.second_part = 0;
			}
			else if (FieldType == MYSQL_TYPE_DATE) {
				data_.year = atoi(cptr);
				data_.month = atoi(cptr + 5);
				data_.day = atoi(cptr + 8);
				data_.hour = 0;
				data_.minute = 0;
				data_.second = 0;
				data_.second_part = 0;
			}
			else if (FieldType == MYSQL_TYPE_TIME) {
				data_.year = 0;
				data_.month = 0;
				data_.day = 0;
				data_.hour = atoi(cptr);
				data_.minute = atoi(cptr + 3);
				data_.second = atoi(cptr + 6);
				data_.second_part = 0;
				data_.neg = 0;
			}
			is_null_ = false;
		}
		std::string encode_time() const {
			std::stringstream ss;
			if (FieldType == MYSQL_TYPE_DATETIME) {
				ss << data_.year << "-" << fix_number(data_.month) << "-" << fix_number(data_.day) << " " << fix_number(data_.hour) << ":" << fix_number(data_.minute) << ":" << fix_number(data_.second); //<< "." << data_.second_part
			}
			else if (FieldType == MYSQL_TYPE_DATE) {
				ss << data_.year << "-" << fix_number(data_.month) << "-" << fix_number(data_.day);
			}
			else if (FieldType == MYSQL_TYPE_TIME) {
				ss << fix_number(data_.hour) << ":" << fix_number(data_.minute) << ":" << fix_number(data_.second); //<< "." << data_.second_part
			}
			return ss.str();
		}
		std::string fix_number(int v) const {
			std::stringstream ss;
			if (v < 10) {
				ss << "0" << v;
			}
			else {
				ss << v;
			}
			return ss.str();
		}
	private:
		char is_null_;
		MYSQL_TIME data_;
	};

	template<typename T>
	struct is_date_type :std::false_type {

	};

	template<enum_field_types FieldType>
	struct is_date_type<TimeDate<FieldType>> :std::true_type {

	};
}
