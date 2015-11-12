/*
 * Author: number
 * Date  : Nov 12, 2015
 */

#ifndef __COMMON_C_MAP_H_H___
#define __COMMON_C_MAP_H_H___

#include <string>
#include <stack>
#include <vector>
#include <unordered_map>

namespace ef {

class CMap {
private:
    enum CMapItemType {
        kCMapItemGlobal,
        kCMapItemObject,
        kCMapItemArray
    };

    typedef std::string String;
    typedef std::unordered_map<std::string, String> Object;
    typedef std::vector<Object> Array;
    union Storage {
        Storage() {}
        ~Storage() {}

        String str;
        Object obj;
        Array arr;
    };

    struct Value {
        Value () {}
        virtual ~Value () {}
        Value & operator= (const Value & v)
        {
            type = v.type;
            if (type  == kCMapItemGlobal) {
                value.str = v.value.str;
            } else if (type == kCMapItemArray) {
                value.arr = v.value.arr;
            } else if (type == kCMapItemObject) {
                value.obj = v.value.obj;
            }
        }

        Storage value;
        CMapItemType type;
    };
    typedef std::unordered_map<std::string, Value> CMapContent;

public:
    CMap();
    virtual ~CMap();
    bool Load(const char * path);
    const std::string & GetErrMsg() const;

private:
    bool HandleContent(const std::string &str, int line,
                       std::stack<CMapItemType> *tst,
                       std::stack<void *> *pst);

private:
    CMapContent _content;
    std::string _errmsg;
};

} /* namespace ef */

#endif /* __COMMON_C_MAP_H__ */
