/*
 * Author: number
 * Date  : Nov 1, 2015
 */

#include "time_counter.h"

#include <stdio.h>

namespace ef {

TimeCounter::TimeCounter()
{
    gettimeofday(&_begin, nullptr);
}

TimeCounter::~TimeCounter()
{
    gettimeofday(&_end, nullptr);
    int sec = _end.tv_sec - _begin.tv_sec;
    int usec = _end.tv_usec - _begin.tv_usec;
    if (usec < 0) {
        usec += 1000 * 1000;
        --sec;
    }
    int ms = usec / 1000;
    usec = usec % 1000;
    printf("time use: %dsec, %dms, %dusec\n\n", sec, ms, usec);
}

} /* namespace ef */

#if 1

#include <mutex>
#include <atomic>
#include <thread>
#include <random>

void TestNewAndDeleteRandom()
{
    int n = 10000000;
    int * arr = new int[n];
    std::random_device rd;
    for (int i = 0; i < n; ++i) {
        arr[i] = rd() % 100000;
    }

    puts("1000,0000 times new and delete, new size random % 100000");
    ef::TimeCounter counter;
    for (int i=0; i<n; i++) {
        char * t = new char[arr[i]];
        delete []t;
    }
}

void TestNewAndDelete()
{
    int n = 10000000;
    puts("1000,0000 times new and delete, new size 1000");
    ef::TimeCounter counter;
    for (int i=0; i<n; i++) {
        char * t = new char[1000];
        delete []t;
    }
}

void TestStdMutex()
{
    puts("1000,0000 times std mutex lock and unlock");
    int n = 10000000;
    std::mutex lock;
    ef::TimeCounter counter;
    for (int i=0; i<n; i++) {
        lock.lock();
        lock.unlock();
    }
}

void TestGetTimeofday()
{
    puts("1000,0000 times gettimeofday");
    int n = 10000000;
    timeval tv;
    ef::TimeCounter counter;
    for (int i=0; i<n; i++) {
        gettimeofday(&tv, 0);
    }
}

void TestTime()
{
    puts("1000,0000 times time(NULL)");
    int n = 10000000;
    ef::TimeCounter counter;
    for (int i=0; i<n; i++) {
        time(NULL);
    }
}
void TestAtomicInt()
{
    puts("1000,0000 times std atomic_int add");
    std::atomic_int t(0);
    int n = 10000000;
    ef::TimeCounter counter;
    for (int i=0; i<n; i++) {
        ++t;
    }
}

class A {
};

void TestEmptyObject()
{
    puts("1000,0000 times empty object create");
    int n = 10000000;
    ef::TimeCounter counter;
    for (int i=0; i<n; i++) {
        A a;
    }
}
int main(int argc, char *argv[])
{
    TestStdMutex();
    TestGetTimeofday();
    TestNewAndDelete();
    TestNewAndDeleteRandom();
    TestAtomicInt();
    TestEmptyObject();
    TestTime();
    return 0;
}

#endif
