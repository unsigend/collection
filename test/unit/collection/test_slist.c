/* collection - A generic data structure and algorithms library for modern C
 * Copyright (C) 2025 Yixiang Qiu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <collection/slist.h>
#include <collection/common.h>
#include <utest.h>
#include <stdlib.h>
#include <string.h>

/* Global destroy counter for testing */
static int destroy_count = 0;

static void destroy_counter(void* data) {
    if (data != NULL) {
        destroy_count++;
    }
}

/* Global destroy counter with context for testing */
static int destroy_context_count = 0;
static void* test_context = NULL;

static void destroy_counter_context(void* data, void* context) {
    if (data != NULL) {
        destroy_context_count++;
    }
    test_context = context;
}

/**
 * Test: slist_init
 * Dependencies: None
 * Description: Tests basic initialization of the singly linked list structure.
 */
UTEST_TEST_CASE(slist_init){
    // Test 1: Initialize with NULL destroy function
    {
        SList list;
        slist_init(&list, NULL);
        EXPECT_EQUAL_UINT(slist_size(&list), 0);
        EXPECT_TRUE(slist_empty(&list));
        EXPECT_NULL(slist_head(&list));
        EXPECT_NULL(slist_tail(&list));
        slist_destroy(&list);
    }

    // Test 2: Initialize with non-NULL destroy function
    {
        SList list;
        slist_init(&list, free);
        EXPECT_EQUAL_UINT(slist_size(&list), 0);
        EXPECT_TRUE(slist_empty(&list));
        EXPECT_NULL(slist_head(&list));
        EXPECT_NULL(slist_tail(&list));
        slist_destroy(&list);
    }
}

/**
 * Test: slist_init_context
 * Dependencies: None
 * Description: Tests initialization with context-aware destroy function.
 */
UTEST_TEST_CASE(slist_init_context){
    // Test 1: Initialize with context
    {
        SList list;
        int context_value = 42;
        slist_init_context(&list, &context_value, destroy_counter_context);
        EXPECT_EQUAL_UINT(slist_size(&list), 0);
        EXPECT_TRUE(slist_empty(&list));
        EXPECT_NULL(slist_head(&list));
        EXPECT_NULL(slist_tail(&list));
        slist_destroy(&list);
    }
}

/**
 * Test: slist_size
 * Dependencies: slist_init
 * Description: Tests the slist_size function to get the number of elements.
 */
UTEST_TEST_CASE(slist_size){
    // Test 1: Size of empty list
    {
        SList list;
        slist_init(&list, NULL);
        EXPECT_EQUAL_UINT(slist_size(&list), 0);
        slist_destroy(&list);
    }

    // Test 2: Size after push_front operations
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        slist_push_front(&list, &a);
        EXPECT_EQUAL_UINT(slist_size(&list), 1);
        slist_push_front(&list, &b);
        EXPECT_EQUAL_UINT(slist_size(&list), 2);
        slist_push_front(&list, &c);
        EXPECT_EQUAL_UINT(slist_size(&list), 3);
        slist_destroy(&list);
    }

    // Test 3: Size after push_back operations
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        slist_push_back(&list, &a);
        EXPECT_EQUAL_UINT(slist_size(&list), 1);
        slist_push_back(&list, &b);
        EXPECT_EQUAL_UINT(slist_size(&list), 2);
        slist_push_back(&list, &c);
        EXPECT_EQUAL_UINT(slist_size(&list), 3);
        slist_destroy(&list);
    }

    // Test 4: Size after pop_front
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2;
        slist_push_back(&list, &a);
        slist_push_back(&list, &b);
        slist_pop_front(&list, NULL);
        EXPECT_EQUAL_UINT(slist_size(&list), 1);
        slist_destroy(&list);
    }

    // Test 5: Size with NULL list
    {
        EXPECT_EQUAL_UINT(slist_size(NULL), 0);
    }
}

/**
 * Test: slist_empty
 * Dependencies: slist_init, slist_size
 * Description: Tests the slist_empty function to check if list is empty.
 */
UTEST_TEST_CASE(slist_empty){
    // Test 1: Empty list
    {
        SList list;
        slist_init(&list, NULL);
        EXPECT_TRUE(slist_empty(&list));
        slist_destroy(&list);
    }

    // Test 2: Non-empty list
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1;
        slist_push_front(&list, &a);
        EXPECT_FALSE(slist_empty(&list));
        slist_destroy(&list);
    }

    // Test 3: Empty after clear
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2;
        slist_push_back(&list, &a);
        slist_push_back(&list, &b);
        EXPECT_FALSE(slist_empty(&list));
        slist_clear(&list);
        EXPECT_TRUE(slist_empty(&list));
        slist_destroy(&list);
    }

    // Test 4: Empty with NULL list
    {
        EXPECT_FALSE(slist_empty(NULL));
    }
}

/**
 * Test: slist_head
 * Dependencies: slist_init
 * Description: Tests the slist_head function to get the head node.
 */
UTEST_TEST_CASE(slist_head){
    // Test 1: Head of empty list
    {
        SList list;
        slist_init(&list, NULL);
        EXPECT_NULL(slist_head(&list));
        slist_destroy(&list);
    }

    // Test 2: Head of non-empty list
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2;
        slist_push_back(&list, &a);
        EXPECT_NOT_NULL(slist_head(&list));
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_head(&list)), 1);
        slist_push_back(&list, &b);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_head(&list)), 1);
        slist_destroy(&list);
    }

    // Test 3: Head after push_front
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2;
        slist_push_back(&list, &a);
        slist_push_front(&list, &b);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_head(&list)), 2);
        slist_destroy(&list);
    }

    // Test 4: Head with NULL list
    {
        EXPECT_NULL(slist_head(NULL));
    }
}

/**
 * Test: slist_tail
 * Dependencies: slist_init
 * Description: Tests the slist_tail function to get the tail node.
 */
UTEST_TEST_CASE(slist_tail){
    // Test 1: Tail of empty list
    {
        SList list;
        slist_init(&list, NULL);
        EXPECT_NULL(slist_tail(&list));
        slist_destroy(&list);
    }

    // Test 2: Tail of non-empty list
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2;
        slist_push_back(&list, &a);
        EXPECT_NOT_NULL(slist_tail(&list));
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_tail(&list)), 1);
        slist_push_back(&list, &b);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_tail(&list)), 2);
        slist_destroy(&list);
    }

    // Test 3: Tail equals head for single element
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1;
        slist_push_back(&list, &a);
        EXPECT_EQUAL_UINT(slist_head(&list), slist_tail(&list));
        slist_destroy(&list);
    }

    // Test 4: Tail with NULL list
    {
        EXPECT_NULL(slist_tail(NULL));
    }
}

/**
 * Test: slist_front
 * Dependencies: slist_init
 * Description: Tests the slist_front function to get the first node.
 */
UTEST_TEST_CASE(slist_front){
    // Test 1: Front of empty list
    {
        SList list;
        slist_init(&list, NULL);
        EXPECT_NULL(slist_front(&list));
        slist_destroy(&list);
    }

    // Test 2: Front of non-empty list
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2;
        slist_push_back(&list, &a);
        EXPECT_NOT_NULL(slist_front(&list));
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_front(&list)), 1);
        slist_push_back(&list, &b);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_front(&list)), 1);
        slist_destroy(&list);
    }

    // Test 3: Front equals head
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1;
        slist_push_back(&list, &a);
        EXPECT_EQUAL_UINT(slist_front(&list), slist_head(&list));
        slist_destroy(&list);
    }

    // Test 4: Front with NULL list
    {
        EXPECT_NULL(slist_front(NULL));
    }
}

/**
 * Test: slist_back
 * Dependencies: slist_init
 * Description: Tests the slist_back function to get the last node.
 */
UTEST_TEST_CASE(slist_back){
    // Test 1: Back of empty list
    {
        SList list;
        slist_init(&list, NULL);
        EXPECT_NULL(slist_back(&list));
        slist_destroy(&list);
    }

    // Test 2: Back of non-empty list
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        slist_push_back(&list, &a);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_back(&list)), 1);
        slist_push_back(&list, &b);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_back(&list)), 2);
        slist_push_back(&list, &c);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_back(&list)), 3);
        slist_destroy(&list);
    }

    // Test 3: Back equals tail
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1;
        slist_push_back(&list, &a);
        EXPECT_EQUAL_UINT(slist_back(&list), slist_tail(&list));
        slist_destroy(&list);
    }

    // Test 4: Back with NULL list
    {
        EXPECT_NULL(slist_back(NULL));
    }
}

/**
 * Test: slist_next
 * Dependencies: slist_init, slist_push_back
 * Description: Tests the slist_next function to get the next node.
 */
UTEST_TEST_CASE(slist_next){
    // Test 1: Next of NULL node
    {
        EXPECT_NULL(slist_next(NULL));
    }

    // Test 2: Next of single node
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1;
        slist_push_back(&list, &a);
        SListNode* node = slist_front(&list);
        EXPECT_NULL(slist_next(node));
        slist_destroy(&list);
    }

    // Test 3: Next of multiple nodes
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        slist_push_back(&list, &a);
        slist_push_back(&list, &b);
        slist_push_back(&list, &c);
        SListNode* node = slist_front(&list);
        EXPECT_NOT_NULL(slist_next(node));
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_next(node)), 2);
        node = slist_next(node);
        EXPECT_NOT_NULL(slist_next(node));
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_next(node)), 3);
        node = slist_next(node);
        EXPECT_NULL(slist_next(node));
        slist_destroy(&list);
    }
}

/**
 * Test: slist_data
 * Dependencies: slist_init, slist_push_back
 * Description: Tests the slist_data function to get node data.
 */
UTEST_TEST_CASE(slist_data){
    // Test 1: Data of NULL node
    {
        EXPECT_NULL(slist_data(NULL));
    }

    // Test 2: Data of valid node
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2;
        slist_push_back(&list, &a);
        slist_push_back(&list, &b);
        SListNode* node = slist_front(&list);
        EXPECT_EQUAL_UINT(*(int*)slist_data(node), 1);
        node = slist_next(node);
        EXPECT_EQUAL_UINT(*(int*)slist_data(node), 2);
        slist_destroy(&list);
    }

    // Test 3: Data can be NULL
    {
        SList list;
        slist_init(&list, NULL);
        slist_push_back(&list, NULL);
        SListNode* node = slist_front(&list);
        EXPECT_NULL(slist_data(node));
        slist_destroy(&list);
    }
}

/**
 * Test: slist_destroy
 * Dependencies: slist_init
 * Description: Tests destruction of the list and cleanup.
 */
UTEST_TEST_CASE(slist_destroy){
    // Test 1: Destroy empty list
    {
        SList list;
        slist_init(&list, NULL);
        slist_destroy(&list);
        EXPECT_EQUAL_UINT(slist_size(&list), 0);
        EXPECT_TRUE(slist_empty(&list));
    }

    // Test 2: Destroy list with elements and destroy function
    {
        SList list;
        slist_init(&list, destroy_counter);
        int a = 1, b = 2, c = 3;
        slist_push_back(&list, &a);
        slist_push_back(&list, &b);
        slist_push_back(&list, &c);
        destroy_count = 0;
        slist_destroy(&list);
        EXPECT_EQUAL_INT(destroy_count, 3);
    }

    // Test 3: Destroy list without destroy function
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2;
        slist_push_back(&list, &a);
        slist_push_back(&list, &b);
        slist_destroy(&list);
        EXPECT_EQUAL_UINT(slist_size(&list), 0);
    }
}

/**
 * Test: slist_push_front
 * Dependencies: slist_init, slist_size
 * Description: Tests inserting elements at the beginning of the list.
 */
UTEST_TEST_CASE(slist_push_front){
    // Test 1: Push to empty list
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1;
        EXPECT_EQUAL_INT(slist_push_front(&list, &a), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(slist_size(&list), 1);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_front(&list)), 1);
        slist_destroy(&list);
    }

    // Test 2: Push multiple elements
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        EXPECT_EQUAL_INT(slist_push_front(&list, &a), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(slist_push_front(&list, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(slist_push_front(&list, &c), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(slist_size(&list), 3);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_front(&list)), 3);
        SListNode* node = slist_front(&list);
        EXPECT_EQUAL_UINT(*(int*)slist_data(node), 3);
        node = slist_next(node);
        EXPECT_EQUAL_UINT(*(int*)slist_data(node), 2);
        node = slist_next(node);
        EXPECT_EQUAL_UINT(*(int*)slist_data(node), 1);
        slist_destroy(&list);
    }

    // Test 3: Push NULL element
    {
        SList list;
        slist_init(&list, NULL);
        EXPECT_EQUAL_INT(slist_push_front(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(slist_size(&list), 1);
        EXPECT_NULL(slist_data(slist_front(&list)));
        slist_destroy(&list);
    }

    // Test 4: Head and tail update correctly
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2;
        slist_push_front(&list, &a);
        EXPECT_EQUAL_UINT(slist_head(&list), slist_tail(&list));
        slist_push_front(&list, &b);
        EXPECT_NOT_EQUAL_UINT(slist_head(&list), slist_tail(&list));
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_tail(&list)), 1);
        slist_destroy(&list);
    }
}

/**
 * Test: slist_push_back
 * Dependencies: slist_init, slist_size
 * Description: Tests appending elements to the end of the list.
 */
UTEST_TEST_CASE(slist_push_back){
    // Test 1: Push to empty list
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1;
        EXPECT_EQUAL_INT(slist_push_back(&list, &a), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(slist_size(&list), 1);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_back(&list)), 1);
        slist_destroy(&list);
    }

    // Test 2: Push multiple elements
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        EXPECT_EQUAL_INT(slist_push_back(&list, &a), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(slist_push_back(&list, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(slist_push_back(&list, &c), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(slist_size(&list), 3);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_front(&list)), 1);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_back(&list)), 3);
        slist_destroy(&list);
    }

    // Test 3: Push NULL element
    {
        SList list;
        slist_init(&list, NULL);
        EXPECT_EQUAL_INT(slist_push_back(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(slist_size(&list), 1);
        EXPECT_NULL(slist_data(slist_back(&list)));
        slist_destroy(&list);
    }

    // Test 4: Head and tail update correctly
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2;
        slist_push_back(&list, &a);
        EXPECT_EQUAL_UINT(slist_head(&list), slist_tail(&list));
        slist_push_back(&list, &b);
        EXPECT_NOT_EQUAL_UINT(slist_head(&list), slist_tail(&list));
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_head(&list)), 1);
        slist_destroy(&list);
    }
}

/**
 * Test: slist_pop_front
 * Dependencies: slist_init, slist_push_back, slist_size
 * Description: Tests removing the first element from the list.
 */
UTEST_TEST_CASE(slist_pop_front){
    // Test 1: Pop from empty list
    {
        SList list;
        slist_init(&list, NULL);
        EXPECT_EQUAL_INT(slist_pop_front(&list, NULL), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(slist_size(&list), 0);
        slist_destroy(&list);
    }

    // Test 2: Pop without retrieving data
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2;
        slist_push_back(&list, &a);
        slist_push_back(&list, &b);
        EXPECT_EQUAL_INT(slist_pop_front(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(slist_size(&list), 1);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_front(&list)), 2);
        slist_destroy(&list);
    }

    // Test 3: Pop with retrieving data
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2;
        slist_push_back(&list, &a);
        slist_push_back(&list, &b);
        void* popped = NULL;
        EXPECT_EQUAL_INT(slist_pop_front(&list, &popped), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)popped, 1);
        EXPECT_EQUAL_UINT(slist_size(&list), 1);
        slist_destroy(&list);
    }

    // Test 4: Pop with destroy function
    {
        SList list;
        slist_init(&list, destroy_counter);
        int a = 1, b = 2;
        slist_push_back(&list, &a);
        slist_push_back(&list, &b);
        destroy_count = 0;
        EXPECT_EQUAL_INT(slist_pop_front(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(destroy_count, 1);
        EXPECT_EQUAL_UINT(slist_size(&list), 1);
        slist_destroy(&list);
    }

    // Test 5: Pop all elements
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        slist_push_back(&list, &a);
        slist_push_back(&list, &b);
        slist_push_back(&list, &c);
        EXPECT_EQUAL_INT(slist_pop_front(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(slist_pop_front(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(slist_pop_front(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(slist_size(&list), 0);
        EXPECT_TRUE(slist_empty(&list));
        slist_destroy(&list);
    }

    // Test 6: Pop single element updates tail
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1;
        slist_push_back(&list, &a);
        slist_pop_front(&list, NULL);
        EXPECT_NULL(slist_head(&list));
        EXPECT_NULL(slist_tail(&list));
        slist_destroy(&list);
    }
}

/**
 * Test: slist_clear
 * Dependencies: slist_init, slist_push_back, slist_size
 * Description: Tests removing all elements from the list.
 */
UTEST_TEST_CASE(slist_clear){
    // Test 1: Clear empty list
    {
        SList list;
        slist_init(&list, NULL);
        slist_clear(&list);
        EXPECT_EQUAL_UINT(slist_size(&list), 0);
        EXPECT_TRUE(slist_empty(&list));
        slist_destroy(&list);
    }

    // Test 2: Clear list with elements
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        slist_push_back(&list, &a);
        slist_push_back(&list, &b);
        slist_push_back(&list, &c);
        slist_clear(&list);
        EXPECT_EQUAL_UINT(slist_size(&list), 0);
        EXPECT_TRUE(slist_empty(&list));
        EXPECT_NULL(slist_head(&list));
        EXPECT_NULL(slist_tail(&list));
        slist_destroy(&list);
    }

    // Test 3: Clear with destroy function
    {
        SList list;
        slist_init(&list, destroy_counter);
        int a = 1, b = 2, c = 3;
        slist_push_back(&list, &a);
        slist_push_back(&list, &b);
        slist_push_back(&list, &c);
        destroy_count = 0;
        slist_clear(&list);
        EXPECT_EQUAL_INT(destroy_count, 3);
        EXPECT_EQUAL_UINT(slist_size(&list), 0);
        slist_destroy(&list);
    }

    // Test 4: Use after clear
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2;
        slist_push_back(&list, &a);
        slist_clear(&list);
        slist_push_back(&list, &b);
        EXPECT_EQUAL_UINT(slist_size(&list), 1);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_front(&list)), 2);
        slist_destroy(&list);
    }
}

/**
 * Test: slist_insert_after
 * Dependencies: slist_init, slist_push_back, slist_size
 * Description: Tests inserting elements after a given node.
 */
UTEST_TEST_CASE(slist_insert_after){
    // Test 1: Insert after head
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        slist_push_back(&list, &a);
        slist_push_back(&list, &c);
        SListNode* node = slist_front(&list);
        EXPECT_EQUAL_INT(slist_insert_after(&list, node, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(slist_size(&list), 3);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_front(&list)), 1);
        node = slist_next(node);
        EXPECT_EQUAL_UINT(*(int*)slist_data(node), 2);
        node = slist_next(node);
        EXPECT_EQUAL_UINT(*(int*)slist_data(node), 3);
        slist_destroy(&list);
    }

    // Test 2: Insert after tail
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2;
        slist_push_back(&list, &a);
        SListNode* node = slist_tail(&list);
        EXPECT_EQUAL_INT(slist_insert_after(&list, node, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(slist_size(&list), 2);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_tail(&list)), 2);
        slist_destroy(&list);
    }

    // Test 3: Insert NULL element
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1;
        slist_push_back(&list, &a);
        SListNode* node = slist_front(&list);
        EXPECT_EQUAL_INT(slist_insert_after(&list, node, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(slist_size(&list), 2);
        EXPECT_NULL(slist_data(slist_next(node)));
        slist_destroy(&list);
    }

    // Test 4: Insert at invalid node
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1;
        EXPECT_EQUAL_INT(slist_insert_after(&list, NULL, &a), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(slist_size(&list), 0);
        slist_destroy(&list);
    }

    // Test 5: Insert after middle node
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2, c = 3, d = 4;
        slist_push_back(&list, &a);
        slist_push_back(&list, &b);
        slist_push_back(&list, &d);
        SListNode* node = slist_front(&list);
        node = slist_next(node);
        EXPECT_EQUAL_INT(slist_insert_after(&list, node, &c), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(slist_size(&list), 4);
        node = slist_front(&list);
        EXPECT_EQUAL_UINT(*(int*)slist_data(node), 1);
        node = slist_next(node);
        EXPECT_EQUAL_UINT(*(int*)slist_data(node), 2);
        node = slist_next(node);
        EXPECT_EQUAL_UINT(*(int*)slist_data(node), 3);
        node = slist_next(node);
        EXPECT_EQUAL_UINT(*(int*)slist_data(node), 4);
        slist_destroy(&list);
    }
}

/**
 * Test: slist_remove_after
 * Dependencies: slist_init, slist_push_back, slist_size
 * Description: Tests removing the element after a given node.
 */
UTEST_TEST_CASE(slist_remove_after){
    // Test 1: Remove after head
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        slist_push_back(&list, &a);
        slist_push_back(&list, &b);
        slist_push_back(&list, &c);
        SListNode* node = slist_front(&list);
        EXPECT_EQUAL_INT(slist_remove_after(&list, node, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(slist_size(&list), 2);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_front(&list)), 1);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_next(node)), 3);
        slist_destroy(&list);
    }

    // Test 2: Remove with retrieving data
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        slist_push_back(&list, &a);
        slist_push_back(&list, &b);
        slist_push_back(&list, &c);
        SListNode* node = slist_front(&list);
        void* removed = NULL;
        EXPECT_EQUAL_INT(slist_remove_after(&list, node, &removed), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)removed, 2);
        EXPECT_EQUAL_UINT(slist_size(&list), 2);
        slist_destroy(&list);
    }

    // Test 3: Remove with destroy function
    {
        SList list;
        slist_init(&list, destroy_counter);
        int a = 1, b = 2, c = 3;
        slist_push_back(&list, &a);
        slist_push_back(&list, &b);
        slist_push_back(&list, &c);
        SListNode* node = slist_front(&list);
        destroy_count = 0;
        EXPECT_EQUAL_INT(slist_remove_after(&list, node, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(destroy_count, 1);
        EXPECT_EQUAL_UINT(slist_size(&list), 2);
        slist_destroy(&list);
    }

    // Test 4: Remove at invalid node
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1;
        slist_push_back(&list, &a);
        EXPECT_EQUAL_INT(slist_remove_after(&list, NULL, NULL), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(slist_size(&list), 1);
        slist_destroy(&list);
    }

    // Test 5: Remove when node has no next
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1;
        slist_push_back(&list, &a);
        SListNode* node = slist_front(&list);
        EXPECT_EQUAL_INT(slist_remove_after(&list, node, NULL), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(slist_size(&list), 1);
        slist_destroy(&list);
    }

    // Test 6: Remove updates tail
    {
        SList list;
        slist_init(&list, NULL);
        int a = 1, b = 2;
        slist_push_back(&list, &a);
        slist_push_back(&list, &b);
        SListNode* node = slist_front(&list);
        EXPECT_EQUAL_INT(slist_remove_after(&list, node, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(slist_tail(&list), node);
        slist_destroy(&list);
    }
}

/**
 * Test: slist_memory_leak
 * Dependencies: slist_init, slist_destroy, slist_push_back, slist_pop_front, slist_insert_after, slist_remove_after, slist_clear
 * Description: Tests for memory leaks by tracking destroy function calls.
 */
UTEST_TEST_CASE(slist_memory_leak){
    // Test 1: All elements destroyed on slist_destroy
    {
        SList list;
        slist_init(&list, destroy_counter);
        int values[10];
        for (int i = 0; i < 10; i++) {
            values[i] = i;
            slist_push_back(&list, &values[i]);
        }
        destroy_count = 0;
        slist_destroy(&list);
        EXPECT_EQUAL_INT(destroy_count, 10);
    }

    // Test 2: Elements destroyed on pop_front
    {
        SList list;
        slist_init(&list, destroy_counter);
        int values[5];
        for (int i = 0; i < 5; i++) {
            values[i] = i;
            slist_push_back(&list, &values[i]);
        }
        destroy_count = 0;
        for (int i = 0; i < 5; i++) {
            slist_pop_front(&list, NULL);
        }
        EXPECT_EQUAL_INT(destroy_count, 5);
        slist_destroy(&list);
        EXPECT_EQUAL_INT(destroy_count, 5);
    }

    // Test 3: Elements destroyed on remove_after
    {
        SList list;
        slist_init(&list, destroy_counter);
        int values[5];
        for (int i = 0; i < 5; i++) {
            values[i] = i;
            slist_push_back(&list, &values[i]);
        }
        destroy_count = 0;
        SListNode* node = slist_front(&list);
        slist_remove_after(&list, node, NULL);
        EXPECT_EQUAL_INT(destroy_count, 1);
        slist_remove_after(&list, node, NULL);
        EXPECT_EQUAL_INT(destroy_count, 2);
        slist_destroy(&list);
        EXPECT_EQUAL_INT(destroy_count, 5);
    }

    // Test 4: Elements destroyed on clear
    {
        SList list;
        slist_init(&list, destroy_counter);
        int values[10];
        for (int i = 0; i < 10; i++) {
            values[i] = i;
            slist_push_back(&list, &values[i]);
        }
        destroy_count = 0;
        slist_clear(&list);
        EXPECT_EQUAL_INT(destroy_count, 10);
        slist_destroy(&list);
        EXPECT_EQUAL_INT(destroy_count, 10);
    }
}

/**
 * Test: slist_integration
 * Dependencies: All slist functions
 * Description: Integration test combining multiple operations.
 */
UTEST_TEST_CASE(slist_integration){
    // Test 1: Complex sequence of operations
    {
        SList list;
        slist_init(&list, NULL);
        
        int values[10];
        for (int i = 0; i < 10; i++) {
            values[i] = i;
            EXPECT_EQUAL_INT(slist_push_back(&list, &values[i]), COLLECTION_SUCCESS);
        }
        EXPECT_EQUAL_UINT(slist_size(&list), 10);
        
        void* popped = NULL;
        EXPECT_EQUAL_INT(slist_pop_front(&list, &popped), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)popped, 0);
        EXPECT_EQUAL_UINT(slist_size(&list), 9);
        
        int new_val = 99;
        SListNode* node = slist_front(&list);
        EXPECT_EQUAL_INT(slist_insert_after(&list, node, &new_val), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(slist_size(&list), 10);
        
        void* removed = NULL;
        EXPECT_EQUAL_INT(slist_remove_after(&list, node, &removed), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)removed, 99);
        EXPECT_EQUAL_UINT(slist_size(&list), 9);
        
        slist_clear(&list);
        EXPECT_EQUAL_UINT(slist_size(&list), 0);
        EXPECT_TRUE(slist_empty(&list));
        
        slist_destroy(&list);
    }

    // Test 2: Push front and back combination
    {
        SList list;
        slist_init(&list, NULL);
        
        int a = 1, b = 2, c = 3, d = 4;
        slist_push_front(&list, &a);
        slist_push_back(&list, &b);
        slist_push_front(&list, &c);
        slist_push_back(&list, &d);
        
        EXPECT_EQUAL_UINT(slist_size(&list), 4);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_front(&list)), 3);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_back(&list)), 4);
        
        slist_destroy(&list);
    }

    // Test 3: Iteration through list
    {
        SList list;
        slist_init(&list, NULL);
        
        int values[5] = {1, 2, 3, 4, 5};
        for (int i = 0; i < 5; i++) {
            slist_push_back(&list, &values[i]);
        }
        
        int count = 0;
        for (SListNode* node = slist_front(&list); node != NULL; node = slist_next(node)) {
            EXPECT_EQUAL_UINT(*(int*)slist_data(node), values[count]);
            count++;
        }
        EXPECT_EQUAL_UINT(count, 5);
        
        slist_destroy(&list);
    }

    // Test 4: Edge cases combination
    {
        SList list;
        slist_init(&list, NULL);
        
        EXPECT_TRUE(slist_empty(&list));
        EXPECT_NULL(slist_front(&list));
        EXPECT_NULL(slist_back(&list));
        
        int a = 1;
        slist_push_back(&list, &a);
        EXPECT_FALSE(slist_empty(&list));
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_front(&list)), 1);
        EXPECT_EQUAL_UINT(*(int*)slist_data(slist_back(&list)), 1);
        EXPECT_EQUAL_UINT(slist_head(&list), slist_tail(&list));
        
        slist_pop_front(&list, NULL);
        EXPECT_TRUE(slist_empty(&list));
        EXPECT_NULL(slist_front(&list));
        EXPECT_NULL(slist_back(&list));
        
        slist_destroy(&list);
    }
}

/**
 * Test suite: slist
 * Description: Test suite for singly linked list data structure
 */
UTEST_TEST_SUITE(slist){
    UTEST_RUN_TEST_CASE(slist_init);
    UTEST_RUN_TEST_CASE(slist_init_context);
    UTEST_RUN_TEST_CASE(slist_size);
    UTEST_RUN_TEST_CASE(slist_empty);
    UTEST_RUN_TEST_CASE(slist_head);
    UTEST_RUN_TEST_CASE(slist_tail);
    UTEST_RUN_TEST_CASE(slist_front);
    UTEST_RUN_TEST_CASE(slist_back);
    UTEST_RUN_TEST_CASE(slist_next);
    UTEST_RUN_TEST_CASE(slist_data);
    UTEST_RUN_TEST_CASE(slist_destroy);
    UTEST_RUN_TEST_CASE(slist_push_front);
    UTEST_RUN_TEST_CASE(slist_push_back);
    UTEST_RUN_TEST_CASE(slist_pop_front);
    UTEST_RUN_TEST_CASE(slist_clear);
    UTEST_RUN_TEST_CASE(slist_insert_after);
    UTEST_RUN_TEST_CASE(slist_remove_after);
    UTEST_RUN_TEST_CASE(slist_memory_leak);
    UTEST_RUN_TEST_CASE(slist_integration);
}

