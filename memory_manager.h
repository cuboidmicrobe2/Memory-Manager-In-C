#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Block {
    void* address;
    size_t size;
    struct Block* next;
} Block;

void defragment_memory();
void mem_init(size_t size);
void* mem_alloc(size_t size);
void mem_free(void* block);
void* mem_resize(void* block, size_t size);
void mem_deinit();

#endif