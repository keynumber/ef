/*
 * Author: number
 * Date  : Nov 8, 2015
 */

#include "task_queue.h"

#include <assert.h>

#include "event_notifier.h"

namespace ef { 
TaskQueue::TaskQueue(uint32_t size)
    : _queue(size)
{
    _notifier = new EventNotifier();
    assert(_notifier != nullptr);
}

bool TaskQueue::Initialize(bool blocked, bool edge_trigger)
{
    return _notifier->Initialize(blocked, edge_trigger);
}

TaskQueue::~TaskQueue()
{
    delete _notifier;
}

bool TaskQueue::Put(void* task)
{
    _lock.lock();
    bool ret = _queue.push(task);
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
    if (_queue.empty()) {
        *task = nullptr;
    } else {
        *task = _queue.front();
        _queue.pop();
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
