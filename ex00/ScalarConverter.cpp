#include "ScalarConverter.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cerrno>
#include <cctype>

// プライベートコンストラクタ・デストラクタ定義
ScalarConverter::ScalarConverter() {}
ScalarConverter::~ScalarConverter() {}
ScalarConverter::ScalarConverter(const ScalarConverter &) {}
ScalarConverter &ScalarConverter::operator=(const ScalarConverter &) { return *this; }

// ---------------------- 型判定ヘルパー ----------------------
bool ScalarConverter::isCharLiteral(const std::string &s) {
    return s.size() == 3 && s[0] == '\'' && s[2] == '\'';
}

bool ScalarConverter::isPseudoLiteral(const std::string &s) {
    static const char *pseudos[] = {"nan", "+inf", "-inf", "nanf", "+inff", "-inff"};
    for (int i = 0; i < 6; ++i) {
        if (s == pseudos[i]) return true;
    }
    return false;
}

bool ScalarConverter::isIntLiteral(const std::string &s) {
    if (isPseudoLiteral(s) || isCharLiteral(s)) return false;
    errno = 0;
    char *end;
    long val = std::strtol(s.c_str(), &end, 10);
    if (end == s.c_str() || *end != '\0') return false;
    if (errno == ERANGE || val < std::numeric_limits<int>::min() || val > std::numeric_limits<int>::max()) return false;
    return true;
}

bool ScalarConverter::isFloatLiteral(const std::string &s) {
    if (!s.empty() && s[s.size() - 1] == 'f') {
        if (isPseudoLiteral(s)) return true;
        std::string core = s.substr(0, s.size() - 1);
        char *end;
        std::strtod(core.c_str(), &end);
        return end != core.c_str() && *end == '\0' && core.find('.') != std::string::npos;
    }
    return false;
}

bool ScalarConverter::isDoubleLiteral(const std::string &s) {
    if (isPseudoLiteral(s)) return (s.find('f') == std::string::npos);
    if (isCharLiteral(s)) return false;
    char *end;
    std::strtod(s.c_str(), &end);
    return end != s.c_str() && *end == '\0' && s.find('.') != std::string::npos;
}

// ---------------------- 出力ヘルパー ----------------------
bool ScalarConverter::isDisplayableChar(int v) {
    return v >= 32 && v <= 126;
}

void ScalarConverter::printChar(double d) {
    std::cout << "char: ";
    if (std::isnan(d)) {
        std::cout << "impossible\n";
    } else if (std::isinf(d)) {
        std::cout << "overflow\n";
    } else {
        int v = static_cast<int>(d);
        if (v < std::numeric_limits<char>::min() || v > std::numeric_limits<char>::max()) {
            std::cout << "Non displayable\n";
        } else if (!isDisplayableChar(v)) {
            std::cout << "Non displayable\n";
        } else {
            std::cout << "'" << static_cast<char>(v) << "'\n";
        }
    }
}

void ScalarConverter::printInt(double d) {
    std::cout << "int: ";
    if (std::isnan(d)) {
        std::cout << "impossible\n";
    } else if (std::isinf(d)) {
        std::cout << "overflow\n";
    } else if (d < std::numeric_limits<int>::min() || d > std::numeric_limits<int>::max()) {
        std::cout << "overflow\n";
    } else {
        std::cout << static_cast<int>(d) << "\n";
    }
}

// ---------------------- 浮動小数点疑似リテラル分類 ----------------------
static bool isFloatPseudoLiteral(const std::string &s) {
    return (s == "nanf" || s == "+inff" || s == "-inff");
}

static bool isDoublePseudoLiteral(const std::string &s) {
    return (s == "nan" || s == "+inf" || s == "-inf");
}

// ---------------------- convert 本体 ----------------------
void ScalarConverter::convert(const std::string &literal) {
    const std::string &s = literal;
    bool isFloatPseudo = isFloatPseudoLiteral(s);
    bool isDoublePseudo = isDoublePseudoLiteral(s);
    bool isFloatDec = isFloatLiteral(s) && !isFloatPseudo;
    bool isDoubleDec = isDoubleLiteral(s) && !isDoublePseudo;
    bool isPureIntSyntax = false;
    size_t start = 0;
    if (!s.empty() && (s[0] == '+' || s[0] == '-')) start = 1;
    if (start < s.size()) {
        isPureIntSyntax = true;
        for (size_t i = start; i < s.size(); ++i) {
            if (!std::isdigit(static_cast<unsigned char>(s[i]))) { isPureIntSyntax = false; break; }
        }
    }
    bool isIntLit = isIntLiteral(s);
    bool intOverflowCase = isPureIntSyntax && !isIntLit;

    double value = 0.0;
    if (isCharLiteral(s)) {
        value = static_cast<double>(s[1]);
    } else if (isFloatPseudo) {
        std::string core = s.substr(0, s.size() - 1);
        value = std::strtod(core.c_str(), (char **)NULL);
    } else if (isDoublePseudo) {
        value = std::strtod(s.c_str(), (char **)NULL);
    } else if (isIntLit) {
        value = static_cast<double>(std::strtol(s.c_str(), (char **)NULL, 10));
    } else if (isFloatDec) {
        std::string core = s.substr(0, s.size() - 1);
        value = std::strtod(core.c_str(), (char **)NULL);
    } else if (isDoubleDec || intOverflowCase) {
        value = std::strtod(s.c_str(), (char **)NULL);
    } else {
        std::cerr << "Error: invalid literal\n";
        return;
    }

    // char/int 出力
    printChar(value);
    printInt(value);

    // float 出力
    std::cout << "float: ";
    if (isFloatPseudo) {
        std::cout << s << "\n";
    } else if (isDoublePseudo) {
        std::cout << s << "f\n";
    } else if (isFloatDec) {
        std::cout << s << "\n";
    } else if (isDoubleDec) {
        std::cout << s << "f\n";
    } else if (intOverflowCase) {
        std::ostringstream oss;
        if (!s.empty() && s[0] == '-') oss << std::fixed << std::setprecision(1) << static_cast<float>(value);
        else oss << static_cast<float>(value);
        std::cout << oss.str() << "f\n";
    } else {
        std::ostringstream oss;
        oss << static_cast<float>(value);
        std::cout << oss.str() << "f\n";
    }

    // double 出力
    std::cout << "double: ";
    if (isDoublePseudo) {
        std::cout << s << "\n";
    } else if (isFloatPseudo) {
        std::cout << s.substr(0, s.size() - 1) << "\n";
    } else if (isDoubleDec) {
        std::cout << s << "\n";
    } else if (isFloatDec) {
        std::cout << s.substr(0, s.size() - 1) << "\n";
    } else {
        std::ostringstream oss;
        oss << static_cast<double>(value);
        std::cout << oss.str() << "\n";
    }
}
