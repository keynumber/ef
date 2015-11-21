/*
 * Author: number
 * Date  : Nov 10, 2015
 */

#ifndef __COMMON_STRING_UTIL_H_H___
#define __COMMON_STRING_UTIL_H_H___

#include <string>
#include <vector>

namespace ef {

void split(const std::string &str, const std::string &delim, std::vector<std::string> *vec);
void splitany(const std::string &str, const std::string &charset, std::vector<std::string> *vec);
bool startwith(const std::string &str, const std::string &prefix);
bool endwith(const std::string &str, const std::string &prefix);

std::string trim(const std::string &str);
std::string ltrim(const std::string &str);
std::string rtrim(const std::string &str);

} /* namespace ef */

#endif /* __COMMON_STRING_UTIL_H__ */
