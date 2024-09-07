#include "linked_list.h"

void list_init(Node **head) {
    mem_init(MEMORY_SIZE);
    *head = NULL;
}

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
        if (current) {
            printf(", ");
        }
    }
    printf("]\n");
}

int list_count_nodes(Node **head) {
    int count = 0;
    Node* current = *head;
    while(current != NULL){
        count++;
        current = current->next;
    }
    return count;
}

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
