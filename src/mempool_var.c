#include <mempool_var.h>

#include <stdlib.h>
#include <string.h>

#define MEMORY_BLOCK_SIZE (1 << 3)

#define ALIGN(x, size) (((x) + (size) - 1) & ~((size) - 1))

typedef struct mempool_block_t mempool_block_t;

struct mempool_block_t
{
    unsigned char *address;
    unsigned int capacity;
    mempool_block_t *next;
};

struct mempool_var_t
{
    unsigned char *chunk;
    unsigned int chunk_size;
    unsigned int chunk_aligned_size;
    unsigned int chunk_free_size;

    mempool_block_t *ready_blocks;
    mempool_block_t *using_blocks;
    mempool_block_t *blocks;
};

static inline mempool_block_t * mempool_block_allocate(mempool_var_t *pool,
                                                       unsigned char *address,
                                                       unsigned int capacity,
                                                       mempool_block_t *next)
{
    mempool_block_t *block = pool->blocks;

    if (block) {
        pool->blocks = block->next;
    }
    else {
        block = (mempool_block_t *)malloc(sizeof(mempool_block_t));

        if (!block) {
            return NULL;
        }
    }

    memset(block, 0, sizeof(mempool_block_t));

    block->address = address;
    block->capacity = capacity;
    block->next = next;

    return block;
}

static inline void mempool_block_free(mempool_var_t *pool, mempool_block_t *block)
{
    block->next = pool->blocks;

    pool->blocks = block;
}

static inline void insert_sort(mempool_block_t **container, mempool_block_t *element)
{
    mempool_block_t *previous = NULL;
    mempool_block_t *block = *container;

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

mempool_var_t * mempool_var_create(unsigned int size)
{
    mempool_var_t *pool = (mempool_var_t *)malloc(sizeof(mempool_var_t));

    if (!pool) {
        return NULL;
    }

    memset(pool, 0, sizeof(mempool_var_t));

    const unsigned int aligned_size = ALIGN(size, MEMORY_BLOCK_SIZE);

    pool->chunk = (unsigned char *)malloc(sizeof(unsigned char) * aligned_size);

    if (!pool->chunk) {
        free(pool);
        return NULL;
    }

    pool->chunk_size = size;
    pool->chunk_aligned_size = aligned_size;
    pool->chunk_free_size = pool->chunk_aligned_size;

    mempool_block_t *block = mempool_block_allocate(pool,
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

void mempool_var_destroy(mempool_var_t *pool)
{
    // TODO -- optimize there ...
    mempool_block_t *blocks[] = {pool->blocks, pool->ready_blocks, pool->using_blocks};
    const unsigned int blocks_size = sizeof(blocks) / sizeof(*blocks);

    for (unsigned int index = 0; index < blocks_size; ++index) {
        mempool_block_t *block = blocks[index];

        while (block) {
            mempool_block_t *next = block->next;

            free(block);

            block = next;
        }
    }

    free(pool->chunk);
    free(pool);
}

void * mempool_var_allocate(mempool_var_t *pool, unsigned int size)
{
    if (!size) {
        return NULL;
    }

    const unsigned int aligned_size = ALIGN(size, MEMORY_BLOCK_SIZE);

    if (pool->chunk_free_size < aligned_size) {
        // The size of pool is too small
        return NULL;
    }

    mempool_block_t *previous = NULL;
    mempool_block_t *block = pool->ready_blocks;

    while (block && (block->capacity < aligned_size)) {
        previous = block;
        block = block->next;
    }

    if (!block) {
        // Maybe the memory fragmentation occured
        return NULL;
    }

    mempool_block_t *new_block = NULL;

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

void mempool_var_free(mempool_var_t *pool, void *address)
{
    if (!address) {
        return;
    }

    mempool_block_t *previous = NULL;
    mempool_block_t *block = pool->using_blocks;

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
        mempool_block_t *next = block->next;

        if ((block->address + block->capacity) == next->address) {
            block->capacity += next->capacity;
            block->next = next->next;

            mempool_block_free(pool, next);

            next = block->next;
        }

        block = next;
    }
}