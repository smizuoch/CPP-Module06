#ifndef SCALARCONVERTER_HPP
#define SCALARCONVERTER_HPP

#include <cmath>
#include <limits>
#include <string>

class ScalarConverter
{
  public:
	// 文字列リテラルを解析し、各スカラー型に変換して出力
	static void convert(const std::string &literal);

  private:
	// インスタンス化・コピー・ムーブ禁止
	ScalarConverter();
	~ScalarConverter();
	ScalarConverter(const ScalarConverter &);
	ScalarConverter &operator=(const ScalarConverter &);

	// リテラル判定ヘルパー
	static bool isCharLiteral(const std::string &s);
	static bool isIntLiteral(const std::string &s);
	static bool isFloatLiteral(const std::string &s);
	static bool isDoubleLiteral(const std::string &s);
	static bool isPseudoLiteral(const std::string &s);

	// 変換後の出力ヘルパー
	static bool isDisplayableChar(int value);
	static void printChar(double d);
	static void printInt(double d);
	static void printFloat(double d);
	static void printDouble(double d);
};

#endif // SCALARCONVERTER_HPP
