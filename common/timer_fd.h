/*
 * Author: number
 * Date  : Nov 10, 2015
 */

#ifndef __COMMON_TIMER_FD_H_H___
#define __COMMON_TIMER_FD_H_H___

#include <sys/timerfd.h>

namespace ef {

class TimerFd {
public:
    TimerFd();
    virtual ~TimerFd();

    bool Initialize(int flag = 0, int clockId = CLOCK_MONOTONIC);

    bool SetOneShotTimer(const struct timespec &timeSpec);
    bool SetPeriodTimer(const struct timespec &timeSpec);
    bool SetTimer(const struct timespec firstExpireTime, const struct timespec interval);

    int GetTimerFd();

private:
    int _timer_fd;
};

} /* namespace ef */

#endif /* __COMMON_TIMER_FD_H__ */
