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

enum CMapItemType {
    kValueNull,   // use internal
    kValueGlobal,   // use internal
    kValueString,
    kValueObject,
    kValueArray,
};

struct Value;
typedef std::unordered_map<std::string, Value> Object;
typedef std::vector<Value> Array;
typedef std::unordered_map<std::string, Value> CMapContent;

extern const Value kNullValue;

struct Value {
    Value () : value(nullptr), type(kValueNull) {};
    Value (CMapItemType t) : value(nullptr), type(t) {};
    Value(const Value &val) { Copy(val); }
    Value & operator=(const Value &val) { Copy(val); return *this;}

    virtual ~Value ()
    {
        Destory();
    }

    void Destory() {
        if (type  == kValueString) {
            std::string *str = static_cast<std::string*>(value);
            delete str;
        } else if (type == kValueArray) {
            Array *arr = static_cast<Array*>(value);
            delete arr;
        } else if (type == kValueObject) {
            Object *obj = static_cast<Object*>(value);
            delete obj;
        }

        value  = nullptr;
    }

    void Copy(const Value& val) {
        // 如果有数据,先删除
        if (value) {
            Destory();
        }

        // 少一个type赋值导致的大坑,使用vecotr保存的对象,因为内存需要连续,因此每次push的时候
        // 有时候会复制整个vector中的每个对象,因而type混乱,导致type全乱
        // 做好类型检查很重要啊
        type = val.type;
        if (val.value) {
            if (type  == kValueString) {
                std::string *str = static_cast<std::string*>(val.value);
                value = static_cast<void*>(new std::string(*str));
            } else if (type == kValueArray) {
                Array *arr = static_cast<Array*>(val.value);
                value = static_cast<void*>(new Array(*arr));
            } else if (type == kValueObject) {
                Object *obj = static_cast<Object*>(val.value);
                value = static_cast<void*>(new Object(*obj));
            }
        }
    }

    const std::string str() const {
        if (type != kValueString) {
            return "";
        }
        return *static_cast<std::string*>(value);
    }

    const Value & operator[] (const std::string key) const
    {
        if (type != kValueObject) {
            return kNullValue;
        }

        const Object *obj = static_cast<Object*>(value);
        if (!obj) {
            return kNullValue;
        }

        Object::const_iterator it = obj->find(key);
        if (it == obj->end()) {
            return kNullValue;
        }

        return it->second;
    }

    const Value & operator[] (uint32_t index) const
    {
        if (type != kValueArray) {
            return kNullValue;
        }

        Array * arr = static_cast<Array*>(value);
        if (!arr || arr->size() <= index) {
            return kNullValue;
        }

        return (*arr)[index];
    }

    void *value;
    CMapItemType type;
};


class CMap {
public:
    CMap();
    virtual ~CMap();
    bool Load(const char * path);
    const std::string & GetErrMsg() const;
    void Print() const;
    const Value & operator[] (const std::string key) const;

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
