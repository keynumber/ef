/*
 * Author: number
 * Date  : Nov 12, 2015
 */

#include "c_map.h"

#include <cassert>
#include <cstdio>
#include <fstream>

#include "string_util.h"

namespace ef {

const Value kNullValue(kValueNull);

static const char kCMapIdentifier[] = "{}[]=";

CMap::CMap()
{
}

CMap::~CMap()
{
}

bool CMap::Load(const char * path)
{
    std::ifstream in(path, std::ifstream::in);
    if (!in.is_open()) {
        _errmsg = std::string("open ") + path + " failed";
        return false;
    }

    std::stack<CMapItemType> type_st;
    std::stack<void *> content_pst;

    type_st.push(kValueGlobal);
    content_pst.push(static_cast<void*>(&_content));

    int line = 0;
    char buf[2048];
    while (!in.eof()) {
        ++ line;
        in.getline(buf, 2048);
        std::string str(buf);

        // 删除注释
        std::string::size_type pos = str.find('#');
        str = str.substr(0, pos);
        str = trim(str);

        if(str == "") {
            continue;
        }

        if (!HandleContent(str, line, &type_st, &content_pst)) {
            return false;
        }
    }

    in.close();

    if (type_st.size() != 1 || content_pst.size() != 1) {
        _errmsg = "wrong format, uncomplete file";
        return false;
    }
    return true;
}

bool CMap::HandleContent(const std::string &str, int line,
                         std::stack<CMapItemType> *tst,
                         std::stack<void *> *pst)
{
    std::string::size_type pos = str.find_first_of(kCMapIdentifier);
    if (pos == std::string::npos) {
        _errmsg = "format error at line " + std::to_string(line) + ": " + str;
        return false;
    }

    CMapItemType type = tst->top();
    void * cur_content = pst->top();

    switch (str[pos]) {
    case '=':
    {
        std::vector<std::string> sp;
        splitany(str, "\t\r\n\r =", &sp);
        if (sp.size() != 2) {
            _errmsg = "format error at line " + std::to_string(line) + ": " + str;
            return false;
        }

        const std::string &key = sp[0];
        const std::string &value= sp[1];

        if (type == kValueGlobal)
        {
            CMapContent &content = *(static_cast<CMapContent*>(cur_content));
            CMapContent::iterator it = content.find(key);
            if (it != content.end()) {
                content.erase(it);
            }

            Value &val = content[key];
            val.type = kValueString;
            val.value = static_cast<void*>(new std::string(value));
        }
        else if (type == kValueObject)
        {
            Object & obj =  *static_cast<Object*>(cur_content);
            Object::iterator it = obj.find(key);
            if (it != obj.end()) {
                obj.erase(it);
            }

            Value &val = obj[key];
            val.type = kValueString;
            val.value = static_cast<void*>(new std::string(value));
        }
        else
        {
            assert(false);
        }

        break;
    }

    case '{':
    {
        void *p = nullptr;
        std::string key = trim(str.substr(0, pos));


        if (type == kValueGlobal)
        {
            CMapContent & content = *static_cast<CMapContent*>(cur_content);
            // 为了得到val.value,因而不能使用拷贝构造函数,通过获取在map中的引用,得到val.value
            CMapContent::iterator it = content.find(key);
            if (it != content.end()) {
                content.erase(it);
            }

            Value &val = content[key];
            val.type = kValueObject;
            val.value = static_cast<void*>(new Object);
            p = val.value;
        }
        else if (type == kValueArray)
        {
            Array & arr =  *static_cast<Array*>(cur_content);
            Value v;
            arr.push_back(v);
            Value &val = arr.back();
            val.type = kValueObject;
            val.value = static_cast<void*>(new Object);
            p = val.value;
        }
        else
        {
            assert(false);
        }

        tst->push(kValueObject);
        pst->push(p);
        break;
    }

    case '}':
    {
        pst->pop();
        tst->pop();
        break;
    }

    case '[':
    {
        void *p = nullptr;
        std::string key = trim(str.substr(0, pos));

        if (type == kValueGlobal) {
            CMapContent & content = *static_cast<CMapContent*>(cur_content);
            CMapContent::iterator it = content.find(key);
            if (it != content.end()) {
                content.erase(it);
            }

            Value &val = content[key];
            val.type = kValueArray;
            val.value = static_cast<void*>(new Array);
            p = val.value;
        } else {
            assert(false);
        }

        tst->push(kValueArray);
        pst->push(p);
        break;
    }

    case ']':
    {
        pst->pop();
        tst->pop();
        break;
    }

    default:
    {
        assert(false);
        break;
    }

    }
    return true;
}

void CMap::Print() const
{
    for (const auto & it : _content) {
        if (it.second.type == kValueString)
        {
            const std::string & second = *static_cast<std::string*>(it.second.value);
            printf("%s = %s\n", it.first.c_str(), second.c_str());
        }
        else if (it.second.type == kValueObject)
        {
            printf("%s {\n", it.first.c_str());
            // 这里其实应该做类型检查的||-_-..........
            // 还是有时间再做吧.....
            const Object & object = *static_cast<Object*>(it.second.value);
            for (const auto &item : object)
            {
                const std::string & second = *static_cast<std::string*>(item.second.value);
                printf("\t%s = %s\n", item.first.c_str(), second.c_str());
            }
            printf("}\n");

        }
        else if (it.second.type == kValueArray)
        {
            int index = 0;
            printf("%s [\n", it.first.c_str());
            const Array & array = *static_cast<Array*>(it.second.value);
            for (const auto &ait : array)
            {
                printf("\t%d {\n", index++);
                const Object & object = *static_cast<Object*>(ait.value);
                for (const auto & item : object)
                {
                    const std::string & second = *static_cast<std::string*>(item.second.value);
                    printf("\t\t%s = %s\n", item.first.c_str(), second.c_str());
                }
                printf("\t}\n");
            }
            printf("]\n");
        }
    }
}

const Value & CMap::operator[] (const std::string key) const
{
    CMapContent::const_iterator it = _content.find(key);
    if (it == _content.end()) {
        return kNullValue;
    }

    return it->second;
}
const std::string & CMap::GetErrMsg() const
{
    return _errmsg;
}

} /* namespace ef */

#if 1

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <load file name>\n", argv[0]);
        return -1;
    }

    ef::CMap map;
    if (!map.Load(argv[1])) {
        printf("error: %s\n", map.GetErrMsg().c_str());
        return -1;
    }

    cout << "map[key]:\t" << map["key"].str() << endl;
    cout << "map[server][port]:\t" << map["server"]["port"].str() << endl;
    cout << "map[mysql][0][port]:\t" << map["mysql"][0]["port"].str() << endl << endl;

    map.Print();
    return 0;
}

#endif
