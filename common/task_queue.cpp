/*
 * Author: number
 * Date  : Nov 8, 2015
 */

#include "task_queue.h"

#include <assert.h>

#include "event_notifier.h"
#include "data_struct/fix_queue.h"


namespace ef { 
TaskQueue::TaskQueue()
{
}

bool TaskQueue::Initialize(uint32_t size, bool blocked)
{
    _queue = new FixQueue<void*>(size);
    assert(_queue != nullptr);
    _notifier = new EventNotifier();
    assert(_notifier != nullptr);
    return _notifier->Initialize(blocked, false);
}

TaskQueue::~TaskQueue()
{
    if (_notifier) {
        delete _notifier;
        _notifier = nullptr;
    }
    if (_queue) {
        delete _queue;
        _queue = nullptr;
    }
}

bool TaskQueue::Put(void* task)
{
    _lock.lock();
    bool ret = _queue->push(task);
    _lock.unlock();

    if (ret) {
        _notifier->Notify(1);
    }
    return ret;
}

void * TaskQueue::Take()
{
    void * ret = nullptr;
    _lock.lock();
    if (_queue->empty()) {
        return ret;
    } else {
        ret = _queue->front();
        _queue->pop();
    }
    _lock.unlock();

    _notifier->GetEvent();
    return ret;
}

int TaskQueue::GetNotifier() const
{
    return _notifier->GetEventFd();
}

} /* namespace ef */
