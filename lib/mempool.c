#include "mempool.h"

#include <stdlib.h>
#include <string.h>

struct mempool_block
{
    void *address;
    struct mempool_block *next;
};

struct mempool
{
    unsigned int block_size;
    unsigned int block_count;

    struct mempool_block *blocks;

    void **slots;
    unsigned int slots_length;
    unsigned int slots_index;
};

static inline struct mempool_block * mempool_block_allocate(unsigned int size, struct mempool_block *next)
{
    struct mempool_block *block = (struct mempool_block *)malloc(sizeof(struct mempool_block) + size);

    if (block) {
        memset(block, 0, sizeof(struct mempool_block));

        block->address = (unsigned char *)block + sizeof(struct mempool_block);
        block->next = next;
    }

    return block;
}

static inline void mempool_block_free(struct mempool_block *block)
{
    free(block);
}

struct mempool * mempool_create(unsigned int block_size, unsigned int min_block_count)
{
    if (!block_size) {
        return NULL;
    }

    struct mempool *pool = (struct mempool *)malloc(sizeof(struct mempool));

    if (!pool) {
        return NULL;
    }

    memset(pool, 0, sizeof(struct mempool));

    pool->block_size = block_size;

    if (!min_block_count) {
        return pool;
    }

    pool->block_count = min_block_count;

    pool->slots = (void **)malloc(sizeof(void *) * pool->block_count);

    if (!pool->slots) {
        free(pool);
        return NULL;
    }

    while (min_block_count--) {
        struct mempool_block *block = mempool_block_allocate(pool->block_size, pool->blocks);

        if (!block) {
            mempool_destroy(pool);
            return NULL;
        }

        pool->blocks = block;
        pool->slots[pool->slots_index++] = block->address;
    }

    return pool;
}

void mempool_destroy(struct mempool *pool)
{
    while (pool->blocks) {
        struct mempool_block *block = pool->blocks;

        pool->blocks = block->next;

        mempool_block_free(block);
    }

    free(pool->slots);
    free(pool);
}

void * mempool_allocate(struct mempool *pool)
{
    if (!pool->slots_index) {
        struct mempool_block *block = mempool_block_allocate(pool->block_size, pool->blocks);

        if (!block) {
            return NULL;
        }

        if (pool->block_count >= pool->slots_length) {
            const unsigned int new_slots_length = (pool->slots_length)?  (pool->slots_length << 1) : 1;
            const unsigned int total_size = sizeof(void *) * new_slots_length;

            if (pool->slots) {
                void **slots = (void **)realloc(pool->slots, total_size);

                if (!slots) {
                    mempool_block_free(block);
                    return NULL;
                }

                pool->slots = slots;
            }
            else {
                pool->slots = (void **)malloc(total_size);

                if (!pool->slots) {
                    mempool_block_free(block);
                    return NULL;
                }
            }

            pool->slots_length = new_slots_length;
        }

        ++pool->block_count;

        pool->blocks = block;
        pool->slots[pool->slots_index++] = block->address;
    }

    return pool->slots[--pool->slots_index];
}

void mempool_free(struct mempool *pool, void *address)
{
    if (address && (pool->slots_index < pool->slots_length)) {
        pool->slots[pool->slots_index++] = address;
    }
}

unsigned int mempool_block_size(struct mempool *pool)
{
    return pool->block_size;
}