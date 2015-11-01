/*
 * Author: number
 * Date  : Nov 1, 2015
 */

#ifndef __COMMON_MEM_POOL_H_H___
#define __COMMON_MEM_POOL_H_H___

#include <cstdint>
#include <vector>
#include <stack>

// TODO line 30, use a thread safe queue to replace std::stack, line 12 to be deleted

#define MEMPOOL_DEFAULT_GAP 128
#define MEMPOOL_DEFAULT_MEMBLOCK_MAX_SIZE 102400

namespace ef {

class MemPool {
public:
    MemPool(uint32_t min_size, uint32_t max_size = MEMPOOL_DEFAULT_GAP,
            uint32_t gap = MEMPOOL_DEFAULT_MEMBLOCK_MAX_SIZE);
    virtual ~MemPool();
    void * Malloc(int size);
    void Free(void * buf);
    void Shrink();

private:
    typedef std::stack<char*> queue;
    struct mem_chain_t {
        uint32_t mem_size;
        uint32_t min_queue_size;
        queue mem_queue;
    };

private:
    std::vector<mem_chain_t> _mem_pool;
    uint32_t _min_size;
    uint32_t _max_size;
    uint32_t _gap;

};

} /* namespace ef */

#endif /* __COMMON_MEM_POOL_H__ */
