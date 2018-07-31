#include "../src/mempool.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define TEST_ROUND 500
#define TEST_COUNT (4 * 1000)
#define TEST_BLOCK_SIZE 256

static clock_t malloc_test_case1()
{
    const clock_t start = clock();

    for (unsigned int round = 0; round < TEST_ROUND; ++round) {
        for (unsigned int count = 0; count < TEST_COUNT; ++count) {
            void *address = malloc(TEST_BLOCK_SIZE);
            assert(address);
            free(address);
        }
    }

    const clock_t diff = clock() - start;

    return diff;
}

static clock_t malloc_test_case2()
{
    void **address = (void **)malloc(sizeof(void *) * TEST_COUNT);

    assert(address);

    const clock_t start = clock();

    for (unsigned int round = 0; round < TEST_ROUND; ++round) {
        for (unsigned int count = 0; count < TEST_COUNT; ++count) {
            address[count] = malloc(TEST_BLOCK_SIZE);
            assert(address[count]);
        }

        for (unsigned int count = 0; count < TEST_COUNT; ++count) {
            free(address[count]);
        }
    }

    const clock_t diff = clock() - start;

    free(address);

    return diff;
}

static clock_t malloc_test_case3()
{
    void **address = (void **)malloc(sizeof(void *) * TEST_COUNT);

    assert(address);

    const clock_t start = clock();

    for (unsigned int round = 0; round < TEST_ROUND; ++round) {
        for (unsigned int count = 0; count < TEST_COUNT; ++count) {
            address[count] = malloc(TEST_BLOCK_SIZE);
            assert(address[count]);
        }

        for (unsigned int count = TEST_COUNT; count > 0; --count) {
            free(address[count - 1]);
        }
    }

    const clock_t diff = clock() - start;

    free(address);

    return diff;
}

static clock_t mempool_test_case1()
{
    struct mempool pool;
    const clock_t start = clock();

    mempool_init(&pool, TEST_BLOCK_SIZE, 8);

    for (unsigned int round = 0; round < TEST_ROUND; ++round) {
        for (unsigned int count = 0; count < TEST_COUNT; ++count) {
            void *address = mempool_alloc(&pool);
            assert(address);
            mempool_free(&pool, address);
        }
    }

    mempool_term(&pool);

    const clock_t diff = clock() - start;

    return diff;
}

static clock_t mempool_test_case2()
{
    struct mempool pool;
    void **address = (void **)malloc(sizeof(void *) * TEST_COUNT);

    assert(address);

    const clock_t start = clock();

    mempool_init(&pool, TEST_BLOCK_SIZE, TEST_COUNT);

    for (unsigned int round = 0; round < TEST_ROUND; ++round) {
        for (unsigned int count = 0; count < TEST_COUNT; ++count) {
            address[count] = mempool_alloc(&pool);
            assert(address[count]);
        }

        for (unsigned int count = 0; count < TEST_COUNT; ++count) {
            mempool_free(&pool, address[count]);
        }
    }

    mempool_term(&pool);

    const clock_t diff = clock() - start;

    free(address);

    return diff;
}

static clock_t mempool_test_case3()
{
    struct mempool pool;
    void **address = (void **)malloc(sizeof(void *) * TEST_COUNT);

    assert(address);

    const clock_t start = clock();

    mempool_init(&pool, TEST_BLOCK_SIZE, TEST_COUNT);

    for (unsigned int round = 0; round < TEST_ROUND; ++round) {
        for (unsigned int count = 0; count < TEST_COUNT; ++count) {
            address[count] = mempool_alloc(&pool);
            assert(address[count]);
        }

        for (unsigned int count = TEST_COUNT; count > 0; --count) {
            mempool_free(&pool, address[count - 1]);
        }
    }

    mempool_term(&pool);

    const clock_t diff = clock() - start;

    free(address);

    return diff;
}

int main()
{
    typedef clock_t (*case_func_t)(void);

    printf("===== malloc/free =====\n");

    case_func_t malloc_test_cases[] = {malloc_test_case1,
                                       malloc_test_case2,
                                       malloc_test_case3,
                                       NULL};

    for (unsigned int index = 0; malloc_test_cases[index]; ++index) {
        const clock_t total_clocks = malloc_test_cases[index]();

        printf("[Case %u]: %.2fms\n", index + 1, (double)total_clocks / CLOCKS_PER_SEC * 1000);
    }

    printf("\n===== Fixed-size memroy pool =====\n");

    case_func_t mempool_test_cases[] = {mempool_test_case1,
                                        mempool_test_case2,
                                        mempool_test_case3,
                                        NULL};

    for (unsigned int index = 0; mempool_test_cases[index]; ++index) {
        const clock_t total_clocks = mempool_test_cases[index]();

        printf("[Case %u]: %.2fms\n", index + 1, (double)total_clocks / CLOCKS_PER_SEC * 1000);
    }

    return 0;
}