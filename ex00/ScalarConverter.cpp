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
ScalarConverter::ScalarConverter(const ScalarConverter&) {}
ScalarConverter& ScalarConverter::operator=(const ScalarConverter&) { return *this; }

// 型判定ヘルパー
bool ScalarConverter::isCharLiteral(const std::string &s) {
    return s.size() == 3 && s[0] == '\'' && s[2] == '\'';
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
    if (!s.empty() && s[s.size()-1] == 'f') {
        if (isPseudoLiteral(s)) return true;
        std::string core = s.substr(0, s.size()-1);
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
bool ScalarConverter::isPseudoLiteral(const std::string &s) {
    static const char *pseudos[] = {"nan","+inf","-inf","nanf","+inff","-inff"};
    for (int i = 0; i < 6; ++i) if (s == pseudos[i]) return true;
    return false;
}
bool ScalarConverter::isDisplayableChar(int v) {
    return v >= 32 && v <= 126;
}

void ScalarConverter::printChar(double d) {
    std::cout << "char: ";
    if (std::isnan(d))            std::cout << "impossible\n";
    else if (std::isinf(d))       std::cout << "overflow\n";
    else {
        int v = static_cast<int>(d);
        if (v < std::numeric_limits<char>::min() || v > std::numeric_limits<char>::max())
            std::cout << "Non displayable\n";
        else if (!isDisplayableChar(v))
            std::cout << "Non displayable\n";
        else
            std::cout << "'" << static_cast<char>(v) << "'\n";
    }
}

void ScalarConverter::printInt(double d) {
    std::cout << "int: ";
    if (std::isnan(d))            std::cout << "impossible\n";
    else if (std::isinf(d))       std::cout << "overflow\n";
    else if (d < std::numeric_limits<int>::min() || d > std::numeric_limits<int>::max())
        std::cout << "overflow\n";
    else
        std::cout << static_cast<int>(d) << "\n";
}

static bool isFloatPseudoLiteral(const std::string &s) {
    return (s == "nanf" || s == "+inff" || s == "-inff");
}
static bool isDoublePseudoLiteral(const std::string &s) {
    return (s == "nan" || s == "+inf" || s == "-inf");
}

void ScalarConverter::convert(const std::string &literal) {
    const std::string &s = literal;
    bool isFloatPseudo  = isFloatPseudoLiteral(s);
    bool isDoublePseudo = isDoublePseudoLiteral(s);
    bool isFloatDec     = isFloatLiteral(s) && !isFloatPseudo;
    bool isDoubleDec    = isDoubleLiteral(s) && !isDoublePseudo;
    bool isCharLit      = isCharLiteral(s);
    // 純粋整数文字列か
    size_t start = (!s.empty() && (s[0]=='+'||s[0]=='-'))?1:0;
    bool isPureInt = (start < s.size());
    for (size_t i = start; i < s.size(); ++i) if (!std::isdigit((unsigned char)s[i])) { isPureInt = false; break; }
    bool isIntLit    = isIntLiteral(s);
    bool intOverflow = isPureInt && !isIntLit;

    double value = 0.0;
    if      (isCharLit)             value = static_cast<double>(s[1]);
    else if (isFloatPseudo)         { std::string c = s.substr(0, s.size()-1); value = std::strtod(c.c_str(), (char**)NULL); }
    else if (isDoublePseudo)        value = std::strtod(s.c_str(), (char**)NULL);
    else if (isIntLit)              value = static_cast<double>(std::strtol(s.c_str(), (char**)NULL, 10));
    else if (isFloatDec)            { std::string c = s.substr(0, s.size()-1); value = std::strtod(c.c_str(), (char**)NULL); }
    else if (isDoubleDec || intOverflow) value = std::strtod(s.c_str(), (char**)NULL);
    else { std::cerr << "Error: invalid literal\n"; return; }

    printChar(value);
    printInt(value);

    // float 出力
    std::cout << "float: ";
    if      (isFloatPseudo)      std::cout << s << "\n";
    else if (isDoublePseudo)     std::cout << s << "f\n";
    else if (isFloatDec)         std::cout << s << "\n";
    else if (isDoubleDec)        std::cout << s << "f\n";
    else if (intOverflow) {
        std::ostringstream oss;
        if (!s.empty() && s[0] == '-')
            oss << std::fixed << std::setprecision(1) << static_cast<float>(value);
        else
            oss << static_cast<float>(value);
        std::cout << oss.str() << "f\n";
    }
    else if (isIntLit || isCharLit) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << static_cast<float>(value);
        std::cout << oss.str() << "f\n";
    }
    else {
        std::ostringstream oss;
        oss << static_cast<float>(value);
        std::cout << oss.str() << "f\n";
    }

    // double 出力
    std::cout << "double: ";
    if      (isDoublePseudo)     std::cout << s << "\n";
    else if (isFloatPseudo)      std::cout << s.substr(0, s.size()-1) << "\n";
    else if (isDoubleDec)        std::cout << s << "\n";
    else if (isFloatDec)         std::cout << s.substr(0, s.size()-1) << "\n";
    else if (isIntLit || isCharLit) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << value;
        std::cout << oss.str() << "\n";
    }
    else {
        std::ostringstream oss;
        oss << static_cast<double>(value);
        std::cout << oss.str() << "\n";
    }
}
