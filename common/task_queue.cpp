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

bool TaskQueue::Initialize(uint32_t size, bool blocked, bool edge_trigger)
{
    _queue = new FixQueue<void*>(size);
    assert(_queue != nullptr);
    _notifier = new EventNotifier();
    assert(_notifier != nullptr);
    return _notifier->Initialize(blocked, edge_trigger);
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

int TaskQueue::Take(void** task)
{
    int ret = 0;
    _lock.lock();
    if (_queue->empty()) {
        *task = nullptr;
    } else {
        *task = _queue->front();
        _queue->pop();
    }
    _lock.unlock();

    if (*task) {
        ret = _notifier->GetEvent();
    }
    return ret - 1;
}

int TaskQueue::GetNotifier() const
{
    return _notifier->GetEventFd();
}

} /* namespace ef */
