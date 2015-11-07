/*
 * Author: number
 * Date  : Nov 1, 2015
 */

#include "mem_pool.h"

#include <malloc.h>

namespace ef {

MemPool::MemPool()
{
}


MemPool::~MemPool()
{
}

void* MemPool::Malloc(uint32_t size)
{
    return malloc(size);
}

void MemPool::Free(void* buf)
{
    free(buf);
}

} /* namespace ef */
