#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include <mempool.h>
#include <mempool_var.h>

#define TEST_COUNT (4 * 1000)
#define TEST_BLOCK_SIZE 256
#define REPEAT_TIMES 1000

static clock_t malloc_test_case1()
{
    clock_t start = clock();

    for (unsigned int times = 0; times < REPEAT_TIMES; ++times) {
        for (unsigned int count = 0; count < TEST_COUNT; ++count) {
            void *address = malloc(TEST_BLOCK_SIZE);

            assert(address);

            free(address);
        }
    }

    clock_t diff = clock() - start;

    return diff;
}

static clock_t malloc_test_case2()
{
    void **address = (void **)malloc(sizeof(void *) * TEST_COUNT);

    assert(address);

    clock_t start = clock();

    for (unsigned int times = 0; times < REPEAT_TIMES; ++times) {
        for (unsigned int count = 0; count < TEST_COUNT; ++count) {
            address[count] = malloc(TEST_BLOCK_SIZE);

            assert(address[count]);
        }

        for (unsigned int count = 0; count < TEST_COUNT; ++count) {
            free(address[count]);
        }
    }

    clock_t diff = clock() - start;

    free(address);

    return diff;
}

static clock_t malloc_test_case3()
{
    void **address = (void **)malloc(sizeof(void *) * TEST_COUNT);

    assert(address);

    clock_t start = clock();

    for (unsigned int times = 0; times < REPEAT_TIMES; ++times) {
        for (unsigned int count = 0; count < TEST_COUNT; ++count) {
            address[count] = malloc(TEST_BLOCK_SIZE);

            assert(address[count]);
        }

        for (unsigned int count = TEST_COUNT; count > 0; --count) {
            free(address[count - 1]);
        }
    }

    clock_t diff = clock() - start;

    free(address);

    return diff;
}

static clock_t mempool_test_case1()
{
    clock_t start = clock();

    mempool_t *pool = mempool_create(TEST_BLOCK_SIZE, 8);

    assert(pool);
    assert(mempool_block_size(pool) == TEST_BLOCK_SIZE);

    for (unsigned int times = 0; times < REPEAT_TIMES; ++times) {
        for (unsigned int count = 0; count < TEST_COUNT; ++count) {
            void *address = mempool_allocate(pool);

            assert(address);

            mempool_free(pool, address);
        }
    }

    mempool_destroy(pool);

    clock_t diff = clock() - start;

    return diff;
}

static clock_t mempool_test_case2()
{
    void **address = (void **)malloc(sizeof(void *) * TEST_COUNT);

    assert(address);

    clock_t start = clock();

    mempool_t *pool = mempool_create(TEST_BLOCK_SIZE, TEST_COUNT);

    assert(pool);
    assert(mempool_block_size(pool) == TEST_BLOCK_SIZE);

    for (unsigned int times = 0; times < REPEAT_TIMES; ++times) {
        for (unsigned int count = 0; count < TEST_COUNT; ++count) {
            address[count] = mempool_allocate(pool);

            assert(address[count]);
        }

        for (unsigned int count = 0; count < TEST_COUNT; ++count) {
            mempool_free(pool, address[count]);
        }
    }

    mempool_destroy(pool);

    clock_t diff = clock() - start;

    free(address);

    return diff;
}

static clock_t mempool_test_case3()
{
    void **address = (void **)malloc(sizeof(void *) * TEST_COUNT);

    assert(address);

    clock_t start = clock();

    mempool_t *pool = mempool_create(TEST_BLOCK_SIZE, TEST_COUNT);

    assert(pool);
    assert(mempool_block_size(pool) == TEST_BLOCK_SIZE);

    for (unsigned int times = 0; times < REPEAT_TIMES; ++times) {
        for (unsigned int count = 0; count < TEST_COUNT; ++count) {
            address[count] = mempool_allocate(pool);

            assert(address[count]);
        }

        for (unsigned int count = TEST_COUNT; count > 0; --count) {
            mempool_free(pool, address[count - 1]);
        }
    }

    mempool_destroy(pool);

    clock_t diff = clock() - start;

    free(address);

    return diff;
}


static clock_t mempool_var_test_case1()
{
    clock_t start = clock();

    mempool_var_t *pool = mempool_var_create(TEST_BLOCK_SIZE * TEST_COUNT);

    assert(pool);

    for (unsigned int times = 0; times < REPEAT_TIMES; ++times) {
        for (unsigned int count = 0; count < TEST_COUNT; ++count) {
            void *address = mempool_var_allocate(pool, TEST_BLOCK_SIZE);

            assert(address);

            mempool_var_free(pool, address);
        }
    }

    mempool_var_destroy(pool);

    clock_t diff = clock() - start;

    return diff;
}

static clock_t mempool_var_test_case2()
{
    void **address = (void **)malloc(sizeof(void *) * TEST_COUNT);

    assert(address);

    clock_t start = clock();

    mempool_var_t *pool = mempool_var_create(TEST_BLOCK_SIZE * TEST_COUNT);

    assert(pool);

    for (unsigned int times = 0; times < REPEAT_TIMES; ++times) {
        for (unsigned int count = 0; count < TEST_COUNT; ++count) {
            address[count] = mempool_var_allocate(pool, TEST_BLOCK_SIZE);

            assert(address[count]);
        }

        for (unsigned int count = 0; count < TEST_COUNT; ++count) {
            mempool_var_free(pool, address[count]);
        }
    }

    mempool_var_destroy(pool);

    clock_t diff = clock() - start;

    free(address);

    return diff;
}

static clock_t mempool_var_test_case3()
{
    void **address = (void **)malloc(sizeof(void *) * TEST_COUNT);

    assert(address);

    clock_t start = clock();

    mempool_var_t *pool = mempool_var_create(TEST_BLOCK_SIZE * TEST_COUNT);

    assert(pool);

    for (unsigned int times = 0; times < REPEAT_TIMES; ++times) {
        for (unsigned int count = 0; count < TEST_COUNT; ++count) {
            address[count] = mempool_var_allocate(pool, TEST_BLOCK_SIZE);

            assert(address[count]);
        }

        for (unsigned int count = TEST_COUNT; count > 0; --count) {
            mempool_var_free(pool, address[count - 1]);
        }
    }

    mempool_var_destroy(pool);

    clock_t diff = clock() - start;

    free(address);

    return diff;
}

int main(/*int argc, char *argv[]*/)
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

    printf("\n===== Variable-size memroy pool =====\n");

    case_func_t mempool_var_test_cases[] = {mempool_var_test_case1,
                                            mempool_var_test_case2,
                                            mempool_var_test_case3,
                                            NULL};

    for (unsigned int index = 0; mempool_var_test_cases[index]; ++index) {
        const clock_t total_clocks = mempool_var_test_cases[index]();

        printf("[Case %u]: %.2fms\n", index + 1, (double)total_clocks / CLOCKS_PER_SEC * 1000);
    }

    return 0;
}