/*
 * Author: number
 * Date  : Nov 1, 2015
 */

#include "mem_pool.h"

#include <cassert>

namespace ef { 
MemPool::MemPool(uint32_t min_buf_size, uint32_t max_buf_size, uint32_t gap)
    : _pool_size((max_buf_size - min_buf_size + gap - 1) / gap + 1)
    , _min_buf_size(min_buf_size)
    , _max_buf_size(max_buf_size)
    , _gap(gap)
{
    _mem_pool = new mem_chain_t[_pool_size];
    assert(_mem_pool != nullptr);
    int mem_size = min_buf_size;
    for (uint32_t i = 0; i < _pool_size; ++i) {
        _mem_pool[i].mem_size = mem_size;
        _mem_pool[i].min_queue_size = 0;
        mem_size += gap;
    }
}


MemPool::~MemPool()
{
    // TODO
    for (uint32_t i = 0; i < _pool_size; ++i) {     // lock free queue's size is fixed .....
    }
    delete []_mem_pool;
}

void* MemPool::Malloc(int size)
{
    return nullptr;
}

void MemPool::Free(void* buf)
{
}

void MemPool::Shrink()
{
}

} /* namespace ef */
