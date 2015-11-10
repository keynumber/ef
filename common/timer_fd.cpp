/*
 * Author: number
 * Date  : Nov 10, 2015
 */

#include "timer_fd.h"

#include <unistd.h>

namespace ef {

TimerFd::TimerFd()
    : _timer_fd(-1)
{
}

TimerFd::~TimerFd()
{
    if (_timer_fd != -1)
        close(_timer_fd);
}

bool TimerFd::Initialize(int flag, int clockId)
{
    if ((_timer_fd = timerfd_create(clockId, flag)) == -1) {
        return false;
    }
    return true;
}

bool TimerFd::SetOneShotTimer(const struct timespec& timeSpec)
{
    struct itimerspec value;
    value.it_value.tv_sec = timeSpec.tv_sec;
    value.it_value.tv_nsec = timeSpec.tv_nsec;
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_nsec = 0;
    if (-1 ==timerfd_settime(_timer_fd, TFD_TIMER_ABSTIME, &value, NULL)) {
        return false;
    }
    return true;
}

bool TimerFd::SetPeriodTimer(const struct timespec& timeSpec)
{
    struct itimerspec value;
    value.it_value.tv_sec = timeSpec.tv_sec;
    value.it_value.tv_nsec = timeSpec.tv_nsec;
    value.it_interval.tv_sec = timeSpec.tv_sec;
    value.it_interval.tv_nsec = timeSpec.tv_nsec;
    if (-1 ==timerfd_settime(_timer_fd, 0, &value, NULL)) {
    // if (-1 ==timerfd_settime(_timer_fd, TFD_TIMER_ABSTIME, &value, NULL)) {
        return false;
    }
    return true;
}

bool TimerFd::SetTimer(const struct timespec firstExpireTime,
                              const struct timespec interval)
{
    struct itimerspec value;
    value.it_value.tv_sec = firstExpireTime.tv_sec;
    value.it_value.tv_nsec = firstExpireTime.tv_nsec;
    value.it_interval.tv_sec = interval.tv_sec;
    value.it_interval.tv_nsec = interval.tv_nsec;
    if (-1 ==timerfd_settime(_timer_fd, 0, &value, NULL)) {
    // if (-1 ==timerfd_settime(_timer_fd, TFD_TIMER_ABSTIME, &value, NULL)) {
        return false;
    }
    return true;
}

int TimerFd::GetTimerFd()
{
    return _timer_fd;
}

} /* namespace ef */


#if 1

int main(int argc, char *argv[])
{
    struct timespec now;
    struct timespec timeSpec, inter;

    timeSpec.tv_sec = 1;
    timeSpec.tv_nsec = now.tv_nsec;
    inter.tv_sec = 2;
    inter.tv_nsec = 0;


    ef::TimerFd timer;
    timer.Initialize();

    int fd = timer.GetTimerFd();

    while (true) {
        
    }
    return 0;
}

#endif
