#ifndef __LIBMEMPOOL_H_
#define __LIBMEMPOOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>

struct mempool
{
    char *memory;
    void **blocks;
    unsigned int count;
};

/* initialize the memory pool */
static inline int mempool_init(struct mempool *pool, unsigned int size,
    unsigned int count)
{
    if (!size || !count) {
        return 0;
    }

    memset(pool, 0, sizeof(struct mempool));
    pool->memory = (char *)malloc(size * count);

    if (!pool->memory) {
        return 0;
    }

    pool->blocks = (void **)malloc(sizeof(void *) * count);

    if (!pool->blocks) {
        free(pool->memory);
        return 0;
    }

    while (count--) {
        pool->blocks[pool->count] = pool->memory + pool->count * size;
        pool->count++;
    }

    return 1;
}

/* finalize the memory pool */
static inline void mempool_term(struct mempool *pool)
{
    free(pool->blocks);
    free(pool->memory);

    memset(pool, 0, sizeof(struct mempool));
}

/* return a memory block from the pool */
static inline void * mempool_alloc(struct mempool *pool)
{
    return pool->count ? pool->blocks[--pool->count] : NULL;
}

/* release a memory block */
static inline void mempool_free(struct mempool *pool, void *address)
{
    if (address) {
        pool->blocks[pool->count++] = address;
    }
}

#ifdef __cplusplus
}
#endif

#endif /* __LIBMEMPOOL_H_ */