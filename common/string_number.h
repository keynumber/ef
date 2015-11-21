/*
 * Author: number
 * Date  : Nov 22, 2015
 */

#ifndef __COMMON_STRING_NUMBER_H_H___
#define __COMMON_STRING_NUMBER_H_H___

#include <string>

namespace ef {

// ---------------- string to number ----------------
// for (unsigned) char/short/long/long long
template <class T> T convert(const char * const str, char **endptr = nullptr, int base = 10);
// for const std::string &
template <class T> T convert(const std::string & str, char **endptr = nullptr, int base = 10);
// for float/double/long double
template <class T> T convert(const char * const str, char **endptr = nullptr);
// for const std::string &
template <class T> T convert(const std::string & str, char **endptr = nullptr);

// ---------------- number to string ----------------
template <class T> std::string tostring(T val);




// for float/double/long double
template <class T>
T convert(const char * const str, char **endptr = nullptr);

template <> char convert(const char * const str, char **endptr, int base);
template <> unsigned char convert(const char * const str, char **endptr, int base);
template <> short convert(const char * const str, char **endptr, int base);
template <> unsigned short convert(const char * const str, char **endptr, int base);
template <> int convert(const char * const str, char **endptr, int base);
template <> unsigned int convert(const char * const str, char **endptr, int base);
template <> long convert(const char * const str, char **endptr, int base);
template <> unsigned long convert(const char * const str, char **endptr, int base);
template <> long long convert(const char * const str, char **endptr, int base);
template <> unsigned long long convert(const char * const str, char **endptr, int base);
template <> float convert(const char * const str, char **endptr);
template <> double convert(const char * const str, char **endptr);
template <> long double convert(const char * const str, char **endptr);

template <> char convert(const std::string & str, char **endptr, int base);
template <> unsigned char convert(const std::string & str, char **endptr, int base);
template <> short convert(const std::string & str, char **endptr, int base);
template <> unsigned short convert(const std::string & str, char **endptr, int base);
template <> int convert(const std::string & str, char **endptr, int base);
template <> unsigned int convert(const std::string & str, char **endptr, int base);
template <> long convert(const std::string & str, char **endptr, int base);
template <> unsigned long convert(const std::string & str, char **endptr, int base);
template <> long long convert(const std::string & str, char **endptr, int base);
template <> unsigned long long convert(const std::string & str, char **endptr, int base);
template <> float convert(const std::string & str, char **endptr);
template <> double convert(const std::string & str, char **endptr);
template <> long double convert(const std::string & str, char **endptr);

} /* namespace ef */

#endif /* __COMMON_STRING_NUMBER_H__ */
