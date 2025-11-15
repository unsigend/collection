#include <utest.h>
#include <collection/clist.h>
#include <stdlib.h>
#include <string.h>

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
 * Test: clist_init
 * Dependencies: None
 * Description: Tests basic initialization of the clist structure.
 */
UTEST_TEST_CASE(clist_init){
    // Test 1: Initialize with NULL destroy function
    {
        CList list;
        clist_init(&list, NULL);
        
        EXPECT_EQUAL_UINT64(clist_size(&list), 0);
        EXPECT_TRUE(clist_empty(&list));
        EXPECT_NULL(clist_head(&list));
    }
    
    // Test 2: Initialize with non-NULL destroy function
    {
        CList list;
        clist_init(&list, destroy_noop);
        
        EXPECT_EQUAL_UINT64(clist_size(&list), 0);
        EXPECT_TRUE(clist_empty(&list));
    }
}

/**
 * Test: clist_size
 * Dependencies: clist_init
 */
UTEST_TEST_CASE(clist_size){
    // Test 1: Size of newly initialized list
    {
        CList list;
        clist_init(&list, NULL);
        
        EXPECT_EQUAL_UINT64(clist_size(&list), 0);
    }
    
    // Test 2: Size after adding elements
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        EXPECT_EQUAL_UINT64(clist_size(&list), 1);
        
        clist_push_back(&list, (void*)2);
        EXPECT_EQUAL_UINT64(clist_size(&list), 2);
        
        clist_push_back(&list, (void*)3);
        EXPECT_EQUAL_UINT64(clist_size(&list), 3);
        
        clist_destroy(&list);
    }
    
    // Test 3: Size after removing elements
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        clist_push_back(&list, (void*)2);
        clist_push_back(&list, (void*)3);
        
        clist_pop_front(&list, NULL);
        EXPECT_EQUAL_UINT64(clist_size(&list), 2);
        
        clist_pop_front(&list, NULL);
        EXPECT_EQUAL_UINT64(clist_size(&list), 1);
        
        clist_pop_front(&list, NULL);
        EXPECT_EQUAL_UINT64(clist_size(&list), 0);
        
        clist_destroy(&list);
    }
}

/**
 * Test: clist_empty
 * Dependencies: clist_init
 */
UTEST_TEST_CASE(clist_empty){
    // Test 1: Empty after initialization
    {
        CList list;
        clist_init(&list, NULL);
        
        EXPECT_TRUE(clist_empty(&list));
        
        clist_destroy(&list);
    }
    
    // Test 2: Consistency with size
    {
        CList list;
        clist_init(&list, NULL);
        
        EXPECT_TRUE(clist_empty(&list) == (clist_size(&list) == 0));
        
        clist_push_back(&list, (void*)1);
        EXPECT_FALSE(clist_empty(&list));
        
        clist_pop_front(&list, NULL);
        EXPECT_TRUE(clist_empty(&list));
        
        clist_destroy(&list);
    }
}

/**
 * Test: clist_destroy
 * Dependencies: clist_init
 */
UTEST_TEST_CASE(clist_destroy){
    // Test 1: Destroy empty list without destroy function
    {
        CList list;
        clist_init(&list, NULL);
        clist_destroy(&list);
        
        EXPECT_EQUAL_UINT64(clist_size(&list), 0);
        EXPECT_NULL(clist_head(&list));
    }
    
    // Test 2: Destroy list with elements and destroy function
    {
        int destroy_count = 0;
        CList list;
        clist_init(&list, destroy_counter);
        
        clist_push_back(&list, &destroy_count);
        clist_push_back(&list, &destroy_count);
        clist_push_back(&list, &destroy_count);
        
        clist_destroy(&list);
        
        EXPECT_EQUAL_INT(destroy_count, 3);
        EXPECT_EQUAL_UINT64(clist_size(&list), 0);
        EXPECT_NULL(clist_head(&list));
    }
    
    // Test 3: Multiple destroy calls
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        clist_destroy(&list);
        clist_destroy(&list);
        
        EXPECT_EQUAL_UINT64(clist_size(&list), 0);
    }
}

/**
 * Test: clist_push_front
 * Dependencies: clist_init, clist_size, clist_head
 */
UTEST_TEST_CASE(clist_push_front){
    // Test 1: Push to empty list
    {
        CList list;
        clist_init(&list, NULL);
        
        int result = clist_push_front(&list, (void*)1);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(clist_size(&list), 1);
        EXPECT_EQUAL_INT(1, (int)(long)clist_data(clist_head(&list)));
        
        clist_destroy(&list);
    }
    
    // Test 2: Push to single element - verify circular
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_front(&list, (void*)1);
        
        CListNode* head = clist_head(&list);
        EXPECT_EQUAL_UINT64(head, clist_next(head));
        EXPECT_EQUAL_UINT64(head, clist_prev(head));
        
        clist_destroy(&list);
    }
    
    // Test 3: Push multiple elements
    {
        CList list;
        clist_init(&list, NULL);
        
        for (int i = 0; i < 10; i++) {
            int result = clist_push_front(&list, (void*)(long)i);
            EXPECT_EQUAL_INT(result, 0);
        }
        
        EXPECT_EQUAL_UINT64(clist_size(&list), 10);
        
        clist_destroy(&list);
    }
    
    // Test 4: Order verification
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_front(&list, (void*)3);
        clist_push_front(&list, (void*)2);
        clist_push_front(&list, (void*)1);
        
        CListNode* node = clist_head(&list);
        EXPECT_EQUAL_INT(1, (int)(long)clist_data(node));
        node = clist_next(node);
        EXPECT_EQUAL_INT(2, (int)(long)clist_data(node));
        node = clist_next(node);
        EXPECT_EQUAL_INT(3, (int)(long)clist_data(node));
        
        clist_destroy(&list);
    }
    
    // Test 5: Verify circular structure
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_front(&list, (void*)1);
        clist_push_front(&list, (void*)2);
        clist_push_front(&list, (void*)3);
        
        CListNode* head = clist_head(&list);
        CListNode* tail = clist_tail(&list);
        
        EXPECT_EQUAL_UINT64(tail, clist_prev(head));
        EXPECT_EQUAL_UINT64(head, clist_next(tail));
        
        clist_destroy(&list);
    }
}

/**
 * Test: clist_push_back
 * Dependencies: clist_init, clist_size, clist_head
 */
UTEST_TEST_CASE(clist_push_back){
    // Test 1: Push to empty list
    {
        CList list;
        clist_init(&list, NULL);
        
        int result = clist_push_back(&list, (void*)1);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(clist_size(&list), 1);
        EXPECT_EQUAL_INT(1, (int)(long)clist_data(clist_head(&list)));
        
        clist_destroy(&list);
    }
    
    // Test 2: Push to single element - verify circular
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        
        CListNode* head = clist_head(&list);
        EXPECT_EQUAL_UINT64(head, clist_next(head));
        EXPECT_EQUAL_UINT64(head, clist_prev(head));
        
        clist_destroy(&list);
    }
    
    // Test 3: Push multiple elements
    {
        CList list;
        clist_init(&list, NULL);
        
        for (int i = 0; i < 10; i++) {
            int result = clist_push_back(&list, (void*)(long)i);
            EXPECT_EQUAL_INT(result, 0);
        }
        
        EXPECT_EQUAL_UINT64(clist_size(&list), 10);
        
        clist_destroy(&list);
    }
    
    // Test 4: Order verification
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        clist_push_back(&list, (void*)2);
        clist_push_back(&list, (void*)3);
        
        CListNode* node = clist_head(&list);
        EXPECT_EQUAL_INT(1, (int)(long)clist_data(node));
        node = clist_next(node);
        EXPECT_EQUAL_INT(2, (int)(long)clist_data(node));
        node = clist_next(node);
        EXPECT_EQUAL_INT(3, (int)(long)clist_data(node));
        
        clist_destroy(&list);
    }
    
    // Test 5: Verify circular structure
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        clist_push_back(&list, (void*)2);
        clist_push_back(&list, (void*)3);
        
        CListNode* head = clist_head(&list);
        CListNode* tail = clist_tail(&list);
        
        EXPECT_EQUAL_UINT64(tail, clist_prev(head));
        EXPECT_EQUAL_UINT64(head, clist_next(tail));
        EXPECT_EQUAL_UINT64(clist_head(&list), clist_next(tail));
        
        clist_destroy(&list);
    }
    
    // Test 6: Single to double element transition
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        CListNode* first = clist_head(&list);
        
        EXPECT_EQUAL_UINT64(first, clist_next(first));
        EXPECT_EQUAL_UINT64(first, clist_prev(first));
        
        clist_push_back(&list, (void*)2);
        CListNode* second = clist_next(first);
        
        EXPECT_EQUAL_UINT64(second, clist_next(first));
        EXPECT_EQUAL_UINT64(first, clist_prev(second));
        EXPECT_EQUAL_UINT64(first, clist_next(second));
        EXPECT_EQUAL_UINT64(second, clist_prev(first));
        
        clist_destroy(&list);
    }
    
    // Test 7: Three element bidirectional structure
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        clist_push_back(&list, (void*)2);
        clist_push_back(&list, (void*)3);
        
        CListNode* first = clist_head(&list);
        CListNode* second = clist_next(first);
        CListNode* third = clist_tail(&list);
        
        EXPECT_EQUAL_UINT64(second, clist_next(first));
        EXPECT_EQUAL_UINT64(third, clist_next(second));
        EXPECT_EQUAL_UINT64(first, clist_next(third));
        EXPECT_EQUAL_UINT64(third, clist_prev(first));
        EXPECT_EQUAL_UINT64(first, clist_prev(second));
        EXPECT_EQUAL_UINT64(second, clist_prev(third));
        
        clist_destroy(&list);
    }
}

/**
 * Test: clist_pop_front
 * Dependencies: clist_init, clist_push_back, clist_size
 */
UTEST_TEST_CASE(clist_pop_front){
    // Test 1: Pop from empty list
    {
        CList list;
        clist_init(&list, NULL);
        
        void* data = NULL;
        int result = clist_pop_front(&list, &data);
        
        EXPECT_EQUAL_INT(result, -1);
        EXPECT_EQUAL_UINT64(clist_size(&list), 0);
        
        clist_destroy(&list);
    }
    
    // Test 2: Pop single element without retrieving data
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)0x1234);
        
        int result = clist_pop_front(&list, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(clist_size(&list), 0);
        
        clist_destroy(&list);
    }
    
    // Test 3: Pop single element and retrieve data
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)42);
        
        void* data = NULL;
        int result = clist_pop_front(&list, &data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_INT(42, (int)(long)data);
        EXPECT_TRUE(clist_empty(&list));
        
        clist_destroy(&list);
    }
    
    // Test 4: Pop down to one element - verify self-circular
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        clist_push_back(&list, (void*)2);
        clist_push_back(&list, (void*)3);
        
        clist_pop_front(&list, NULL);
        clist_pop_front(&list, NULL);
        
        EXPECT_EQUAL_UINT64(clist_size(&list), 1);
        
        CListNode* only = clist_head(&list);
        EXPECT_EQUAL_UINT64(only, clist_next(only));
        EXPECT_EQUAL_UINT64(only, clist_prev(only));
        
        clist_destroy(&list);
    }
    
    // Test 5: Pop with destroy function
    {
        int destroy_count = 0;
        CList list;
        clist_init(&list, destroy_counter);
        
        clist_push_back(&list, &destroy_count);
        clist_push_back(&list, &destroy_count);
        clist_push_back(&list, &destroy_count);
        
        clist_pop_front(&list, NULL);
        EXPECT_EQUAL_INT(destroy_count, 1);
        
        clist_pop_front(&list, NULL);
        EXPECT_EQUAL_INT(destroy_count, 2);
        
        clist_destroy(&list);
    }
    
    // Test 6: Pop with data retrieval (destroy not called)
    {
        int destroy_count = 0;
        CList list;
        clist_init(&list, destroy_counter);
        
        clist_push_back(&list, &destroy_count);
        clist_push_back(&list, &destroy_count);
        
        void* data = NULL;
        clist_pop_front(&list, &data);
        
        EXPECT_EQUAL_INT(destroy_count, 0);
        EXPECT_NOT_NULL(data);
        
        clist_destroy(&list);
    }
}

/**
 * Test: clist_pop_back
 * Dependencies: clist_init, clist_push_back, clist_size
 */
UTEST_TEST_CASE(clist_pop_back){
    // Test 1: Pop from empty list
    {
        CList list;
        clist_init(&list, NULL);
        
        void* data = NULL;
        int result = clist_pop_back(&list, &data);
        
        EXPECT_EQUAL_INT(result, -1);
        EXPECT_EQUAL_UINT64(clist_size(&list), 0);
        
        clist_destroy(&list);
    }
    
    // Test 2: Pop single element without retrieving data
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)0x1234);
        
        int result = clist_pop_back(&list, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(clist_size(&list), 0);
        
        clist_destroy(&list);
    }
    
    // Test 3: Pop single element and retrieve data
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)42);
        
        void* data = NULL;
        int result = clist_pop_back(&list, &data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_INT(42, (int)(long)data);
        EXPECT_TRUE(clist_empty(&list));
        
        clist_destroy(&list);
    }
    
    // Test 4: Pop from two-element list
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        clist_push_back(&list, (void*)2);
        
        void* data = NULL;
        int result = clist_pop_back(&list, &data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_INT(2, (int)(long)data);
        EXPECT_EQUAL_UINT64(clist_size(&list), 1);
        
        clist_destroy(&list);
    }
    
    // Test 5: Pop down to one element - verify self-circular
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        clist_push_back(&list, (void*)2);
        clist_push_back(&list, (void*)3);
        
        clist_pop_back(&list, NULL);
        clist_pop_back(&list, NULL);
        
        EXPECT_EQUAL_UINT64(clist_size(&list), 1);
        
        CListNode* only = clist_head(&list);
        EXPECT_EQUAL_UINT64(only, clist_next(only));
        EXPECT_EQUAL_UINT64(only, clist_prev(only));
        
        clist_destroy(&list);
    }
    
    // Test 6: Pop with destroy function
    {
        int destroy_count = 0;
        CList list;
        clist_init(&list, destroy_counter);
        
        clist_push_back(&list, &destroy_count);
        clist_push_back(&list, &destroy_count);
        clist_push_back(&list, &destroy_count);
        
        clist_pop_back(&list, NULL);
        EXPECT_EQUAL_INT(destroy_count, 1);
        
        clist_pop_back(&list, NULL);
        EXPECT_EQUAL_INT(destroy_count, 2);
        
        clist_destroy(&list);
    }
}

/**
 * Test: clist_next
 * Dependencies: clist_init, clist_push_back, clist_head
 */
UTEST_TEST_CASE(clist_next){
    // Test 1: Next of single element (circular)
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        
        CListNode* node = clist_head(&list);
        EXPECT_EQUAL_UINT64(node, clist_next(node));
        
        clist_destroy(&list);
    }
    
    // Test 2: Iterate forward through list
    {
        CList list;
        clist_init(&list, NULL);
        
        for (int i = 1; i <= 5; i++) {
            clist_push_back(&list, (void*)(long)i);
        }
        
        CListNode* node = clist_head(&list);
        for (int i = 1; i <= 5; i++) {
            EXPECT_EQUAL_INT(i, (int)(long)clist_data(node));
            node = clist_next(node);
        }
        
        EXPECT_EQUAL_UINT64(clist_head(&list), node);
        
        clist_destroy(&list);
    }
    
    // Test 3: Forward circular traversal multiple laps
    {
        CList list;
        clist_init(&list, NULL);
        
        for (int i = 1; i <= 5; i++) {
            clist_push_back(&list, (void*)(long)i);
        }
        
        CListNode* node = clist_head(&list);
        for (int lap = 0; lap < 3; lap++) {
            for (int i = 1; i <= 5; i++) {
                EXPECT_EQUAL_INT(i, (int)(long)clist_data(node));
                node = clist_next(node);
            }
        }
        
        EXPECT_EQUAL_UINT64(clist_head(&list), node);
        
        clist_destroy(&list);
    }
}

/**
 * Test: clist_prev
 * Dependencies: clist_init, clist_push_back, clist_head, clist_tail
 */
UTEST_TEST_CASE(clist_prev){
    // Test 1: Prev of single element (circular)
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        
        CListNode* node = clist_head(&list);
        EXPECT_EQUAL_UINT64(node, clist_prev(node));
        
        clist_destroy(&list);
    }
    
    // Test 2: Iterate backward through list
    {
        CList list;
        clist_init(&list, NULL);
        
        for (int i = 1; i <= 5; i++) {
            clist_push_back(&list, (void*)(long)i);
        }
        
        CListNode* node = clist_tail(&list);
        for (int i = 5; i >= 1; i--) {
            EXPECT_EQUAL_INT(i, (int)(long)clist_data(node));
            node = clist_prev(node);
        }
        
        EXPECT_EQUAL_UINT64(clist_tail(&list), node);
        
        clist_destroy(&list);
    }
    
    // Test 3: Backward wrapping from head
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        clist_push_back(&list, (void*)2);
        clist_push_back(&list, (void*)3);
        
        CListNode* node = clist_head(&list);
        node = clist_prev(node);
        
        EXPECT_EQUAL_INT(3, (int)(long)clist_data(node));
        
        clist_destroy(&list);
    }
    
    // Test 4: Backward circular traversal multiple laps
    {
        CList list;
        clist_init(&list, NULL);
        
        for (int i = 1; i <= 5; i++) {
            clist_push_back(&list, (void*)(long)i);
        }
        
        CListNode* node = clist_tail(&list);
        for (int lap = 0; lap < 3; lap++) {
            for (int i = 5; i >= 1; i--) {
                EXPECT_EQUAL_INT(i, (int)(long)clist_data(node));
                node = clist_prev(node);
            }
        }
        
        EXPECT_EQUAL_UINT64(clist_tail(&list), node);
        
        clist_destroy(&list);
    }
}

/**
 * Test: clist_data
 * Dependencies: clist_init, clist_push_back, clist_head
 */
UTEST_TEST_CASE(clist_data){
    // Test 1: Data of single node
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)100);
        
        CListNode* node = clist_head(&list);
        EXPECT_EQUAL_INT(100, (int)(long)clist_data(node));
        
        clist_destroy(&list);
    }
    
    // Test 2: Data consistency with direct access
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)200);
        
        CListNode* node = clist_head(&list);
        EXPECT_TRUE(clist_data(node) == node->data);
        
        clist_destroy(&list);
    }
}

/**
 * Test: clist_head
 * Dependencies: clist_init, clist_push_back, clist_push_front
 */
UTEST_TEST_CASE(clist_head){
    // Test 1: Head of empty list
    {
        CList list;
        clist_init(&list, NULL);
        
        EXPECT_NULL(clist_head(&list));
        
        clist_destroy(&list);
    }
    
    // Test 2: Head after push_back
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        
        CListNode* head = clist_head(&list);
        EXPECT_NOT_NULL(head);
        EXPECT_EQUAL_INT(1, (int)(long)clist_data(head));
        
        clist_destroy(&list);
    }
    
    // Test 3: Head after push_front
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        clist_push_front(&list, (void*)2);
        
        CListNode* head = clist_head(&list);
        EXPECT_EQUAL_INT(2, (int)(long)clist_data(head));
        
        clist_destroy(&list);
    }
}

/**
 * Test: clist_tail
 * Dependencies: clist_init, clist_push_back
 */
UTEST_TEST_CASE(clist_tail){
    // Test 1: Tail of empty list
    {
        CList list;
        clist_init(&list, NULL);
        
        EXPECT_NULL(clist_tail(&list));
        
        clist_destroy(&list);
    }
    
    // Test 2: Tail of single element
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        
        CListNode* tail = clist_tail(&list);
        EXPECT_NOT_NULL(tail);
        EXPECT_EQUAL_INT(1, (int)(long)clist_data(tail));
        EXPECT_EQUAL_UINT64(clist_head(&list), tail);
        
        clist_destroy(&list);
    }
    
    // Test 3: Tail of multiple elements
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        clist_push_back(&list, (void*)2);
        clist_push_back(&list, (void*)3);
        
        CListNode* tail = clist_tail(&list);
        EXPECT_EQUAL_INT(3, (int)(long)clist_data(tail));
        EXPECT_EQUAL_UINT64(clist_head(&list), clist_next(tail));
        
        clist_destroy(&list);
    }
}

/**
 * Test: clist_insert_after
 * Dependencies: clist_init, clist_push_back, clist_head, clist_next
 */
UTEST_TEST_CASE(clist_insert_after){
    // Test 1: Insert after NULL node
    {
        CList list;
        clist_init(&list, NULL);
        
        int result = clist_insert_after(&list, NULL, (void*)1);
        
        EXPECT_EQUAL_INT(result, -1);
        
        clist_destroy(&list);
    }
    
    // Test 2: Insert after only node
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        
        CListNode* node = clist_head(&list);
        int result = clist_insert_after(&list, node, (void*)2);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(clist_size(&list), 2);
        
        CListNode* next = clist_next(node);
        EXPECT_EQUAL_INT(2, (int)(long)clist_data(next));
        
        clist_destroy(&list);
    }
    
    // Test 3: Insert in middle
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        clist_push_back(&list, (void*)3);
        
        CListNode* node = clist_head(&list);
        int result = clist_insert_after(&list, node, (void*)2);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(clist_size(&list), 3);
        
        node = clist_head(&list);
        EXPECT_EQUAL_INT(1, (int)(long)clist_data(node));
        node = clist_next(node);
        EXPECT_EQUAL_INT(2, (int)(long)clist_data(node));
        node = clist_next(node);
        EXPECT_EQUAL_INT(3, (int)(long)clist_data(node));
        
        clist_destroy(&list);
    }
    
    // Test 4: Verify bidirectional links after insert
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        clist_push_back(&list, (void*)3);
        
        CListNode* node = clist_head(&list);
        clist_insert_after(&list, node, (void*)2);
        
        CListNode* inserted = clist_next(node);
        EXPECT_EQUAL_UINT64(clist_head(&list), clist_prev(inserted));
        EXPECT_EQUAL_INT(3, (int)(long)clist_data(clist_next(inserted)));
        
        clist_destroy(&list);
    }
}

/**
 * Test: clist_insert_before
 * Dependencies: clist_init, clist_push_back, clist_head, clist_next
 */
UTEST_TEST_CASE(clist_insert_before){
    // Test 1: Insert before NULL node
    {
        CList list;
        clist_init(&list, NULL);
        
        int result = clist_insert_before(&list, NULL, (void*)1);
        
        EXPECT_EQUAL_INT(result, -1);
        
        clist_destroy(&list);
    }
    
    // Test 2: Insert before head
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)2);
        
        CListNode* node = clist_head(&list);
        int result = clist_insert_before(&list, node, (void*)1);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(clist_size(&list), 2);
        
        CListNode* head = clist_head(&list);
        EXPECT_EQUAL_INT(1, (int)(long)clist_data(head));
        
        clist_destroy(&list);
    }
    
    // Test 3: Insert in middle
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        clist_push_back(&list, (void*)3);
        
        CListNode* node = clist_tail(&list);
        int result = clist_insert_before(&list, node, (void*)2);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(clist_size(&list), 3);
        
        node = clist_head(&list);
        EXPECT_EQUAL_INT(1, (int)(long)clist_data(node));
        node = clist_next(node);
        EXPECT_EQUAL_INT(2, (int)(long)clist_data(node));
        node = clist_next(node);
        EXPECT_EQUAL_INT(3, (int)(long)clist_data(node));
        
        clist_destroy(&list);
    }
    
    // Test 4: Verify bidirectional links after insert
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        clist_push_back(&list, (void*)2);
        
        CListNode* node = clist_head(&list);
        clist_insert_before(&list, node, (void*)0);
        
        CListNode* old_head = clist_next(clist_head(&list));
        EXPECT_EQUAL_UINT64(clist_head(&list), clist_prev(old_head));
        
        clist_destroy(&list);
    }
}

/**
 * Test: clist_remove
 * Dependencies: clist_init, clist_push_back, clist_head, clist_size
 */
UTEST_TEST_CASE(clist_remove){
    // Test 1: Remove NULL node
    {
        CList list;
        clist_init(&list, NULL);
        
        void* data = NULL;
        int result = clist_remove(&list, NULL, &data);
        
        EXPECT_EQUAL_INT(result, -1);
        
        clist_destroy(&list);
    }
    
    // Test 2: Remove only node without retrieving data
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        
        CListNode* node = clist_head(&list);
        int result = clist_remove(&list, node, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(clist_size(&list), 0);
        EXPECT_NULL(clist_head(&list));
        
        clist_destroy(&list);
    }
    
    // Test 3: Remove node and retrieve data
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)42);
        
        CListNode* node = clist_head(&list);
        void* data = NULL;
        int result = clist_remove(&list, node, &data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_INT(42, (int)(long)data);
        EXPECT_EQUAL_UINT64(clist_size(&list), 0);
        
        clist_destroy(&list);
    }
    
    // Test 4: Remove head node
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        clist_push_back(&list, (void*)2);
        clist_push_back(&list, (void*)3);
        
        int result = clist_remove(&list, clist_head(&list), NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(clist_size(&list), 2);
        EXPECT_EQUAL_INT(2, (int)(long)clist_data(clist_head(&list)));
        
        clist_destroy(&list);
    }
    
    // Test 5: Remove middle node
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        clist_push_back(&list, (void*)2);
        clist_push_back(&list, (void*)3);
        
        CListNode* node = clist_next(clist_head(&list));
        int result = clist_remove(&list, node, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(clist_size(&list), 2);
        
        node = clist_head(&list);
        EXPECT_EQUAL_INT(1, (int)(long)clist_data(node));
        node = clist_next(node);
        EXPECT_EQUAL_INT(3, (int)(long)clist_data(node));
        
        clist_destroy(&list);
    }
    
    // Test 6: Remove with destroy function
    {
        int destroy_count = 0;
        CList list;
        clist_init(&list, destroy_counter);
        
        clist_push_back(&list, &destroy_count);
        clist_push_back(&list, &destroy_count);
        
        CListNode* node = clist_head(&list);
        int result = clist_remove(&list, node, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_INT(destroy_count, 1);
        
        clist_destroy(&list);
    }
    
    // Test 7: Remove with data retrieval (destroy not called)
    {
        int destroy_count = 0;
        CList list;
        clist_init(&list, destroy_counter);
        
        clist_push_back(&list, &destroy_count);
        clist_push_back(&list, &destroy_count);
        
        CListNode* node = clist_head(&list);
        void* data = NULL;
        int result = clist_remove(&list, node, &data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_INT(destroy_count, 0);
        EXPECT_NOT_NULL(data);
        
        clist_destroy(&list);
    }
}

/**
 * Test: clist_clear
 * Dependencies: clist_init, clist_push_back, clist_size
 */
UTEST_TEST_CASE(clist_clear){
    // Test 1: Clear empty list
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_clear(&list);
        
        EXPECT_EQUAL_UINT64(clist_size(&list), 0);
        EXPECT_NULL(clist_head(&list));
        
        clist_destroy(&list);
    }
    
    // Test 2: Clear single element list
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        clist_clear(&list);
        
        EXPECT_EQUAL_UINT64(clist_size(&list), 0);
        EXPECT_NULL(clist_head(&list));
        
        clist_destroy(&list);
    }
    
    // Test 3: Clear multiple element list
    {
        CList list;
        clist_init(&list, NULL);
        
        for (int i = 0; i < 10; i++) {
            clist_push_back(&list, (void*)(long)i);
        }
        
        clist_clear(&list);
        
        EXPECT_EQUAL_UINT64(clist_size(&list), 0);
        EXPECT_NULL(clist_head(&list));
        
        clist_destroy(&list);
    }
    
    // Test 4: Clear with destroy function
    {
        int destroy_count = 0;
        CList list;
        clist_init(&list, destroy_counter);
        
        clist_push_back(&list, &destroy_count);
        clist_push_back(&list, &destroy_count);
        clist_push_back(&list, &destroy_count);
        
        clist_clear(&list);
        
        EXPECT_EQUAL_INT(destroy_count, 3);
        EXPECT_EQUAL_UINT64(clist_size(&list), 0);
        
        clist_destroy(&list);
    }
    
    // Test 5: Clear and reuse list
    {
        CList list;
        clist_init(&list, NULL);
        
        clist_push_back(&list, (void*)1);
        clist_push_back(&list, (void*)2);
        
        clist_clear(&list);
        
        EXPECT_EQUAL_UINT64(clist_size(&list), 0);
        
        clist_push_back(&list, (void*)10);
        
        EXPECT_EQUAL_UINT64(clist_size(&list), 1);
        EXPECT_EQUAL_INT(10, (int)(long)clist_data(clist_head(&list)));
        
        clist_destroy(&list);
    }
}

/**
 * Test Suite: clist
 */
UTEST_TEST_SUITE(clist){
    UTEST_RUN_TEST_CASE(clist_init);
    UTEST_RUN_TEST_CASE(clist_size);
    UTEST_RUN_TEST_CASE(clist_empty);
    UTEST_RUN_TEST_CASE(clist_destroy);
    UTEST_RUN_TEST_CASE(clist_push_front);
    UTEST_RUN_TEST_CASE(clist_push_back);
    UTEST_RUN_TEST_CASE(clist_pop_front);
    UTEST_RUN_TEST_CASE(clist_pop_back);
    UTEST_RUN_TEST_CASE(clist_next);
    UTEST_RUN_TEST_CASE(clist_prev);
    UTEST_RUN_TEST_CASE(clist_data);
    UTEST_RUN_TEST_CASE(clist_head);
    UTEST_RUN_TEST_CASE(clist_tail);
    UTEST_RUN_TEST_CASE(clist_insert_after);
    UTEST_RUN_TEST_CASE(clist_insert_before);
    UTEST_RUN_TEST_CASE(clist_remove);
    UTEST_RUN_TEST_CASE(clist_clear);
}
