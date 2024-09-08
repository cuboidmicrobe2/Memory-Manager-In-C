#include "memory_manager.h"

void* memory = NULL;
size_t memorySize = 0;
Block* firstBlock = NULL;

/**
 * Initializes the memory manager with a given size.
 *
 * @param size The size of the memory pool to allocate.
 */
void mem_init(size_t size) {
    memory = malloc(size);
    memorySize = size;
    firstBlock = NULL;
}

/**
 * Allocates a block of memory of the given size from the memory pool.
 *
 * @param size The size of the memory block to allocate.
 * @return A pointer to the allocated memory block, or NULL if the allocation
 * fails.
 */
void* mem_alloc(size_t size) {
    if (memory == NULL || size == 0 || size > memorySize) {
        return NULL;
    }

    Block* newBlock = (Block*)malloc(sizeof(Block));
    if (!newBlock) return NULL;

    if (!firstBlock) {
        newBlock->address = memory;
        newBlock->size = size;
        newBlock->next = NULL;

        return newBlock->address;
    }

    Block* current = firstBlock;
    if ((current->address - memory) >= size) {
        newBlock->next = firstBlock;
        firstBlock = newBlock;
        newBlock->address = memory;
        newBlock->size = size;

        return newBlock->address;
    }

    do {
        // Free space between blocks
        size_t spaceBetween = (current->next->address - memory) -
                              ((current->address - memory) + current->size);

        if (spaceBetween >= size) {
            newBlock->next = current->next;
            current->next = newBlock;
            newBlock->size = size;
            newBlock->address = current->address + current->size;

            return newBlock->address;
        }

        current = current->next;
    } while (current->next);

    if (((current->address - memory) + (current->size + size)) < memorySize) {
        current->next = newBlock;
        newBlock->address = (current->address + current->size);
        current->size = size;
        current->next = NULL;

        return newBlock->address;
    }

    return NULL;  // Ber till Gud
}

/**
 * Frees a previously allocated block of memory.
 *
 * @param block A pointer to the memory block to free.
 */
void mem_free(void* block) {
    if (!block || !firstBlock) {
        return;
    }

    Block* current = firstBlock;
    Block* previous = NULL;

    while (current) {
        if (current->address == block) {
            if (previous) {
                previous->next = current->next;
            } else {
                firstBlock = current->next;
            }

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
 * @return A pointer to the resized memory block, or NULL if the allocation
 * fails.
 */
void* mem_resize(void* block, size_t size) {
    Block* current = firstBlock;
    while (current != NULL && current->address != block) {
        current = current->next;
    }

    if (!current) return NULL;

    void* oldPtr = current->address;
    size_t oldSize = current->size;
    mem_free(block);
    void* newBlock = mem_alloc(size);

    if (!newBlock) return NULL;
    size_t minSize = (oldSize < size) ? oldSize : size;
    memcpy(newBlock, oldPtr, minSize);
    return newBlock;

    // if (!block) {
    //     return mem_alloc(size);
    // }

    // if (size == 0) {
    //     mem_free(block);
    //     return NULL;
    // }

    // Block* current = firstBlock;
    // while (current) {
    //     if (current->address == block) {
    //         if (size <= current->size) {
    //             return block;
    //         }

    //         void* newBlock = mem_alloc(size);
    //         if (newBlock) {
    //             memcpy(newBlock, block, current->size);
    //             mem_free(block);
    //         }
    //         return newBlock;
    //     }
    //     current = current->next;
    // }
    // return NULL;
}

/**
 * Deinitializes the memory manager by freeing all allocated memory blocks and
 * resetting the memory manager state.
 */
void mem_deinit() {
    while (firstBlock) {
        Block* block = firstBlock;
        firstBlock = firstBlock->next;
        free(block);
    }
    free(memory);
    memory = NULL;
    memorySize = 0;
}