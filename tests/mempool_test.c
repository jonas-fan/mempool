#include "../mempool.h"

#include <stdio.h>
#include <assert.h>

#define TEST(str) printf("# Test %03d " str "\n", ++count);

static unsigned int count = 0;

int main()
{
    struct mempool pool;
    void *mem;
    int rc;

    TEST("mempool_init");
    rc = mempool_init(&pool, 4, 1);
    assert(rc == 1);

    TEST("mempool_alloc");
    mem = mempool_alloc(&pool);
    assert(mem);

    TEST("mempool_free");
    mempool_free(&pool, mem);

    TEST("mempool_alloc/free");
    for (unsigned int i = 0; i < 10000; ++i) {
        mem = mempool_alloc(&pool);
        assert(mem);
        mempool_free(&pool, mem);
    }

    TEST("mempool_alloc failed");
    mem = mempool_alloc(&pool);
    assert(mem);
    assert(mempool_alloc(&pool) == NULL);
    mempool_free(&pool, mem);

    TEST("mempool_term");
    mempool_term(&pool);

    printf("OK\n");

    return 0;
}
