#include <utest.h>
#include <collection/dlist.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * Global helper functions for testing destroy callbacks.
 */

static void destroy_counter(void *data) {
    int *counter = (int *)data;
    if (counter != NULL) {
        (*counter)++;
    }
}

static void destroy_noop(void *data) {
    (void)data;
}

/**
 * Test: dlist_init
 * Dependencies: None
 * Description: Tests basic initialization of the dlist structure.
 */
UTEST_TEST_CASE(dlist_init){
    // Test 1: Initialize with NULL destroy function
    {
        DList list;
        dlist_init(&list, NULL);
        
        EXPECT_EQUAL_UINT64(list.size, 0);
        EXPECT_NULL(list.head);
        EXPECT_NULL(list.tail);
        EXPECT_NULL(list.destroy);
    }
    
    // Test 2: Initialize with non-NULL destroy function
    {
        DList list;
        dlist_init(&list, destroy_noop);
        
        EXPECT_EQUAL_UINT64(list.size, 0);
        EXPECT_NULL(list.head);
        EXPECT_NULL(list.tail);
        EXPECT_NOT_NULL(list.destroy);
    }
    
    // Test 3: Multiple initializations
    {
        DList list1, list2;
        dlist_init(&list1, NULL);
        dlist_init(&list2, destroy_noop);
        
        EXPECT_NULL(list1.destroy);
        EXPECT_NOT_NULL(list2.destroy);
    }
}

/**
 * Test: dlist_size
 * Dependencies: dlist_init
 */
UTEST_TEST_CASE(dlist_size){
    // Test 1: Size of newly initialized list
    {
        DList list;
        dlist_init(&list, NULL);
        
        EXPECT_EQUAL_UINT64(dlist_size(&list), 0);
    }
    
    // Test 2: Size consistency
    {
        DList list;
        dlist_init(&list, NULL);
        
        EXPECT_EQUAL_UINT64(dlist_size(&list), list.size);
    }
}

/**
 * Test: dlist_empty
 * Dependencies: dlist_init
 */
UTEST_TEST_CASE(dlist_empty){
    // Test 1: Empty after initialization
    {
        DList list;
        dlist_init(&list, NULL);
        
        EXPECT_TRUE(dlist_empty(&list));
        
        dlist_destroy(&list);
    }
    
    // Test 2: Consistency with size
    {
        DList list;
        dlist_init(&list, NULL);
        
        EXPECT_TRUE(dlist_empty(&list) == (dlist_size(&list) == 0));
        
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_destroy
 * Dependencies: dlist_init
 */
UTEST_TEST_CASE(dlist_destroy){
    // Test 1: Destroy empty list without destroy function
    {
        DList list;
        dlist_init(&list, NULL);
        dlist_destroy(&list);
        
        EXPECT_EQUAL_UINT64(list.size, 0);
        EXPECT_NULL(list.head);
        EXPECT_NULL(list.tail);
    }
    
    // Test 2: Destroy empty list with destroy function
    {
        DList list;
        dlist_init(&list, destroy_noop);
        dlist_destroy(&list);
        
        EXPECT_EQUAL_UINT64(list.size, 0);
        EXPECT_NULL(list.head);
        EXPECT_NULL(list.tail);
    }
    
    // Test 3: Multiple destroy calls
    {
        DList list;
        dlist_init(&list, NULL);
        dlist_destroy(&list);
        dlist_destroy(&list);
        
        EXPECT_EQUAL_UINT64(list.size, 0);
    }
}

/**
 * Test: dlist_push_front
 * Dependencies: dlist_init, dlist_size
 */
UTEST_TEST_CASE(dlist_push_front){
    // Test 1: Push to empty list
    {
        DList list;
        dlist_init(&list, NULL);
        
        const char *data = "first";
        int result = dlist_push_front(&list, (void *)data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 1);
        EXPECT_NOT_NULL(list.head);
        EXPECT_TRUE(list.head == list.tail);
        
        dlist_destroy(&list);
    }
    
    // Test 2: Push multiple elements
    {
        DList list;
        dlist_init(&list, NULL);
        
        for (int i = 0; i < 10; i++) {
            int result = dlist_push_front(&list, (void *)(long)i);
            EXPECT_EQUAL_INT(result, 0);
        }
        
        EXPECT_EQUAL_UINT64(dlist_size(&list), 10);
        
        dlist_destroy(&list);
    }
    
    // Test 3: Push NULL pointer
    {
        DList list;
        dlist_init(&list, NULL);
        
        int result = dlist_push_front(&list, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 1);
        EXPECT_NOT_NULL(list.head);
        
        dlist_destroy(&list);
    }
    
    // Test 4: Order verification
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_front(&list, (void *)"third");
        dlist_push_front(&list, (void *)"second");
        dlist_push_front(&list, (void *)"first");
        
        DListNode* node = list.head;
        EXPECT_EQUAL_STRING((char*)node->data, "first");
        
        dlist_destroy(&list);
    }
    
    // Test 5: Verify bidirectional links
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_front(&list, (void *)"second");
        dlist_push_front(&list, (void *)"first");
        
        // Check forward link
        EXPECT_TRUE(list.head->next == list.tail);
        // Check backward link
        EXPECT_TRUE(list.tail->prev == list.head);
        // Check head has no prev
        EXPECT_NULL(list.head->prev);
        // Check tail has no next
        EXPECT_NULL(list.tail->next);
        
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_push_back
 * Dependencies: dlist_init, dlist_size
 */
UTEST_TEST_CASE(dlist_push_back){
    // Test 1: Push to empty list
    {
        DList list;
        dlist_init(&list, NULL);
        
        const char *data = "first";
        int result = dlist_push_back(&list, (void *)data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 1);
        EXPECT_NOT_NULL(list.head);
        EXPECT_TRUE(list.head == list.tail);
        
        dlist_destroy(&list);
    }
    
    // Test 2: Push multiple elements
    {
        DList list;
        dlist_init(&list, NULL);
        
        for (int i = 0; i < 10; i++) {
            int result = dlist_push_back(&list, (void *)(long)i);
            EXPECT_EQUAL_INT(result, 0);
        }
        
        EXPECT_EQUAL_UINT64(dlist_size(&list), 10);
        
        dlist_destroy(&list);
    }
    
    // Test 3: Push NULL pointer
    {
        DList list;
        dlist_init(&list, NULL);
        
        int result = dlist_push_back(&list, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 1);
        
        dlist_destroy(&list);
    }
    
    // Test 4: Order verification
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"first");
        dlist_push_back(&list, (void *)"second");
        dlist_push_back(&list, (void *)"third");
        
        DListNode* node = list.head;
        EXPECT_EQUAL_STRING((char*)node->data, "first");
        
        dlist_destroy(&list);
    }
    
    // Test 5: Verify bidirectional links
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"first");
        dlist_push_back(&list, (void *)"second");
        
        // Check forward link
        EXPECT_TRUE(list.head->next == list.tail);
        // Check backward link
        EXPECT_TRUE(list.tail->prev == list.head);
        // Check head has no prev
        EXPECT_NULL(list.head->prev);
        // Check tail has no next
        EXPECT_NULL(list.tail->next);
        
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_pop_front
 * Dependencies: dlist_init, dlist_push_front, dlist_size
 */
UTEST_TEST_CASE(dlist_pop_front){
    // Test 1: Pop from empty list
    {
        DList list;
        dlist_init(&list, NULL);
        
        void* data = NULL;
        int result = dlist_pop_front(&list, &data);
        
        EXPECT_EQUAL_INT(result, -1);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 0);
        
        dlist_destroy(&list);
    }
    
    // Test 2: Pop single element without retrieving data
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_front(&list, (void *)0x1234);
        
        int result = dlist_pop_front(&list, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 0);
        
        dlist_destroy(&list);
    }
    
    // Test 3: Pop single element and retrieve data
    {
        DList list;
        dlist_init(&list, NULL);
        
        const char* test_data = "test";
        dlist_push_front(&list, (void *)test_data);
        
        void* data = NULL;
        int result = dlist_pop_front(&list, &data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_TRUE(data == (void *)test_data);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 0);
        
        dlist_destroy(&list);
    }
    
    // Test 4: Pop multiple elements
    {
        DList list;
        dlist_init(&list, NULL);
        
        for (int i = 0; i < 10; i++) {
            dlist_push_front(&list, (void *)(long)i);
        }
        
        for (int i = 9; i >= 0; i--) {
            void* data = NULL;
            int result = dlist_pop_front(&list, &data);
            EXPECT_EQUAL_INT(result, 0);
            EXPECT_TRUE(data == (void *)(long)i);
        }
        
        EXPECT_EQUAL_UINT64(dlist_size(&list), 0);
        
        dlist_destroy(&list);
    }
    
    // Test 5: Pop with destroy function (data not retrieved)
    {
        int destroy_count = 0;
        DList list;
        dlist_init(&list, destroy_counter);
        
        dlist_push_front(&list, &destroy_count);
        dlist_push_front(&list, &destroy_count);
        dlist_push_front(&list, &destroy_count);
        
        dlist_pop_front(&list, NULL);
        EXPECT_EQUAL_INT(destroy_count, 1);
        
        dlist_pop_front(&list, NULL);
        EXPECT_EQUAL_INT(destroy_count, 2);
        
        dlist_pop_front(&list, NULL);
        EXPECT_EQUAL_INT(destroy_count, 3);
        
        dlist_destroy(&list);
    }
    
    // Test 6: Pop with data retrieval (destroy not called)
    {
        int destroy_count = 0;
        DList list;
        dlist_init(&list, destroy_counter);
        
        dlist_push_front(&list, &destroy_count);
        dlist_push_front(&list, &destroy_count);
        
        void* data = NULL;
        dlist_pop_front(&list, &data);
        
        EXPECT_EQUAL_INT(destroy_count, 0);
        EXPECT_NOT_NULL(data);
        
        dlist_destroy(&list);
    }
    
    // Test 7: Pop updates tail when list becomes empty
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_front(&list, (void *)"only");
        dlist_pop_front(&list, NULL);
        
        EXPECT_NULL(list.head);
        EXPECT_NULL(list.tail);
        
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_pop_back
 * Dependencies: dlist_init, dlist_push_back, dlist_size
 */
UTEST_TEST_CASE(dlist_pop_back){
    // Test 1: Pop from empty list
    {
        DList list;
        dlist_init(&list, NULL);
        
        void* data = NULL;
        int result = dlist_pop_back(&list, &data);
        
        EXPECT_EQUAL_INT(result, -1);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 0);
        
        dlist_destroy(&list);
    }
    
    // Test 2: Pop single element without retrieving data
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)0x1234);
        
        int result = dlist_pop_back(&list, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 0);
        EXPECT_NULL(list.head);
        EXPECT_NULL(list.tail);
        
        dlist_destroy(&list);
    }
    
    // Test 3: Pop single element and retrieve data
    {
        DList list;
        dlist_init(&list, NULL);
        
        const char* test_data = "test";
        dlist_push_back(&list, (void *)test_data);
        
        void* data = NULL;
        int result = dlist_pop_back(&list, &data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_TRUE(data == (void *)test_data);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 0);
        
        dlist_destroy(&list);
    }
    
    // Test 4: Pop from two-element list
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"first");
        dlist_push_back(&list, (void *)"second");
        
        void* data = NULL;
        int result = dlist_pop_back(&list, &data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_STRING((char*)data, "second");
        EXPECT_EQUAL_UINT64(dlist_size(&list), 1);
        EXPECT_NOT_NULL(list.head);
        EXPECT_TRUE(list.head == list.tail);
        
        dlist_destroy(&list);
    }
    
    // Test 5: Pop multiple elements
    {
        DList list;
        dlist_init(&list, NULL);
        
        for (int i = 0; i < 10; i++) {
            dlist_push_back(&list, (void *)(long)i);
        }
        
        for (int i = 9; i >= 0; i--) {
            void* data = NULL;
            int result = dlist_pop_back(&list, &data);
            EXPECT_EQUAL_INT(result, 0);
            EXPECT_TRUE(data == (void *)(long)i);
        }
        
        EXPECT_EQUAL_UINT64(dlist_size(&list), 0);
        
        dlist_destroy(&list);
    }
    
    // Test 6: Pop with destroy function (data not retrieved)
    {
        int destroy_count = 0;
        DList list;
        dlist_init(&list, destroy_counter);
        
        dlist_push_back(&list, &destroy_count);
        dlist_push_back(&list, &destroy_count);
        dlist_push_back(&list, &destroy_count);
        
        dlist_pop_back(&list, NULL);
        EXPECT_EQUAL_INT(destroy_count, 1);
        
        dlist_pop_back(&list, NULL);
        EXPECT_EQUAL_INT(destroy_count, 2);
        
        dlist_pop_back(&list, NULL);
        EXPECT_EQUAL_INT(destroy_count, 3);
        
        dlist_destroy(&list);
    }
    
    // Test 7: Pop with data retrieval (destroy not called)
    {
        int destroy_count = 0;
        DList list;
        dlist_init(&list, destroy_counter);
        
        dlist_push_back(&list, &destroy_count);
        dlist_push_back(&list, &destroy_count);
        
        void* data = NULL;
        dlist_pop_back(&list, &data);
        
        EXPECT_EQUAL_INT(destroy_count, 0);
        EXPECT_NOT_NULL(data);
        
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_next
 * Dependencies: dlist_init, dlist_push_back, dlist_head
 */
UTEST_TEST_CASE(dlist_next){
    // Test 1: Next of single element
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"only");
        
        DListNode* node = dlist_head(&list);
        EXPECT_NULL(dlist_next(node));
        
        dlist_destroy(&list);
    }
    
    // Test 2: Iterate through list forward
    {
        DList list;
        dlist_init(&list, NULL);
        
        for (int i = 0; i < 10; i++) {
            dlist_push_back(&list, (void *)(long)i);
        }
        
        int count = 0;
        for (DListNode* node = dlist_head(&list); node != NULL; node = dlist_next(node)) {
            EXPECT_TRUE(node->data == (void *)(long)count);
            count++;
        }
        
        EXPECT_EQUAL_INT(count, 10);
        
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_prev
 * Dependencies: dlist_init, dlist_push_back, dlist_tail
 */
UTEST_TEST_CASE(dlist_prev){
    // Test 1: Prev of single element
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"only");
        
        DListNode* node = dlist_tail(&list);
        EXPECT_NULL(dlist_prev(node));
        
        dlist_destroy(&list);
    }
    
    // Test 2: Iterate through list backward
    {
        DList list;
        dlist_init(&list, NULL);
        
        for (int i = 0; i < 10; i++) {
            dlist_push_back(&list, (void *)(long)i);
        }
        
        int count = 9;
        for (DListNode* node = dlist_tail(&list); node != NULL; node = dlist_prev(node)) {
            EXPECT_TRUE(node->data == (void *)(long)count);
            count--;
        }
        
        EXPECT_EQUAL_INT(count, -1);
        
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_data
 * Dependencies: dlist_init, dlist_push_front
 */
UTEST_TEST_CASE(dlist_data){
    // Test 1: Data of single node
    {
        DList list;
        dlist_init(&list, NULL);
        
        const char* test_data = "test";
        dlist_push_front(&list, (void *)test_data);
        
        DListNode* node = dlist_head(&list);
        EXPECT_TRUE(dlist_data(node) == (void *)test_data);
        
        dlist_destroy(&list);
    }
    
    // Test 2: Data consistency with direct access
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"data");
        
        DListNode* node = dlist_head(&list);
        EXPECT_TRUE(dlist_data(node) == node->data);
        
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_head
 * Dependencies: dlist_init, dlist_push_front
 */
UTEST_TEST_CASE(dlist_head){
    // Test 1: Head of empty list
    {
        DList list;
        dlist_init(&list, NULL);
        
        EXPECT_NULL(dlist_head(&list));
        
        dlist_destroy(&list);
    }
    
    // Test 2: Head of multiple element list
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"first");
        dlist_push_back(&list, (void *)"second");
        
        DListNode* head = dlist_head(&list);
        EXPECT_EQUAL_STRING((char*)head->data, "first");
        
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_tail
 * Dependencies: dlist_init, dlist_push_back
 */
UTEST_TEST_CASE(dlist_tail){
    // Test 1: Tail of empty list
    {
        DList list;
        dlist_init(&list, NULL);
        
        EXPECT_NULL(dlist_tail(&list));
        
        dlist_destroy(&list);
    }
    
    // Test 2: Tail of multiple element list
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"first");
        dlist_push_back(&list, (void *)"second");
        
        DListNode* tail = dlist_tail(&list);
        EXPECT_EQUAL_STRING((char*)tail->data, "second");
        
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_insert_after
 * Dependencies: dlist_init, dlist_push_back, dlist_head, dlist_next
 */
UTEST_TEST_CASE(dlist_insert_after){
    // Test 1: Insert after NULL node
    {
        DList list;
        dlist_init(&list, NULL);
        
        int result = dlist_insert_after(&list, NULL, (void *)"data");
        
        EXPECT_EQUAL_INT(result, -1);
        
        dlist_destroy(&list);
    }
    
    // Test 2: Insert after only node
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"first");
        
        DListNode* node = dlist_head(&list);
        int result = dlist_insert_after(&list, node, (void *)"second");
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 2);
        
        DListNode* next = dlist_next(node);
        EXPECT_EQUAL_STRING((char*)next->data, "second");
        
        dlist_destroy(&list);
    }
    
    // Test 3: Insert in middle
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"first");
        dlist_push_back(&list, (void *)"third");
        
        DListNode* node = dlist_head(&list);
        int result = dlist_insert_after(&list, node, (void *)"second");
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 3);
        
        node = dlist_head(&list);
        EXPECT_EQUAL_STRING((char*)node->data, "first");
        node = dlist_next(node);
        EXPECT_EQUAL_STRING((char*)node->data, "second");
        node = dlist_next(node);
        EXPECT_EQUAL_STRING((char*)node->data, "third");
        
        dlist_destroy(&list);
    }
    
    // Test 4: Insert NULL data
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"first");
        
        DListNode* node = dlist_head(&list);
        int result = dlist_insert_after(&list, node, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 2);
        
        dlist_destroy(&list);
    }
    
    // Test 5: Verify bidirectional links after insert
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"first");
        dlist_push_back(&list, (void *)"third");
        
        DListNode* node = dlist_head(&list);
        dlist_insert_after(&list, node, (void *)"second");
        
        DListNode* middle = dlist_next(node);
        EXPECT_TRUE(middle->prev == node);
        EXPECT_TRUE(middle->next == dlist_tail(&list));
        
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_insert_before
 * Dependencies: dlist_init, dlist_push_back, dlist_head, dlist_next
 */
UTEST_TEST_CASE(dlist_insert_before){
    // Test 1: Insert before NULL node
    {
        DList list;
        dlist_init(&list, NULL);
        
        int result = dlist_insert_before(&list, NULL, (void *)"data");
        
        EXPECT_EQUAL_INT(result, -1);
        
        dlist_destroy(&list);
    }
    
    // Test 2: Insert before head
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"second");
        
        DListNode* node = dlist_head(&list);
        int result = dlist_insert_before(&list, node, (void *)"first");
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 2);
        
        DListNode* head = dlist_head(&list);
        EXPECT_EQUAL_STRING((char*)head->data, "first");
        
        dlist_destroy(&list);
    }
    
    // Test 3: Insert in middle
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"first");
        dlist_push_back(&list, (void *)"third");
        
        DListNode* node = dlist_tail(&list);
        int result = dlist_insert_before(&list, node, (void *)"second");
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 3);
        
        node = dlist_head(&list);
        EXPECT_EQUAL_STRING((char*)node->data, "first");
        node = dlist_next(node);
        EXPECT_EQUAL_STRING((char*)node->data, "second");
        node = dlist_next(node);
        EXPECT_EQUAL_STRING((char*)node->data, "third");
        
        dlist_destroy(&list);
    }
    
    // Test 4: Insert NULL data
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"first");
        
        DListNode* node = dlist_head(&list);
        int result = dlist_insert_before(&list, node, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 2);
        
        dlist_destroy(&list);
    }
    
    // Test 5: Verify bidirectional links after insert
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"first");
        dlist_push_back(&list, (void *)"third");
        
        DListNode* node = dlist_tail(&list);
        dlist_insert_before(&list, node, (void *)"second");
        
        DListNode* middle = dlist_prev(node);
        EXPECT_TRUE(middle->next == node);
        EXPECT_TRUE(middle->prev == dlist_head(&list));
        
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_remove
 * Dependencies: dlist_init, dlist_push_back, dlist_head, dlist_size
 */
UTEST_TEST_CASE(dlist_remove){
    // Test 1: Remove NULL node
    {
        DList list;
        dlist_init(&list, NULL);
        
        void* data = NULL;
        int result = dlist_remove(&list, NULL, &data);
        
        EXPECT_EQUAL_INT(result, -1);
        
        dlist_destroy(&list);
    }
    
    // Test 2: Remove only node without retrieving data
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"only");
        
        DListNode* node = dlist_head(&list);
        int result = dlist_remove(&list, node, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 0);
        EXPECT_NULL(list.head);
        EXPECT_NULL(list.tail);
        
        dlist_destroy(&list);
    }
    
    // Test 3: Remove node and retrieve data
    {
        DList list;
        dlist_init(&list, NULL);
        
        const char* test_data = "test";
        dlist_push_back(&list, (void *)test_data);
        
        DListNode* node = dlist_head(&list);
        void* data = NULL;
        int result = dlist_remove(&list, node, &data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_TRUE(data == (void *)test_data);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 0);
        
        dlist_destroy(&list);
    }
    
    // Test 4: Remove head node
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"first");
        dlist_push_back(&list, (void *)"second");
        dlist_push_back(&list, (void *)"third");
        
        DListNode* node = dlist_head(&list);
        int result = dlist_remove(&list, node, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 2);
        EXPECT_EQUAL_STRING((char*)dlist_head(&list)->data, "second");
        
        dlist_destroy(&list);
    }
    
    // Test 5: Remove middle node
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"first");
        dlist_push_back(&list, (void *)"second");
        dlist_push_back(&list, (void *)"third");
        
        DListNode* node = dlist_next(dlist_head(&list));
        int result = dlist_remove(&list, node, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 2);
        
        node = dlist_head(&list);
        EXPECT_EQUAL_STRING((char*)node->data, "first");
        node = dlist_next(node);
        EXPECT_EQUAL_STRING((char*)node->data, "third");
        
        dlist_destroy(&list);
    }
    
    // Test 6: Remove tail node
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"first");
        dlist_push_back(&list, (void *)"second");
        dlist_push_back(&list, (void *)"third");
        
        DListNode* node = dlist_tail(&list);
        int result = dlist_remove(&list, node, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 2);
        EXPECT_EQUAL_STRING((char*)dlist_tail(&list)->data, "second");
        
        dlist_destroy(&list);
    }
    
    // Test 7: Remove with destroy function (data not retrieved)
    {
        int destroy_count = 0;
        DList list;
        dlist_init(&list, destroy_counter);
        
        dlist_push_back(&list, &destroy_count);
        dlist_push_back(&list, &destroy_count);
        dlist_push_back(&list, &destroy_count);
        
        DListNode* node = dlist_next(dlist_head(&list));
        int result = dlist_remove(&list, node, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_INT(destroy_count, 1);
        
        dlist_destroy(&list);
    }
    
    // Test 8: Remove with data retrieval (destroy not called)
    {
        int destroy_count = 0;
        DList list;
        dlist_init(&list, destroy_counter);
        
        dlist_push_back(&list, &destroy_count);
        dlist_push_back(&list, &destroy_count);
        
        DListNode* node = dlist_head(&list);
        void* data = NULL;
        int result = dlist_remove(&list, node, &data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_INT(destroy_count, 0);
        EXPECT_NOT_NULL(data);
        
        dlist_destroy(&list);
    }
    
    // Test 9: Verify bidirectional links after remove
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"first");
        dlist_push_back(&list, (void *)"second");
        dlist_push_back(&list, (void *)"third");
        
        DListNode* node = dlist_next(dlist_head(&list));
        dlist_remove(&list, node, NULL);
        
        DListNode* first = dlist_head(&list);
        DListNode* third = dlist_tail(&list);
        
        EXPECT_TRUE(first->next == third);
        EXPECT_TRUE(third->prev == first);
        
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_clear
 * Dependencies: dlist_init, dlist_push_back, dlist_size
 */
UTEST_TEST_CASE(dlist_clear){
    // Test 1: Clear empty list
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_clear(&list);
        
        EXPECT_EQUAL_UINT64(dlist_size(&list), 0);
        EXPECT_NULL(list.head);
        EXPECT_NULL(list.tail);
        
        dlist_destroy(&list);
    }
    
    // Test 2: Clear single element list
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"test");
        dlist_clear(&list);
        
        EXPECT_EQUAL_UINT64(dlist_size(&list), 0);
        EXPECT_NULL(list.head);
        EXPECT_NULL(list.tail);
        
        dlist_destroy(&list);
    }
    
    // Test 3: Clear multiple element list
    {
        DList list;
        dlist_init(&list, NULL);
        
        for (int i = 0; i < 10; i++) {
            dlist_push_back(&list, (void *)(long)i);
        }
        
        dlist_clear(&list);
        
        EXPECT_EQUAL_UINT64(dlist_size(&list), 0);
        EXPECT_NULL(list.head);
        EXPECT_NULL(list.tail);
        
        dlist_destroy(&list);
    }
    
    // Test 4: Clear with destroy function
    {
        int destroy_count = 0;
        DList list;
        dlist_init(&list, destroy_counter);
        
        dlist_push_back(&list, &destroy_count);
        dlist_push_back(&list, &destroy_count);
        dlist_push_back(&list, &destroy_count);
        
        dlist_clear(&list);
        
        EXPECT_EQUAL_INT(destroy_count, 3);
        EXPECT_EQUAL_UINT64(dlist_size(&list), 0);
        
        dlist_destroy(&list);
    }
    
    // Test 5: Clear and reuse list
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"first");
        dlist_push_back(&list, (void *)"second");
        
        dlist_clear(&list);
        
        EXPECT_EQUAL_UINT64(dlist_size(&list), 0);
        
        dlist_push_back(&list, (void *)"new");
        
        EXPECT_EQUAL_UINT64(dlist_size(&list), 1);
        EXPECT_EQUAL_STRING((char*)dlist_head(&list)->data, "new");
        
        dlist_destroy(&list);
    }
    
    // Test 6: Multiple clears
    {
        DList list;
        dlist_init(&list, NULL);
        
        dlist_push_back(&list, (void *)"test");
        dlist_clear(&list);
        dlist_clear(&list);
        
        EXPECT_EQUAL_UINT64(dlist_size(&list), 0);
        
        dlist_destroy(&list);
    }
}

/**
 * Test Suite: dlist
 */
UTEST_TEST_SUITE(dlist){
    UTEST_RUN_TEST_CASE(dlist_init);
    UTEST_RUN_TEST_CASE(dlist_size);
    UTEST_RUN_TEST_CASE(dlist_empty);
    UTEST_RUN_TEST_CASE(dlist_destroy);
    UTEST_RUN_TEST_CASE(dlist_push_front);
    UTEST_RUN_TEST_CASE(dlist_push_back);
    UTEST_RUN_TEST_CASE(dlist_pop_front);
    UTEST_RUN_TEST_CASE(dlist_pop_back);
    UTEST_RUN_TEST_CASE(dlist_next);
    UTEST_RUN_TEST_CASE(dlist_prev);
    UTEST_RUN_TEST_CASE(dlist_data);
    UTEST_RUN_TEST_CASE(dlist_head);
    UTEST_RUN_TEST_CASE(dlist_tail);
    UTEST_RUN_TEST_CASE(dlist_insert_after);
    UTEST_RUN_TEST_CASE(dlist_insert_before);
    UTEST_RUN_TEST_CASE(dlist_remove);
    UTEST_RUN_TEST_CASE(dlist_clear);
}


