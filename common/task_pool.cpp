/*
 * Author: number
 * Date  : Nov 8, 2015
 */

#include "task_pool.h"

#include <assert.h>

#include "event_notifier.h"

namespace ef {

TaskPool::TaskPool(uint32_t size)
    : _queue(size)
    , _notifier(nullptr)
{
    _notifier = new EventNotifier();
    assert(_notifier != nullptr);
}

bool TaskPool::Initialize(bool blocked, bool edge_trigger)
{
    return _notifier->Initialize(blocked, edge_trigger);
}

TaskPool::~TaskPool()
{
    delete _notifier;
}

bool TaskPool::Put(void* task)
{
    _lock.lock();
    bool ret = _queue.push(task);
    _lock.unlock();

    return ret;
}

bool TaskPool::Get(void** task)
{
    bool ret = true;
    _lock.lock();
    if (_queue.empty()) {
        ret = false;
    }
    *task = _queue.front();
    _queue.pop();
    _lock.unlock();
    return ret;
}

int TaskPool::GetNotifier() const
{
    return _notifier->GetEventFd();
}

} /* namespace ef */
