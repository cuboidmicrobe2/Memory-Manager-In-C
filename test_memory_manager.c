#include "memory_manager.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "common_defs.h"

void test_init()
{
    printf_yellow("Testing mem_init...\n");
    mem_init(1024);               // Initialize with 1KB of memory
    void *block = mem_alloc(100); // Try allocating to check if init was successful
    my_assert(block != NULL);
    mem_free(block);
    mem_deinit();
    printf_green("mem_init passed.\n");
}

void test_alloc_and_free()
{
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

void test_resize()
{
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

void test_exceed_single_allocation()
{
    printf_yellow("Testing allocation exceeding pool size...\n");
    mem_init(1024);                // Initialize with 1KB of memory
    void *block = mem_alloc(2048); // Try allocating more than available
    my_assert(block == NULL);      // Allocation should fail
    mem_deinit();
    printf_green("Single allocation exceeding pool size passed.\n");
}

void test_exceed_cumulative_allocation()
{
    printf_yellow("Testing cumulative allocations exceeding pool size...\n");
    mem_init(1024); // Initialize with 1KB of memory
    void *block1 = mem_alloc(512);
    my_assert(block1 != NULL);
    void *block2 = mem_alloc(512);
    my_assert(block2 != NULL);
    void *block3 = mem_alloc(100); // This should fail, no space left
    my_assert(block3 == NULL);
    mem_free(block1);
    mem_free(block2);
    mem_deinit();
    printf_green("Cumulative allocations exceeding pool size passed.\n");
}

void test_double_free()
{
    printf_yellow("Testing double deallocation...\n");
    mem_init(1024); // Initialize with 1KB of memory

    void *block = mem_alloc(100); // Allocate a block of 100 bytes
    my_assert(block != NULL);     // Ensure the block was allocated

    mem_free(block); // Free the block for the first time
    mem_free(block); // Attempt to free the block a second time

    printf_green("Double deallocation test passed (if no crash and handled gracefully).\n");
    mem_deinit(); // Cleanup memory
}

void test_memory_fragmentation()
{
    printf_yellow("Testing memory fragmentation handling...\n");
    mem_init(1024); // Initialize with 2048 bytes

    void *block1 = mem_alloc(200);
    void *block2 = mem_alloc(300);
    void *block3 = mem_alloc(500);
    mem_free(block1);              // Free first block
    mem_free(block3);              // Free third block, leaving a fragmented hole before and after block2
    void *block4 = mem_alloc(500); // Should fit into the space of block1
    assert(block4 != NULL);

    mem_free(block2);
    mem_free(block4);
    mem_deinit();
    printf_green("Memory fragmentation test passed.\n");
}

void test_edge_case_allocations()
{
    printf_yellow("Testing edge case allocations...\n");
    mem_init(1024); // Initialize with 1024 bytes

    void *block0 = mem_alloc(0); // Edge case: zero allocation
    assert(block0 == NULL);      // Depending on handling, this could also be NULL

    void *block1 = mem_alloc(1024); // Exactly remaining
    assert(block1 != NULL);

    void *block2 = mem_alloc(1); // Attempt to allocate with no space left
    assert(block2 == NULL);

    mem_free(block0);
    mem_free(block1);
    mem_deinit();
    printf_green("Edge case allocations test passed.\n");
}

//  Extra tests
void test_expand() {
    printf_yellow("Testing expand...\n");
    mem_init(500);  // Initialize with 500 bytes of memory

    void *block = mem_alloc(300);  // Allocate 300 bytes
    my_assert(block != NULL);      // Allocation should succeed

    block = mem_resize(block, 400);  // Attempt to resize to 400 bytes
    my_assert(block != NULL);        // Resize should expand the memory

    mem_free(block);
    mem_deinit();
    printf_green("Expand test passed.\n");
}

void test_shrink() {
    printf_yellow("Testing shrink...\n");
    mem_init(500);  // Initialize with 500 bytes of memory

    void *block = mem_alloc(300);  // Allocate 300 bytes
    my_assert(block != NULL);      // Allocation should succeed

    block = mem_resize(block, 200);  // Attempt to resize to 200 bytes
    my_assert(block != NULL);        // Resize should shrink the memory

    mem_free(block);
    mem_deinit();
    printf_green("Shrink test passed.\n");
}

void test_resize_to_same_size() {
    printf_yellow("Testing resize to same size...\n");
    mem_init(500);  // Initialize with 500 bytes of memory

    void *block = mem_alloc(300);  // Allocate 300 bytes
    my_assert(block != NULL);      // Allocation should succeed

    block = mem_resize(block, 300);  // Attempt to resize to the same size
    my_assert(block != NULL);        // Resize should succeed

    mem_free(block);
    mem_deinit();
    printf_green("Resize to same size test passed.\n");
}

void test_resize_with_move() {
    printf_yellow("Testing resize with move...\n");
    mem_init(500);  // Initialize with 500 bytes of memory

    void *block1 = mem_alloc(100);  // Allocate 100 bytes
    void *block2 = mem_alloc(100);  // Allocate another 100 bytes
    my_assert(block1 != NULL);
    my_assert(block2 != NULL);

    mem_free(block1);  // Free the first block

    block2 = mem_resize(
        block2, 200);  // Attempt to resize the second block to 200 bytes
    my_assert(block2 != NULL);  // Resize should succeed and move the block

    mem_free(block2);
    mem_deinit();
    printf_green("Resize with move test passed.\n");
}

void test_resize_fail_due_to_no_space() {
    printf_yellow("Testing resize fail due to no space...\n");
    mem_init(500);  // Initialize with 500 bytes of memory

    void *block1 = mem_alloc(300);  // Allocate 300 bytes
    void *block2 = mem_alloc(150);  // Allocate another 150 bytes
    my_assert(block1 != NULL);
    my_assert(block2 != NULL);

    block1 = mem_resize(block1,
                        400);  // Attempt to resize the first block to 400 bytes
    my_assert(block1 == NULL);  // Resize should fail due to no space

    mem_free(block2);
    mem_free(block1);
    mem_deinit();
    printf_green("Resize fail due to no space test passed.\n");
}

void test_resize_null_block() {
    printf_yellow("Testing resize with null block...\n");
    mem_init(500);  // Initialize with 500 bytes of memory

    void *block = mem_resize(NULL, 100);  // Attempt to resize a null block
    my_assert(block != NULL);             // Resize should allocate new block

    mem_deinit();
    printf_green("Resize with null block test passed.\n");
}

void test_mem_init_deinit() {
    printf_yellow("Testing mem_init and mem_deinit...\n");

    // Test initialization with a valid size
    mem_init(1024);  // Initialize memory manager with 1024 bytes
    printf_green("Memory initialized with 1024 bytes.\n");

    // Test de-initialization
    mem_deinit();
    printf_green("Memory deinitialized successfully.\n");
}

void test_mem_alloc_free() {
    printf_yellow("Testing mem_alloc and mem_free...\n");

    mem_init(1024);  // Initialize with 1024 bytes

    // Allocate a small block
    void *block = mem_alloc(128);
    my_assert(block != NULL);  // Ensure allocation succeeds
    printf_green("Allocated 128 bytes successfully.\n");

    // Allocate a larger block
    void *block2 = mem_alloc(256);
    my_assert(block2 != NULL);  // Ensure allocation succeeds
    printf_green("Allocated 256 bytes successfully.\n");

    // Free the first block
    mem_free(block);
    printf_green("Freed 128-byte block successfully.\n");

    // Free the second block
    mem_free(block2);
    printf_green("Freed 256-byte block successfully.\n");

    mem_deinit();  // Cleanup
}

void test_mem_alloc_insufficient_space() {
    printf_yellow("Testing mem_alloc with insufficient space...\n");

    mem_init(512);  // Initialize with 512 bytes

    // Allocate a block larger than available space
    void *block = mem_alloc(1024);
    my_assert(block == NULL);  // Allocation should fail
    printf_green("Allocation failed as expected due to insufficient space.\n");

    mem_deinit();  // Cleanup
}

void test_mem_free_null() {
    printf_yellow("Testing mem_free with NULL...\n");

    mem_init(1024);  // Initialize with 1024 bytes

    // Freeing a NULL pointer should not crash the program
    mem_free(NULL);
    printf_green("Freeing NULL did not cause any issues.\n");

    mem_deinit();  // Cleanup
}

void test_mem_resize_shrink() {
    printf_yellow("Testing mem_resize (shrink)...\n");

    mem_init(1024);  // Initialize with 1024 bytes

    void *block = mem_alloc(256);
    my_assert(block != NULL);  // Ensure allocation succeeds
    printf_green("Allocated 256 bytes successfully.\n");

    // Resize the block to a smaller size
    block = mem_resize(block, 128);
    my_assert(block != NULL);  // Ensure resize succeeds
    printf_green("Resized block to 128 bytes successfully.\n");

    mem_free(block);
    mem_deinit();  // Cleanup
}

void test_mem_resize_expand() {
    printf_yellow("Testing mem_resize (expand)...\n");

    mem_init(1024);  // Initialize with 1024 bytes

    void *block = mem_alloc(256);
    my_assert(block != NULL);  // Ensure allocation succeeds
    printf_green("Allocated 256 bytes successfully.\n");

    // Resize the block to a larger size
    block = mem_resize(block, 512);
    my_assert(block != NULL);  // Ensure resize succeeds
    printf_green("Resized block to 512 bytes successfully.\n");

    mem_free(block);
    mem_deinit();  // Cleanup
}

void test_mem_resize_insufficient_space() {
    printf_yellow("Testing mem_resize with insufficient space...\n");

    mem_init(512);  // Initialize with 512 bytes

    void *block = mem_alloc(256);
    my_assert(block != NULL);  // Ensure allocation succeeds
    printf_green("Allocated 256 bytes successfully.\n");

    // Try resizing beyond available memory
    void *new_block = mem_resize(block, 1024);
    my_assert(new_block == NULL);  // Resize should fail
    printf_green("Resize failed as expected due to insufficient space.\n");

    mem_free(block);
    mem_deinit();  // Cleanup
}

void test_mem_alloc_all_memory() {
    printf_yellow("Testing mem_alloc to consume all memory...\n");

    mem_init(512);  // Initialize with 512 bytes

    // Allocate exactly 512 bytes
    void *block = mem_alloc(512);
    my_assert(block != NULL);  // Ensure allocation succeeds
    printf_green("Allocated exactly 512 bytes successfully.\n");

    // Attempt to allocate more memory, which should fail
    void *block2 = mem_alloc(1);
    my_assert(block2 == NULL);  // Allocation should fail
    printf_green("Allocation failed as expected when out of memory.\n");

    mem_free(block);
    mem_deinit();  // Cleanup
}

void test_mem_alloc_zero_size() {
    printf_yellow("Testing mem_alloc with zero size...\n");

    mem_init(1024);  // Initialize with 1024 bytes

    // Allocate zero bytes, which should return NULL or handle gracefully
    void *block = mem_alloc(0);
    my_assert(block == NULL);  // Should fail or return NULL
    printf_green("Allocation of zero bytes handled correctly.\n");

    mem_deinit();  // Cleanup
}

void test_mem_resize_null_pointer() {
    printf_yellow("Testing mem_resize with a NULL pointer...\n");

    mem_init(1024);  // Initialize with 1024 bytes

    // Resizing a NULL pointer should behave like mem_alloc
    void *block = mem_resize(NULL, 128);
    my_assert(block != NULL);  // Should allocate a new block
    printf_green("Resized NULL pointer resulted in a successful allocation.\n");

    mem_free(block);
    mem_deinit();  // Cleanup
}

void test_mem_free_after_deinit() {
    printf_yellow("Testing mem_free after mem_deinit...\n");

    mem_init(1024);  // Initialize with 1024 bytes

    void *block = mem_alloc(256);
    my_assert(block != NULL);  // Ensure allocation succeeds

    mem_deinit();  // Deinitialize memory manager

    // Freeing after deinit should not crash or cause undefined behavior
    mem_free(block);
    printf_green("Freed block after deinit without any issues.\n");
}

void test_large_alloc_exceeding_size() {
    printf_yellow("Testing large allocation exceeding memory size...\n");

    mem_init(1024);  // Initialize with 1024 bytes

    // Attempt to allocate a very large block, more than available memory
    void *block = mem_alloc(2048);
    my_assert(block == NULL);  // Allocation should fail
    printf_green(
        "Large allocation exceeding memory size failed as expected.\n");

    mem_deinit();  // Cleanup
}

void test_alloc_boundary_conditions() {
    printf_yellow("Testing allocation at boundary conditions...\n");

    mem_init(1024);  // Initialize with 1024 bytes

    // Allocate exactly all available memory
    void *block1 = mem_alloc(1024);
    my_assert(block1 != NULL);
    printf_green("Successfully allocated memory at boundary (1024 bytes).\n");

    // Free the memory
    mem_free(block1);

    // Try allocating a small amount at the boundary
    void *block2 = mem_alloc(1024 - 1);
    my_assert(block2 != NULL);
    printf_green(
        "Successfully allocated one byte less than total memory (1023 "
        "bytes).\n");

    // Try allocating the remaining 1 byte
    void *block3 = mem_alloc(1);
    my_assert(block3 != NULL);
    printf_green("Successfully allocated remaining 1 byte.\n");

    // Cleanup
    mem_free(block2);
    mem_free(block3);
    mem_deinit();
}

void test_mem_resize_to_zero() {
    printf_yellow("Testing mem_resize to zero...\n");

    mem_init(1024);  // Initialize with 1024 bytes

    // Allocate a block
    void *block = mem_alloc(256);
    my_assert(block != NULL);  // Ensure allocation succeeds

    // Resize the block to zero
    void *resized_block = mem_resize(block, 0);
    my_assert(resized_block ==
              NULL);  // Should either return NULL or deallocate
    printf_green("Resized block to zero bytes successfully.\n");

    // Clean up (if resize didn't free, free manually)
    if (resized_block != NULL) {
        mem_free(resized_block);
    }

    mem_deinit();  // Cleanup
}

void test_free_invalid_pointer() {
    printf_yellow("Testing free of invalid pointer...\n");

    mem_init(1024);  // Initialize with 1024 bytes

    // Allocate a block
    void *block = mem_alloc(256);
    my_assert(block != NULL);  // Ensure allocation succeeds

    // Attempt to free a pointer that was never allocated
    int dummy;
    mem_free(&dummy);
    printf_green("Freeing invalid pointer handled gracefully.\n");

    // Cleanup
    mem_free(block);
    mem_deinit();
}

void test_memory_overwrite() {
    printf_yellow("Testing memory overwrite protection...\n");

    mem_init(1024);  // Initialize with 1024 bytes

    // Allocate two blocks
    char *block1 = (char *)mem_alloc(128);
    char *block2 = (char *)mem_alloc(128);

    my_assert(block1 != NULL && block2 != NULL);  // Ensure allocations succeed

    // Write data to the first block
    for (int i = 0; i < 128; i++) {
        block1[i] = 'A';
    }

    // Verify that the second block is untouched
    for (int i = 0; i < 128; i++) {
        my_assert(block2[i] == 0);  // Assuming memory is initialized to zero
    }
    printf_green(
        "Memory overwrite protection successful, block2 remained intact.\n");

    // Cleanup
    mem_free(block1);
    mem_free(block2);
    mem_deinit();
}
int main()
{
    test_init();
    test_alloc_and_free();
    test_resize();
    test_exceed_single_allocation();
    test_exceed_cumulative_allocation();
    test_double_free();
    test_memory_fragmentation();
    test_edge_case_allocations();

    // Extra tests
    test_expand();
    test_shrink();
    test_resize_to_same_size();
    test_resize_with_move();
    test_resize_fail_due_to_no_space();
    test_resize_null_block();

    test_mem_init_deinit();
    test_mem_alloc_free();
    test_mem_alloc_insufficient_space();
    test_mem_free_null();
    test_mem_resize_shrink();
    test_mem_resize_expand();
    test_mem_resize_insufficient_space();
    test_mem_alloc_all_memory();
    test_mem_alloc_zero_size();
    test_mem_resize_null_pointer();
    test_mem_free_after_deinit();

    test_large_alloc_exceeding_size();
    test_alloc_boundary_conditions();
    test_mem_resize_to_zero();
    test_free_invalid_pointer();
    test_memory_overwrite();

    printf_green("All tests passed successfully!\n");
    return 0;
}