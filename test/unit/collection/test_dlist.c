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

#include <collection/dlist.h>
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
        EXPECT_EQUAL_UINT(dlist_size(&list), 0);
        EXPECT_TRUE(dlist_empty(&list));
        EXPECT_NULL(dlist_head(&list));
        EXPECT_NULL(dlist_tail(&list));
        dlist_destroy(&list);
    }

    // Test 2: Initialize with non-NULL destroy function
    {
        DList list;
        dlist_init(&list, free);
        EXPECT_EQUAL_UINT(dlist_size(&list), 0);
        EXPECT_TRUE(dlist_empty(&list));
        EXPECT_NULL(dlist_head(&list));
        EXPECT_NULL(dlist_tail(&list));
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_size
 * Dependencies: dlist_init
 * Description: Tests the dlist_size function to get the number of elements.
 */
UTEST_TEST_CASE(dlist_size){
    // Test 1: Size of empty list
    {
        DList list;
        dlist_init(&list, NULL);
        EXPECT_EQUAL_UINT(dlist_size(&list), 0);
        dlist_destroy(&list);
    }

    // Test 2: Size after push_front operations
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        dlist_push_front(&list, &a);
        EXPECT_EQUAL_UINT(dlist_size(&list), 1);
        dlist_push_front(&list, &b);
        EXPECT_EQUAL_UINT(dlist_size(&list), 2);
        dlist_push_front(&list, &c);
        EXPECT_EQUAL_UINT(dlist_size(&list), 3);
        dlist_destroy(&list);
    }

    // Test 3: Size after push_back operations
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        dlist_push_back(&list, &a);
        EXPECT_EQUAL_UINT(dlist_size(&list), 1);
        dlist_push_back(&list, &b);
        EXPECT_EQUAL_UINT(dlist_size(&list), 2);
        dlist_push_back(&list, &c);
        EXPECT_EQUAL_UINT(dlist_size(&list), 3);
        dlist_destroy(&list);
    }

    // Test 4: Size after pop_front
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        dlist_pop_front(&list, NULL);
        EXPECT_EQUAL_UINT(dlist_size(&list), 1);
        dlist_destroy(&list);
    }

    // Test 5: Size after pop_back
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        dlist_pop_back(&list, NULL);
        EXPECT_EQUAL_UINT(dlist_size(&list), 1);
        dlist_destroy(&list);
    }

    // Test 6: Size with NULL list
    {
        EXPECT_EQUAL_UINT(dlist_size(NULL), 0);
    }
}

/**
 * Test: dlist_empty
 * Dependencies: dlist_init, dlist_size
 * Description: Tests the dlist_empty function to check if list is empty.
 */
UTEST_TEST_CASE(dlist_empty){
    // Test 1: Empty list
    {
        DList list;
        dlist_init(&list, NULL);
        EXPECT_TRUE(dlist_empty(&list));
        dlist_destroy(&list);
    }

    // Test 2: Non-empty list
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1;
        dlist_push_front(&list, &a);
        EXPECT_FALSE(dlist_empty(&list));
        dlist_destroy(&list);
    }

    // Test 3: Empty after clear
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        EXPECT_FALSE(dlist_empty(&list));
        dlist_clear(&list);
        EXPECT_TRUE(dlist_empty(&list));
        dlist_destroy(&list);
    }

    // Test 4: Empty with NULL list
    {
        EXPECT_FALSE(dlist_empty(NULL));
    }
}

/**
 * Test: dlist_head
 * Dependencies: dlist_init
 * Description: Tests the dlist_head function to get the head node.
 */
UTEST_TEST_CASE(dlist_head){
    // Test 1: Head of empty list
    {
        DList list;
        dlist_init(&list, NULL);
        EXPECT_NULL(dlist_head(&list));
        dlist_destroy(&list);
    }

    // Test 2: Head of non-empty list
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2;
        dlist_push_back(&list, &a);
        EXPECT_NOT_NULL(dlist_head(&list));
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_head(&list)), 1);
        dlist_push_back(&list, &b);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_head(&list)), 1);
        dlist_destroy(&list);
    }

    // Test 3: Head after push_front
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2;
        dlist_push_back(&list, &a);
        dlist_push_front(&list, &b);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_head(&list)), 2);
        dlist_destroy(&list);
    }

    // Test 4: Head with NULL list
    {
        EXPECT_NULL(dlist_head(NULL));
    }
}

/**
 * Test: dlist_tail
 * Dependencies: dlist_init
 * Description: Tests the dlist_tail function to get the tail node.
 */
UTEST_TEST_CASE(dlist_tail){
    // Test 1: Tail of empty list
    {
        DList list;
        dlist_init(&list, NULL);
        EXPECT_NULL(dlist_tail(&list));
        dlist_destroy(&list);
    }

    // Test 2: Tail of non-empty list
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2;
        dlist_push_back(&list, &a);
        EXPECT_NOT_NULL(dlist_tail(&list));
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_tail(&list)), 1);
        dlist_push_back(&list, &b);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_tail(&list)), 2);
        dlist_destroy(&list);
    }

    // Test 3: Tail equals head for single element
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1;
        dlist_push_back(&list, &a);
        EXPECT_EQUAL_UINT(dlist_head(&list), dlist_tail(&list));
        dlist_destroy(&list);
    }

    // Test 4: Tail with NULL list
    {
        EXPECT_NULL(dlist_tail(NULL));
    }
}

/**
 * Test: dlist_next
 * Dependencies: dlist_init, dlist_push_back
 * Description: Tests the dlist_next function to get the next node.
 */
UTEST_TEST_CASE(dlist_next){
    // Test 1: Next of NULL node
    {
        EXPECT_NULL(dlist_next(NULL));
    }

    // Test 2: Next of single node
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1;
        dlist_push_back(&list, &a);
        DListNode* node = dlist_head(&list);
        EXPECT_NULL(dlist_next(node));
        dlist_destroy(&list);
    }

    // Test 3: Next of multiple nodes
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        dlist_push_back(&list, &c);
        DListNode* node = dlist_head(&list);
        EXPECT_NOT_NULL(dlist_next(node));
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_next(node)), 2);
        node = dlist_next(node);
        EXPECT_NOT_NULL(dlist_next(node));
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_next(node)), 3);
        node = dlist_next(node);
        EXPECT_NULL(dlist_next(node));
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_prev
 * Dependencies: dlist_init, dlist_push_back
 * Description: Tests the dlist_prev function to get the previous node.
 */
UTEST_TEST_CASE(dlist_prev){
    // Test 1: Prev of NULL node
    {
        EXPECT_NULL(dlist_prev(NULL));
    }

    // Test 2: Prev of single node
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1;
        dlist_push_back(&list, &a);
        DListNode* node = dlist_tail(&list);
        EXPECT_NULL(dlist_prev(node));
        dlist_destroy(&list);
    }

    // Test 3: Prev of multiple nodes
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        dlist_push_back(&list, &c);
        DListNode* node = dlist_tail(&list);
        EXPECT_NOT_NULL(dlist_prev(node));
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_prev(node)), 2);
        node = dlist_prev(node);
        EXPECT_NOT_NULL(dlist_prev(node));
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_prev(node)), 1);
        node = dlist_prev(node);
        EXPECT_NULL(dlist_prev(node));
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_data
 * Dependencies: dlist_init, dlist_push_back
 * Description: Tests the dlist_data function to get node data.
 */
UTEST_TEST_CASE(dlist_data){
    // Test 1: Data of NULL node
    {
        EXPECT_NULL(dlist_data(NULL));
    }

    // Test 2: Data of valid node
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        DListNode* node = dlist_head(&list);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 1);
        node = dlist_next(node);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 2);
        dlist_destroy(&list);
    }

    // Test 3: Data can be NULL
    {
        DList list;
        dlist_init(&list, NULL);
        dlist_push_back(&list, NULL);
        DListNode* node = dlist_head(&list);
        EXPECT_NULL(dlist_data(node));
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_destroy
 * Dependencies: dlist_init
 * Description: Tests destruction of the list and cleanup.
 */
UTEST_TEST_CASE(dlist_destroy){
    // Test 1: Destroy empty list
    {
        DList list;
        dlist_init(&list, NULL);
        dlist_destroy(&list);
        EXPECT_EQUAL_UINT(dlist_size(&list), 0);
        EXPECT_TRUE(dlist_empty(&list));
    }

    // Test 2: Destroy list with elements and destroy function
    {
        DList list;
        dlist_init(&list, destroy_counter);
        int a = 1, b = 2, c = 3;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        dlist_push_back(&list, &c);
        destroy_count = 0;
        dlist_destroy(&list);
        EXPECT_EQUAL_INT(destroy_count, 3);
    }

    // Test 3: Destroy list without destroy function
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        dlist_destroy(&list);
        EXPECT_EQUAL_UINT(dlist_size(&list), 0);
    }
}

/**
 * Test: dlist_push_front
 * Dependencies: dlist_init, dlist_size
 * Description: Tests inserting elements at the beginning of the list.
 */
UTEST_TEST_CASE(dlist_push_front){
    // Test 1: Push to empty list
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1;
        EXPECT_EQUAL_INT(dlist_push_front(&list, &a), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 1);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_head(&list)), 1);
        dlist_destroy(&list);
    }

    // Test 2: Push multiple elements
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        EXPECT_EQUAL_INT(dlist_push_front(&list, &a), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(dlist_push_front(&list, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(dlist_push_front(&list, &c), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 3);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_head(&list)), 3);
        DListNode* node = dlist_head(&list);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 3);
        node = dlist_next(node);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 2);
        node = dlist_next(node);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 1);
        dlist_destroy(&list);
    }

    // Test 3: Push NULL element
    {
        DList list;
        dlist_init(&list, NULL);
        EXPECT_EQUAL_INT(dlist_push_front(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 1);
        EXPECT_NULL(dlist_data(dlist_head(&list)));
        dlist_destroy(&list);
    }

    // Test 4: Head and tail update correctly
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2;
        dlist_push_front(&list, &a);
        EXPECT_EQUAL_UINT(dlist_head(&list), dlist_tail(&list));
        dlist_push_front(&list, &b);
        EXPECT_NOT_EQUAL_UINT(dlist_head(&list), dlist_tail(&list));
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_tail(&list)), 1);
        dlist_destroy(&list);
    }

    // Test 5: Push with NULL list
    {
        EXPECT_EQUAL_INT(dlist_push_front(NULL, NULL), COLLECTION_FAILURE);
    }
}

/**
 * Test: dlist_push_back
 * Dependencies: dlist_init, dlist_size
 * Description: Tests appending elements to the end of the list.
 */
UTEST_TEST_CASE(dlist_push_back){
    // Test 1: Push to empty list
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1;
        EXPECT_EQUAL_INT(dlist_push_back(&list, &a), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 1);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_tail(&list)), 1);
        dlist_destroy(&list);
    }

    // Test 2: Push multiple elements
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        EXPECT_EQUAL_INT(dlist_push_back(&list, &a), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(dlist_push_back(&list, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(dlist_push_back(&list, &c), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 3);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_head(&list)), 1);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_tail(&list)), 3);
        dlist_destroy(&list);
    }

    // Test 3: Push NULL element
    {
        DList list;
        dlist_init(&list, NULL);
        EXPECT_EQUAL_INT(dlist_push_back(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 1);
        EXPECT_NULL(dlist_data(dlist_tail(&list)));
        dlist_destroy(&list);
    }

    // Test 4: Head and tail update correctly
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2;
        dlist_push_back(&list, &a);
        EXPECT_EQUAL_UINT(dlist_head(&list), dlist_tail(&list));
        dlist_push_back(&list, &b);
        EXPECT_NOT_EQUAL_UINT(dlist_head(&list), dlist_tail(&list));
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_head(&list)), 1);
        dlist_destroy(&list);
    }

    // Test 5: Push with NULL list
    {
        EXPECT_EQUAL_INT(dlist_push_back(NULL, NULL), COLLECTION_FAILURE);
    }
}

/**
 * Test: dlist_pop_front
 * Dependencies: dlist_init, dlist_push_back, dlist_size
 * Description: Tests removing the first element from the list.
 */
UTEST_TEST_CASE(dlist_pop_front){
    // Test 1: Pop from empty list
    {
        DList list;
        dlist_init(&list, NULL);
        EXPECT_EQUAL_INT(dlist_pop_front(&list, NULL), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(dlist_size(&list), 0);
        dlist_destroy(&list);
    }

    // Test 2: Pop without retrieving data
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        EXPECT_EQUAL_INT(dlist_pop_front(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 1);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_head(&list)), 2);
        dlist_destroy(&list);
    }

    // Test 3: Pop with retrieving data
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        void* popped = NULL;
        EXPECT_EQUAL_INT(dlist_pop_front(&list, &popped), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)popped, 1);
        EXPECT_EQUAL_UINT(dlist_size(&list), 1);
        dlist_destroy(&list);
    }

    // Test 4: Pop with destroy function
    {
        DList list;
        dlist_init(&list, destroy_counter);
        int a = 1, b = 2;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        destroy_count = 0;
        EXPECT_EQUAL_INT(dlist_pop_front(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(destroy_count, 1);
        EXPECT_EQUAL_UINT(dlist_size(&list), 1);
        dlist_destroy(&list);
    }

    // Test 5: Pop all elements
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        dlist_push_back(&list, &c);
        EXPECT_EQUAL_INT(dlist_pop_front(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(dlist_pop_front(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(dlist_pop_front(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 0);
        EXPECT_TRUE(dlist_empty(&list));
        dlist_destroy(&list);
    }

    // Test 6: Pop single element updates tail
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1;
        dlist_push_back(&list, &a);
        dlist_pop_front(&list, NULL);
        EXPECT_NULL(dlist_head(&list));
        EXPECT_NULL(dlist_tail(&list));
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_pop_back
 * Dependencies: dlist_init, dlist_push_back, dlist_size
 * Description: Tests removing the last element from the list.
 */
UTEST_TEST_CASE(dlist_pop_back){
    // Test 1: Pop from empty list
    {
        DList list;
        dlist_init(&list, NULL);
        EXPECT_EQUAL_INT(dlist_pop_back(&list, NULL), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(dlist_size(&list), 0);
        dlist_destroy(&list);
    }

    // Test 2: Pop without retrieving data
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        EXPECT_EQUAL_INT(dlist_pop_back(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 1);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_tail(&list)), 1);
        dlist_destroy(&list);
    }

    // Test 3: Pop with retrieving data
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        void* popped = NULL;
        EXPECT_EQUAL_INT(dlist_pop_back(&list, &popped), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)popped, 2);
        EXPECT_EQUAL_UINT(dlist_size(&list), 1);
        dlist_destroy(&list);
    }

    // Test 4: Pop with destroy function
    {
        DList list;
        dlist_init(&list, destroy_counter);
        int a = 1, b = 2;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        destroy_count = 0;
        EXPECT_EQUAL_INT(dlist_pop_back(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(destroy_count, 1);
        EXPECT_EQUAL_UINT(dlist_size(&list), 1);
        dlist_destroy(&list);
    }

    // Test 5: Pop all elements
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        dlist_push_back(&list, &c);
        EXPECT_EQUAL_INT(dlist_pop_back(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(dlist_pop_back(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(dlist_pop_back(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 0);
        EXPECT_TRUE(dlist_empty(&list));
        dlist_destroy(&list);
    }

    // Test 6: Pop single element updates head
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1;
        dlist_push_back(&list, &a);
        dlist_pop_back(&list, NULL);
        EXPECT_NULL(dlist_head(&list));
        EXPECT_NULL(dlist_tail(&list));
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_clear
 * Dependencies: dlist_init, dlist_push_back, dlist_size
 * Description: Tests removing all elements from the list.
 */
UTEST_TEST_CASE(dlist_clear){
    // Test 1: Clear empty list
    {
        DList list;
        dlist_init(&list, NULL);
        dlist_clear(&list);
        EXPECT_EQUAL_UINT(dlist_size(&list), 0);
        EXPECT_TRUE(dlist_empty(&list));
        dlist_destroy(&list);
    }

    // Test 2: Clear list with elements
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        dlist_push_back(&list, &c);
        dlist_clear(&list);
        EXPECT_EQUAL_UINT(dlist_size(&list), 0);
        EXPECT_TRUE(dlist_empty(&list));
        EXPECT_NULL(dlist_head(&list));
        EXPECT_NULL(dlist_tail(&list));
        dlist_destroy(&list);
    }

    // Test 3: Clear with destroy function
    {
        DList list;
        dlist_init(&list, destroy_counter);
        int a = 1, b = 2, c = 3;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        dlist_push_back(&list, &c);
        destroy_count = 0;
        dlist_clear(&list);
        EXPECT_EQUAL_INT(destroy_count, 3);
        EXPECT_EQUAL_UINT(dlist_size(&list), 0);
        dlist_destroy(&list);
    }

    // Test 4: Use after clear
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2;
        dlist_push_back(&list, &a);
        dlist_clear(&list);
        dlist_push_back(&list, &b);
        EXPECT_EQUAL_UINT(dlist_size(&list), 1);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_head(&list)), 2);
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_insert_after
 * Dependencies: dlist_init, dlist_push_back, dlist_size
 * Description: Tests inserting elements after a given node.
 */
UTEST_TEST_CASE(dlist_insert_after){
    // Test 1: Insert after head
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &c);
        DListNode* node = dlist_head(&list);
        EXPECT_EQUAL_INT(dlist_insert_after(&list, node, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 3);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_head(&list)), 1);
        node = dlist_next(node);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 2);
        node = dlist_next(node);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 3);
        dlist_destroy(&list);
    }

    // Test 2: Insert after tail
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2;
        dlist_push_back(&list, &a);
        DListNode* node = dlist_tail(&list);
        EXPECT_EQUAL_INT(dlist_insert_after(&list, node, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 2);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_tail(&list)), 2);
        dlist_destroy(&list);
    }

    // Test 3: Insert NULL element
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1;
        dlist_push_back(&list, &a);
        DListNode* node = dlist_head(&list);
        EXPECT_EQUAL_INT(dlist_insert_after(&list, node, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 2);
        EXPECT_NULL(dlist_data(dlist_next(node)));
        dlist_destroy(&list);
    }

    // Test 4: Insert at invalid node
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1;
        EXPECT_EQUAL_INT(dlist_insert_after(&list, NULL, &a), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(dlist_size(&list), 0);
        dlist_destroy(&list);
    }

    // Test 5: Insert with NULL list
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2;
        dlist_push_back(&list, &a);
        DListNode* node = dlist_head(&list);
        EXPECT_EQUAL_INT(dlist_insert_after(NULL, node, &b), COLLECTION_FAILURE);
        dlist_destroy(&list);
    }

    // Test 6: Insert after middle node
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2, c = 3, d = 4;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        dlist_push_back(&list, &d);
        DListNode* node = dlist_head(&list);
        node = dlist_next(node);
        EXPECT_EQUAL_INT(dlist_insert_after(&list, node, &c), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 4);
        node = dlist_head(&list);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 1);
        node = dlist_next(node);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 2);
        node = dlist_next(node);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 3);
        node = dlist_next(node);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 4);
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_insert_before
 * Dependencies: dlist_init, dlist_push_back, dlist_size
 * Description: Tests inserting elements before a given node.
 */
UTEST_TEST_CASE(dlist_insert_before){
    // Test 1: Insert before tail
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &c);
        DListNode* node = dlist_tail(&list);
        EXPECT_EQUAL_INT(dlist_insert_before(&list, node, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 3);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_tail(&list)), 3);
        node = dlist_prev(node);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 2);
        node = dlist_prev(node);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 1);
        dlist_destroy(&list);
    }

    // Test 2: Insert before head
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2;
        dlist_push_back(&list, &a);
        DListNode* node = dlist_head(&list);
        EXPECT_EQUAL_INT(dlist_insert_before(&list, node, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 2);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_head(&list)), 2);
        dlist_destroy(&list);
    }

    // Test 3: Insert NULL element
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1;
        dlist_push_back(&list, &a);
        DListNode* node = dlist_tail(&list);
        EXPECT_EQUAL_INT(dlist_insert_before(&list, node, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 2);
        EXPECT_NULL(dlist_data(dlist_prev(node)));
        dlist_destroy(&list);
    }

    // Test 4: Insert at invalid node
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1;
        EXPECT_EQUAL_INT(dlist_insert_before(&list, NULL, &a), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(dlist_size(&list), 0);
        dlist_destroy(&list);
    }

    // Test 5: Insert with NULL list
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2;
        dlist_push_back(&list, &a);
        DListNode* node = dlist_head(&list);
        EXPECT_EQUAL_INT(dlist_insert_before(NULL, node, &b), COLLECTION_FAILURE);
        dlist_destroy(&list);
    }

    // Test 6: Insert before middle node
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2, c = 3, d = 4;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &c);
        dlist_push_back(&list, &d);
        DListNode* node = dlist_tail(&list);
        node = dlist_prev(node);
        EXPECT_EQUAL_INT(dlist_insert_before(&list, node, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 4);
        node = dlist_head(&list);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 1);
        node = dlist_next(node);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 2);
        node = dlist_next(node);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 3);
        node = dlist_next(node);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 4);
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_remove
 * Dependencies: dlist_init, dlist_push_back, dlist_size
 * Description: Tests removing a specific node from the list.
 */
UTEST_TEST_CASE(dlist_remove){
    // Test 1: Remove head
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        dlist_push_back(&list, &c);
        DListNode* node = dlist_head(&list);
        EXPECT_EQUAL_INT(dlist_remove(&list, node, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 2);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_head(&list)), 2);
        dlist_destroy(&list);
    }

    // Test 2: Remove tail
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        dlist_push_back(&list, &c);
        DListNode* node = dlist_tail(&list);
        EXPECT_EQUAL_INT(dlist_remove(&list, node, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 2);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_tail(&list)), 2);
        dlist_destroy(&list);
    }

    // Test 3: Remove with retrieving data
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        dlist_push_back(&list, &c);
        DListNode* node = dlist_head(&list);
        node = dlist_next(node);
        void* removed = NULL;
        EXPECT_EQUAL_INT(dlist_remove(&list, node, &removed), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)removed, 2);
        EXPECT_EQUAL_UINT(dlist_size(&list), 2);
        dlist_destroy(&list);
    }

    // Test 4: Remove with destroy function
    {
        DList list;
        dlist_init(&list, destroy_counter);
        int a = 1, b = 2, c = 3;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        dlist_push_back(&list, &c);
        DListNode* node = dlist_head(&list);
        destroy_count = 0;
        EXPECT_EQUAL_INT(dlist_remove(&list, node, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(destroy_count, 1);
        EXPECT_EQUAL_UINT(dlist_size(&list), 2);
        dlist_destroy(&list);
    }

    // Test 5: Remove at invalid node
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1;
        dlist_push_back(&list, &a);
        EXPECT_EQUAL_INT(dlist_remove(&list, NULL, NULL), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(dlist_size(&list), 1);
        dlist_destroy(&list);
    }

    // Test 6: Remove with NULL list
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1;
        dlist_push_back(&list, &a);
        DListNode* node = dlist_head(&list);
        EXPECT_EQUAL_INT(dlist_remove(NULL, node, NULL), COLLECTION_FAILURE);
        dlist_destroy(&list);
    }

    // Test 7: Remove middle node
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &b);
        dlist_push_back(&list, &c);
        DListNode* node = dlist_head(&list);
        node = dlist_next(node);
        EXPECT_EQUAL_INT(dlist_remove(&list, node, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 2);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_head(&list)), 1);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_tail(&list)), 3);
        node = dlist_head(&list);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_next(node)), 3);
        dlist_destroy(&list);
    }

    // Test 8: Remove single element
    {
        DList list;
        dlist_init(&list, NULL);
        int a = 1;
        dlist_push_back(&list, &a);
        DListNode* node = dlist_head(&list);
        EXPECT_EQUAL_INT(dlist_remove(&list, node, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 0);
        EXPECT_NULL(dlist_head(&list));
        EXPECT_NULL(dlist_tail(&list));
        dlist_destroy(&list);
    }
}

/**
 * Test: dlist_memory_leak
 * Dependencies: dlist_init, dlist_destroy, dlist_push_back, dlist_pop_front, dlist_pop_back, dlist_insert_after, dlist_insert_before, dlist_remove, dlist_clear
 * Description: Tests for memory leaks by tracking destroy function calls.
 */
UTEST_TEST_CASE(dlist_memory_leak){
    // Test 1: All elements destroyed on dlist_destroy
    {
        DList list;
        dlist_init(&list, destroy_counter);
        int values[10];
        for (int i = 0; i < 10; i++) {
            values[i] = i;
            dlist_push_back(&list, &values[i]);
        }
        destroy_count = 0;
        dlist_destroy(&list);
        EXPECT_EQUAL_INT(destroy_count, 10);
    }

    // Test 2: Elements destroyed on pop_front
    {
        DList list;
        dlist_init(&list, destroy_counter);
        int values[5];
        for (int i = 0; i < 5; i++) {
            values[i] = i;
            dlist_push_back(&list, &values[i]);
        }
        destroy_count = 0;
        for (int i = 0; i < 5; i++) {
            dlist_pop_front(&list, NULL);
        }
        EXPECT_EQUAL_INT(destroy_count, 5);
        dlist_destroy(&list);
        EXPECT_EQUAL_INT(destroy_count, 5);
    }

    // Test 3: Elements destroyed on pop_back
    {
        DList list;
        dlist_init(&list, destroy_counter);
        int values[5];
        for (int i = 0; i < 5; i++) {
            values[i] = i;
            dlist_push_back(&list, &values[i]);
        }
        destroy_count = 0;
        for (int i = 0; i < 5; i++) {
            dlist_pop_back(&list, NULL);
        }
        EXPECT_EQUAL_INT(destroy_count, 5);
        dlist_destroy(&list);
        EXPECT_EQUAL_INT(destroy_count, 5);
    }

    // Test 4: Elements destroyed on remove
    {
        DList list;
        dlist_init(&list, destroy_counter);
        int values[5];
        for (int i = 0; i < 5; i++) {
            values[i] = i;
            dlist_push_back(&list, &values[i]);
        }
        destroy_count = 0;
        DListNode* node = dlist_head(&list);
        dlist_remove(&list, node, NULL);
        EXPECT_EQUAL_INT(destroy_count, 1);
        node = dlist_head(&list);
        dlist_remove(&list, node, NULL);
        EXPECT_EQUAL_INT(destroy_count, 2);
        dlist_destroy(&list);
        EXPECT_EQUAL_INT(destroy_count, 5);
    }

    // Test 5: Elements destroyed on clear
    {
        DList list;
        dlist_init(&list, destroy_counter);
        int values[10];
        for (int i = 0; i < 10; i++) {
            values[i] = i;
            dlist_push_back(&list, &values[i]);
        }
        destroy_count = 0;
        dlist_clear(&list);
        EXPECT_EQUAL_INT(destroy_count, 10);
        dlist_destroy(&list);
        EXPECT_EQUAL_INT(destroy_count, 10);
    }
}

/**
 * Test: dlist_integration
 * Dependencies: All dlist functions
 * Description: Integration test combining multiple operations.
 */
UTEST_TEST_CASE(dlist_integration){
    // Test 1: Complex sequence of operations
    {
        DList list;
        dlist_init(&list, NULL);
        
        int values[10];
        for (int i = 0; i < 10; i++) {
            values[i] = i;
            EXPECT_EQUAL_INT(dlist_push_back(&list, &values[i]), COLLECTION_SUCCESS);
        }
        EXPECT_EQUAL_UINT(dlist_size(&list), 10);
        
        void* popped = NULL;
        EXPECT_EQUAL_INT(dlist_pop_front(&list, &popped), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)popped, 0);
        EXPECT_EQUAL_UINT(dlist_size(&list), 9);
        
        int new_val = 99;
        DListNode* node = dlist_head(&list);
        EXPECT_EQUAL_INT(dlist_insert_after(&list, node, &new_val), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(dlist_size(&list), 10);
        
        void* removed = NULL;
        EXPECT_EQUAL_INT(dlist_remove(&list, node, &removed), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)removed, 1);
        EXPECT_EQUAL_UINT(dlist_size(&list), 9);
        
        dlist_clear(&list);
        EXPECT_EQUAL_UINT(dlist_size(&list), 0);
        EXPECT_TRUE(dlist_empty(&list));
        
        dlist_destroy(&list);
    }

    // Test 2: Push front and back combination
    {
        DList list;
        dlist_init(&list, NULL);
        
        int a = 1, b = 2, c = 3, d = 4;
        dlist_push_front(&list, &a);
        dlist_push_back(&list, &b);
        dlist_push_front(&list, &c);
        dlist_push_back(&list, &d);
        
        EXPECT_EQUAL_UINT(dlist_size(&list), 4);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_head(&list)), 3);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_tail(&list)), 4);
        
        dlist_destroy(&list);
    }

    // Test 3: Forward iteration through list
    {
        DList list;
        dlist_init(&list, NULL);
        
        int values[5] = {1, 2, 3, 4, 5};
        for (int i = 0; i < 5; i++) {
            dlist_push_back(&list, &values[i]);
        }
        
        int count = 0;
        for (DListNode* node = dlist_head(&list); node != NULL; node = dlist_next(node)) {
            EXPECT_EQUAL_UINT(*(int*)dlist_data(node), values[count]);
            count++;
        }
        EXPECT_EQUAL_UINT(count, 5);
        
        dlist_destroy(&list);
    }

    // Test 4: Backward iteration through list
    {
        DList list;
        dlist_init(&list, NULL);
        
        int values[5] = {1, 2, 3, 4, 5};
        for (int i = 0; i < 5; i++) {
            dlist_push_back(&list, &values[i]);
        }
        
        int count = 4;
        for (DListNode* node = dlist_tail(&list); node != NULL; node = dlist_prev(node)) {
            EXPECT_EQUAL_UINT(*(int*)dlist_data(node), values[count]);
            count--;
        }
        EXPECT_EQUAL_UINT(count, -1);
        
        dlist_destroy(&list);
    }

    // Test 5: Insert before and after operations
    {
        DList list;
        dlist_init(&list, NULL);
        
        int a = 1, b = 2, c = 3, d = 4, e = 5;
        dlist_push_back(&list, &a);
        dlist_push_back(&list, &c);
        
        DListNode* node = dlist_head(&list);
        dlist_insert_after(&list, node, &b);
        node = dlist_tail(&list);
        dlist_insert_before(&list, node, &d);
        
        EXPECT_EQUAL_UINT(dlist_size(&list), 4);
        node = dlist_head(&list);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 1);
        node = dlist_next(node);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 2);
        node = dlist_next(node);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 4);
        node = dlist_next(node);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(node), 3);
        
        dlist_destroy(&list);
    }

    // Test 6: Edge cases combination
    {
        DList list;
        dlist_init(&list, NULL);
        
        EXPECT_TRUE(dlist_empty(&list));
        EXPECT_NULL(dlist_head(&list));
        EXPECT_NULL(dlist_tail(&list));
        
        int a = 1;
        dlist_push_back(&list, &a);
        EXPECT_FALSE(dlist_empty(&list));
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_head(&list)), 1);
        EXPECT_EQUAL_UINT(*(int*)dlist_data(dlist_tail(&list)), 1);
        EXPECT_EQUAL_UINT(dlist_head(&list), dlist_tail(&list));
        
        dlist_pop_front(&list, NULL);
        EXPECT_TRUE(dlist_empty(&list));
        EXPECT_NULL(dlist_head(&list));
        EXPECT_NULL(dlist_tail(&list));
        
        dlist_destroy(&list);
    }
}

/**
 * Test suite: dlist
 * Description: Test suite for doubly linked list data structure
 */
UTEST_TEST_SUITE(dlist){
    UTEST_RUN_TEST_CASE(dlist_init);
    UTEST_RUN_TEST_CASE(dlist_size);
    UTEST_RUN_TEST_CASE(dlist_empty);
    UTEST_RUN_TEST_CASE(dlist_head);
    UTEST_RUN_TEST_CASE(dlist_tail);
    UTEST_RUN_TEST_CASE(dlist_next);
    UTEST_RUN_TEST_CASE(dlist_prev);
    UTEST_RUN_TEST_CASE(dlist_data);
    UTEST_RUN_TEST_CASE(dlist_destroy);
    UTEST_RUN_TEST_CASE(dlist_push_front);
    UTEST_RUN_TEST_CASE(dlist_push_back);
    UTEST_RUN_TEST_CASE(dlist_pop_front);
    UTEST_RUN_TEST_CASE(dlist_pop_back);
    UTEST_RUN_TEST_CASE(dlist_clear);
    UTEST_RUN_TEST_CASE(dlist_insert_after);
    UTEST_RUN_TEST_CASE(dlist_insert_before);
    UTEST_RUN_TEST_CASE(dlist_remove);
    UTEST_RUN_TEST_CASE(dlist_memory_leak);
    UTEST_RUN_TEST_CASE(dlist_integration);
}

