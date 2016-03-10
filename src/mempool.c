#include <mempool.h>

#include <stdlib.h>
#include <string.h>

typedef struct mempool_block_t mempool_block_t;

struct mempool_block_t
{
    void *address;
    mempool_block_t *next;
};

struct mempool_t
{
    unsigned int block_size;
    unsigned int block_count;

    mempool_block_t *blocks;

    void **slots;
    unsigned int slots_length;
    unsigned int slots_index;
};

static inline mempool_block_t * mempool_block_allocate(unsigned int size, mempool_block_t *next)
{
    mempool_block_t *block = (mempool_block_t *)malloc(sizeof(mempool_block_t) + size);

    if (block) {
        memset(block, 0, sizeof(mempool_block_t));

        block->address = (unsigned char *)block + sizeof(mempool_block_t);
        block->next = next;
    }

    return block;
}

static inline void mempool_block_free(mempool_block_t *block)
{
    free(block);
}

mempool_t * mempool_create(unsigned int block_size, unsigned int min_block_count)
{
    if (!block_size) {
        return NULL;
    }

    mempool_t *pool = (mempool_t *)malloc(sizeof(mempool_t));

    if (!pool) {
        return NULL;
    }

    memset(pool, 0, sizeof(mempool_t));

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
        mempool_block_t *block = mempool_block_allocate(pool->block_size, pool->blocks);

        if (!block) {
            mempool_destroy(pool);
            return NULL;
        }

        pool->blocks = block;
        pool->slots[pool->slots_index++] = block->address;
    }

    return pool;
}

void mempool_destroy(mempool_t *pool)
{
    while (pool->blocks) {
        mempool_block_t *block = pool->blocks;

        pool->blocks = block->next;

        mempool_block_free(block);
    }

    free(pool->slots);
    free(pool);
}

void * mempool_allocate(mempool_t *pool)
{
    if (!pool->slots_index) {
        mempool_block_t *block = mempool_block_allocate(pool->block_size, pool->blocks);

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

void mempool_free(mempool_t *pool, void *address)
{
    if (address && (pool->slots_index < pool->slots_length)) {
        pool->slots[pool->slots_index++] = address;
    }
}

unsigned int mempool_block_size(mempool_t *pool)
{
    return pool->block_size;
}