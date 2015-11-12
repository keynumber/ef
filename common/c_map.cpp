/*
 * Author: number
 * Date  : Nov 12, 2015
 */

#include "c_map.h"

#include <cassert>
#include <fstream>

#include "string_util.h"

namespace ef {

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

    type_st.push(kCMapItemGlobal);
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
        str = trim("\r\n\t ");

        if(str == "") {
            continue;
        }

        if (!HandleContent(str, line, &type_st, &content_pst)) {
            return false;
        }
    }

    in.close();
    return true;
}

bool CMap::HandleContent(const std::string &str, int line,
                         std::stack<CMapItemType> *tst,
                         std::stack<void *> *pst)
{
    std::string::size_type pos = str.find_first_of(kCMapIdentifier);
    if (pos == std::string::npos) {
        _errmsg = "format error at line " + std::to_string(line) + str;
        return false;
    }

    CMapItemType type = tst->top();
    void *p = pst->top();

    switch (str[pos]) {
    case '=': {
        std::vector<std::string> sp;
        split(str, "\t\r\n =", &sp);
        if (sp.size() != 2) {
            _errmsg = "format error at line " + std::to_string(line) + str;
            return false;
        }

        const std::string &key = sp[0];
        const std::string &value= sp[1];

        if (type == kCMapItemGlobal) {
            Value val;
            val.type = kCMapItemGlobal;
            val.value.str = value;

            CMapContent &content = *(static_cast<CMapContent*>(p));
            content[key] = val;
        } else if (type == kCMapItemObject) {
            Object & obj =  *(static_cast<Object*>(p));
            obj[key] = value;
        } else {
            assert(false);
        }

        break;
    }

    case '{': {

        void *p = nullptr;
        std::string key = trim(str.substr(0, pos));
        if (type == kCMapItemGlobal) {
            CMapContent * content = static_cast<CMapContent*>(p);
            p = static_cast<void*>(&content[key]);
        } else if (type == kCMapItemArray) {        // TODO compile error
            Array * arr =  static_cast<Array*>(p);
            arr->push_back(Object());
            p = static_cast<void*>(&arr->back());
        } else {
            assert(false);
        }

        tst->push(kCMapItemObject);
        pst->push(p);
        break;
    }

    case '}': {
        break;
    }

    case '[': {
        break;
    }

    case ']': {
        break;
    }

    default: {
        assert(false);
        break;
    }
    }
}

const std::string & CMap::GetErrMsg() const
{
    return _errmsg;
}

} /* namespace ef */

#if 0

int main()
{
    return 0;
}

#endif
