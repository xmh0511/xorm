#pragma once
#include <iostream>
namespace xorm {
	template<typename T>
	struct is_tuple_type:std::false_type {

	};
	template<typename...Args>
	struct is_tuple_type<std::tuple<Args...>> :std::true_type {

	};
	template<typename T>
	constexpr bool is_tuple_type_v = is_tuple_type<T>::value;

	template<std::size_t N,std::size_t Max>
	struct each_tuple {
		template<typename Tuple,typename CallBack>
		static void each(Tuple&& tp, CallBack&& callback) {
			callback(std::get<N>(tp));
			each_tuple<N + 1, Max>::template each(std::forward<Tuple>(tp), std::forward<CallBack>(callback));
		}
	};

	template<std::size_t Max>
	struct each_tuple<Max, Max> {
		template<typename Tuple, typename CallBack>
		static void each(Tuple&& tp, CallBack&& callback) {

		}
	};
}
