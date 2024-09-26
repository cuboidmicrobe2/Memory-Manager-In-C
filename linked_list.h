#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdint.h>
#include <stddef.h>

#include "common_defs.h"
#include "memory_manager.h"

typedef struct Node {
    uint16_t data;      // Stores the data as an unsigned 16-bit integer
    struct Node* next;  // A pointer to the next node in the List
} Node;

void list_init(Node** head, size_t size);
void list_insert(Node** head, uint16_t data);
void list_insert_after(Node* prevNode, uint16_t data);
void list_insert_before(Node** head, Node* nextNode, uint16_t data);
void list_delete(Node** head, uint16_t data);
Node* list_search(Node** head, uint16_t data);
void list_display(Node** head);
void list_display_range(Node** head, Node* startNode, Node* endNode);
int list_count_nodes(Node** head);
void list_cleanup(Node** head);

#endif