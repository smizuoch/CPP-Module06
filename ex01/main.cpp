// main.cpp
#include "Serializer.hpp"
#include <cassert>
#include <iostream>

int main()
{
	Data d;
	d.value = 42;

	// ポインタ → 整数
	uintptr_t raw = Serializer::serialize(&d);
	std::cout << "Serialized: " << raw << "\n";

	// 整数 → ポインタ
	Data *ptr = Serializer::deserialize(raw);
	std::cout << "Deserialized: " << ptr << "\n";

	// 検証
	assert(ptr == &d);
	std::cout << "Pointer matches original: OK\n";
	std::cout << "Value through ptr: " << ptr->value << "\n";

	return 0;
}
