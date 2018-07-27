#ifndef __LIBMEMPOOL_H_
#define __LIBMEMPOOL_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mempool_t mempool_t;
typedef struct mempool_var_t mempool_var_t;

mempool_t * mempool_create(unsigned int block_size, unsigned int min_block_count);
void mempool_destroy(mempool_t *pool);
void * mempool_allocate(mempool_t *pool);
void mempool_free(mempool_t *pool, void *address);
unsigned int mempool_block_size(mempool_t *pool);

mempool_var_t * mempool_var_create(unsigned int size);
void mempool_var_destroy(mempool_var_t *pool);
void * mempool_var_allocate(mempool_var_t *pool, unsigned int size);
void mempool_var_free(mempool_var_t *pool, void *address);

#ifdef __cplusplus
}
#endif

#endif /* __LIBMEMPOOL_H_ */