#include "ScalarConverter.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cerrno>
#include <cstdlib>
#include <cctype>
#include <cmath>

static bool isPseudoLiteral(const std::string& s)
{
    return (s == "nan" || s == "+inf" || s == "-inf" || s == "inf"
         || s == "nanf" || s == "+inff" || s == "-inff" || s == "inff");
}

static bool isFloatLiteral(const std::string& s)
{
    // la chaîne doit finir par f
    if (s.empty() || s[s.size() - 1] != 'f')
        return false;

	// On enlève le f final pour analyser le nombre
    std::string core = s.substr(0, s.size() - 1);
    if (core.empty())
        return false;

    // autoriser nanf, inff...
    if (isPseudoLiteral(s))
        return true;


    char* end = 0;
    errno = 0;
    std::strtod(core.c_str(), &end); //strtod convertit une chaîne en double
    if (end == core.c_str() || *end != '\0')
        return false;


    bool hasDigit = false;
    for (size_t i = 0; i < core.size(); ++i)
        if (std::isdigit(static_cast<unsigned char>(core[i])))
            hasDigit = true;

    return hasDigit;
}

static bool isDoubleLiteral(const std::string& s)
{
    if (s.empty())
        return false;

    if (isPseudoLiteral(s))
        return (s == "nan" || s == "+inf" || s == "-inf" || s == "inf");

    // strtod doit consommer tout
    char* end = 0;
    errno = 0;
    std::strtod(s.c_str(), &end);
    if (end == s.c_str() || *end != '\0')
        return false;

    bool hasDigit = false;
    for (size_t i = 0; i < s.size(); ++i)
        if (std::isdigit(static_cast<unsigned char>(s[i])))
            hasDigit = true;

    return hasDigit;
}

static bool isIntLiteral(const std::string& s)
{
    if (s.empty())
        return false;

    size_t i = 0;
    if (s[i] == '+' || s[i] == '-')
        i++;

    if (i >= s.size())
        return false;

    for (; i < s.size(); ++i)
    {
        if (!std::isdigit(static_cast<unsigned char>(s[i])))
            return false;
    }
    return true;
}

static bool isCharLiteral(const std::string& s)
{
    return (s.size() == 1 && !std::isdigit(static_cast<unsigned char>(s[0])));
}

static double parseToDouble(const std::string& s, bool& ok)
{
    ok = true;

    if (s == "nan" || s == "nanf")
        return std::numeric_limits<double>::quiet_NaN();

    if (s == "+inf" || s == "inf" || s == "+inff" || s == "inff")
        return std::numeric_limits<double>::infinity();

    if (s == "-inf" || s == "-inff")
        return -std::numeric_limits<double>::infinity();

    errno = 0;
    char* end = 0;

    if (isFloatLiteral(s))
    {
        std::string core = s.substr(0, s.size() - 1);
        double d = std::strtod(core.c_str(), &end);
        if (end == core.c_str() || *end != '\0' || errno == ERANGE)
            ok = false;
        return d;
    }
    else
    {
        double d = std::strtod(s.c_str(), &end);
        if (end == s.c_str() || *end != '\0' || errno == ERANGE)
            ok = false;
        return d;
    }
}

static bool isFiniteDouble(double d)
{
    return !(std::isnan(d) || std::isinf(d));
}

static bool hasNoFraction(double d)
{
    if (!isFiniteDouble(d))
        return false;
    double intpart;
    double frac = std::modf(d, &intpart);
    return frac == 0.0;
}

static void printChar(double d)
{
    std::cout << "char: ";
    if (!isFiniteDouble(d))
    {
        std::cout << "impossible" << std::endl;
        return;
    }
    if (d < 0.0 || d > 255.0)
    {
        std::cout << "impossible" << std::endl;
        return;
    }
    char c = static_cast<char>(d);
    if (!std::isprint(static_cast<unsigned char>(c)))
    {
        std::cout << "Non displayable" << std::endl;
        return;
    }
    std::cout << "'" << c << "'" << std::endl;
}

static void printInt(double d)
{
    std::cout << "int: ";
    if (!isFiniteDouble(d))
    {
        std::cout << "impossible" << std::endl;
        return;
    }
    if (d < static_cast<double>(std::numeric_limits<int>::min()) ||
        d > static_cast<double>(std::numeric_limits<int>::max()))
    {
        std::cout << "impossible" << std::endl;
        return;
    }
    int i = static_cast<int>(d);
    std::cout << i << std::endl;
}

static void printFloat(double d)
{
    std::cout << "float: ";

    float f = static_cast<float>(d);

    if (std::isnan(d))
    {
        std::cout << "nanf" << std::endl;
        return;
    }
    if (std::isinf(d))
    {
        std::cout << (d < 0 ? "-inff" : "+inff") << std::endl;
        return;
    }

    if (hasNoFraction(d))
        std::cout << std::fixed << std::setprecision(1) << f << "f" << std::endl;
    else
        std::cout << std::fixed << std::setprecision(6) << f << "f" << std::endl;
}

static void printDouble(double d)
{
    std::cout << "double: ";

    if (std::isnan(d))
    {
        std::cout << "nan" << std::endl;
        return;
    }
    if (std::isinf(d))
    {
        std::cout << (d < 0 ? "-inf" : "+inf") << std::endl;
        return;
    }

    if (hasNoFraction(d))
        std::cout << std::fixed << std::setprecision(1) << d << std::endl;
    else
        std::cout << std::fixed << std::setprecision(6) << d << std::endl;
}

ScalarConverter::ScalarConverter() {}
ScalarConverter::ScalarConverter(const ScalarConverter& other) { (void)other; }
ScalarConverter& ScalarConverter::operator=(const ScalarConverter& other) { (void)other; return *this; }
ScalarConverter::~ScalarConverter() {}

void ScalarConverter::convert(const std::string& literal)
{
    double value = 0.0;
    bool ok = true;

    if (isCharLiteral(literal))
    {
        value = static_cast<double>(literal[0]);
    }
    else if (isPseudoLiteral(literal) || isIntLiteral(literal) || isFloatLiteral(literal) || isDoubleLiteral(literal))
    {
        value = parseToDouble(literal, ok);
    }
    else
    {
        ok = false;
    }

    if (!ok)
    {
        std::cout << "char: impossible" << std::endl;
        std::cout << "int: impossible" << std::endl;
        std::cout << "float: impossible" << std::endl;
        std::cout << "double: impossible" << std::endl;
        return;
    }

    printChar(value);
    printInt(value);
    printFloat(value);
    printDouble(value);
}