#include <utest.h>
#include <collection/slist.h>
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
 * Test: slist_init
 * Dependencies: None
 * Description: Tests basic initialization of the slist structure.
 */
UTEST_TEST_CASE(slist_init){
    // Test 1: Initialize with NULL destroy function
    {
        SList list;
        slist_init(&list, NULL);
        
        EXPECT_EQUAL_UINT64(list.size, 0);
        EXPECT_NULL(list.head);
        EXPECT_NULL(list.tail);
        EXPECT_NULL(list.destroy);
    }
    
    // Test 2: Initialize with non-NULL destroy function
    {
        SList list;
        slist_init(&list, destroy_noop);
        
        EXPECT_EQUAL_UINT64(list.size, 0);
        EXPECT_NULL(list.head);
        EXPECT_NULL(list.tail);
        EXPECT_NOT_NULL(list.destroy);
    }
    
    // Test 3: Multiple initializations
    {
        SList list1, list2;
        slist_init(&list1, NULL);
        slist_init(&list2, destroy_noop);
        
        EXPECT_NULL(list1.destroy);
        EXPECT_NOT_NULL(list2.destroy);
    }
}

/**
 * Test: slist_size
 * Dependencies: slist_init
 */
UTEST_TEST_CASE(slist_size){
    // Test 1: Size of newly initialized list
    {
        SList list;
        slist_init(&list, NULL);
        
        EXPECT_EQUAL_UINT64(slist_size(&list), 0);
    }
    
    // Test 2: Size consistency
    {
        SList list;
        slist_init(&list, NULL);
        
        EXPECT_EQUAL_UINT64(slist_size(&list), list.size);
    }
}

/**
 * Test: slist_empty
 * Dependencies: slist_init
 */
UTEST_TEST_CASE(slist_empty){
    // Test 1: Empty after initialization
    {
        SList list;
        slist_init(&list, NULL);
        
        EXPECT_TRUE(slist_empty(&list));
        
        slist_destroy(&list);
    }
    
    // Test 2: Consistency with size
    {
        SList list;
        slist_init(&list, NULL);
        
        EXPECT_TRUE(slist_empty(&list) == (slist_size(&list) == 0));
        
        slist_destroy(&list);
    }
}

/**
 * Test: slist_destroy
 * Dependencies: slist_init
 */
UTEST_TEST_CASE(slist_destroy){
    // Test 1: Destroy empty list without destroy function
    {
        SList list;
        slist_init(&list, NULL);
        slist_destroy(&list);
        
        EXPECT_EQUAL_UINT64(list.size, 0);
        EXPECT_NULL(list.head);
        EXPECT_NULL(list.tail);
    }
    
    // Test 2: Destroy empty list with destroy function
    {
        SList list;
        slist_init(&list, destroy_noop);
        slist_destroy(&list);
        
        EXPECT_EQUAL_UINT64(list.size, 0);
        EXPECT_NULL(list.head);
        EXPECT_NULL(list.tail);
    }
    
    // Test 3: Multiple destroy calls
    {
        SList list;
        slist_init(&list, NULL);
        slist_destroy(&list);
        slist_destroy(&list);
        
        EXPECT_EQUAL_UINT64(list.size, 0);
    }
}

/**
 * Test: slist_push_front
 * Dependencies: slist_init, slist_size
 */
UTEST_TEST_CASE(slist_push_front){
    // Test 1: Push to empty list
    {
        SList list;
        slist_init(&list, NULL);
        
        const char *data = "first";
        slist_push_front(&list, (void *)data);
        
        EXPECT_EQUAL_UINT64(slist_size(&list), 1);
        EXPECT_NOT_NULL(list.head);
        EXPECT_TRUE(list.head == list.tail);
        
        slist_destroy(&list);
    }
    
    // Test 2: Push multiple elements
    {
        SList list;
        slist_init(&list, NULL);
        
        for (int i = 0; i < 10; i++) {
            slist_push_front(&list, (void *)(long)i);
        }
        
        EXPECT_EQUAL_UINT64(slist_size(&list), 10);
        
        slist_destroy(&list);
    }
    
    // Test 3: Push NULL pointer
    {
        SList list;
        slist_init(&list, NULL);
        
        slist_push_front(&list, NULL);
        
        EXPECT_EQUAL_UINT64(slist_size(&list), 1);
        EXPECT_NOT_NULL(list.head);
        
        slist_destroy(&list);
    }
    
    // Test 4: Order verification
    {
        SList list;
        slist_init(&list, NULL);
        
        slist_push_front(&list, (void *)"third");
        slist_push_front(&list, (void *)"second");
        slist_push_front(&list, (void *)"first");
        
        SListNode* node = list.head;
        EXPECT_EQUAL_STRING((char*)node->data, "first");
        
        slist_destroy(&list);
    }
}

/**
 * Test: slist_push_back
 * Dependencies: slist_init, slist_size
 */
UTEST_TEST_CASE(slist_push_back){
    // Test 1: Push to empty list
    {
        SList list;
        slist_init(&list, NULL);
        
        const char *data = "first";
        slist_push_back(&list, (void *)data);
        
        EXPECT_EQUAL_UINT64(slist_size(&list), 1);
        EXPECT_NOT_NULL(list.head);
        EXPECT_TRUE(list.head == list.tail);
        
        slist_destroy(&list);
    }
    
    // Test 2: Push multiple elements
    {
        SList list;
        slist_init(&list, NULL);
        
        for (int i = 0; i < 10; i++) {
            slist_push_back(&list, (void *)(long)i);
        }
        
        EXPECT_EQUAL_UINT64(slist_size(&list), 10);
        
        slist_destroy(&list);
    }
    
    // Test 3: Push NULL pointer
    {
        SList list;
        slist_init(&list, NULL);
        
        slist_push_back(&list, NULL);
        
        EXPECT_EQUAL_UINT64(slist_size(&list), 1);
        
        slist_destroy(&list);
    }
    
    // Test 4: Order verification
    {
        SList list;
        slist_init(&list, NULL);
        
        slist_push_back(&list, (void *)"first");
        slist_push_back(&list, (void *)"second");
        slist_push_back(&list, (void *)"third");
        
        SListNode* node = list.head;
        EXPECT_EQUAL_STRING((char*)node->data, "first");
        
        slist_destroy(&list);
    }
}

/**
 * Test: slist_front
 * Dependencies: slist_init, slist_push_front
 */
UTEST_TEST_CASE(slist_front){
    // Test 1: Front of empty list
    {
        SList list;
        slist_init(&list, NULL);
        
        EXPECT_NULL(slist_front(&list));
        
        slist_destroy(&list);
    }
    
    // Test 2: Front of single element list
    {
        SList list;
        slist_init(&list, NULL);
        
        const char *data = "only";
        slist_push_front(&list, (void *)data);
        
        SListNode* front = slist_front(&list);
        EXPECT_NOT_NULL(front);
        EXPECT_TRUE(front->data == (void *)data);
        
        slist_destroy(&list);
    }
    
    // Test 3: Front of multiple element list
    {
        SList list;
        slist_init(&list, NULL);
        
        slist_push_front(&list, (void *)"third");
        slist_push_front(&list, (void *)"second");
        slist_push_front(&list, (void *)"first");
        
        SListNode* front = slist_front(&list);
        EXPECT_EQUAL_STRING((char*)front->data, "first");
        
        slist_destroy(&list);
    }
    
    // Test 4: Front remains same after push_back
    {
        SList list;
        slist_init(&list, NULL);
        
        const char *first = "first";
        slist_push_front(&list, (void *)first);
        
        for (int i = 0; i < 10; i++) {
            slist_push_back(&list, (void *)(long)i);
        }
        
        SListNode* front = slist_front(&list);
        EXPECT_TRUE(front->data == (void *)first);
        
        slist_destroy(&list);
    }
}

/**
 * Test: slist_back
 * Dependencies: slist_init, slist_push_back
 */
UTEST_TEST_CASE(slist_back){
    // Test 1: Back of empty list
    {
        SList list;
        slist_init(&list, NULL);
        
        EXPECT_NULL(slist_back(&list));
        
        slist_destroy(&list);
    }
    
    // Test 2: Back of single element list
    {
        SList list;
        slist_init(&list, NULL);
        
        const char *data = "only";
        slist_push_back(&list, (void *)data);
        
        SListNode* back = slist_back(&list);
        EXPECT_NOT_NULL(back);
        EXPECT_TRUE(back->data == (void *)data);
        
        slist_destroy(&list);
    }
    
    // Test 3: Back changes with push_back
    {
        SList list;
        slist_init(&list, NULL);
        
        for (int i = 0; i < 10; i++) {
            slist_push_back(&list, (void *)(long)i);
            SListNode* back = slist_back(&list);
            EXPECT_TRUE(back->data == (void *)(long)i);
        }
        
        slist_destroy(&list);
    }
    
    // Test 4: Back remains same after push_front
    {
        SList list;
        slist_init(&list, NULL);
        
        const char *last = "last";
        slist_push_back(&list, (void *)last);
        
        for (int i = 0; i < 10; i++) {
            slist_push_front(&list, (void *)(long)i);
        }
        
        SListNode* back = slist_back(&list);
        EXPECT_TRUE(back->data == (void *)last);
        
        slist_destroy(&list);
    }
}

/**
 * Test: slist_pop_front
 * Dependencies: slist_init, slist_push_front, slist_size
 */
UTEST_TEST_CASE(slist_pop_front){
    // Test 1: Pop from empty list
    {
        SList list;
        slist_init(&list, NULL);
        
        void* data = NULL;
        int result = slist_pop_front(&list, &data);
        
        EXPECT_EQUAL_INT(result, -1);
        EXPECT_EQUAL_UINT64(slist_size(&list), 0);
        
        slist_destroy(&list);
    }
    
    // Test 2: Pop single element without retrieving data
    {
        SList list;
        slist_init(&list, NULL);
        
        slist_push_front(&list, (void *)0x1234);
        
        int result = slist_pop_front(&list, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(slist_size(&list), 0);
        
        slist_destroy(&list);
    }
    
    // Test 3: Pop single element and retrieve data
    {
        SList list;
        slist_init(&list, NULL);
        
        const char* test_data = "test";
        slist_push_front(&list, (void *)test_data);
        
        void* data = NULL;
        int result = slist_pop_front(&list, &data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_TRUE(data == (void *)test_data);
        EXPECT_EQUAL_UINT64(slist_size(&list), 0);
        
        slist_destroy(&list);
    }
    
    // Test 4: Pop multiple elements
    {
        SList list;
        slist_init(&list, NULL);
        
        for (int i = 0; i < 10; i++) {
            slist_push_front(&list, (void *)(long)i);
        }
        
        for (int i = 9; i >= 0; i--) {
            void* data = NULL;
            int result = slist_pop_front(&list, &data);
            EXPECT_EQUAL_INT(result, 0);
            EXPECT_TRUE(data == (void *)(long)i);
        }
        
        EXPECT_EQUAL_UINT64(slist_size(&list), 0);
        
        slist_destroy(&list);
    }
    
    // Test 5: Pop with destroy function (data not retrieved)
    {
        int destroy_count = 0;
        SList list;
        slist_init(&list, destroy_counter);
        
        slist_push_front(&list, &destroy_count);
        slist_push_front(&list, &destroy_count);
        slist_push_front(&list, &destroy_count);
        
        slist_pop_front(&list, NULL);
        EXPECT_EQUAL_INT(destroy_count, 1);
        
        slist_pop_front(&list, NULL);
        EXPECT_EQUAL_INT(destroy_count, 2);
        
        slist_pop_front(&list, NULL);
        EXPECT_EQUAL_INT(destroy_count, 3);
        
        slist_destroy(&list);
    }
    
    // Test 6: Pop with data retrieval (destroy not called)
    {
        int destroy_count = 0;
        SList list;
        slist_init(&list, destroy_counter);
        
        slist_push_front(&list, &destroy_count);
        slist_push_front(&list, &destroy_count);
        
        void* data = NULL;
        slist_pop_front(&list, &data);
        
        EXPECT_EQUAL_INT(destroy_count, 0);
        EXPECT_NOT_NULL(data);
        
        slist_destroy(&list);
    }
}

/**
 * Test: slist_next
 * Dependencies: slist_init, slist_push_back, slist_front
 */
UTEST_TEST_CASE(slist_next){
    // Test 1: Next of single element
    {
        SList list;
        slist_init(&list, NULL);
        
        slist_push_back(&list, (void *)"only");
        
        SListNode* node = slist_front(&list);
        EXPECT_NULL(slist_next(node));
        
        slist_destroy(&list);
    }
    
    // Test 2: Iterate through list
    {
        SList list;
        slist_init(&list, NULL);
        
        for (int i = 0; i < 10; i++) {
            slist_push_back(&list, (void *)(long)i);
        }
        
        int count = 0;
        for (SListNode* node = slist_front(&list); node != NULL; node = slist_next(node)) {
            EXPECT_TRUE(node->data == (void *)(long)count);
            count++;
        }
        
        EXPECT_EQUAL_INT(count, 10);
        
        slist_destroy(&list);
    }
}

/**
 * Test: slist_insert_after
 * Dependencies: slist_init, slist_push_back, slist_front, slist_next
 */
UTEST_TEST_CASE(slist_insert_after){
    // Test 1: Insert after NULL node
    {
        SList list;
        slist_init(&list, NULL);
        
        int result = slist_insert_after(&list, NULL, (void *)"data");
        
        EXPECT_EQUAL_INT(result, -1);
        
        slist_destroy(&list);
    }
    
    // Test 2: Insert after only node
    {
        SList list;
        slist_init(&list, NULL);
        
        slist_push_back(&list, (void *)"first");
        
        SListNode* node = slist_front(&list);
        int result = slist_insert_after(&list, node, (void *)"second");
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(slist_size(&list), 2);
        
        SListNode* next = slist_next(node);
        EXPECT_EQUAL_STRING((char*)next->data, "second");
        
        slist_destroy(&list);
    }
    
    // Test 3: Insert in middle
    {
        SList list;
        slist_init(&list, NULL);
        
        slist_push_back(&list, (void *)"first");
        slist_push_back(&list, (void *)"third");
        
        SListNode* node = slist_front(&list);
        int result = slist_insert_after(&list, node, (void *)"second");
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(slist_size(&list), 3);
        
        node = slist_front(&list);
        EXPECT_EQUAL_STRING((char*)node->data, "first");
        node = slist_next(node);
        EXPECT_EQUAL_STRING((char*)node->data, "second");
        node = slist_next(node);
        EXPECT_EQUAL_STRING((char*)node->data, "third");
        
        slist_destroy(&list);
    }
    
    // Test 4: Insert NULL data
    {
        SList list;
        slist_init(&list, NULL);
        
        slist_push_back(&list, (void *)"first");
        
        SListNode* node = slist_front(&list);
        int result = slist_insert_after(&list, node, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(slist_size(&list), 2);
        
        slist_destroy(&list);
    }
}

/**
 * Test: slist_remove_after
 * Dependencies: slist_init, slist_push_back, slist_front, slist_next
 */
UTEST_TEST_CASE(slist_remove_after){
    // Test 1: Remove after NULL node
    {
        SList list;
        slist_init(&list, NULL);
        
        void* data = NULL;
        int result = slist_remove_after(&list, NULL, &data);
        
        EXPECT_EQUAL_INT(result, -1);
        
        slist_destroy(&list);
    }
    
    // Test 2: Remove after only node
    {
        SList list;
        slist_init(&list, NULL);
        
        slist_push_back(&list, (void *)"only");
        
        SListNode* node = slist_front(&list);
        int result = slist_remove_after(&list, node, NULL);
        
        EXPECT_EQUAL_INT(result, -1);
        
        slist_destroy(&list);
    }
    
    // Test 3: Remove after first node without retrieving data
    {
        SList list;
        slist_init(&list, NULL);
        
        slist_push_back(&list, (void *)"first");
        slist_push_back(&list, (void *)"second");
        
        SListNode* node = slist_front(&list);
        int result = slist_remove_after(&list, node, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(slist_size(&list), 1);
        
        slist_destroy(&list);
    }
    
    // Test 4: Remove after first node and retrieve data
    {
        SList list;
        slist_init(&list, NULL);
        
        slist_push_back(&list, (void *)"first");
        slist_push_back(&list, (void *)"second");
        
        SListNode* node = slist_front(&list);
        void* data = NULL;
        int result = slist_remove_after(&list, node, &data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_STRING((char*)data, "second");
        EXPECT_EQUAL_UINT64(slist_size(&list), 1);
        
        slist_destroy(&list);
    }
    
    // Test 5: Remove with destroy function
    {
        int destroy_count = 0;
        SList list;
        slist_init(&list, destroy_counter);
        
        slist_push_back(&list, &destroy_count);
        slist_push_back(&list, &destroy_count);
        slist_push_back(&list, &destroy_count);
        
        SListNode* node = slist_front(&list);
        int result = slist_remove_after(&list, node, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_INT(destroy_count, 1);
        
        slist_destroy(&list);
    }
    
    // Test 6: Remove with data retrieval (destroy not called)
    {
        int destroy_count = 0;
        SList list;
        slist_init(&list, destroy_counter);
        
        slist_push_back(&list, &destroy_count);
        slist_push_back(&list, &destroy_count);
        
        SListNode* node = slist_front(&list);
        void* data = NULL;
        int result = slist_remove_after(&list, node, &data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_INT(destroy_count, 0);
        EXPECT_NOT_NULL(data);
        
        slist_destroy(&list);
    }
}

/**
 * Test: slist_data
 * Dependencies: slist_init, slist_push_front
 */
UTEST_TEST_CASE(slist_data){
    // Test 1: Data of single node
    {
        SList list;
        slist_init(&list, NULL);
        
        const char* test_data = "test";
        slist_push_front(&list, (void *)test_data);
        
        SListNode* node = slist_front(&list);
        EXPECT_TRUE(slist_data(node) == (void *)test_data);
        
        slist_destroy(&list);
    }
    
    // Test 2: Data consistency with direct access
    {
        SList list;
        slist_init(&list, NULL);
        
        slist_push_back(&list, (void *)"data");
        
        SListNode* node = slist_front(&list);
        EXPECT_TRUE(slist_data(node) == node->data);
        
        slist_destroy(&list);
    }
}

/**
 * Test: slist_head
 * Dependencies: slist_init, slist_push_front
 */
UTEST_TEST_CASE(slist_head){
    // Test 1: Head of empty list
    {
        SList list;
        slist_init(&list, NULL);
        
        EXPECT_NULL(slist_head(&list));
        
        slist_destroy(&list);
    }
    
    // Test 2: Head consistency with front
    {
        SList list;
        slist_init(&list, NULL);
        
        slist_push_front(&list, (void *)"test");
        
        EXPECT_TRUE(slist_head(&list) == slist_front(&list));
        
        slist_destroy(&list);
    }
    
    // Test 3: Head of multiple element list
    {
        SList list;
        slist_init(&list, NULL);
        
        slist_push_back(&list, (void *)"first");
        slist_push_back(&list, (void *)"second");
        
        SListNode* head = slist_head(&list);
        EXPECT_EQUAL_STRING((char*)head->data, "first");
        
        slist_destroy(&list);
    }
}

/**
 * Test: slist_tail
 * Dependencies: slist_init, slist_push_back
 */
UTEST_TEST_CASE(slist_tail){
    // Test 1: Tail of empty list
    {
        SList list;
        slist_init(&list, NULL);
        
        EXPECT_NULL(slist_tail(&list));
        
        slist_destroy(&list);
    }
    
    // Test 2: Tail consistency with back
    {
        SList list;
        slist_init(&list, NULL);
        
        slist_push_back(&list, (void *)"test");
        
        EXPECT_TRUE(slist_tail(&list) == slist_back(&list));
        
        slist_destroy(&list);
    }
    
    // Test 3: Tail of multiple element list
    {
        SList list;
        slist_init(&list, NULL);
        
        slist_push_back(&list, (void *)"first");
        slist_push_back(&list, (void *)"second");
        
        SListNode* tail = slist_tail(&list);
        EXPECT_EQUAL_STRING((char*)tail->data, "second");
        
        slist_destroy(&list);
    }
}

/**
 * Test Suite: slist
 */
UTEST_TEST_SUITE(slist){
    UTEST_RUN_TEST_CASE(slist_init);
    UTEST_RUN_TEST_CASE(slist_size);
    UTEST_RUN_TEST_CASE(slist_empty);
    UTEST_RUN_TEST_CASE(slist_destroy);
    UTEST_RUN_TEST_CASE(slist_push_front);
    UTEST_RUN_TEST_CASE(slist_push_back);
    UTEST_RUN_TEST_CASE(slist_front);
    UTEST_RUN_TEST_CASE(slist_back);
    UTEST_RUN_TEST_CASE(slist_pop_front);
    UTEST_RUN_TEST_CASE(slist_next);
    UTEST_RUN_TEST_CASE(slist_data);
    UTEST_RUN_TEST_CASE(slist_head);
    UTEST_RUN_TEST_CASE(slist_tail);
    UTEST_RUN_TEST_CASE(slist_insert_after);
    UTEST_RUN_TEST_CASE(slist_remove_after);
}

