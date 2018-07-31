#ifndef __LIBMEMPOOL_H_
#define __LIBMEMPOOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>

struct mempool
{
    unsigned int count;
    void **blocks;
    char *memory;
};


/**
 * Initialize a memory pool
 *
 * @param pool the pool
 * @param size the size of a memory block
 * @param count the count of memory blocks
 * @return 1 on success, 0 otherwise
 */
static inline int mempool_init(struct mempool *pool, unsigned int size,
    unsigned int count)
{
    memset(pool, 0, sizeof(struct mempool));

    if (!size || !count) {
        return 0;
    }

    char *memory = (char *)malloc(size * count);
    void **blocks = (void **)malloc(sizeof(void *) * count);

    if (!memory || !blocks) {
        free(memory);
        free(blocks);
        return 0;
    }

    pool->memory = memory;
    pool->blocks = blocks;

    while (count--) {
        pool->blocks[pool->count] = pool->memory + pool->count * size;
        pool->count++;
    }

    return 1;
}

/**
 * Finalize a memory pool
 *
 * @param pool the pool
 */
static inline void mempool_term(struct mempool *pool)
{
    free(pool->blocks);
    free(pool->memory);

    memset(pool, 0, sizeof(struct mempool));
}

/**
 * Get a memory block from the pool
 *
 * @param pool the pool
 * @return a pointer to memory block on success, NULL on failure
 */
static inline void * mempool_alloc(struct mempool *pool)
{
    return pool->count ? pool->blocks[--pool->count] : NULL;
}

/**
 * Free a memory block
 *
 * @param pool the pool
 * @param block the block
 */
static inline void mempool_free(struct mempool *pool, void *block)
{
    if (block) {
        pool->blocks[pool->count++] = block;
    }
}

#ifdef __cplusplus
}
#endif

#endif /* __LIBMEMPOOL_H_ */