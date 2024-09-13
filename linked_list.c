#include "linked_list.h"

/**
 * Initializes the linked list.
 * 
 * @param head A pointer to the head of the list.
 */
void list_init(Node **head) {
    mem_init(MEMORY_SIZE);
    *head = NULL;
}

/**
 * Inserts a new node at the beginning of the list.
 * 
 * @param head A pointer to the head of the list.
 * @param data The data to be inserted.
 */
void list_insert(Node **head, int data) {
    Node *newNode = (Node *)mem_alloc(sizeof(Node));
    if (newNode == NULL) {
        printf_red("Memory allocation failed\n");
        return;
    }
    newNode->data = data;
    newNode->next = *head;
    *head = newNode;
}

/**
 * Inserts a new node after a given node.
 * 
 * @param prevNode A pointer to the node after which the new node will be inserted.
 * @param data The data to be inserted.
 */
void list_insert_after(Node *prevNode, int data) {
    if (prevNode == NULL) {
        printf_red("Previous node cannot be NULL\n");
        return;
    }
    Node *newNode = (Node *)mem_alloc(sizeof(Node));
    if (newNode == NULL) {
        printf_red("Memory allocation failed\n");
        return;
    }
    newNode->data = data;
    newNode->next = prevNode->next;
    prevNode->next = newNode;
}

/**
 * Inserts a new node before a given node.
 * 
 * @param head A pointer to the head of the list.
 * @param nextNode A pointer to the node before which the new node will be inserted.
 * @param data The data to be inserted.
 */
void list_insert_before(Node **head, Node *nextNode, int data) {
    if (nextNode == NULL) {
        printf_red("Previous node cannot be NULL\n");
        return;
    }
    Node *newNode = (Node *)mem_alloc(sizeof(Node));
    if (newNode == NULL) {
        printf_red("Memory allocation failed\n");
        return;
    }
    newNode->data = data;
    newNode->next = nextNode;

    if (*head == nextNode) {
        *head = newNode;
        return;
    }

    Node *current = *head;
    while (current != NULL && current->next != nextNode) {
        current = current->next;
    }

    if (current != NULL) {  // crazy if
        current->next = newNode;
    }
}

/**
 * Deletes a node with the given data from the list.
 * 
 * @param head A pointer to the head of the list.
 * @param data The data of the node to be deleted.
 */
void list_delete(Node **head, int data) {
    Node *temp = *head;
    Node *prev = NULL;

    if (temp != NULL && temp->data == data) {
        *head = temp->next;
        mem_free(temp);
        return;
    }

    while (temp != NULL && temp->data != data) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return;

    prev->next = temp->next;
    mem_free(temp);
}

/**
 * Searches for a node with the given data in the list.
 * 
 * @param head A pointer to the head of the list.
 * @param data The data to search for.
 * @return A pointer to the node with the given data, or NULL if not found.
 */
Node *list_search(Node **head, int data) {
    Node *current = *head;
    while (current != NULL) {
        if (current->data == data) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * Displays the entire list.
 * 
 * @param head A pointer to the head of the list.
 */
void list_display(Node **head) {
    Node *current = *head;
    if (current == NULL) {
        printf("NULL");
        return;
    }

    printf("[");
    while (current != NULL) {
        printf("%d", current->data);
        current = current->next;
        if (current) {
            printf(", ");
        }
    }
    printf("]\n");
}

/**
 * Displays a selected range of the list.
 * 
 * @param head A pointer to the head of the list.
 * @param startNode A pointer to the starting node of the range.
 * @param endNode A pointer to the ending node of the range.
 */
void list_display_select(Node **head, Node *startNode, Node *endNode) {
    Node *current = startNode;
    if (current == NULL) {
        printf("NULL");
        return;
    }

    printf("[");
    while (current != NULL && current != endNode->next) {
        printf("%d", current->data);
        current = current->next;
        if (current != endNode->next) {
            printf(", ");
        }
    }
    printf("]\n");
}

/**
 * Counts the number of nodes in the list.
 * 
 * @param head A pointer to the head of the list.
 * @return The number of nodes in the list.
 */
int list_count_nodes(Node **head) {
    int count = 0;
    Node* current = *head;
    while(current != NULL){
        count++;
        current = current->next;
    }
    return count;
}

/**
 * Cleans up the list by freeing all nodes.
 * 
 * @param head A pointer to the head of the list.
 */
void list_cleanup(Node **head) {
    Node* current = *head;
    Node* next;
    while(current != NULL){
        next = current->next;
        mem_free(current);
        current = next;
    }
    *head = NULL;
}
