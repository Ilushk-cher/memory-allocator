#include <assert.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "mem.h"

typedef bool (*test_function)(void);

static bool test_successful_allocation(void) {
    void* block1 = _malloc(128);
    if (!block1) return false;

    _free(block1);

    return true;
}

static bool test_free_one_block(void) {
    void* block1 = _malloc(128);
    void* block2 = _malloc(256);
    void* block3 = _malloc(64);

    if (!block1 || !block2 || !block3) return false;

    _free(block1);

    return true;
}

static bool test_free_two_blocks(void) {
    void* block1 = _malloc(128);
    void* block2 = _malloc(256);
    void* block3 = _malloc(64);

    if (!block1 || !block2 || !block3) return false;

    _free(block1);
    _free(block2);

    return true;
}

static bool test_new_and_old_region(void) {
    void* block1 = _malloc(1024 * 1024);
    void* block2 = _malloc(1024 * 1024);

    if (!block1 || !block2) return false;

    _free(block1);
    _free(block2);

    return true;
}

static bool test_new_region(void) {
    void* block1 = _malloc(1024 * 1024);
    void* block2 = _malloc(1024 * 1024);
    void* block3 = _malloc(1024 * 1024);

    if (!block1 || !block2 || !block3) return false;

    _free(block1);
    _free(block2);
    _free(block3);

    return true;
}

static bool test_repeated_allocation(void) {
    for (int i = 0; i < 100; ++i) {
        void* block = _malloc(1024);
        if (!block) return false;
        _free(block);
    }
    return true;
}

static bool test_region_overflow(void) {
    void* blocks[10];
    for (int i = 0; i < 10; ++i) {
        blocks[i] = _malloc(1024 * 1024); // Выделяем по 1 МБ
        if (!blocks[i]) {
            return false;
        }
    }

    for (int i = 0; i < 10; ++i) {
        _free(blocks[i]);
    }

    return true;
}

static bool test_boundary_conditions(void) {
    void* block1 = _malloc(1); // Минимальный размер
    void* block2 = _malloc(1024 * 1024 * 1024); // Максимальный размер (1 ГБ)

    if (!block1 || !block2) return false;

    _free(block1);
    _free(block2);

    return true;
}

static bool test_random_free(void) {
    void* blocks[5];
    for (int i = 0; i < 5; ++i) {
        blocks[i] = _malloc(128);
        if (!blocks[i]) {
            return false;
        }
    }

    _free(blocks[2]);
    _free(blocks[0]);
    _free(blocks[4]);
    _free(blocks[1]);
    _free(blocks[3]);

    return true;
}

const test_function tests[] = {
    test_successful_allocation,
    test_free_one_block,
    test_free_two_blocks,
    test_new_and_old_region,
    test_new_region,
    test_repeated_allocation,
    test_region_overflow,
    test_boundary_conditions,
    test_random_free,
};

const size_t tests_count = sizeof(tests) / sizeof(*tests);

bool test_all(void) {
    heap_init(1024 * 1024);

    bool success = true;
    for (size_t i = 0; i < tests_count; ++i) {
        if (!tests[i]()) {
            printf("Test %zu failed\n", i);
            success = false;
        }
    }

    if (success) {
        printf("All tests passed\n");
    } else {
        printf("Not all tests passed\n");
    }

    heap_term();
    return success;
}

