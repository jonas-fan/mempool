#ifndef MEMORY_POOL_VAR_H_
#define MEMORY_POOL_VAR_H_

/**
 *    Simple variable-size memory pool
 *
 *    @date    2016/03/10
 *    @author  Jian <jianfan.tw@gmail.com>
 */

typedef struct mempool_var_t mempool_var_t;

mempool_var_t * mempool_var_create(unsigned int size);
void mempool_var_destroy(mempool_var_t *pool);

void * mempool_var_allocate(mempool_var_t *pool, unsigned int size);
void mempool_var_free(mempool_var_t *pool, void *address);

#endif  /* MEMORY_POOL_VAR_H_ */