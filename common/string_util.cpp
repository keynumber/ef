/*
 * Author: number
 * Date  : Nov 10, 2015
 */


#include "string_util.h"

#include <cstring>

void Split(const std::string &str, const std::string &delim, std::vector<std::string> *vec)
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

        vec->push_back(str.substr(start, pos-start));
        start = pos;
        if (pos == std::string::npos) {
            break;
        }
    }
}

bool StartWith(const std::string &str, const std::string &prefix)
{
    std::string::size_type str_len = str.length();
    std::string::size_type pre_len = prefix.length();
    if (str_len < pre_len)
        return false;
    return 0 == str.compare(0, pre_len, prefix);
}

bool EndWith(const std::string &str, const std::string &prefix)
{
    std::string::size_type str_len = str.length();
    std::string::size_type pre_len = prefix.length();
    if (str_len < pre_len)
        return false;
    return 0 == str.compare(str_len - pre_len, prefix.length(), prefix);
}


#if 1

#include <stdio.h>

#include <iostream>
using namespace std;

void TestSplit(const char *str, const std::string &delim)
{
    std::vector<std::string> vec;
    Split(str, delim, &vec);
    cout << vec.size() << "\t";
    for (const auto & it : vec) {
        cout << it << "\t";
    }
    cout << endl;
}

void TestStartWithAndEndWith(const std::string &str, const std::string &prefix) {
    printf("%s star with %s \n", str.c_str(), prefix.c_str());
    printf("%d\n", StartWith(str, prefix));
    printf("%s end with %s \n", str.c_str(), prefix.c_str());
    printf("%d\n", EndWith(str, prefix));
}

int main(int argc, char *argv[])
{
    puts("test split");
    TestSplit( "asdf", "asdf");
    TestSplit( "asdfasdf", "asdf");
    TestSplit( "hello world", " ");
    TestSplit( "hello       world", " ");
    TestSplit( "helloasdfasdfworldadsaf", "asdf");
    TestSplit( "asdfasdfhelloasdfasdfworldadsafasdftest", "asdf");
    TestSplit( "asdasdfhelloasdfasdfworldadsafasdftest", "asdf");

    puts("\ntest start with and end with");
    TestStartWithAndEndWith("help", "help");
    TestStartWithAndEndWith("help", "helpa");
    TestStartWithAndEndWith("helpa", "help");
    TestStartWithAndEndWith("helpa", "help");
    TestStartWithAndEndWith("helpa", "elp");
    TestStartWithAndEndWith("help", "elp");

    return 0;
}

#endif

