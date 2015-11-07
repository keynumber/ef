/*
 * Author: number
 * Date  : Nov 8, 2015
 */

#ifndef __COMMON_TASK_POOL_H_H___
#define __COMMON_TASK_POOL_H_H___

#include <stdint.h>

#include <mutex>

#include "data_struct/fix_queue.h"

namespace ef {

class EventNotifier;

class TaskPool {
public:
    TaskPool(uint32_t size);
    virtual ~TaskPool();

    /**
     * @desc 设置task到来后的通知方式
     * @param blocked 是否已阻塞方式通知
     * @param edge_trigger 是否边沿触发方式通知
     */
    bool Initialize(bool blocked, bool edge_trigger);

    bool Put(void *task);
    bool Get(void **task);

    int GetNotifier() const;

private:
    FixQueue<void*> _queue;
    std::mutex _lock;
    EventNotifier * _notifier;
};

} /* namespace ef */

#endif /* __COMMON_TASK_POOL_H__ */
