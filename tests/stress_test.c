#include "../src/mempool.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define TEST_ROUND 500
#define TEST_COUNT (4 * 1000)
#define TEST_BLOCK_SIZE 256

#define CLOCK_TO_SEC(clock) ((double)(clock) / CLOCKS_PER_SEC)
#define CLOCK_TO_MSEC(clock) (CLOCK_TO_SEC(clock) * 1000)
#define TEST(str) printf("# Test %03d " str "\n", ++count)

static unsigned int count = 0;
static clock_t start_clock;

static void profiler_start()
{
    start_clock = clock();
}

static void profiler_stop()
{
    printf("elapsed %.2fms\n", CLOCK_TO_MSEC(clock() - start_clock));
}

#define test1(ALLOC_CODE, FREE_CODE) \
    do { \
        for (unsigned int round = 0; round < TEST_ROUND; ++round) { \
            for (unsigned int count = 0; count < TEST_COUNT; ++count) { \
                void *ptr = ALLOC_CODE; \
                assert(ptr); \
                FREE_CODE; \
            } \
        } \
    } while (0)

#define test2(ALLOC_CODE, FREE_CODE) \
    do { \
        void **address = (void **)malloc(sizeof(void *) * TEST_COUNT); \
        assert(address); \
        for (unsigned int round = 0; round < TEST_ROUND; ++round) { \
            for (unsigned int count = 0; count < TEST_COUNT; ++count) { \
                address[count] = ALLOC_CODE; \
                assert(address[count]); \
            } \
            for (unsigned int count = 0; count < TEST_COUNT; ++count) { \
                void *ptr = address[count]; \
                FREE_CODE; \
            } \
        } \
        free(address); \
    } while (0)

#define test3(ALLOC_CODE, FREE_CODE) \
    do { \
        void **address = (void **)malloc(sizeof(void *) * TEST_COUNT); \
        assert(address); \
        for (unsigned int round = 0; round < TEST_ROUND; ++round) { \
            for (unsigned int count = 0; count < TEST_COUNT; ++count) { \
                address[count] = ALLOC_CODE; \
                assert(address[count]); \
            } \
            for (unsigned int count = TEST_COUNT; count > 0; --count) { \
                void *ptr = address[count - 1]; \
                FREE_CODE; \
            } \
        } \
        free(address); \
    } while (0)

static void os_test1()
{
    test1(malloc(TEST_BLOCK_SIZE), free(ptr));
}

static void mempool_test1()
{
    struct mempool pool;

    mempool_init(&pool, TEST_BLOCK_SIZE, TEST_COUNT);
    test1(mempool_alloc(&pool), mempool_free(&pool, ptr));
    mempool_term(&pool);
}

static void os_test2()
{
    test2(malloc(TEST_BLOCK_SIZE), free(ptr));
}

static void mempool_test2()
{
    struct mempool pool;

    mempool_init(&pool, TEST_BLOCK_SIZE, TEST_COUNT);
    test2(mempool_alloc(&pool), mempool_free(&pool, ptr));
    mempool_term(&pool);
}

static void os_test3()
{
    test3(malloc(TEST_BLOCK_SIZE), free(ptr));
}

static void mempool_test3()
{
    struct mempool pool;

    mempool_init(&pool, TEST_BLOCK_SIZE, TEST_COUNT);
    test3(mempool_alloc(&pool), mempool_free(&pool, ptr));
    mempool_term(&pool);
}

int main()
{
    TEST("");
    printf("os malloc/free ... ");
    profiler_start();
    os_test1();
    profiler_stop();
    printf("mempool alloc/free ... ");
    profiler_start();
    mempool_test1();
    profiler_stop();
    printf("\n");

    TEST("");
    printf("os malloc/free ... ");
    profiler_start();
    os_test2();
    profiler_stop();
    printf("mempool alloc/free ... ");
    profiler_start();
    mempool_test2();
    profiler_stop();
    printf("\n");

    TEST("");
    printf("os malloc/free ... ");
    profiler_start();
    os_test3();
    profiler_stop();
    printf("mempool alloc/free ... ");
    profiler_start();
    mempool_test3();
    profiler_stop();
    printf("\n");

    return 0;
}