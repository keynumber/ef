/*
 * Author: number
 * Date  : Nov 1, 2015
 */

#ifndef __COMMON_MEM_POOL_H_H___
#define __COMMON_MEM_POOL_H_H___

#include <stdint.h>
// #include <queue>

// TODO line 29, use a thread safe queue to replace std::stack, line 10 to be deleted

#define MEMPOOL_DEFAULT_GAP 128
#define MEMPOOL_DEFAULT_MEMBLOCK_MAX_SIZE 102400

namespace ef {

class MemPool {
public:
    MemPool();
    // MemPool(uint32_t min_buf_size, uint32_t max_buf_size = MEMPOOL_DEFAULT_GAP,
    //        uint32_t gap = MEMPOOL_DEFAULT_MEMBLOCK_MAX_SIZE);
    virtual ~MemPool();
    void * Malloc(uint32_t size);
    void Free(void * buf);
    // void Shrink();

private:
    // typedef std::queue<char*> queue;
    /* struct mem_chain_t {
        uint32_t mem_size;
        uint32_t min_queue_size;
        queue mem_queue;
    };*/

private:
    // mem_chain_t *_mem_pool;
    // const uint32_t _pool_size;
    // const uint32_t _min_buf_size;
    // const uint32_t _max_buf_size;
    // const uint32_t _gap;

};

} /* namespace ef */

#endif /* __COMMON_MEM_POOL_H__ */
