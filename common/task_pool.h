/*
 * Author: number
 * Date  : Nov 22, 2015
 */

#ifndef __COMMON_TASK_POOL_H_H___
#define __COMMON_TASK_POOL_H_H___

#include <cstdint>

namespace ef {

class TaskQueue;

class TaskPool {
public:
    TaskPool();
    virtual ~TaskPool();

    bool Initialize(uint32_t pool_size, uint32_t queue_size);
    void * Get(uint32_t task_queue_idx);
    bool Put(uint32_t task_queue_idx, void * task);
    int GetFd(uint32_t task_queue_idx) const;

private:
    uint32_t _task_pool_size;
    TaskQueue * _task_pool;
};

} /* namespace ef */

#endif /* __COMMON_TASK_POOL_H__ */
