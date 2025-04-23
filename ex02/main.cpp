#include "identify.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

int main()
{
	// 1. 一度だけシードを初期化
	std::srand(static_cast<unsigned>(std::time(NULL)));

	for (int i = 0; i < 10; ++i)
	{
		Base *p = generate();
		std::cout << "Pointer identify: ";
		identify(p);
		std::cout << "Reference identify: ";
		identify(*p);
		delete p;
	}
    std::cout << "------------------------\n";
    // 存在しない場合
    Base *p = new Base();
    std::cout << "Pointer identify: ";
    identify(p);
    std::cout << "Reference identify: ";
    identify(*p);
    delete p;
    std::cout << "------------------------\n";
	return 0;
}
