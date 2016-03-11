#ifndef MEMORY_POOL_H_
#define MEMORY_POOL_H_

/**
 *    Simple fixed-size memory pool
 *
 *    @date    2016/03/10
 *    @author  Jian <jianfan.tw@gmail.com>
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mempool_t mempool_t;

mempool_t * mempool_create(unsigned int block_size, unsigned int min_block_count);
void mempool_destroy(mempool_t *pool);

void * mempool_allocate(mempool_t *pool);
void mempool_free(mempool_t *pool, void *address);

unsigned int mempool_block_size(mempool_t *pool);

#ifdef __cplusplus
}
#endif

#endif  /* MEMORY_POOL_H_ */