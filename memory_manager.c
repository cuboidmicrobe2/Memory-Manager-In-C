#include "memory_manager.h"

void* memory = NULL;
size_t memorySize = 0;
Block* allocatedBlocks = NULL;

/**
 * Initializes the memory manager with a given size.
 *
 * @param size The size of the memory pool to allocate.
 */
void mem_init(size_t size) {
    memory = malloc(size);
    memorySize = size;
    allocatedBlocks = NULL;
}

/**
 * Allocates a block of memory of the given size from the memory pool.
 *
 * @param size The size of the memory block to allocate.
 * @return A pointer to the allocated memory block, or NULL if the allocation fails.
 */
void* mem_alloc(size_t size) {
    if (memory == NULL || size == 0 || size > memorySize) {
        return NULL;
    }

    Block* newBlock = (Block*)malloc(sizeof(Block));
    if (!newBlock) {
        return NULL;
    }

    newBlock->address = (char*)memory + memorySize - size;
    newBlock->size = size;
    newBlock->next = allocatedBlocks;
    allocatedBlocks = newBlock;

    memorySize -= size;
    return newBlock->address;
}

/**
 * Frees a previously allocated block of memory.
 *
 * @param block A pointer to the memory block to free.
 */
void mem_free(void *block) {
    if (!block || !allocatedBlocks) {
        return;
    }

    Block* current = allocatedBlocks;
    Block* previous = NULL;

    while (current) {
        if (current->address == block) {
            if (previous) {
                previous->next = current->next;
            } else {
                allocatedBlocks = current->next;
            }

            memorySize += current->size;
            free(current);
            return;
        }

        previous = current;
        current = current->next;
    }
}

/**
 * Resizes a previously allocated block of memory.
 *
 * @param block A pointer to the memory block to resize.
 * @param size The new size of the memory block.
 * @return A pointer to the resized memory block, or NULL if the allocation fails.
 */
void* mem_resize(void *block, size_t size) {
    if (!block) {
        return mem_alloc(size);
    }

    Block* current = allocatedBlocks;
    while (current) {
        if (current->address == block) {
            if (size <= current->size) {
                return block;
            }

            void* newBlock = mem_alloc(size);
            if (newBlock) {
                memcpy(newBlock, block, current->size);
                mem_free(block);
            }
            return newBlock;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * Deinitializes the memory manager by freeing all allocated memory blocks and resetting the memory manager state.
 */
void mem_deinit() {
    while (allocatedBlocks) {
        Block* block = allocatedBlocks;
        allocatedBlocks = allocatedBlocks->next;
        free(block);
    }
    free(memory);
    memory = NULL;
    memorySize = 0;
}