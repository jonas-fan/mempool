#ifndef __LIBMEMPOOL_H_
#define __LIBMEMPOOL_H_

#ifdef __cplusplus
extern "C" {
#endif

struct mempool * mempool_create(unsigned int block_size, unsigned int min_block_count);
void mempool_destroy(struct mempool *pool);
void * mempool_allocate(struct mempool *pool);
void mempool_free(struct mempool *pool, void *address);
unsigned int mempool_block_size(struct mempool *pool);

struct mempool_var * mempool_var_create(unsigned int size);
void mempool_var_destroy(struct mempool_var *pool);
void * mempool_var_allocate(struct mempool_var *pool, unsigned int size);
void mempool_var_free(struct mempool_var *pool, void *address);

#ifdef __cplusplus
}
#endif

#endif /* __LIBMEMPOOL_H_ */