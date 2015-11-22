/*
 * Author: number
 * Date  : Nov 8, 2015
 */

#ifndef __COMMON_TASK_QUEUE_H_H___
#define __COMMON_TASK_QUEUE_H_H___

#include <stdint.h>

#include <mutex>

namespace ef {

class EventNotifier;
template<class T> class FixQueue;

/**
 * @desc 线程安全,固定长度的任务池,采用eventfd进行通知
 */
class TaskQueue {
public:
    TaskQueue();
    virtual ~TaskQueue();

    TaskQueue(const TaskQueue &) = delete;
    TaskQueue(const TaskQueue &&) = delete;
    TaskQueue & operator = (const TaskQueue &) = delete;
    TaskQueue & operator = (const TaskQueue &&) = delete;

    /**
     * @desc 设置task到来后的通知方式
     * @param blocked 是否已阻塞方式通知
     */
    bool Initialize(uint32_t size, bool blocked);

    /**
     * @desc 将任务放入队列,并通知
     */
    bool Put(void *task);

    /**
     * @desc 将获得的任务放入*task
     * @return 成功,返回task指针
     *         没有数据,返回nullptr
     */
    void * Take();

    int GetNotifier() const;

private:
    FixQueue<void*> *_queue;
    std::mutex _lock;
    EventNotifier * _notifier;
};

} /* namespace ef */

#endif /* __COMMON_TASK_QUEUE_H__ */
