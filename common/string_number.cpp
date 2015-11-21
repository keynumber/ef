/*
 * Author: number
 * Date  : Nov 22, 2015
 */

#include "string_number.h"

#include <cstdlib>

namespace ef {

// char
template <>
char convert(const char * const str, char **endptr, int base)
{
    return static_cast<char>(strtol(str, endptr, base));
}

template <>
unsigned char convert(const char * const str, char **endptr, int base)
{
    return static_cast<unsigned char>(strtoul(str, endptr, base));
}

// short
template <>
short convert(const char * const str, char **endptr, int base)
{
    return static_cast<short>(strtol(str, endptr, base));
}

template <>
unsigned short convert(const char * const str, char **endptr, int base)
{
    return static_cast<unsigned short>(strtoul(str, endptr, base));
}

// int
template <>
int convert(const char * const str, char **endptr, int base)
{
    return static_cast<int>(strtol(str, endptr, base));
}

template <>
unsigned int convert(const char * const str, char **endptr, int base)
{
    return static_cast<unsigned int>(strtoul(str, endptr, base));
}

// long
template <>
long convert(const char * const str, char **endptr, int base)
{
    return static_cast<long>(strtol(str, endptr, base));
}

template <>
unsigned long convert(const char * const str, char **endptr, int base)
{
    return static_cast<unsigned long>(strtoul(str, endptr, base));
}

// long long
template <>
long long convert(const char * const str, char **endptr, int base)
{
    return static_cast<long long>(strtoll(str, endptr, base));
}

template <>
unsigned long long convert(const char * const str, char **endptr, int base)
{
    return static_cast<unsigned long long>(strtoull(str, endptr, base));
}

// float
template <>
float convert(const char * const str, char **endptr, int base)
{
    base = base;            // just to escape warning
    return static_cast<float>(strtof(str, endptr));
}

// double
template <>
double convert(const char * const str, char **endptr, int base)
{
    base = base;            // just to escape warning
    return static_cast<double>(strtod(str, endptr));
}

// long double
template <>
long double convert(const char * const str, char **endptr, int base)
{
    base = base;            // just to escape warning
    return static_cast<long double>(strtold(str, endptr));
}

// char
template <>
char convert(const std::string & str, char **endptr, int base)
{
    return static_cast<char>(strtol(str.c_str(), endptr, base));
}

template <>
unsigned char convert(const std::string & str, char **endptr, int base)
{
    return static_cast<unsigned char>(strtoul(str.c_str(), endptr, base));
}

// short
template <>
short convert(const std::string & str, char **endptr, int base)
{
    return static_cast<short>(strtol(str.c_str(), endptr, base));
}

template <>
unsigned short convert(const std::string & str, char **endptr, int base)
{
    return static_cast<unsigned short>(strtoul(str.c_str(), endptr, base));
}

// int
template <>
int convert(const std::string & str, char **endptr, int base)
{
    return static_cast<int>(strtol(str.c_str(), endptr, base));
}

template <>
unsigned int convert(const std::string & str, char **endptr, int base)
{
    return static_cast<unsigned int>(strtoul(str.c_str(), endptr, base));
}

// long
template <>
long convert(const std::string & str, char **endptr, int base)
{
    return static_cast<long>(strtol(str.c_str(), endptr, base));
}

template <>
unsigned long convert(const std::string & str, char **endptr, int base)
{
    return static_cast<unsigned long>(strtoul(str.c_str(), endptr, base));
}

// long long
template <>
long long convert(const std::string & str, char **endptr, int base)
{
    return static_cast<long long>(strtoll(str.c_str(), endptr, base));
}

template <>
unsigned long long convert(const std::string & str, char **endptr, int base)
{
    return static_cast<unsigned long long>(strtoull(str.c_str(), endptr, base));
}

// float
template <>
float convert(const std::string & str, char **endptr, int base)
{
    base = base;            // just to escape warning
    return static_cast<float>(strtof(str.c_str(), endptr));
}

// double
template <>
double convert(const std::string & str, char **endptr, int base)
{
    base = base;            // just to escape warning
    return static_cast<double>(strtod(str.c_str(), endptr));
}

// long double
template <>
long double convert(const std::string & str, char **endptr, int base)
{
    base = base;            // just to escape warning
    return static_cast<long double>(strtold(str.c_str(), endptr));
}

} /* namespace ef */
