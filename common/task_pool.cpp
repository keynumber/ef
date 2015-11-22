/*
 * Author: number
 * Date  : Nov 22, 2015
 */

#include "task_pool.h"

#include <cassert>

#include "task_queue.h"

namespace ef {

TaskPool::TaskPool()
    : _task_pool_size(0)
    , _task_pool(nullptr)
{
}

TaskPool::~TaskPool()
{
    if (_task_pool) {
        delete [] _task_pool;
        _task_pool = 0;
    }
}

bool TaskPool::Initialize(uint32_t pool_size, uint32_t queue_size)
{
    _task_pool_size = pool_size;
    _task_pool = new TaskQueue[_task_pool_size];
    assert(_task_pool);

    for (uint32_t i = 0; i < _task_pool_size; ++i) {
        assert(_task_pool[i].Initialize(queue_size, false));
    }
    return true;
}

void* TaskPool::Get(uint32_t task_queue_idx)
{
    return _task_pool[task_queue_idx].Take();
}

bool TaskPool::Put(uint32_t task_queue_idx, void * task)
{
    return _task_pool[task_queue_idx].Put(task);
}

int TaskPool::GetFd(uint32_t task_queue_idx) const
{
    return _task_pool[task_queue_idx].GetNotifier();
}

} /* namespace ef */
