#include "linked_list.h"
#include <stdio.h>
#include <assert.h>

#include "common_defs.h"

// Helper to assert node values and report
void assert_node_value(Node *node, uint16_t expected_value, const char *test_name)
{
    if (node == NULL || node->data != expected_value)
    {
        printf_red("FAIL [%s]: Expected %hu, got %hu\n", test_name, expected_value, node ? node->data : 0);
    }
    else
    {
        printf_green("PASS [%s]: Expected %hu\n", test_name, expected_value);
    }
}

// Test inserting elements
void test_insert()
{
    printf_yellow("Testing list_insert...\n");
    Node *head = NULL;
    list_init(&head);
    list_insert(&head, 10);
    list_insert(&head, 20);
    assert_node_value(head, 20, "test_insert");
    assert_node_value(head->next, 10, "test_insert");

    list_cleanup(&head);
}

// Test inserting elements
void test_insert_loop(int count)
{
    printf_yellow("Testing list_insert...\n");
    Node *head = NULL;
    list_init(&head);
    for (int i = 0; i < count; i++)
    {
        list_insert(&head, i);
    }
    Node *current = head;
    for (int i = count - 1; i >= 0; i--)
    {
        assert_node_value(current, i, "test_insert");
        current = current->next;
    }

    list_cleanup(&head);
}

// Test inserting after
void test_insert_after()
{
    printf_yellow("Testing list_insert_after...\n");
    Node *head = NULL;
    list_init(&head);
    list_insert(&head, 10);
    list_insert(&head, 20);
    Node *node = list_search(&head, 20);
    list_insert_after(node, 15);
    assert_node_value(node->next, 15, "test_insert_after");

    list_cleanup(&head);
}

// Test deletion
void test_delete()
{
    printf_yellow("Testing list_delete...\n");
    Node *head = NULL;
    list_init(&head);
    list_insert(&head, 10);
    list_insert(&head, 20);
    list_delete(&head, 10);
    assert_node_value(head, 20, "test_delete");

    list_cleanup(&head);
}

// Test searching
void test_search()
{
    printf_yellow("Testing list_search...\n");
    Node *head = NULL;
    list_init(&head);
    list_insert(&head, 10);
    list_insert(&head, 20);
    Node *found = list_search(&head, 10);
    assert_node_value(found, 10, "test_search");

    list_cleanup(&head);
}


// EXTRA
// Test list initialization
void test_list_init() {
    Node *head = NULL;
    list_init(&head);
    if (head != NULL) {
        printf_red("FAIL [test_list_init]: Head should be NULL after initialization\n");
    } else {
        printf_green("PASS [test_list_init]: Head is NULL after initialization\n");
    }
}

// Test inserting before a node
void test_insert_before() {
    Node *head = NULL;
    list_init(&head);
    list_insert(&head, 20);
    list_insert(&head, 10);
    Node *node20 = list_search(&head, 20);
    list_insert_before(&head, node20, 15);
    Node *found = list_search(&head, 15);
    assert_node_value(found, 15, "test_insert_before");
    list_cleanup(&head);
}

// Test displaying the list
void test_display() {
    Node *head = NULL;
    list_init(&head);
    list_insert(&head, 10);
    list_insert(&head, 20);
    printf("Expected list: [20, 10]\n");
    printf("Actual list: ");
    list_display(&head);
    list_cleanup(&head);
}

// Test displaying a selected range of the list
void test_display_select() {
    Node *head = NULL;
    list_init(&head);
    list_insert(&head, 10);
    list_insert(&head, 20);
    list_insert(&head, 30);
    Node *startNode = list_search(&head, 20);
    Node *endNode = list_search(&head, 10);
    printf("Expected selected list: [20, 10]\n");
    printf("Actual selected list: ");
    list_display_select(&head, startNode, endNode);
    list_cleanup(&head);
}

// Test counting nodes
void test_count_nodes() {
    Node *head = NULL;
    list_init(&head);
    list_insert(&head, 10);
    list_insert(&head, 20);
    int count = list_count_nodes(&head);
    if (count != 2) {
        printf_red("FAIL [test_count_nodes]: Expected 2 but got %d\n", count);
    } else {
        printf_green("PASS [test_count_nodes]: Expected 2\n");
    }
    list_cleanup(&head);
}

// Test cleaning up the list
void test_cleanup() {
    Node *head = NULL;
    list_init(&head);
    list_insert(&head, 10);
    list_insert(&head, 20);
    list_cleanup(&head);
    if (head != NULL) {
        printf_red("FAIL [test_cleanup]: Head should be NULL after cleanup\n");
    } else {
        printf_green("PASS [test_cleanup]: Head is NULL after cleanup\n");
    }
}

// Test inserting before and after
void test_insert_all() {
    printf_yellow("Starting test [insert_both]\n");
    Node *head = NULL;
    list_init(&head);
    list_insert(&head, 30);
    list_insert(&head, 50);
    list_insert_before(&head, head->next, 70);
    list_insert(&head, 20);
    list_insert(&head, 10);
    list_delete(&head, 50);
    Node* node10 = list_search(&head, 10);
    list_insert_after(node10, 50);
    list_insert_before(&head, node10, 60);
    list_insert(&head, 40);

    printf("List should be: [40, 60, 10, 50, 20, 70, 30]\nYour was:       ");
    list_display(&head);
    printf("List should be: [60, 10, 50, 20, 70]\nYour was:       ");
    list_display_select(&head, list_search(&head, 60), list_search(&head, 70));
}

// Main function to run all tests
int main()
{
    test_insert();
    test_insert_loop(100);
    test_insert_after();
    test_delete();
    test_search();

    // Extra
    test_list_init();
    test_insert_before();
    test_display();
    test_display_select();
    test_count_nodes();
    test_cleanup();
    test_insert_all();
    
    return 0;
}
