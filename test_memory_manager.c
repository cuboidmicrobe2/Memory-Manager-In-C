#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_defs.h"
#include "memory_manager.h"

void test_init() {
    printf_yellow("Testing mem_init...\n");
    mem_init(1024);  // Initialize with 1KB of memory
    void *block =
        mem_alloc(100);  // Try allocating to check if init was successful
    my_assert(block != NULL);
    mem_free(block);
    mem_deinit();
    printf_green("mem_init passed.\n");
}

void test_alloc_and_free() {
    printf_yellow("Testing mem_alloc and mem_free...\n");
    mem_init(1024);
    void *block1 = mem_alloc(100);
    my_assert(block1 != NULL);
    void *block2 = mem_alloc(200);
    my_assert(block2 != NULL);
    mem_free(block1);
    mem_free(block2);
    mem_deinit();
    printf_green("mem_alloc and mem_free passed.\n");
}

void test_resize() {
    printf_yellow("Testing mem_resize...\n");
    mem_init(1024);
    void *block = mem_alloc(100);
    my_assert(block != NULL);
    block = mem_resize(block, 200);
    my_assert(block != NULL);
    mem_free(block);
    mem_deinit();
    printf_green("mem_resize passed.\n");
}

void test_exceed_single_allocation() {
    printf_yellow("Testing allocation exceeding pool size...\n");
    mem_init(1024);                 // Initialize with 1KB of memory
    void *block = mem_alloc(2048);  // Try allocating more than available
    my_assert(block == NULL);       // Allocation should fail
    mem_deinit();
    printf_green("Single allocation exceeding pool size passed.\n");
}

void test_exceed_cumulative_allocation() {
    printf_yellow("Testing cumulative allocations exceeding pool size...\n");
    mem_init(1024);  // Initialize with 1KB of memory
    void *block1 = mem_alloc(512);
    my_assert(block1 != NULL);
    void *block2 = mem_alloc(512);
    my_assert(block2 != NULL);
    void *block3 = mem_alloc(100);  // This should fail, no space left
    my_assert(block3 == NULL);
    mem_free(block1);
    mem_free(block2);
    mem_deinit();
    printf_green("Cumulative allocations exceeding pool size passed.\n");
}

void test_double_free() {
    printf_yellow("Testing double deallocation...\n");
    mem_init(1024);  // Initialize with 1KB of memory

    void *block = mem_alloc(100);  // Allocate a block of 100 bytes
    my_assert(block != NULL);      // Ensure the block was allocated

    mem_free(block);  // Free the block for the first time
    mem_free(block);  // Attempt to free the block a second time

    printf_green(
        "Double deallocation test passed (if no crash and handled "
        "gracefully).\n");
    mem_deinit();  // Cleanup memory
}

void test_memory_fragmentation() {
    printf_yellow("Testing memory fragmentation handling...\n");
    mem_init(1024);  // Initialize with 2048 bytes

    void *block1 = mem_alloc(200);
    void *block2 = mem_alloc(300);
    void *block3 = mem_alloc(500);
    mem_free(block1);  // Free first block
    mem_free(block3);  // Free third block, leaving a fragmented hole before and
                       // after block2
    void *block4 = mem_alloc(500);  // Should fit into the space of block1
    assert(block4 != NULL);

    mem_free(block2);
    mem_free(block4);
    mem_deinit();
    printf_green("Memory fragmentation test passed.\n");
}

void test_edge_case_allocations() {
    printf_yellow("Testing edge case allocations...\n");
    mem_init(1024);  // Initialize with 1024 bytes

    void *block0 = mem_alloc(0);  // Edge case: zero allocation
    assert(block0 == NULL);  // Depending on handling, this could also be NULL

    void *block1 = mem_alloc(1024);  // Exactly remaining
    assert(block1 != NULL);

    void *block2 = mem_alloc(1);  // Attempt to allocate with no space left
    assert(block2 == NULL);

    mem_free(block0);
    mem_free(block1);
    mem_deinit();
    printf_green("Edge case allocations test passed.\n");
}

void test_full_allocation_and_free() {
    printf_yellow("Testing full allocation and free...\n");
    mem_init(1024);  // Initialize with 1KB of memory

    void *block = mem_alloc(1024);  // Allocate the entire pool
    my_assert(block != NULL);       // Ensure allocation was successful

    mem_free(block);  // Free the entire pool
    mem_deinit();
    printf_green("Full allocation and free test passed.\n");
}

void test_alloc_free_different_orders() {
    printf_yellow("Testing allocation and free in different orders...\n");
    mem_init(1024);  // Initialize with 1KB of memory

    void *block1 = mem_alloc(100);
    void *block2 = mem_alloc(200);
    void *block3 = mem_alloc(300);

    my_assert(block1 != NULL);
    my_assert(block2 != NULL);
    my_assert(block3 != NULL);

    mem_free(block2);
    mem_free(block1);
    mem_free(block3);

    mem_deinit();
    printf_green("Allocation and free in different orders test passed.\n");
}

void test_resize_smaller() {
    printf_yellow("Testing resize to smaller size...\n");
    mem_init(1024);  // Initialize with 1KB of memory

    void *block = mem_alloc(200);
    my_assert(block != NULL);

    block = mem_resize(block, 100);  // Resize to smaller size
    my_assert(block != NULL);

    mem_free(block);
    mem_deinit();
    printf_green("Resize to smaller size test passed.\n");
}

void test_resize_larger_no_space() {
    printf_yellow("Testing resize to larger size without enough space...\n");
    mem_init(1024);  // Initialize with 1KB of memory

    void *block1 = mem_alloc(500);
    void *block2 = mem_alloc(400);

    my_assert(block1 != NULL);
    my_assert(block2 != NULL);

    block1 = mem_resize(block1,
                        600);  // Attempt to resize larger without enough space
    my_assert(block1 == NULL);

    mem_free(block2);
    mem_free(block1);
    mem_deinit();
    printf_green("Resize to larger size without enough space test passed.\n");
}

void test_alloc_after_deinit() {
    printf_yellow("Testing allocation after deinitialization...\n");
    mem_init(1024);  // Initialize with 1KB of memory

    mem_deinit();  // Deinitialize the memory pool

    void *block = mem_alloc(100);  // Attempt to allocate after deinit
    my_assert(block == NULL);      // Allocation should fail

    printf_green("Allocation after deinitialization test passed.\n");
}

void test_alloc_zero_bytes() {
    printf_yellow("Testing allocation of zero bytes...\n");
    mem_init(1024);  // Initialize with 1KB of memory

    void *block = mem_alloc(0);  // Attempt to allocate zero bytes
    my_assert(block == NULL);    // Allocation should fail

    mem_deinit();
    printf_green("Allocation of zero bytes test passed.\n");
}

void test_free_null_pointer() {
    printf_yellow("Testing freeing a null pointer...\n");
    mem_init(1024);  // Initialize with 1KB of memory

    mem_free(NULL);  // Attempt to free a null pointer

    mem_deinit();
    printf_green("Freeing a null pointer test passed.\n");
}

void test_resize_to_zero_bytes() {
    printf_yellow("Testing resize to zero bytes...\n");
    mem_init(1024);  // Initialize with 1KB of memory

    void *block = mem_alloc(100);
    my_assert(block != NULL);

    block = mem_resize(block, 0);  // Resize to zero bytes
    my_assert(block == NULL);      // Should effectively free the block

    mem_deinit();
    printf_green("Resize to zero bytes test passed.\n");
}

void test_alloc_after_freeing_all_blocks() {
    printf_yellow("Testing allocation after freeing all blocks...\n");
    mem_init(1024);  // Initialize with 1KB of memory

    void *block1 = mem_alloc(100);
    void *block2 = mem_alloc(200);
    void *block3 = mem_alloc(300);

    my_assert(block1 != NULL);
    my_assert(block2 != NULL);
    my_assert(block3 != NULL);

    mem_free(block1);
    mem_free(block2);
    mem_free(block3);

    void *block4 =
        mem_alloc(600);         // Attempt to allocate after freeing all blocks
    my_assert(block4 != NULL);  // Allocation should succeed

    mem_free(block4);
    mem_deinit();
    printf_green("Allocation after freeing all blocks test passed.\n");
}

void test_expand() {
    printf_yellow("Testing expand...\n");
    mem_init(500);  // Initialize with 500 bytes of memory

    void *block = mem_alloc(300);  // Allocate 300 bytes
    my_assert(block != NULL);      // Allocation should succeed

    block = mem_resize(block, 400);  // Attempt to resize to 400 bytes
    my_assert(block != NULL);       // Resize should expand the memory

    mem_free(block);
    mem_deinit();
    printf_green("Expand test passed.\n");
}

int main() {
    test_init();
    test_alloc_and_free();
    test_resize();
    test_exceed_single_allocation();
    test_exceed_cumulative_allocation();
    test_double_free();
    test_memory_fragmentation();
    test_edge_case_allocations();
    test_full_allocation_and_free();
    test_alloc_free_different_orders();
    test_resize_smaller();
    test_resize_larger_no_space();
    test_alloc_after_deinit();
    test_alloc_zero_bytes();
    test_free_null_pointer();
    test_resize_to_zero_bytes();
    test_alloc_after_freeing_all_blocks();
    test_expand();
    printf_green("All tests passed successfully!\n");
    return 0;
}