#include "../mempool.h"

#include <stdio.h>
#include <assert.h>

#define TEST(str) printf("# Test %03d " str "\n", ++count);

static unsigned int count = 0;

int main()
{
    struct mempool pool;
    void *mem1;
    void *mem2;
    void *mem3;
    int rc;

    TEST("mempool_init");
    rc = mempool_init(&pool, 4, 2);
    assert(rc == 1);

    TEST("mempool_alloc");
    mem1 = mempool_alloc(&pool);
    assert(mem1);
    mem2 = mempool_alloc(&pool);
    assert(mem2);
    mem3 = mempool_alloc(&pool);
    assert(mem3 == NULL);

    TEST("mempool_free");
    mempool_free(&pool, NULL);
    mem3 = mempool_alloc(&pool);
    assert(mem3 == NULL);
    mempool_free(&pool, mem1);
    mem1 = mempool_alloc(&pool);
    assert(mem1);
    mempool_free(&pool, mem1);
    mempool_free(&pool, mem2);

    TEST("mempool_term");
    mempool_term(&pool);

    printf("OK\n");

    return 0;
}