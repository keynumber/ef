/*
 * Author: number
 * Date  : Nov 1, 2015
 */

#include "mem_pool.h"

namespace ef {

MemPool::MemPool(uint32_t min_size, uint32_t max_size, uint32_t gap)
{
}


MemPool::~MemPool()
{
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
