// Serializer.hpp
#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <stdint.h>  // for std::uintptr_t
#include "Data.hpp"

class Serializer {
public:
    // コンストラクタ／デストラクタを公開しないことで生成を防ぐ
    static uintptr_t serialize(Data* ptr);
    static Data*         deserialize(uintptr_t raw);

private:
    Serializer();                         // private にしてインスタンス禁止
    ~Serializer();
    Serializer(const Serializer&);
    Serializer& operator=(const Serializer&);
};

#endif // SERIALIZER_HPP
