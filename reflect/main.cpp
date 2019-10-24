#include <iostream>
#include "reflector.hpp"
using namespace reflector;
struct Test
{
	int a = 1024;
	double b = 20.12;
	void show()
	{
		std::cout << "Test" << std::endl;
	}
};
REFLECTION(Test, a, b, show)

int main()
{
	Test t;
	each_object(t, [](auto& t,auto name,auto v) {
		if constexpr (std::is_member_function_pointer_v<decltype(v)>) {
			(t.*v)();
		}
		else if constexpr (!std::is_member_function_pointer_v<decltype(v)>) {
			std::cout<< name<<" = " << t.*v << std::endl;
		}
	});

	find_protype("show", t, [](auto& t, auto name, auto v) {
		/*std::cout << t.*v << std::endl;*/
		if constexpr (std::is_member_function_pointer_v<decltype(v)>) {
			(t.*v)();
		}
		else if constexpr(!std::is_member_function_pointer_v<decltype(v)>){
			std::cout << t.*v << std::endl;
		}
	});
	std::cin.get();
}