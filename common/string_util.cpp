/*
 * Author: number
 * Date  : Nov 10, 2015
 */


#include "string_util.h"

#include <cstring>

const char *kDelimiter = " \t\r\n\f\v\b\a";

void split(const std::string &str, const std::string &delim, std::vector<std::string> *vec)
{
    std::string::size_type str_len = str.length();
    std::string::size_type delim_len = delim.length();
    std::string::size_type start = 0;
    std::string::size_type pos = 0;

    while (start < str_len) {
        pos = str.find(delim, start);
        if (pos == start) {
            start += delim_len;
            continue;
        }

        if (pos == std::string::npos) {
            vec->push_back(str.substr(start));
            break;
        } else {
            vec->push_back(str.substr(start, pos-start));
        }

        start = pos;
    }
}

void splitany(const std::string &str, const std::string &charset, std::vector<std::string> *vec)
{
    std::string::size_type start = str.find_first_not_of(charset);
    std::string::size_type pos = start;

    while (start != std::string::npos) {
        pos = str.find_first_of(charset, start);

        if (pos == std::string::npos) {
            vec->push_back(str.substr(start));
            break;
        }
        vec->push_back(str.substr(start, pos-start));
        start = str.find_first_not_of(charset, pos);
    }
}

bool startwith(const std::string &str, const std::string &prefix)
{
    std::string::size_type str_len = str.length();
    std::string::size_type pre_len = prefix.length();
    if (str_len < pre_len)
        return false;
    return 0 == str.compare(0, pre_len, prefix);
}

bool endwith(const std::string &str, const std::string &prefix)
{
    std::string::size_type str_len = str.length();
    std::string::size_type pre_len = prefix.length();
    if (str_len < pre_len)
        return false;
    return 0 == str.compare(str_len - pre_len, prefix.length(), prefix);
}

std::string trim(const std::string &str) {
    std::string::size_type strhead = str.find_first_not_of(kDelimiter);
    if (strhead == std::string::npos) {
        return std::string();
    }

    std::string::size_type strtail = str.find_last_not_of(kDelimiter);
    return str.substr(strhead, strtail - strhead + 1);
}

std::string ltrim(const std::string &str) {
    std::string::size_type strhead = str.find_first_not_of(kDelimiter);
    if (strhead == std::string::npos) {
        return std::string();
    } else {
        return str.substr(strhead);
    }
}

std::string rtrim(const std::string &str) {
    std::string::size_type strtail = str.find_last_not_of(kDelimiter);
    if (strtail == std::string::npos) {
        return std::string();
    } else {
        return str.substr(0, strtail + 1);
    }
}

#if 1

#include <stdio.h>

#include <iostream>
using namespace std;

void testsplit(const char *str, const std::string &delim)
{
    cout << str << " testsplit with " << delim << endl;
    std::vector<std::string> vec;
    split(str, delim, &vec);
    cout << vec.size() << "\t";
    for (const auto & it : vec) {
        cout << it << "\t";
    }
    cout << endl;
}

void testsplitany(const char *str, const std::string &charset)
{
    cout << str << " testsplitany with " << charset << endl;
    std::vector<std::string> vec;
    splitany(str, charset, &vec);
    cout << vec.size() << "\t";
    for (const auto & it : vec) {
        cout << it << "\t";
    }
    cout << endl;
}

void teststartwithandendwith(const std::string &str, const std::string &prefix) {
    printf("%s star with %s \n", str.c_str(), prefix.c_str());
    printf("%d\n", startwith(str, prefix));
    printf("%s end with %s \n", str.c_str(), prefix.c_str());
    printf("%d\n", endwith(str, prefix));
}

int main(int argc, char *argv[])
{
    puts(" ---- test split --------");
    testsplit( "asdf", "asdf");
    testsplit( "asdfasdf", "asdf");
    testsplit( "hello world", " ");
    testsplit( "hello       world", " ");
    testsplit( "helloasdfasdfworldadsaf", "asdf");
    testsplit( "asdfasdfhelloasdfasdfworldadsafasdftest", "asdf");
    testsplit( "asdasdfhelloasdfasdfworldadsafasdftest", "asdf");

    puts("\n ---- test splitany --------");
    testsplitany("asdfasdffdadf", "ekpo");
    testsplitany("asdfasdffdadf", "asdf");
    testsplitany("asdfasdffdadf", "as");
    testsplitany("asdfassssaasasa", "as");
    testsplitany("asassssaasasafd", "as");
    testsplitany("asasssdfsaasasa", "as");
    testsplitany("asdfssdfsaassfd", "as");
    testsplitany("dfssdasdsaassfd", "as");

    puts("\n ---------- test start with and end with --------");
    teststartwithandendwith("help", "help");
    teststartwithandendwith("help", "helpa");
    teststartwithandendwith("helpa", "help");
    teststartwithandendwith("helpa", "help");
    teststartwithandendwith("helpa", "elp");
    teststartwithandendwith("help", "elp");

    return 0;
}

#endif

