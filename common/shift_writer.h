/*
 * Author: number
 * Date  : Nov 3, 2015
 */

#ifndef __COMMON_SHIFT_WRITER_H_H___
#define __COMMON_SHIFT_WRITER_H_H___

namespace ef {

/**
 * 支持按照文件大小对文件及进行切分,
 */
template <int max_size = 10*1024*1024, int max_num>
class ShiftWriter {
public:
    ShiftWriter();
    virtual ~ShiftWriter();

    void Write(void * buf, int len);
};

} /* namespace ef */

#endif /* __COMMON_SHIFT_WRITER_H__ */
