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

/**
 * @desc 线程安全,固定长度的任务池,采用eventfd进行通知
 */
class TaskPool {
public:
    TaskPool(uint32_t size);
    virtual ~TaskPool();

    TaskPool(const TaskPool &) = delete;
    TaskPool(const TaskPool &&) = delete;
    TaskPool & operator = (const TaskPool &) = delete;
    TaskPool & operator = (const TaskPool &&) = delete;

    /**
     * @desc 设置task到来后的通知方式
     * @param blocked 是否已阻塞方式通知
     * @param edge_trigger 是否边沿触发方式通知
     */
    bool Initialize(bool blocked, bool edge_trigger);

    /**
     * @desc 将任务放入队列,并通知
     */
    bool Put(void *task);

    /**
     * @desc 水平触发,讲获得的任务放入*task
     *       边沿触发,放入一个任务到*task,返回队列中待处理task的数量(不包括已经放入*task的)
     * @param [out] 获得任务放入Task
     * @return 成功,返回剩余task的数量,水平触发总返回0(空队列返回0,*task置为nullptr))
     *         失败,返回 < 0
     */
    int Take(void **task);

    int GetNotifier() const;

private:
    FixQueue<void*> _queue;
    std::mutex _lock;
    EventNotifier * _notifier;
};

} /* namespace ef */

#endif /* __COMMON_TASK_POOL_H__ */
