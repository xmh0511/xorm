#pragma once
#include <array>
#include <tuple>
#include <string>
namespace reflector
{
#define place_holder 120,119,118,117,116,115,114,113,112,111,110,109,108,107,106,105,104,103,102,101,100,99,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0
#define expand_marco(...) __VA_ARGS__
#define concat_a_b_(a,b) a##b
#define concat_a_b(a,b) concat_a_b_(a,b)

#ifdef  _MSC_VER

#define inner_var(...) unused,__VA_ARGS__
#define get_count_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,_63,_64,_65,_66,_67,_68,_69,_70,_71,_72,_73,_74,_75,_76,_77,_78,_79,_80,_81,_82,_83,_84,_85,_86,_87,_88,_89,_90,_91,_92,_93,_94,_95,_96,_97,_98,_99,_100,_101,_102,_103,_104,_105,_106,_107,_108,_109,_110,_111,_112,_113,_114,_115,_116,_117,_118,_119,_120,count,...) count
#define get_count_help(...) expand_marco(get_count_(__VA_ARGS__))
#define get_count(...) get_count_help(inner_var(__VA_ARGS__),place_holder)

#else //  _NOT_MSC_VER
#define get_count_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,_63,_64,_65,_66,_67,_68,_69,_70,_71,_72,_73,_74,_75,_76,_77,_78,_79,_80,_81,_82,_83,_84,_85,_86,_87,_88,_89,_90,_91,_92,_93,_94,_95,_96,_97,_98,_99,_100,_101,_102,_103,_104,_105,_106,_107,_108,_109,_110,_111,_112,_113,_114,_115,_116,_117,_118,_119,_120,count,...) count
#define get_count_help(...) get_count_(__VA_ARGS__)
#define get_count(...) get_count_help(0,## __VA_ARGS__,place_holder)
#endif

#define For_1(method,ClassName,element,...) method(ClassName,element)
#define For_2(method,ClassName,element,...) method(ClassName,element),expand_marco(For_1(method,ClassName,__VA_ARGS__))
#define For_3(method,ClassName,element,...) method(ClassName,element),expand_marco(For_2(method,ClassName,__VA_ARGS__))
#define For_4(method,ClassName,element,...) method(ClassName,element),expand_marco(For_3(method,ClassName,__VA_ARGS__))
#define For_5(method,ClassName,element,...) method(ClassName,element),expand_marco(For_4(method,ClassName,__VA_ARGS__))
#define For_6(method,ClassName,element,...) method(ClassName,element),expand_marco(For_5(method,ClassName,__VA_ARGS__))
#define For_7(method,ClassName,element,...) method(ClassName,element),expand_marco(For_6(method,ClassName,__VA_ARGS__))
#define For_8(method,ClassName,element,...) method(ClassName,element),expand_marco(For_7(method,ClassName,__VA_ARGS__))
#define For_9(method,ClassName,element,...) method(ClassName,element),expand_marco(For_8(method,ClassName,__VA_ARGS__))
#define For_10(method,ClassName,element,...) method(ClassName,element),expand_marco(For_9(method,ClassName,__VA_ARGS__))
#define For_11(method,ClassName,element,...) method(ClassName,element),expand_marco(For_10(method,ClassName,__VA_ARGS__))
#define For_12(method,ClassName,element,...) method(ClassName,element),expand_marco(For_11(method,ClassName,__VA_ARGS__))
#define For_13(method,ClassName,element,...) method(ClassName,element),expand_marco(For_12(method,ClassName,__VA_ARGS__))
#define For_14(method,ClassName,element,...) method(ClassName,element),expand_marco(For_13(method,ClassName,__VA_ARGS__))
#define For_15(method,ClassName,element,...) method(ClassName,element),expand_marco(For_14(method,ClassName,__VA_ARGS__))
#define For_16(method,ClassName,element,...) method(ClassName,element),expand_marco(For_15(method,ClassName,__VA_ARGS__))
#define For_17(method,ClassName,element,...) method(ClassName,element),expand_marco(For_16(method,ClassName,__VA_ARGS__))
#define For_18(method,ClassName,element,...) method(ClassName,element),expand_marco(For_17(method,ClassName,__VA_ARGS__))
#define For_19(method,ClassName,element,...) method(ClassName,element),expand_marco(For_18(method,ClassName,__VA_ARGS__))
#define For_20(method,ClassName,element,...) method(ClassName,element),expand_marco(For_19(method,ClassName,__VA_ARGS__))
#define For_21(method,ClassName,element,...) method(ClassName,element),expand_marco(For_20(method,ClassName,__VA_ARGS__))
#define For_22(method,ClassName,element,...) method(ClassName,element),expand_marco(For_21(method,ClassName,__VA_ARGS__))
#define For_23(method,ClassName,element,...) method(ClassName,element),expand_marco(For_22(method,ClassName,__VA_ARGS__))
#define For_24(method,ClassName,element,...) method(ClassName,element),expand_marco(For_23(method,ClassName,__VA_ARGS__))
#define For_25(method,ClassName,element,...) method(ClassName,element),expand_marco(For_24(method,ClassName,__VA_ARGS__))
#define For_26(method,ClassName,element,...) method(ClassName,element),expand_marco(For_25(method,ClassName,__VA_ARGS__))
#define For_27(method,ClassName,element,...) method(ClassName,element),expand_marco(For_26(method,ClassName,__VA_ARGS__))
#define For_28(method,ClassName,element,...) method(ClassName,element),expand_marco(For_27(method,ClassName,__VA_ARGS__))
#define For_29(method,ClassName,element,...) method(ClassName,element),expand_marco(For_28(method,ClassName,__VA_ARGS__))
#define For_30(method,ClassName,element,...) method(ClassName,element),expand_marco(For_29(method,ClassName,__VA_ARGS__))
#define For_31(method,ClassName,element,...) method(ClassName,element),expand_marco(For_30(method,ClassName,__VA_ARGS__))
#define For_32(method,ClassName,element,...) method(ClassName,element),expand_marco(For_31(method,ClassName,__VA_ARGS__))
#define For_33(method,ClassName,element,...) method(ClassName,element),expand_marco(For_32(method,ClassName,__VA_ARGS__))
#define For_34(method,ClassName,element,...) method(ClassName,element),expand_marco(For_33(method,ClassName,__VA_ARGS__))
#define For_35(method,ClassName,element,...) method(ClassName,element),expand_marco(For_34(method,ClassName,__VA_ARGS__))
#define For_36(method,ClassName,element,...) method(ClassName,element),expand_marco(For_35(method,ClassName,__VA_ARGS__))
#define For_37(method,ClassName,element,...) method(ClassName,element),expand_marco(For_36(method,ClassName,__VA_ARGS__))
#define For_38(method,ClassName,element,...) method(ClassName,element),expand_marco(For_37(method,ClassName,__VA_ARGS__))
#define For_39(method,ClassName,element,...) method(ClassName,element),expand_marco(For_38(method,ClassName,__VA_ARGS__))
#define For_40(method,ClassName,element,...) method(ClassName,element),expand_marco(For_39(method,ClassName,__VA_ARGS__))
#define For_41(method,ClassName,element,...) method(ClassName,element),expand_marco(For_40(method,ClassName,__VA_ARGS__))
#define For_42(method,ClassName,element,...) method(ClassName,element),expand_marco(For_41(method,ClassName,__VA_ARGS__))
#define For_43(method,ClassName,element,...) method(ClassName,element),expand_marco(For_42(method,ClassName,__VA_ARGS__))
#define For_44(method,ClassName,element,...) method(ClassName,element),expand_marco(For_43(method,ClassName,__VA_ARGS__))
#define For_45(method,ClassName,element,...) method(ClassName,element),expand_marco(For_44(method,ClassName,__VA_ARGS__))
#define For_46(method,ClassName,element,...) method(ClassName,element),expand_marco(For_45(method,ClassName,__VA_ARGS__))
#define For_47(method,ClassName,element,...) method(ClassName,element),expand_marco(For_46(method,ClassName,__VA_ARGS__))
#define For_48(method,ClassName,element,...) method(ClassName,element),expand_marco(For_47(method,ClassName,__VA_ARGS__))
#define For_49(method,ClassName,element,...) method(ClassName,element),expand_marco(For_48(method,ClassName,__VA_ARGS__))
#define For_50(method,ClassName,element,...) method(ClassName,element),expand_marco(For_49(method,ClassName,__VA_ARGS__))
#define For_51(method,ClassName,element,...) method(ClassName,element),expand_marco(For_50(method,ClassName,__VA_ARGS__))
#define For_52(method,ClassName,element,...) method(ClassName,element),expand_marco(For_51(method,ClassName,__VA_ARGS__))
#define For_53(method,ClassName,element,...) method(ClassName,element),expand_marco(For_52(method,ClassName,__VA_ARGS__))
#define For_54(method,ClassName,element,...) method(ClassName,element),expand_marco(For_53(method,ClassName,__VA_ARGS__))
#define For_55(method,ClassName,element,...) method(ClassName,element),expand_marco(For_54(method,ClassName,__VA_ARGS__))
#define For_56(method,ClassName,element,...) method(ClassName,element),expand_marco(For_55(method,ClassName,__VA_ARGS__))
#define For_57(method,ClassName,element,...) method(ClassName,element),expand_marco(For_56(method,ClassName,__VA_ARGS__))
#define For_58(method,ClassName,element,...) method(ClassName,element),expand_marco(For_57(method,ClassName,__VA_ARGS__))
#define For_59(method,ClassName,element,...) method(ClassName,element),expand_marco(For_58(method,ClassName,__VA_ARGS__))
#define For_60(method,ClassName,element,...) method(ClassName,element),expand_marco(For_59(method,ClassName,__VA_ARGS__))
#define For_61(method,ClassName,element,...) method(ClassName,element),expand_marco(For_60(method,ClassName,__VA_ARGS__))
#define For_62(method,ClassName,element,...) method(ClassName,element),expand_marco(For_61(method,ClassName,__VA_ARGS__))
#define For_63(method,ClassName,element,...) method(ClassName,element),expand_marco(For_62(method,ClassName,__VA_ARGS__))
#define For_64(method,ClassName,element,...) method(ClassName,element),expand_marco(For_63(method,ClassName,__VA_ARGS__))
#define For_65(method,ClassName,element,...) method(ClassName,element),expand_marco(For_64(method,ClassName,__VA_ARGS__))
#define For_66(method,ClassName,element,...) method(ClassName,element),expand_marco(For_65(method,ClassName,__VA_ARGS__))
#define For_67(method,ClassName,element,...) method(ClassName,element),expand_marco(For_66(method,ClassName,__VA_ARGS__))
#define For_68(method,ClassName,element,...) method(ClassName,element),expand_marco(For_67(method,ClassName,__VA_ARGS__))
#define For_69(method,ClassName,element,...) method(ClassName,element),expand_marco(For_68(method,ClassName,__VA_ARGS__))
#define For_70(method,ClassName,element,...) method(ClassName,element),expand_marco(For_69(method,ClassName,__VA_ARGS__))
#define For_71(method,ClassName,element,...) method(ClassName,element),expand_marco(For_70(method,ClassName,__VA_ARGS__))
#define For_72(method,ClassName,element,...) method(ClassName,element),expand_marco(For_71(method,ClassName,__VA_ARGS__))
#define For_73(method,ClassName,element,...) method(ClassName,element),expand_marco(For_72(method,ClassName,__VA_ARGS__))
#define For_74(method,ClassName,element,...) method(ClassName,element),expand_marco(For_73(method,ClassName,__VA_ARGS__))
#define For_75(method,ClassName,element,...) method(ClassName,element),expand_marco(For_74(method,ClassName,__VA_ARGS__))
#define For_76(method,ClassName,element,...) method(ClassName,element),expand_marco(For_75(method,ClassName,__VA_ARGS__))
#define For_77(method,ClassName,element,...) method(ClassName,element),expand_marco(For_76(method,ClassName,__VA_ARGS__))
#define For_78(method,ClassName,element,...) method(ClassName,element),expand_marco(For_77(method,ClassName,__VA_ARGS__))
#define For_79(method,ClassName,element,...) method(ClassName,element),expand_marco(For_78(method,ClassName,__VA_ARGS__))
#define For_80(method,ClassName,element,...) method(ClassName,element),expand_marco(For_79(method,ClassName,__VA_ARGS__))
#define For_81(method,ClassName,element,...) method(ClassName,element),expand_marco(For_80(method,ClassName,__VA_ARGS__))
#define For_82(method,ClassName,element,...) method(ClassName,element),expand_marco(For_81(method,ClassName,__VA_ARGS__))
#define For_83(method,ClassName,element,...) method(ClassName,element),expand_marco(For_82(method,ClassName,__VA_ARGS__))
#define For_84(method,ClassName,element,...) method(ClassName,element),expand_marco(For_83(method,ClassName,__VA_ARGS__))
#define For_85(method,ClassName,element,...) method(ClassName,element),expand_marco(For_84(method,ClassName,__VA_ARGS__))
#define For_86(method,ClassName,element,...) method(ClassName,element),expand_marco(For_85(method,ClassName,__VA_ARGS__))
#define For_87(method,ClassName,element,...) method(ClassName,element),expand_marco(For_86(method,ClassName,__VA_ARGS__))
#define For_88(method,ClassName,element,...) method(ClassName,element),expand_marco(For_87(method,ClassName,__VA_ARGS__))
#define For_89(method,ClassName,element,...) method(ClassName,element),expand_marco(For_88(method,ClassName,__VA_ARGS__))
#define For_90(method,ClassName,element,...) method(ClassName,element),expand_marco(For_89(method,ClassName,__VA_ARGS__))
#define For_91(method,ClassName,element,...) method(ClassName,element),expand_marco(For_90(method,ClassName,__VA_ARGS__))
#define For_92(method,ClassName,element,...) method(ClassName,element),expand_marco(For_91(method,ClassName,__VA_ARGS__))
#define For_93(method,ClassName,element,...) method(ClassName,element),expand_marco(For_92(method,ClassName,__VA_ARGS__))
#define For_94(method,ClassName,element,...) method(ClassName,element),expand_marco(For_93(method,ClassName,__VA_ARGS__))
#define For_95(method,ClassName,element,...) method(ClassName,element),expand_marco(For_94(method,ClassName,__VA_ARGS__))
#define For_96(method,ClassName,element,...) method(ClassName,element),expand_marco(For_95(method,ClassName,__VA_ARGS__))
#define For_97(method,ClassName,element,...) method(ClassName,element),expand_marco(For_96(method,ClassName,__VA_ARGS__))
#define For_98(method,ClassName,element,...) method(ClassName,element),expand_marco(For_97(method,ClassName,__VA_ARGS__))
#define For_99(method,ClassName,element,...) method(ClassName,element),expand_marco(For_98(method,ClassName,__VA_ARGS__))
#define For_100(method,ClassName,element,...) method(ClassName,element),expand_marco(For_99(method,ClassName,__VA_ARGS__))
#define For_101(method,ClassName,element,...) method(ClassName,element),expand_marco(For_100(method,ClassName,__VA_ARGS__))
#define For_102(method,ClassName,element,...) method(ClassName,element),expand_marco(For_101(method,ClassName,__VA_ARGS__))
#define For_103(method,ClassName,element,...) method(ClassName,element),expand_marco(For_102(method,ClassName,__VA_ARGS__))
#define For_104(method,ClassName,element,...) method(ClassName,element),expand_marco(For_103(method,ClassName,__VA_ARGS__))
#define For_105(method,ClassName,element,...) method(ClassName,element),expand_marco(For_104(method,ClassName,__VA_ARGS__))
#define For_106(method,ClassName,element,...) method(ClassName,element),expand_marco(For_105(method,ClassName,__VA_ARGS__))
#define For_107(method,ClassName,element,...) method(ClassName,element),expand_marco(For_106(method,ClassName,__VA_ARGS__))
#define For_108(method,ClassName,element,...) method(ClassName,element),expand_marco(For_107(method,ClassName,__VA_ARGS__))
#define For_109(method,ClassName,element,...) method(ClassName,element),expand_marco(For_108(method,ClassName,__VA_ARGS__))
#define For_110(method,ClassName,element,...) method(ClassName,element),expand_marco(For_109(method,ClassName,__VA_ARGS__))
#define For_111(method,ClassName,element,...) method(ClassName,element),expand_marco(For_110(method,ClassName,__VA_ARGS__))
#define For_112(method,ClassName,element,...) method(ClassName,element),expand_marco(For_111(method,ClassName,__VA_ARGS__))
#define For_113(method,ClassName,element,...) method(ClassName,element),expand_marco(For_112(method,ClassName,__VA_ARGS__))
#define For_114(method,ClassName,element,...) method(ClassName,element),expand_marco(For_113(method,ClassName,__VA_ARGS__))
#define For_115(method,ClassName,element,...) method(ClassName,element),expand_marco(For_114(method,ClassName,__VA_ARGS__))
#define For_116(method,ClassName,element,...) method(ClassName,element),expand_marco(For_115(method,ClassName,__VA_ARGS__))
#define For_117(method,ClassName,element,...) method(ClassName,element),expand_marco(For_116(method,ClassName,__VA_ARGS__))
#define For_118(method,ClassName,element,...) method(ClassName,element),expand_marco(For_117(method,ClassName,__VA_ARGS__))
#define For_119(method,ClassName,element,...) method(ClassName,element),expand_marco(For_118(method,ClassName,__VA_ARGS__))
#define For_120(method,ClassName,element,...) method(ClassName,element),expand_marco(For_119(method,ClassName,__VA_ARGS__))
#define For_121(method,ClassName,element,...) method(ClassName,element),expand_marco(For_120(method,ClassName,__VA_ARGS__))
#define For_122(method,ClassName,element,...) method(ClassName,element),expand_marco(For_121(method,ClassName,__VA_ARGS__))
#define For_123(method,ClassName,element,...) method(ClassName,element),expand_marco(For_122(method,ClassName,__VA_ARGS__))
#define For_124(method,ClassName,element,...) method(ClassName,element),expand_marco(For_123(method,ClassName,__VA_ARGS__))
#define For_125(method,ClassName,element,...) method(ClassName,element),expand_marco(For_124(method,ClassName,__VA_ARGS__))

#define address_macro(Class,Element) &Class::Element
#define element_name_macro(Class,Element) #Element

#define GENERATOR_META(N,ClassName,...) \
static std::array<char const*,N> ClassName##_element_name_arr = { expand_marco(concat_a_b(For_,N)(element_name_macro,ClassName,__VA_ARGS__)) }; \
auto meta_info_reflect(ClassName const& t) \
{ \
	struct meta_info \
	{   \
        auto get_element_names()-> std::array<char const*,N> \
		 { \
             return  ClassName##_element_name_arr; \
		 } \
        auto get_element_meta_protype() \
		{ \
           return std::make_tuple(expand_marco(concat_a_b(For_,N)(address_macro,ClassName,__VA_ARGS__))); \
		} \
        std::string get_class_name() \
        { \
            return #ClassName ; \
        } \
        constexpr std::size_t element_size() { return N; } \
    }; \
    return meta_info{}; \
}
#define REFLECTION(ClassName,...) GENERATOR_META(get_count(__VA_ARGS__),ClassName,__VA_ARGS__)

template<typename T, typename U = void>
struct is_reflect_class :std::false_type
{

};

template<typename T>
struct is_reflect_class<T, std::void_t<decltype(meta_info_reflect(std::declval<T>()))>> :std::true_type
{

};

template<std::size_t N, std::size_t Size>
struct each_
{
	template<typename name_tuple, typename protype_tuple, typename T, typename Function>
	static void each(name_tuple&& tuple0, protype_tuple&& tuple1, T&& t, Function&& callback)
	{
		callback(std::forward<T>(t), std::get<N>(tuple0), std::get<N>(tuple1));
		each_<N + 1, Size>::template each(std::forward<name_tuple>(tuple0), std::forward<protype_tuple>(tuple1), std::forward<T>(t), std::forward<Function>(callback));
	}
};

template<std::size_t Size>
struct each_<Size, Size>
{
	template<typename name_tuple, typename protype_tuple, typename T, typename Function>
	static void each(name_tuple&& tuple0, protype_tuple&& tuple1, T&& t, Function&& callback)
	{

	}
};

template<std::size_t N, std::size_t Size>
struct each_1
{
	template<typename name_tuple, typename protype_tuple, typename T, typename Function>
	static void each(name_tuple&& tuple0, protype_tuple&& tuple1, std::string const& name, T&& t, Function&& callback)
	{
		if (name == std::get<N>(tuple0)) {
			callback(std::forward<T>(t), std::get<N>(tuple0), std::get<N>(tuple1));
		}
		each_1<N + 1, Size>::template each(std::forward<name_tuple>(tuple0), std::forward<protype_tuple>(tuple1), name, std::forward<T>(t), std::forward<Function>(callback));
	}
};

template<std::size_t Size>
struct each_1<Size, Size>
{
	template<typename name_tuple, typename protype_tuple, typename T, typename Function>
	static void each(name_tuple&& tuple0, protype_tuple&& tuple1, std::string const& name, T&& t, Function&& callback)
	{

	}
};

template<typename T, typename Function>
std::enable_if_t<is_reflect_class<std::remove_reference_t<T>>::value> each_object(T&& t, Function&& callback)
{
	using class_type = std::remove_reference_t<T>;
	using meta_info_type = decltype(meta_info_reflect(std::declval<class_type>()));
	auto meta = meta_info_type{};
	each_<0, meta.element_size()>::template each(meta.get_element_names(), meta.get_element_meta_protype(), std::forward<T>(t), std::forward<Function>(callback));
}

template<typename T, typename Function>
std::enable_if_t<is_reflect_class<std::remove_reference_t<T>>::value> find_protype(std::string const& name, T&& t, Function&& callback)
{
	using class_type = std::remove_reference_t<T>;
	using meta_info_type = decltype(meta_info_reflect(std::declval<class_type>()));
	auto meta = meta_info_type{};
	each_1<0, meta.element_size()>::template each(meta.get_element_names(), meta.get_element_meta_protype(), name, std::forward<T>(t), std::forward<Function>(callback));
}

}