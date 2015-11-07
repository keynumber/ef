/*
 * Author: number
 * Date  : Nov 8, 2015
 */

#ifndef __COMMON_DATA_STRUCT_FIX_QUEUE_H_H___
#define __COMMON_DATA_STRUCT_FIX_QUEUE_H_H___

#include <assert.h>
#include <stdint.h>

namespace ef {

/**
 * 定长的队列,长度满后插入失败
 * 通过两个变量分别记录队列的队首和队尾:
 * _head: 记录
 * _tail:
 *
 */
template<class T>
class FixQueue {
public:
    FixQueue(uint32_t size)
    {
        _max_size = size + 2;
        _queue = new T[_max_size];
        assert(_queue != nullptr);
        _head = 0;
        _tail = 1;
    }

    virtual ~FixQueue()
    {
        assert(_queue != nullptr);
        delete [] _queue;
    }

    bool push(T v)
    {
        if (!full()) {
            _queue[_tail] = v;
            _tail = (_tail + 1) % _max_size;
            return true;
        }
        return false;
    }

    void pop()
    {
        if (!empty()) {
            _head = (_head + 1) % _max_size;
        }
    }

    const T & front() const
    {
        return _queue[(_head + 1) % _max_size];
    }

    const T & back() const
    {
        return _queue[(_tail - 1 + _max_size) % _max_size];
    }

    inline bool size() const
    {
        return _max_size - 1;
    }

    inline bool empty() const
    {
        return (_head + 1) % _max_size == _tail;
    }

    inline bool full() const
    {
        return (_tail + 1) % _max_size == _head;
    }

private:
    T * _queue;
    uint32_t _max_size;
    uint32_t _head;
    uint32_t _tail;
};

} /* namespace ef */

#endif /* __COMMON_DATA_STRUCT_FIX_QUEUE_H__ */
