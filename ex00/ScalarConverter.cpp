#include "ScalarConverter.hpp"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>

// プライベートコンストラクタ・デストラクタの定義（未使用）
ScalarConverter::ScalarConverter()
{
}
ScalarConverter::~ScalarConverter()
{
}
ScalarConverter::ScalarConverter(const ScalarConverter &)
{
}
ScalarConverter &ScalarConverter::operator=(const ScalarConverter &)
{
	return *this;
}

// ---------------------- 型判定ヘルパー ----------------------
bool ScalarConverter::isCharLiteral(const std::string &s)
{
	return s.size() == 3 && s[0] == '\'' && s[2] == '\'';
}

bool ScalarConverter::isPseudoLiteral(const std::string &s)
{
	static const char *pseudos[] = {"nan", "+inf", "-inf", "nanf", "+inff", "-inff"};
	for (int i = 0; i < 6; ++i)
	{
		if (s == pseudos[i])
			return true;
	}
	return false;
}

bool ScalarConverter::isIntLiteral(const std::string &s)
{
	if (isPseudoLiteral(s) || isCharLiteral(s))
		return false;
	char *end;
	long val = std::strtol(s.c_str(), &end, 10);
	(void)val;
	return end != s.c_str() && *end == '\0';
}

bool ScalarConverter::isFloatLiteral(const std::string &s)
{
	if (!s.empty() && s[s.size() - 1] == 'f')
	{
		if (isPseudoLiteral(s))
			return true;
		std::string core = s.substr(0, s.size() - 1);
		char *end;
		std::strtod(core.c_str(), &end);
		return end != core.c_str() && *end == '\0' && core.find('.') != std::string::npos;
	}
	return false;
}

bool ScalarConverter::isDoubleLiteral(const std::string &s)
{
	if (isPseudoLiteral(s))
	{
		// pseudo double は 'f' がつかないもの
		return s.find('f') == std::string::npos;
	}
	if (isCharLiteral(s))
		return false;
	char *end;
	std::strtod(s.c_str(), &end);
	return end != s.c_str() && *end == '\0' && s.find('.') != std::string::npos;
}

// ---------------------- 出力ヘルパー ----------------------
bool ScalarConverter::isDisplayableChar(int v)
{
	return v >= 32 && v <= 126;
}

void ScalarConverter::printChar(double d)
{
	std::cout << "char: ";
	if (std::isnan(d) || d < std::numeric_limits<char>::min() || d > std::numeric_limits<char>::max())
	{
		std::cout << "impossible\n";
	}
	else
	{
		int iv = static_cast<int>(d);
		if (!isDisplayableChar(iv))
		{
			std::cout << "Non displayable\n";
		}
		else
		{
			std::cout << "'" << static_cast<char>(d) << "'\n";
		}
	}
}

void ScalarConverter::printInt(double d)
{
	std::cout << "int: ";
	if (std::isnan(d) || d < std::numeric_limits<int>::min() || d > std::numeric_limits<int>::max())
	{
		std::cout << "impossible\n";
	}
	else
	{
		std::cout << static_cast<int>(d) << "\n";
	}
}

void ScalarConverter::printFloat(double d)
{
	std::cout << "float: ";
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(1) << static_cast<float>(d);
	oss << 'f';
	std::cout << oss.str() << "\n";
}

void ScalarConverter::printDouble(double d)
{
	std::cout << "double: ";
	std::cout << std::fixed << std::setprecision(1) << d << "\n";
}

// ---------------------- convert 本体 ----------------------
void ScalarConverter::convert(const std::string &literal)
{
	double value = 0.0;

	if (isCharLiteral(literal))
	{
		value = static_cast<double>(literal[1]);
	}
	else if (isPseudoLiteral(literal))
	{
		std::string core = literal;
		if (core[core.size() - 1] == 'f')
		{
			core = core.substr(0, core.size() - 1);
		}
		value = std::strtod(core.c_str(), (char **)NULL);
	}
	else if (isIntLiteral(literal))
	{
		value = static_cast<double>(std::strtol(literal.c_str(), (char **)NULL, 10));
	}
	else if (isFloatLiteral(literal))
	{
		std::string core = literal.substr(0, literal.size() - 1);
		value = std::strtod(core.c_str(), (char **)NULL);
	}
	else if (isDoubleLiteral(literal))
	{
		value = std::strtod(literal.c_str(), (char **)NULL);
	}
	else
	{
		std::cerr << "Error: invalid literal\n";
		return;
	}

	printChar(value);
	printInt(value);
	printFloat(value);
	printDouble(value);
}
