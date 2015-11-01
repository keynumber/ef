/*
 * Author: number
 * Date  : Nov 1, 2015
 */

#ifndef __TOOLS_TIME_COUNTER_H_H___
#define __TOOLS_TIME_COUNTER_H_H___

#include <sys/time.h>

namespace ef {

class TimeCounter {
public:
    TimeCounter();
    virtual ~TimeCounter();

private:
    timeval _begin;
    timeval _end;
};

} /* namespace ef */

#endif /* __TOOLS_TIME_COUNTER_H__ */
