#pragma once
#include <iostream>
namespace xorm {
	template<typename T>
	struct is_tuple_type:std::false_type {

	};
	template<typename...Args>
	struct is_tuple_type<std::tuple<Args...>> :std::true_type {

	};
	//template<typename T>
	//constexpr bool is_tuple_type_v = is_tuple_type<T>::value;

	template<std::size_t N,std::size_t Max>
	struct each_tuple {
		template<typename Tuple,typename CallBack>
		static void each(Tuple&& tp, CallBack&& callback) {
			callback(std::get<N>(tp));
			each_tuple<N + 1, Max>::template each(std::forward<Tuple>(tp), std::forward<CallBack>(callback));
		}

		template<typename Tuple,typename Tuple2, typename CallBack>
		static void each2(Tuple&& tp, Tuple2&& copytp, CallBack&& callback) {
			callback(std::get<N>(tp),std::get<N>(copytp));
			each_tuple<N + 1, Max>::template each2(std::forward<Tuple>(tp), std::forward<Tuple2>(copytp),std::forward<CallBack>(callback));
		}
	};

	template<std::size_t Max>
	struct each_tuple<Max, Max> {
		template<typename Tuple, typename CallBack>
		static void each(Tuple&& tp, CallBack&& callback) {

		}

		template<typename Tuple, typename Tuple2, typename CallBack>
		static void each2(Tuple&& tp, Tuple2&& copytp, CallBack&& callback) {

		}
	};

	namespace xorm_utils {

		template<std::size_t...>
		struct index_package {

		};

		template<std::size_t Max, std::size_t...Index>
		struct index_package<Max, Index...> {
			using type = typename index_package<Max - 1, Max - 1, Index... >::type;
		};

		template<std::size_t...Index>
		struct index_package<0, Index...> {
			using type = index_package<Index...>;
		};

		template<std::size_t Max>
		using make_index_package = typename index_package<Max>::type;
	}

}
