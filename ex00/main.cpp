#include "ScalarConverter.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: ./convert <literal>\n";
		return 1;
	}

	ScalarConverter::convert(argv[1]);
	return 0;
}
// 42.195
// 2147483648 overflow
// -2147483649 underflow
// +inf
// +inff
// -inf
// -inff

/*
// Decimal number
TEST(FloatInputTest, DecimalNumberTest) {
  std::string coinTossLandingOnHeads = "0.51";

  capturedConvertStdoutTest("0.0f", "char: Non displayable\nint: 0\nfloat: 0.0f\ndouble: 0.0\n");
  capturedConvertStdoutTest("42.195", "char: '*'\nint: 42\nfloat: 42.195f\ndouble: 42.195\n");
  capturedConvertStdoutTest(coinTossLandingOnHeads, "char: Non displayable\nint: 0\nfloat: 0.51f\ndouble: 0.51\n");
}

// Overflow
TEST(IntInputTest, IntUnprintableTest) {
  // 2147483648 == INT_MAX(2147483647) + 1
  capturedConvertStdoutTest("2147483648", "char: Non displayable\nint: overflow\nfloat: 2.14748e+09f\ndouble: 2.14748e+09\n");
  // -2147483649 == INT_MIN(-2147483648) - 1
  capturedConvertStdoutTest("-2147483649", "char: Non displayable\nint: overflow\nfloat: -2147483648.0f\ndouble: -2.14748e+09\n");
}

// NaN: Not a Number
TEST(PseudoInputTest, NaNTest) {
  capturedConvertStdoutTest("nan", "char: impossible\nint: impossible\nfloat: nanf\ndouble: nan\n");
  capturedConvertStdoutTest("nanf", "char: impossible\nint: impossible\nfloat: nanf\ndouble: nan\n");
}

// Inf: Infinity
TEST(PseudoInputTest, PlusInfTest) {
  capturedConvertStdoutTest("+inf", "char: overflow\nint: overflow\nfloat: +inff\ndouble: +inf\n");
  capturedConvertStdoutTest("+inff", "char: overflow\nint: overflow\nfloat: +inff\ndouble: +inf\n");
}

// Inf: Infinity
TEST(PseudoInputTest, MinusInfTest) {
  capturedConvertStdoutTest("-inf", "char: overflow\nint: overflow\nfloat: -inff\ndouble: -inf\n");
  capturedConvertStdoutTest("-inff", "char: overflow\nint: overflow\nfloat: -inff\ndouble: -inf\n");
}
*/