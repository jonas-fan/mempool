#include "mempool.h"

#include <stdlib.h>
#include <string.h>

#define MEMORY_BLOCK_SIZE (1 << 3)

#define ALIGN(x, size) (((x) + (size) - 1) & ~((size) - 1))

struct mempool_block
{
    unsigned char *address;
    unsigned int capacity;
    struct mempool_block *next;
};

struct mempool_var
{
    unsigned char *chunk;
    unsigned int chunk_size;
    unsigned int chunk_aligned_size;
    unsigned int chunk_free_size;

    struct mempool_block *ready_blocks;
    struct mempool_block *using_blocks;
    struct mempool_block *blocks;
};

static inline struct mempool_block * mempool_block_allocate(struct mempool_var *pool,
                                                       unsigned char *address,
                                                       unsigned int capacity,
                                                       struct mempool_block *next)
{
    struct mempool_block *block = pool->blocks;

    if (block) {
        pool->blocks = block->next;
    }
    else {
        block = (struct mempool_block *)malloc(sizeof(struct mempool_block));

        if (!block) {
            return NULL;
        }
    }

    memset(block, 0, sizeof(struct mempool_block));

    block->address = address;
    block->capacity = capacity;
    block->next = next;

    return block;
}

static inline void mempool_block_free(struct mempool_var *pool, struct mempool_block *block)
{
    block->next = pool->blocks;

    pool->blocks = block;
}

static inline void insert_sort(struct mempool_block **container, struct mempool_block *element)
{
    struct mempool_block *previous = NULL;
    struct mempool_block *block = *container;

    while (block && (block->address < element->address)) {
        previous = block;
        block = block->next;
    }

    if (previous) {
        previous->next = element;
        element->next = block;
    }
    else {
        element->next = *container;
        *container = element;
    }
}

struct mempool_var * mempool_var_create(unsigned int size)
{
    struct mempool_var *pool = (struct mempool_var *)malloc(sizeof(struct mempool_var));

    if (!pool) {
        return NULL;
    }

    memset(pool, 0, sizeof(struct mempool_var));

    const unsigned int aligned_size = ALIGN(size, MEMORY_BLOCK_SIZE);

    pool->chunk = (unsigned char *)malloc(sizeof(unsigned char) * aligned_size);

    if (!pool->chunk) {
        free(pool);
        return NULL;
    }

    pool->chunk_size = size;
    pool->chunk_aligned_size = aligned_size;
    pool->chunk_free_size = pool->chunk_aligned_size;

    struct mempool_block *block = mempool_block_allocate(pool,
                                                    pool->chunk,
                                                    pool->chunk_free_size,
                                                    NULL);

    if (!block) {
        free(pool->chunk);
        free(pool);
        return NULL;
    }

    pool->ready_blocks = block;

    return pool;
}

void mempool_var_destroy(struct mempool_var *pool)
{
    // TODO -- optimize there ...
    struct mempool_block *blocks[] = {pool->blocks, pool->ready_blocks, pool->using_blocks};
    const unsigned int blocks_size = sizeof(blocks) / sizeof(*blocks);

    for (unsigned int index = 0; index < blocks_size; ++index) {
        struct mempool_block *block = blocks[index];

        while (block) {
            struct mempool_block *next = block->next;

            free(block);

            block = next;
        }
    }

    free(pool->chunk);
    free(pool);
}

void * mempool_var_allocate(struct mempool_var *pool, unsigned int size)
{
    if (!size) {
        return NULL;
    }

    const unsigned int aligned_size = ALIGN(size, MEMORY_BLOCK_SIZE);

    if (pool->chunk_free_size < aligned_size) {
        // The size of pool is too small
        return NULL;
    }

    struct mempool_block *previous = NULL;
    struct mempool_block *block = pool->ready_blocks;

    while (block && (block->capacity < aligned_size)) {
        previous = block;
        block = block->next;
    }

    if (!block) {
        // Maybe the memory fragmentation occured
        return NULL;
    }

    struct mempool_block *new_block = NULL;

    if (block->capacity > aligned_size) {
        new_block = mempool_block_allocate(pool,
                                           block->address + aligned_size,
                                           block->capacity - aligned_size,
                                           block->next);

        if (!new_block) {
            return NULL;
        }
    }

    if (previous) {
        previous->next = new_block;
    }
    else {
        pool->ready_blocks = new_block;
    }

    block->capacity = aligned_size;
    block->next = pool->using_blocks;

    pool->using_blocks = block;
    pool->chunk_free_size -= block->capacity;

    return block->address;
}

void mempool_var_free(struct mempool_var *pool, void *address)
{
    if (!address) {
        return;
    }

    struct mempool_block *previous = NULL;
    struct mempool_block *block = pool->using_blocks;

    while (block && (block->address != address)) {
        previous = block;
        block = block->next;
    }

    if (!block) {
        // Invalid address
        return;
    }

    if (previous) {
        previous->next = block->next;
    }
    else {
        pool->using_blocks = block->next;
    }

    pool->chunk_free_size += block->capacity;

    insert_sort(&(pool->ready_blocks), block);

    block = pool->ready_blocks;

    while (block && block->next) {
        struct mempool_block *next = block->next;

        if ((block->address + block->capacity) == next->address) {
            block->capacity += next->capacity;
            block->next = next->next;

            mempool_block_free(pool, next);

            next = block->next;
        }

        block = next;
    }
}