/*
 * Author: number
 * Date  : Nov 15, 2015
 */

#ifndef __COMMON_DATA_STRUCT_ANY_VALUE_H_H___
#define __COMMON_DATA_STRUCT_ANY_VALUE_H_H___

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

namespace ef {

class AnyValue {
public:
    AnyValue();
    virtual ~AnyValue();

    template<class Para, class Value>
    Value & operator[] (const Para & index);

    template<class Para, class Value>
    const Value & operator[] (const Para & index) const;

private:
    enum AnyValueItemType {
        kAnyValueInt8,
        kAnyValueuInt8,
        kAnyValueInt16,
        kAnyValueuInt16,
        kAnyValueInt32,
        kAnyValueuInt32,
        kAnyValueInt64,
        kAnyValueuInt64,
        kAnyValueFloat,
        kAnyValueDouble,
        kAnyValueItemGlobal,
        kAnyValueItemObject,
        kAnyValueItemArray,
    };

private:
    union {
        int8_t _char;
        uint8_t _uchar;
        int16_t _short;
        uint16_t _ushort;
        int32_t _int;
        uint32_t _uint;
        int64_t _int64;
        uint64_t _uint64;
        float _float;
        double _double;
        std::string *str;
        std::vector<AnyValue> *_vector;
        std::unordered_map<std::string, AnyValue> *_map;
        void * _void;
    } _value;

    AnyValueItemType _type;
};

} /* namespace ef */

#endif /* __COMMON_DATA_STRUCT_ANY_VALUE_H__ */
