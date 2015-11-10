/*
 * Author: number
 * Date  : Nov 10, 2015
 */

#ifndef __COMMON_STRING_UTIL_H_H___
#define __COMMON_STRING_UTIL_H_H___


#include <string>
#include <vector>

void Split(const std::string &str, const std::string &delim, std::vector<std::string> *vec);
bool StartWith(const std::string &str, const std::string &prefix);
bool EndWith(const std::string &str, const std::string &prefix);


#endif /* __COMMON_STRING_UTIL_H__ */
