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

#include <collection/clist.h>
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
 * Test: clist_init
 * Dependencies: None
 * Description: Tests basic initialization of the clist structure.
 */
UTEST_TEST_CASE(clist_init){
    // Test 1: Initialize with NULL destroy function
    {
        CList list;
        clist_init(&list, NULL);
        EXPECT_EQUAL_UINT(clist_size(&list), 0);
        EXPECT_TRUE(clist_empty(&list));
        EXPECT_NULL(clist_head(&list));
        EXPECT_NULL(clist_tail(&list));
        clist_destroy(&list);
    }

    // Test 2: Initialize with non-NULL destroy function
    {
        CList list;
        clist_init(&list, free);
        EXPECT_EQUAL_UINT(clist_size(&list), 0);
        EXPECT_TRUE(clist_empty(&list));
        EXPECT_NULL(clist_head(&list));
        EXPECT_NULL(clist_tail(&list));
        clist_destroy(&list);
    }
}

/**
 * Test: clist_size
 * Dependencies: clist_init
 * Description: Tests the clist_size function to get the number of elements.
 */
UTEST_TEST_CASE(clist_size){
    // Test 1: Size of empty list
    {
        CList list;
        clist_init(&list, NULL);
        EXPECT_EQUAL_UINT(clist_size(&list), 0);
        clist_destroy(&list);
    }

    // Test 2: Size after push_front operations
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        clist_push_front(&list, &a);
        EXPECT_EQUAL_UINT(clist_size(&list), 1);
        clist_push_front(&list, &b);
        EXPECT_EQUAL_UINT(clist_size(&list), 2);
        clist_push_front(&list, &c);
        EXPECT_EQUAL_UINT(clist_size(&list), 3);
        clist_destroy(&list);
    }

    // Test 3: Size after push_back operations
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        clist_push_back(&list, &a);
        EXPECT_EQUAL_UINT(clist_size(&list), 1);
        clist_push_back(&list, &b);
        EXPECT_EQUAL_UINT(clist_size(&list), 2);
        clist_push_back(&list, &c);
        EXPECT_EQUAL_UINT(clist_size(&list), 3);
        clist_destroy(&list);
    }

    // Test 4: Size after pop_front
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        clist_pop_front(&list, NULL);
        EXPECT_EQUAL_UINT(clist_size(&list), 1);
        clist_destroy(&list);
    }

    // Test 5: Size after pop_back
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        clist_pop_back(&list, NULL);
        EXPECT_EQUAL_UINT(clist_size(&list), 1);
        clist_destroy(&list);
    }

    // Test 6: Size with NULL list
    {
        EXPECT_EQUAL_UINT(clist_size(NULL), 0);
    }
}

/**
 * Test: clist_empty
 * Dependencies: clist_init, clist_size
 * Description: Tests the clist_empty function to check if list is empty.
 */
UTEST_TEST_CASE(clist_empty){
    // Test 1: Empty list
    {
        CList list;
        clist_init(&list, NULL);
        EXPECT_TRUE(clist_empty(&list));
        clist_destroy(&list);
    }

    // Test 2: Non-empty list
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1;
        clist_push_front(&list, &a);
        EXPECT_FALSE(clist_empty(&list));
        clist_destroy(&list);
    }

    // Test 3: Empty after clear
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        EXPECT_FALSE(clist_empty(&list));
        clist_clear(&list);
        EXPECT_TRUE(clist_empty(&list));
        clist_destroy(&list);
    }

    // Test 4: Empty with NULL list
    {
        EXPECT_FALSE(clist_empty(NULL));
    }
}

/**
 * Test: clist_head
 * Dependencies: clist_init
 * Description: Tests the clist_head function to get the head node.
 */
UTEST_TEST_CASE(clist_head){
    // Test 1: Head of empty list
    {
        CList list;
        clist_init(&list, NULL);
        EXPECT_NULL(clist_head(&list));
        clist_destroy(&list);
    }

    // Test 2: Head of non-empty list
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        EXPECT_NOT_NULL(clist_head(&list));
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_head(&list)), 1);
        clist_push_back(&list, &b);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_head(&list)), 1);
        clist_destroy(&list);
    }

    // Test 3: Head after push_front
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        clist_push_front(&list, &b);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_head(&list)), 2);
        clist_destroy(&list);
    }

    // Test 4: Head with NULL list
    {
        EXPECT_NULL(clist_head(NULL));
    }
}

/**
 * Test: clist_tail
 * Dependencies: clist_init
 * Description: Tests the clist_tail function to get the tail node.
 */
UTEST_TEST_CASE(clist_tail){
    // Test 1: Tail of empty list
    {
        CList list;
        clist_init(&list, NULL);
        EXPECT_NULL(clist_tail(&list));
        clist_destroy(&list);
    }

    // Test 2: Tail of non-empty list
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        EXPECT_NOT_NULL(clist_tail(&list));
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_tail(&list)), 1);
        clist_push_back(&list, &b);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_tail(&list)), 2);
        clist_destroy(&list);
    }

    // Test 3: Tail equals head for single element
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1;
        clist_push_back(&list, &a);
        EXPECT_EQUAL_UINT(clist_head(&list), clist_tail(&list));
        clist_destroy(&list);
    }

    // Test 4: Tail with NULL list
    {
        EXPECT_NULL(clist_tail(NULL));
    }

    // Test 5: Circular structure - tail's next points to head
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        CListNode* tail = clist_tail(&list);
        CListNode* head = clist_head(&list);
        EXPECT_EQUAL_UINT(clist_next(tail), head);
        clist_destroy(&list);
    }
}

/**
 * Test: clist_next
 * Dependencies: clist_init, clist_push_back
 * Description: Tests the clist_next function to get the next node.
 */
UTEST_TEST_CASE(clist_next){
    // Test 1: Next of NULL node
    {
        EXPECT_NULL(clist_next(NULL));
    }

    // Test 2: Next of single node (circular)
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1;
        clist_push_back(&list, &a);
        CListNode* node = clist_head(&list);
        EXPECT_NOT_NULL(clist_next(node));
        EXPECT_EQUAL_UINT(clist_next(node), node);
        clist_destroy(&list);
    }

    // Test 3: Next of multiple nodes
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        clist_push_back(&list, &c);
        CListNode* node = clist_head(&list);
        EXPECT_NOT_NULL(clist_next(node));
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_next(node)), 2);
        node = clist_next(node);
        EXPECT_NOT_NULL(clist_next(node));
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_next(node)), 3);
        node = clist_next(node);
        EXPECT_NOT_NULL(clist_next(node));
        EXPECT_EQUAL_UINT(clist_next(node), clist_head(&list));
        clist_destroy(&list);
    }

    // Test 4: Circular structure - last node's next points to head
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        CListNode* tail = clist_tail(&list);
        CListNode* head = clist_head(&list);
        EXPECT_EQUAL_UINT(clist_next(tail), head);
        clist_destroy(&list);
    }
}

/**
 * Test: clist_prev
 * Dependencies: clist_init, clist_push_back
 * Description: Tests the clist_prev function to get the previous node.
 */
UTEST_TEST_CASE(clist_prev){
    // Test 1: Prev of NULL node
    {
        EXPECT_NULL(clist_prev(NULL));
    }

    // Test 2: Prev of single node (circular)
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1;
        clist_push_back(&list, &a);
        CListNode* node = clist_head(&list);
        EXPECT_NOT_NULL(clist_prev(node));
        EXPECT_EQUAL_UINT(clist_prev(node), node);
        clist_destroy(&list);
    }

    // Test 3: Prev of multiple nodes
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        clist_push_back(&list, &c);
        CListNode* node = clist_tail(&list);
        EXPECT_NOT_NULL(clist_prev(node));
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_prev(node)), 2);
        node = clist_prev(node);
        EXPECT_NOT_NULL(clist_prev(node));
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_prev(node)), 1);
        node = clist_prev(node);
        EXPECT_NOT_NULL(clist_prev(node));
        EXPECT_EQUAL_UINT(clist_prev(node), clist_tail(&list));
        clist_destroy(&list);
    }

    // Test 4: Circular structure - head's prev points to tail
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        CListNode* head = clist_head(&list);
        CListNode* tail = clist_tail(&list);
        EXPECT_EQUAL_UINT(clist_prev(head), tail);
        clist_destroy(&list);
    }
}

/**
 * Test: clist_data
 * Dependencies: clist_init, clist_push_back
 * Description: Tests the clist_data function to get node data.
 */
UTEST_TEST_CASE(clist_data){
    // Test 1: Data of NULL node
    {
        EXPECT_NULL(clist_data(NULL));
    }

    // Test 2: Data of valid node
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        CListNode* node = clist_head(&list);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 1);
        node = clist_next(node);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 2);
        clist_destroy(&list);
    }

    // Test 3: Data can be NULL
    {
        CList list;
        clist_init(&list, NULL);
        clist_push_back(&list, NULL);
        CListNode* node = clist_head(&list);
        EXPECT_NULL(clist_data(node));
        clist_destroy(&list);
    }
}

/**
 * Test: clist_destroy
 * Dependencies: clist_init
 * Description: Tests destruction of the list and cleanup.
 */
UTEST_TEST_CASE(clist_destroy){
    // Test 1: Destroy empty list
    {
        CList list;
        clist_init(&list, NULL);
        clist_destroy(&list);
        EXPECT_EQUAL_UINT(clist_size(&list), 0);
        EXPECT_TRUE(clist_empty(&list));
    }

    // Test 2: Destroy list with elements and destroy function
    {
        CList list;
        clist_init(&list, destroy_counter);
        int a = 1, b = 2, c = 3;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        clist_push_back(&list, &c);
        destroy_count = 0;
        clist_destroy(&list);
        EXPECT_EQUAL_INT(destroy_count, 3);
    }

    // Test 3: Destroy list without destroy function
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        clist_destroy(&list);
        EXPECT_EQUAL_UINT(clist_size(&list), 0);
    }
}

/**
 * Test: clist_push_front
 * Dependencies: clist_init, clist_size
 * Description: Tests inserting elements at the beginning of the list.
 */
UTEST_TEST_CASE(clist_push_front){
    // Test 1: Push to empty list
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1;
        EXPECT_EQUAL_INT(clist_push_front(&list, &a), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 1);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_head(&list)), 1);
        clist_destroy(&list);
    }

    // Test 2: Push multiple elements
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        EXPECT_EQUAL_INT(clist_push_front(&list, &a), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(clist_push_front(&list, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(clist_push_front(&list, &c), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 3);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_head(&list)), 3);
        CListNode* node = clist_head(&list);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 3);
        node = clist_next(node);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 2);
        node = clist_next(node);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 1);
        clist_destroy(&list);
    }

    // Test 3: Push NULL element
    {
        CList list;
        clist_init(&list, NULL);
        EXPECT_EQUAL_INT(clist_push_front(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 1);
        EXPECT_NULL(clist_data(clist_head(&list)));
        clist_destroy(&list);
    }

    // Test 4: Circular structure maintained
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_front(&list, &a);
        CListNode* head = clist_head(&list);
        EXPECT_EQUAL_UINT(clist_next(head), head);
        EXPECT_EQUAL_UINT(clist_prev(head), head);
        clist_push_front(&list, &b);
        CListNode* new_head = clist_head(&list);
        CListNode* tail = clist_tail(&list);
        EXPECT_EQUAL_UINT(clist_next(tail), new_head);
        EXPECT_EQUAL_UINT(clist_prev(new_head), tail);
        clist_destroy(&list);
    }

    // Test 5: Push with NULL list
    {
        EXPECT_EQUAL_INT(clist_push_front(NULL, NULL), COLLECTION_FAILURE);
    }
}

/**
 * Test: clist_push_back
 * Dependencies: clist_init, clist_size
 * Description: Tests appending elements to the end of the list.
 */
UTEST_TEST_CASE(clist_push_back){
    // Test 1: Push to empty list
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1;
        EXPECT_EQUAL_INT(clist_push_back(&list, &a), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 1);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_tail(&list)), 1);
        clist_destroy(&list);
    }

    // Test 2: Push multiple elements
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        EXPECT_EQUAL_INT(clist_push_back(&list, &a), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(clist_push_back(&list, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(clist_push_back(&list, &c), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 3);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_head(&list)), 1);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_tail(&list)), 3);
        clist_destroy(&list);
    }

    // Test 3: Push NULL element
    {
        CList list;
        clist_init(&list, NULL);
        EXPECT_EQUAL_INT(clist_push_back(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 1);
        EXPECT_NULL(clist_data(clist_tail(&list)));
        clist_destroy(&list);
    }

    // Test 4: Circular structure maintained
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        CListNode* head = clist_head(&list);
        EXPECT_EQUAL_UINT(clist_next(head), head);
        EXPECT_EQUAL_UINT(clist_prev(head), head);
        clist_push_back(&list, &b);
        CListNode* tail = clist_tail(&list);
        EXPECT_EQUAL_UINT(clist_next(tail), head);
        EXPECT_EQUAL_UINT(clist_prev(head), tail);
        clist_destroy(&list);
    }

    // Test 5: Push with NULL list
    {
        EXPECT_EQUAL_INT(clist_push_back(NULL, NULL), COLLECTION_FAILURE);
    }
}

/**
 * Test: clist_pop_front
 * Dependencies: clist_init, clist_push_back, clist_size
 * Description: Tests removing the first element from the list.
 */
UTEST_TEST_CASE(clist_pop_front){
    // Test 1: Pop from empty list
    {
        CList list;
        clist_init(&list, NULL);
        EXPECT_EQUAL_INT(clist_pop_front(&list, NULL), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(clist_size(&list), 0);
        clist_destroy(&list);
    }

    // Test 2: Pop without retrieving data
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        EXPECT_EQUAL_INT(clist_pop_front(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 1);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_head(&list)), 2);
        clist_destroy(&list);
    }

    // Test 3: Pop with retrieving data
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        void* popped = NULL;
        EXPECT_EQUAL_INT(clist_pop_front(&list, &popped), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)popped, 1);
        EXPECT_EQUAL_UINT(clist_size(&list), 1);
        clist_destroy(&list);
    }

    // Test 4: Pop with destroy function
    {
        CList list;
        clist_init(&list, destroy_counter);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        destroy_count = 0;
        EXPECT_EQUAL_INT(clist_pop_front(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(destroy_count, 1);
        EXPECT_EQUAL_UINT(clist_size(&list), 1);
        clist_destroy(&list);
    }

    // Test 5: Pop all elements
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        clist_push_back(&list, &c);
        EXPECT_EQUAL_INT(clist_pop_front(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(clist_pop_front(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(clist_pop_front(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 0);
        EXPECT_TRUE(clist_empty(&list));
        clist_destroy(&list);
    }

    // Test 6: Pop single element maintains circular structure
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1;
        clist_push_back(&list, &a);
        clist_pop_front(&list, NULL);
        EXPECT_NULL(clist_head(&list));
        EXPECT_NULL(clist_tail(&list));
        clist_destroy(&list);
    }

    // Test 7: Circular structure maintained after pop
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        clist_push_back(&list, &c);
        clist_pop_front(&list, NULL);
        CListNode* head = clist_head(&list);
        CListNode* tail = clist_tail(&list);
        EXPECT_EQUAL_UINT(clist_next(tail), head);
        EXPECT_EQUAL_UINT(clist_prev(head), tail);
        clist_destroy(&list);
    }
}

/**
 * Test: clist_pop_back
 * Dependencies: clist_init, clist_push_back, clist_size
 * Description: Tests removing the last element from the list.
 */
UTEST_TEST_CASE(clist_pop_back){
    // Test 1: Pop from empty list
    {
        CList list;
        clist_init(&list, NULL);
        EXPECT_EQUAL_INT(clist_pop_back(&list, NULL), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(clist_size(&list), 0);
        clist_destroy(&list);
    }

    // Test 2: Pop without retrieving data
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        EXPECT_EQUAL_INT(clist_pop_back(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 1);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_tail(&list)), 1);
        clist_destroy(&list);
    }

    // Test 3: Pop with retrieving data
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        void* popped = NULL;
        EXPECT_EQUAL_INT(clist_pop_back(&list, &popped), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)popped, 2);
        EXPECT_EQUAL_UINT(clist_size(&list), 1);
        clist_destroy(&list);
    }

    // Test 4: Pop with destroy function
    {
        CList list;
        clist_init(&list, destroy_counter);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        destroy_count = 0;
        EXPECT_EQUAL_INT(clist_pop_back(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(destroy_count, 1);
        EXPECT_EQUAL_UINT(clist_size(&list), 1);
        clist_destroy(&list);
    }

    // Test 5: Pop all elements
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        clist_push_back(&list, &c);
        EXPECT_EQUAL_INT(clist_pop_back(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(clist_pop_back(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(clist_pop_back(&list, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 0);
        EXPECT_TRUE(clist_empty(&list));
        clist_destroy(&list);
    }

    // Test 6: Pop single element maintains circular structure
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1;
        clist_push_back(&list, &a);
        clist_pop_back(&list, NULL);
        EXPECT_NULL(clist_head(&list));
        EXPECT_NULL(clist_tail(&list));
        clist_destroy(&list);
    }

    // Test 7: Circular structure maintained after pop
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        clist_push_back(&list, &c);
        clist_pop_back(&list, NULL);
        CListNode* head = clist_head(&list);
        CListNode* tail = clist_tail(&list);
        EXPECT_EQUAL_UINT(clist_next(tail), head);
        EXPECT_EQUAL_UINT(clist_prev(head), tail);
        clist_destroy(&list);
    }
}

/**
 * Test: clist_clear
 * Dependencies: clist_init, clist_push_back, clist_size
 * Description: Tests removing all elements from the list.
 */
UTEST_TEST_CASE(clist_clear){
    // Test 1: Clear empty list
    {
        CList list;
        clist_init(&list, NULL);
        clist_clear(&list);
        EXPECT_EQUAL_UINT(clist_size(&list), 0);
        EXPECT_TRUE(clist_empty(&list));
        clist_destroy(&list);
    }

    // Test 2: Clear list with elements
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        clist_push_back(&list, &c);
        clist_clear(&list);
        EXPECT_EQUAL_UINT(clist_size(&list), 0);
        EXPECT_TRUE(clist_empty(&list));
        EXPECT_NULL(clist_head(&list));
        EXPECT_NULL(clist_tail(&list));
        clist_destroy(&list);
    }

    // Test 3: Clear with destroy function
    {
        CList list;
        clist_init(&list, destroy_counter);
        int a = 1, b = 2, c = 3;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        clist_push_back(&list, &c);
        destroy_count = 0;
        clist_clear(&list);
        EXPECT_EQUAL_INT(destroy_count, 3);
        EXPECT_EQUAL_UINT(clist_size(&list), 0);
        clist_destroy(&list);
    }

    // Test 4: Use after clear
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        clist_clear(&list);
        clist_push_back(&list, &b);
        EXPECT_EQUAL_UINT(clist_size(&list), 1);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_head(&list)), 2);
        clist_destroy(&list);
    }
}

/**
 * Test: clist_insert_after
 * Dependencies: clist_init, clist_push_back, clist_size
 * Description: Tests inserting elements after a given node.
 */
UTEST_TEST_CASE(clist_insert_after){
    // Test 1: Insert after head
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        clist_push_back(&list, &a);
        clist_push_back(&list, &c);
        CListNode* node = clist_head(&list);
        EXPECT_EQUAL_INT(clist_insert_after(&list, node, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 3);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_head(&list)), 1);
        node = clist_next(node);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 2);
        node = clist_next(node);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 3);
        clist_destroy(&list);
    }

    // Test 2: Insert after tail (maintains circular structure)
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        CListNode* node = clist_tail(&list);
        EXPECT_EQUAL_INT(clist_insert_after(&list, node, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 2);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_tail(&list)), 2);
        EXPECT_EQUAL_UINT(clist_next(clist_tail(&list)), clist_head(&list));
        clist_destroy(&list);
    }

    // Test 3: Insert NULL element
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1;
        clist_push_back(&list, &a);
        CListNode* node = clist_head(&list);
        EXPECT_EQUAL_INT(clist_insert_after(&list, node, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 2);
        EXPECT_NULL(clist_data(clist_next(node)));
        clist_destroy(&list);
    }

    // Test 4: Insert at invalid node
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1;
        EXPECT_EQUAL_INT(clist_insert_after(&list, NULL, &a), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(clist_size(&list), 0);
        clist_destroy(&list);
    }

    // Test 5: Insert with NULL list
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        CListNode* node = clist_head(&list);
        EXPECT_EQUAL_INT(clist_insert_after(NULL, node, &b), COLLECTION_FAILURE);
        clist_destroy(&list);
    }

    // Test 6: Insert after middle node
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3, d = 4;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        clist_push_back(&list, &d);
        CListNode* node = clist_head(&list);
        node = clist_next(node);
        EXPECT_EQUAL_INT(clist_insert_after(&list, node, &c), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 4);
        node = clist_head(&list);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 1);
        node = clist_next(node);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 2);
        node = clist_next(node);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 3);
        node = clist_next(node);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 4);
        clist_destroy(&list);
    }

    // Test 7: Circular structure maintained
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        clist_push_back(&list, &a);
        clist_push_back(&list, &c);
        CListNode* node = clist_head(&list);
        clist_insert_after(&list, node, &b);
        CListNode* tail = clist_tail(&list);
        EXPECT_EQUAL_UINT(clist_next(tail), clist_head(&list));
        clist_destroy(&list);
    }
}

/**
 * Test: clist_insert_before
 * Dependencies: clist_init, clist_push_back, clist_size
 * Description: Tests inserting elements before a given node.
 */
UTEST_TEST_CASE(clist_insert_before){
    // Test 1: Insert before tail
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        clist_push_back(&list, &a);
        clist_push_back(&list, &c);
        CListNode* node = clist_tail(&list);
        EXPECT_EQUAL_INT(clist_insert_before(&list, node, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 3);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_tail(&list)), 3);
        node = clist_prev(node);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 2);
        node = clist_prev(node);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 1);
        clist_destroy(&list);
    }

    // Test 2: Insert before head (updates head)
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        CListNode* node = clist_head(&list);
        EXPECT_EQUAL_INT(clist_insert_before(&list, node, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 2);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_head(&list)), 2);
        clist_destroy(&list);
    }

    // Test 3: Insert NULL element
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1;
        clist_push_back(&list, &a);
        CListNode* node = clist_tail(&list);
        EXPECT_EQUAL_INT(clist_insert_before(&list, node, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 2);
        EXPECT_NULL(clist_data(clist_prev(node)));
        clist_destroy(&list);
    }

    // Test 4: Insert at invalid node
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1;
        EXPECT_EQUAL_INT(clist_insert_before(&list, NULL, &a), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(clist_size(&list), 0);
        clist_destroy(&list);
    }

    // Test 5: Insert with NULL list
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2;
        clist_push_back(&list, &a);
        CListNode* node = clist_head(&list);
        EXPECT_EQUAL_INT(clist_insert_before(NULL, node, &b), COLLECTION_FAILURE);
        clist_destroy(&list);
    }

    // Test 6: Insert before middle node
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3, d = 4;
        clist_push_back(&list, &a);
        clist_push_back(&list, &c);
        clist_push_back(&list, &d);
        CListNode* node = clist_tail(&list);
        node = clist_prev(node);
        EXPECT_EQUAL_INT(clist_insert_before(&list, node, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 4);
        node = clist_head(&list);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 1);
        node = clist_next(node);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 2);
        node = clist_next(node);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 3);
        node = clist_next(node);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 4);
        clist_destroy(&list);
    }

    // Test 7: Circular structure maintained
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        clist_push_back(&list, &a);
        clist_push_back(&list, &c);
        CListNode* node = clist_tail(&list);
        clist_insert_before(&list, node, &b);
        CListNode* tail = clist_tail(&list);
        EXPECT_EQUAL_UINT(clist_next(tail), clist_head(&list));
        clist_destroy(&list);
    }
}

/**
 * Test: clist_remove
 * Dependencies: clist_init, clist_push_back, clist_size
 * Description: Tests removing a specific node from the list.
 */
UTEST_TEST_CASE(clist_remove){
    // Test 1: Remove head
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        clist_push_back(&list, &c);
        CListNode* node = clist_head(&list);
        EXPECT_EQUAL_INT(clist_remove(&list, node, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 2);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_head(&list)), 2);
        clist_destroy(&list);
    }

    // Test 2: Remove tail
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        clist_push_back(&list, &c);
        CListNode* node = clist_tail(&list);
        EXPECT_EQUAL_INT(clist_remove(&list, node, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 2);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_tail(&list)), 2);
        clist_destroy(&list);
    }

    // Test 3: Remove with retrieving data
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        clist_push_back(&list, &c);
        CListNode* node = clist_head(&list);
        node = clist_next(node);
        void* removed = NULL;
        EXPECT_EQUAL_INT(clist_remove(&list, node, &removed), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)removed, 2);
        EXPECT_EQUAL_UINT(clist_size(&list), 2);
        clist_destroy(&list);
    }

    // Test 4: Remove with destroy function
    {
        CList list;
        clist_init(&list, destroy_counter);
        int a = 1, b = 2, c = 3;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        clist_push_back(&list, &c);
        CListNode* node = clist_head(&list);
        destroy_count = 0;
        EXPECT_EQUAL_INT(clist_remove(&list, node, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(destroy_count, 1);
        EXPECT_EQUAL_UINT(clist_size(&list), 2);
        clist_destroy(&list);
    }

    // Test 5: Remove at invalid node
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1;
        clist_push_back(&list, &a);
        EXPECT_EQUAL_INT(clist_remove(&list, NULL, NULL), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(clist_size(&list), 1);
        clist_destroy(&list);
    }

    // Test 6: Remove with NULL list
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1;
        clist_push_back(&list, &a);
        CListNode* node = clist_head(&list);
        EXPECT_EQUAL_INT(clist_remove(NULL, node, NULL), COLLECTION_FAILURE);
        clist_destroy(&list);
    }

    // Test 7: Remove middle node
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        clist_push_back(&list, &c);
        CListNode* node = clist_head(&list);
        node = clist_next(node);
        EXPECT_EQUAL_INT(clist_remove(&list, node, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 2);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_head(&list)), 1);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_tail(&list)), 3);
        node = clist_head(&list);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_next(node)), 3);
        clist_destroy(&list);
    }

    // Test 8: Remove single element
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1;
        clist_push_back(&list, &a);
        CListNode* node = clist_head(&list);
        EXPECT_EQUAL_INT(clist_remove(&list, node, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 0);
        EXPECT_NULL(clist_head(&list));
        EXPECT_NULL(clist_tail(&list));
        clist_destroy(&list);
    }

    // Test 9: Circular structure maintained after remove
    {
        CList list;
        clist_init(&list, NULL);
        int a = 1, b = 2, c = 3;
        clist_push_back(&list, &a);
        clist_push_back(&list, &b);
        clist_push_back(&list, &c);
        CListNode* node = clist_head(&list);
        clist_remove(&list, node, NULL);
        CListNode* head = clist_head(&list);
        CListNode* tail = clist_tail(&list);
        EXPECT_EQUAL_UINT(clist_next(tail), head);
        EXPECT_EQUAL_UINT(clist_prev(head), tail);
        clist_destroy(&list);
    }
}

/**
 * Test: clist_memory_leak
 * Dependencies: clist_init, clist_destroy, clist_push_back, clist_pop_front, clist_pop_back, clist_insert_after, clist_insert_before, clist_remove, clist_clear
 * Description: Tests for memory leaks by tracking destroy function calls.
 */
UTEST_TEST_CASE(clist_memory_leak){
    // Test 1: All elements destroyed on clist_destroy
    {
        CList list;
        clist_init(&list, destroy_counter);
        int values[10];
        for (int i = 0; i < 10; i++) {
            values[i] = i;
            clist_push_back(&list, &values[i]);
        }
        destroy_count = 0;
        clist_destroy(&list);
        EXPECT_EQUAL_INT(destroy_count, 10);
    }

    // Test 2: Elements destroyed on pop_front
    {
        CList list;
        clist_init(&list, destroy_counter);
        int values[5];
        for (int i = 0; i < 5; i++) {
            values[i] = i;
            clist_push_back(&list, &values[i]);
        }
        destroy_count = 0;
        for (int i = 0; i < 5; i++) {
            clist_pop_front(&list, NULL);
        }
        EXPECT_EQUAL_INT(destroy_count, 5);
        clist_destroy(&list);
        EXPECT_EQUAL_INT(destroy_count, 5);
    }

    // Test 3: Elements destroyed on pop_back
    {
        CList list;
        clist_init(&list, destroy_counter);
        int values[5];
        for (int i = 0; i < 5; i++) {
            values[i] = i;
            clist_push_back(&list, &values[i]);
        }
        destroy_count = 0;
        for (int i = 0; i < 5; i++) {
            clist_pop_back(&list, NULL);
        }
        EXPECT_EQUAL_INT(destroy_count, 5);
        clist_destroy(&list);
        EXPECT_EQUAL_INT(destroy_count, 5);
    }

    // Test 4: Elements destroyed on remove
    {
        CList list;
        clist_init(&list, destroy_counter);
        int values[5];
        for (int i = 0; i < 5; i++) {
            values[i] = i;
            clist_push_back(&list, &values[i]);
        }
        destroy_count = 0;
        CListNode* node = clist_head(&list);
        clist_remove(&list, node, NULL);
        EXPECT_EQUAL_INT(destroy_count, 1);
        node = clist_head(&list);
        clist_remove(&list, node, NULL);
        EXPECT_EQUAL_INT(destroy_count, 2);
        clist_destroy(&list);
        EXPECT_EQUAL_INT(destroy_count, 5);
    }

    // Test 5: Elements destroyed on clear
    {
        CList list;
        clist_init(&list, destroy_counter);
        int values[10];
        for (int i = 0; i < 10; i++) {
            values[i] = i;
            clist_push_back(&list, &values[i]);
        }
        destroy_count = 0;
        clist_clear(&list);
        EXPECT_EQUAL_INT(destroy_count, 10);
        clist_destroy(&list);
        EXPECT_EQUAL_INT(destroy_count, 10);
    }
    // Test 6: Elements freed by free
    {
        CList list;
        clist_init(&list, free);
        for (int i = 0; i < 10; i++) {
            int* value = malloc(sizeof(int));
            *value = i;
            clist_push_back(&list, value);
        }
        EXPECT_EQUAL_UINT64(clist_size(&list), 10);
        clist_destroy(&list);
    }
}

/**
 * Test: clist_integration
 * Dependencies: All clist functions
 * Description: Integration test combining multiple operations.
 */
UTEST_TEST_CASE(clist_integration){
    // Test 1: Complex sequence of operations
    {
        CList list;
        clist_init(&list, NULL);
        
        int values[10];
        for (int i = 0; i < 10; i++) {
            values[i] = i;
            EXPECT_EQUAL_INT(clist_push_back(&list, &values[i]), COLLECTION_SUCCESS);
        }
        EXPECT_EQUAL_UINT(clist_size(&list), 10);
        
        void* popped = NULL;
        EXPECT_EQUAL_INT(clist_pop_front(&list, &popped), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)popped, 0);
        EXPECT_EQUAL_UINT(clist_size(&list), 9);
        
        int new_val = 99;
        CListNode* node = clist_head(&list);
        EXPECT_EQUAL_INT(clist_insert_after(&list, node, &new_val), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(clist_size(&list), 10);
        
        void* removed = NULL;
        EXPECT_EQUAL_INT(clist_remove(&list, node, &removed), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)removed, 1);
        EXPECT_EQUAL_UINT(clist_size(&list), 9);
        
        clist_clear(&list);
        EXPECT_EQUAL_UINT(clist_size(&list), 0);
        EXPECT_TRUE(clist_empty(&list));
        
        clist_destroy(&list);
    }

    // Test 2: Push front and back combination
    {
        CList list;
        clist_init(&list, NULL);
        
        int a = 1, b = 2, c = 3, d = 4;
        clist_push_front(&list, &a);
        clist_push_back(&list, &b);
        clist_push_front(&list, &c);
        clist_push_back(&list, &d);
        
        EXPECT_EQUAL_UINT(clist_size(&list), 4);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_head(&list)), 3);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_tail(&list)), 4);
        
        clist_destroy(&list);
    }

    // Test 3: Circular traversal
    {
        CList list;
        clist_init(&list, NULL);
        
        int values[5] = {1, 2, 3, 4, 5};
        for (int i = 0; i < 5; i++) {
            clist_push_back(&list, &values[i]);
        }
        
        CListNode* start = clist_head(&list);
        CListNode* node = start;
        int count = 0;
        do {
            EXPECT_EQUAL_UINT(*(int*)clist_data(node), values[count % 5]);
            node = clist_next(node);
            count++;
        } while (node != start && count < 5);
        EXPECT_EQUAL_UINT(count, 5);
        
        clist_destroy(&list);
    }

    // Test 4: Backward circular traversal
    {
        CList list;
        clist_init(&list, NULL);
        
        int values[5] = {1, 2, 3, 4, 5};
        for (int i = 0; i < 5; i++) {
            clist_push_back(&list, &values[i]);
        }
        
        CListNode* start = clist_tail(&list);
        CListNode* node = start;
        int count = 0;
        do {
            EXPECT_EQUAL_UINT(*(int*)clist_data(node), values[4 - (count % 5)]);
            node = clist_prev(node);
            count++;
        } while (node != start && count < 5);
        EXPECT_EQUAL_UINT(count, 5);
        
        clist_destroy(&list);
    }

    // Test 5: Insert before and after operations
    {
        CList list;
        clist_init(&list, NULL);
        
        int a = 1, b = 2, c = 3, d = 4, e = 5;
        clist_push_back(&list, &a);
        clist_push_back(&list, &c);
        
        CListNode* node = clist_head(&list);
        clist_insert_after(&list, node, &b);
        node = clist_tail(&list);
        clist_insert_before(&list, node, &d);
        
        EXPECT_EQUAL_UINT(clist_size(&list), 4);
        node = clist_head(&list);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 1);
        node = clist_next(node);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 2);
        node = clist_next(node);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 4);
        node = clist_next(node);
        EXPECT_EQUAL_UINT(*(int*)clist_data(node), 3);
        
        clist_destroy(&list);
    }

    // Test 6: Edge cases combination
    {
        CList list;
        clist_init(&list, NULL);
        
        EXPECT_TRUE(clist_empty(&list));
        EXPECT_NULL(clist_head(&list));
        EXPECT_NULL(clist_tail(&list));
        
        int a = 1;
        clist_push_back(&list, &a);
        EXPECT_FALSE(clist_empty(&list));
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_head(&list)), 1);
        EXPECT_EQUAL_UINT(*(int*)clist_data(clist_tail(&list)), 1);
        EXPECT_EQUAL_UINT(clist_head(&list), clist_tail(&list));
        
        clist_pop_front(&list, NULL);
        EXPECT_TRUE(clist_empty(&list));
        EXPECT_NULL(clist_head(&list));
        EXPECT_NULL(clist_tail(&list));
        
        clist_destroy(&list);
    }

    // Test 7: Multiple circular traversals
    {
        CList list;
        clist_init(&list, NULL);
        
        int values[3] = {1, 2, 3};
        for (int i = 0; i < 3; i++) {
            clist_push_back(&list, &values[i]);
        }
        
        CListNode* start = clist_head(&list);
        CListNode* node = start;
        int count = 0;
        for (int lap = 0; lap < 2; lap++) {
            for (int i = 0; i < 3; i++) {
                EXPECT_EQUAL_UINT(*(int*)clist_data(node), values[i]);
                node = clist_next(node);
                count++;
            }
        }
        EXPECT_EQUAL_UINT(node, start);
        EXPECT_EQUAL_UINT(count, 6);
        
        clist_destroy(&list);
    }
}

/**
 * Test suite: clist
 * Description: Test suite for circular linked list data structure
 */
UTEST_TEST_SUITE(clist){
    UTEST_RUN_TEST_CASE(clist_init);
    UTEST_RUN_TEST_CASE(clist_size);
    UTEST_RUN_TEST_CASE(clist_empty);
    UTEST_RUN_TEST_CASE(clist_head);
    UTEST_RUN_TEST_CASE(clist_tail);
    UTEST_RUN_TEST_CASE(clist_next);
    UTEST_RUN_TEST_CASE(clist_prev);
    UTEST_RUN_TEST_CASE(clist_data);
    UTEST_RUN_TEST_CASE(clist_destroy);
    UTEST_RUN_TEST_CASE(clist_push_front);
    UTEST_RUN_TEST_CASE(clist_push_back);
    UTEST_RUN_TEST_CASE(clist_pop_front);
    UTEST_RUN_TEST_CASE(clist_pop_back);
    UTEST_RUN_TEST_CASE(clist_clear);
    UTEST_RUN_TEST_CASE(clist_insert_after);
    UTEST_RUN_TEST_CASE(clist_insert_before);
    UTEST_RUN_TEST_CASE(clist_remove);
    UTEST_RUN_TEST_CASE(clist_memory_leak);
    UTEST_RUN_TEST_CASE(clist_integration);
}

