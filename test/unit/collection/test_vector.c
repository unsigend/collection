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

#include <collection/vector.h>
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
 * Test: vector_init
 * Dependencies: None
 * Description: Tests basic initialization of the vector structure.
 */
UTEST_TEST_CASE(vector_init){
    // Test 1: Initialize with NULL destroy function
    {
        Vector vec;
        vector_init(&vec, NULL);
        EXPECT_EQUAL_UINT(vector_size(&vec), 0);
        EXPECT_EQUAL_UINT(vector_capacity(&vec), 0);
        EXPECT_TRUE(vector_empty(&vec));
        EXPECT_NULL(vector_data(&vec));
        vector_destroy(&vec);
    }

    // Test 2: Initialize with non-NULL destroy function
    {
        Vector vec;
        vector_init(&vec, free);
        EXPECT_EQUAL_UINT(vector_size(&vec), 0);
        EXPECT_EQUAL_UINT(vector_capacity(&vec), 0);
        EXPECT_TRUE(vector_empty(&vec));
        EXPECT_NULL(vector_data(&vec));
        vector_destroy(&vec);
    }
}

/**
 * Test: vector_size
 * Dependencies: vector_init
 * Description: Tests the vector_size function to get the number of elements.
 */
UTEST_TEST_CASE(vector_size){
    // Test 1: Size of empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        EXPECT_EQUAL_UINT(vector_size(&vec), 0);
        vector_destroy(&vec);
    }

    // Test 2: Size after push_back operations
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2, c = 3;
        vector_push_back(&vec, &a);
        EXPECT_EQUAL_UINT(vector_size(&vec), 1);
        vector_push_back(&vec, &b);
        EXPECT_EQUAL_UINT(vector_size(&vec), 2);
        vector_push_back(&vec, &c);
        EXPECT_EQUAL_UINT(vector_size(&vec), 3);
        vector_destroy(&vec);
    }

    // Test 3: Size after resize operations
    {
        Vector vec;
        vector_init(&vec, NULL);
        vector_resize(&vec, 10);
        EXPECT_EQUAL_UINT(vector_size(&vec), 10);
        vector_resize(&vec, 5);
        EXPECT_EQUAL_UINT(vector_size(&vec), 5);
        vector_resize(&vec, 0);
        EXPECT_EQUAL_UINT(vector_size(&vec), 0);
        vector_destroy(&vec);
    }

    // Test 4: Size with NULL vector
    {
        EXPECT_EQUAL_UINT(vector_size(NULL), 0);
    }
}

/**
 * Test: vector_empty
 * Dependencies: vector_init, vector_size
 * Description: Tests the vector_empty function to check if vector is empty.
 */
UTEST_TEST_CASE(vector_empty){
    // Test 1: Empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        EXPECT_TRUE(vector_empty(&vec));
        vector_destroy(&vec);
    }

    // Test 2: Non-empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1;
        vector_push_back(&vec, &a);
        EXPECT_FALSE(vector_empty(&vec));
        vector_destroy(&vec);
    }

    // Test 3: Empty after clear
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        EXPECT_FALSE(vector_empty(&vec));
        vector_clear(&vec);
        EXPECT_TRUE(vector_empty(&vec));
        vector_destroy(&vec);
    }

    // Test 4: Empty with NULL vector
    {
        EXPECT_FALSE(vector_empty(NULL));
    }
}

/**
 * Test: vector_capacity
 * Dependencies: vector_init
 * Description: Tests the vector_capacity function to get the capacity.
 */
UTEST_TEST_CASE(vector_capacity){
    // Test 1: Capacity of empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        EXPECT_EQUAL_UINT(vector_capacity(&vec), 0);
        vector_destroy(&vec);
    }

    // Test 2: Capacity growth with push_back
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1;
        vector_push_back(&vec, &a);
        EXPECT_GREATER_EQUAL_UINT(vector_capacity(&vec), 1);
        vector_destroy(&vec);
    }

    // Test 3: Capacity after resize
    {
        Vector vec;
        vector_init(&vec, NULL);
        vector_resize(&vec, 10);
        EXPECT_GREATER_EQUAL_UINT(vector_capacity(&vec), 10);
        vector_resize(&vec, 5);
        EXPECT_GREATER_EQUAL_UINT(vector_capacity(&vec), 10);
        vector_destroy(&vec);
    }

    // Test 4: Capacity after shrink_to_fit
    {
        Vector vec;
        vector_init(&vec, NULL);
        vector_resize(&vec, 10);
        size_t old_capacity = vector_capacity(&vec);
        vector_resize(&vec, 5);
        vector_shrink_to_fit(&vec);
        EXPECT_EQUAL_UINT(vector_capacity(&vec), 5);
        vector_destroy(&vec);
    }

    // Test 5: Capacity with NULL vector
    {
        EXPECT_EQUAL_UINT(vector_capacity(NULL), 0);
    }
}

/**
 * Test: vector_data
 * Dependencies: vector_init
 * Description: Tests the vector_data function to get the underlying array.
 */
UTEST_TEST_CASE(vector_data){
    // Test 1: Data of empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        EXPECT_NULL(vector_data(&vec));
        vector_destroy(&vec);
    }

    // Test 2: Data after push_back
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2;
        vector_push_back(&vec, &a);
        EXPECT_NOT_NULL(vector_data(&vec));
        EXPECT_EQUAL_UINT(*(int*)vector_data(&vec)[0], 1);
        vector_push_back(&vec, &b);
        EXPECT_EQUAL_UINT(*(int*)vector_data(&vec)[1], 2);
        vector_destroy(&vec);
    }

    // Test 3: Data with NULL vector
    {
        EXPECT_NULL(vector_data(NULL));
    }
}

/**
 * Test: vector_destroy
 * Dependencies: vector_init
 * Description: Tests destruction of the vector and cleanup.
 */
UTEST_TEST_CASE(vector_destroy){
    // Test 1: Destroy empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        vector_destroy(&vec);
        EXPECT_EQUAL_UINT(vector_size(&vec), 0);
        EXPECT_EQUAL_UINT(vector_capacity(&vec), 0);
    }

    // Test 2: Destroy vector with elements and destroy function
    {
        Vector vec;
        vector_init(&vec, destroy_counter);
        int a = 1, b = 2, c = 3;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        vector_push_back(&vec, &c);
        destroy_count = 0;
        vector_destroy(&vec);
        EXPECT_EQUAL_INT(destroy_count, 3);
    }

    // Test 3: Destroy vector without destroy function
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        vector_destroy(&vec);
        EXPECT_EQUAL_UINT(vector_size(&vec), 0);
    }
}

/**
 * Test: vector_at
 * Dependencies: vector_init, vector_push_back
 * Description: Tests accessing elements at specific indices.
 */
UTEST_TEST_CASE(vector_at){
    // Test 1: Access valid indices
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2, c = 3;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        vector_push_back(&vec, &c);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 0), 1);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 1), 2);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 2), 3);
        vector_destroy(&vec);
    }

    // Test 2: Access after insert
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2, c = 3;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &c);
        vector_insert(&vec, 1, &b);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 0), 1);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 1), 2);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 2), 3);
        vector_destroy(&vec);
    }

    // Test 3: Access after resize
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1;
        vector_push_back(&vec, &a);
        vector_resize(&vec, 5);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 0), 1);
        EXPECT_NULL(vector_at(&vec, 1));
        EXPECT_NULL(vector_at(&vec, 4));
        vector_destroy(&vec);
    }
}

/**
 * Test: vector_front
 * Dependencies: vector_init, vector_push_back
 * Description: Tests getting the first element of the vector.
 */
UTEST_TEST_CASE(vector_front){
    // Test 1: Front of empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        EXPECT_NULL(vector_front(&vec));
        vector_destroy(&vec);
    }

    // Test 2: Front of non-empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2;
        vector_push_back(&vec, &a);
        EXPECT_EQUAL_UINT(*(int*)vector_front(&vec), 1);
        vector_push_back(&vec, &b);
        EXPECT_EQUAL_UINT(*(int*)vector_front(&vec), 1);
        vector_destroy(&vec);
    }

    // Test 3: Front after insert at beginning
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2;
        vector_push_back(&vec, &a);
        vector_insert(&vec, 0, &b);
        EXPECT_EQUAL_UINT(*(int*)vector_front(&vec), 2);
        vector_destroy(&vec);
    }
}

/**
 * Test: vector_back
 * Dependencies: vector_init, vector_push_back
 * Description: Tests getting the last element of the vector.
 */
UTEST_TEST_CASE(vector_back){
    // Test 1: Back of empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        EXPECT_NULL(vector_back(&vec));
        vector_destroy(&vec);
    }

    // Test 2: Back of non-empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2, c = 3;
        vector_push_back(&vec, &a);
        EXPECT_EQUAL_UINT(*(int*)vector_back(&vec), 1);
        vector_push_back(&vec, &b);
        EXPECT_EQUAL_UINT(*(int*)vector_back(&vec), 2);
        vector_push_back(&vec, &c);
        EXPECT_EQUAL_UINT(*(int*)vector_back(&vec), 3);
        vector_destroy(&vec);
    }

    // Test 3: Back after pop_back
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        vector_pop_back(&vec, NULL);
        EXPECT_EQUAL_UINT(*(int*)vector_back(&vec), 1);
        vector_destroy(&vec);
    }
}

/**
 * Test: vector_resize
 * Dependencies: vector_init, vector_size, vector_capacity
 * Description: Tests resizing the vector to a new size.
 */
UTEST_TEST_CASE(vector_resize){
    // Test 1: Resize empty vector to larger size
    {
        Vector vec;
        vector_init(&vec, NULL);
        EXPECT_EQUAL_INT(vector_resize(&vec, 10), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 10);
        EXPECT_GREATER_EQUAL_UINT(vector_capacity(&vec), 10);
        vector_destroy(&vec);
    }

    // Test 2: Resize to same size
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1;
        vector_push_back(&vec, &a);
        size_t old_size = vector_size(&vec);
        EXPECT_EQUAL_INT(vector_resize(&vec, old_size), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), old_size);
        vector_destroy(&vec);
    }

    // Test 3: Resize to smaller size with destroy function
    {
        Vector vec;
        vector_init(&vec, destroy_counter);
        int a = 1, b = 2, c = 3;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        vector_push_back(&vec, &c);
        destroy_count = 0;
        EXPECT_EQUAL_INT(vector_resize(&vec, 1), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 1);
        EXPECT_EQUAL_INT(destroy_count, 2);
        vector_destroy(&vec);
    }

    // Test 4: Resize to zero
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        EXPECT_EQUAL_INT(vector_resize(&vec, 0), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 0);
        EXPECT_TRUE(vector_empty(&vec));
        vector_destroy(&vec);
    }

    // Test 5: Resize larger preserves existing elements
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        EXPECT_EQUAL_INT(vector_resize(&vec, 5), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 5);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 0), 1);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 1), 2);
        EXPECT_NULL(vector_at(&vec, 2));
        vector_destroy(&vec);
    }
}

/**
 * Test: vector_shrink_to_fit
 * Dependencies: vector_init, vector_size, vector_capacity, vector_resize
 * Description: Tests shrinking the capacity to match the size.
 */
UTEST_TEST_CASE(vector_shrink_to_fit){
    // Test 1: Shrink empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        EXPECT_EQUAL_INT(vector_shrink_to_fit(&vec), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_capacity(&vec), 0);
        vector_destroy(&vec);
    }

    // Test 2: Shrink after resize down
    {
        Vector vec;
        vector_init(&vec, NULL);
        vector_resize(&vec, 100);
        size_t old_capacity = vector_capacity(&vec);
        vector_resize(&vec, 10);
        EXPECT_GREATER_EQUAL_UINT(vector_capacity(&vec), old_capacity);
        EXPECT_EQUAL_INT(vector_shrink_to_fit(&vec), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_capacity(&vec), 10);
        EXPECT_EQUAL_UINT(vector_size(&vec), 10);
        vector_destroy(&vec);
    }

    // Test 3: Shrink when capacity equals size
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1;
        vector_push_back(&vec, &a);
        size_t old_capacity = vector_capacity(&vec);
        EXPECT_EQUAL_INT(vector_shrink_to_fit(&vec), COLLECTION_SUCCESS);
        EXPECT_GREATER_EQUAL_UINT(vector_capacity(&vec), 1);
        vector_destroy(&vec);
    }

    // Test 4: Shrink preserves elements
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2, c = 3;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        vector_push_back(&vec, &c);
        vector_resize(&vec, 100);
        vector_resize(&vec, 3);
        EXPECT_EQUAL_INT(vector_shrink_to_fit(&vec), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 3);
        EXPECT_EQUAL_UINT(vector_capacity(&vec), 3);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 0), 1);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 1), 2);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 2), 3);
        vector_destroy(&vec);
    }
}

/**
 * Test: vector_push_back
 * Dependencies: vector_init, vector_size
 * Description: Tests appending elements to the end of the vector.
 */
UTEST_TEST_CASE(vector_push_back){
    // Test 1: Push to empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1;
        EXPECT_EQUAL_INT(vector_push_back(&vec, &a), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 1);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 0), 1);
        vector_destroy(&vec);
    }

    // Test 2: Push multiple elements
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2, c = 3;
        EXPECT_EQUAL_INT(vector_push_back(&vec, &a), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(vector_push_back(&vec, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(vector_push_back(&vec, &c), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 3);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 0), 1);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 1), 2);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 2), 3);
        vector_destroy(&vec);
    }

    // Test 3: Push NULL element
    {
        Vector vec;
        vector_init(&vec, NULL);
        EXPECT_EQUAL_INT(vector_push_back(&vec, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 1);
        EXPECT_NULL(vector_at(&vec, 0));
        vector_destroy(&vec);
    }

    // Test 4: Capacity growth
    {
        Vector vec;
        vector_init(&vec, NULL);
        int values[100];
        for (int i = 0; i < 100; i++) {
            values[i] = i;
            EXPECT_EQUAL_INT(vector_push_back(&vec, &values[i]), COLLECTION_SUCCESS);
        }
        EXPECT_EQUAL_UINT(vector_size(&vec), 100);
        EXPECT_GREATER_EQUAL_UINT(vector_capacity(&vec), 100);
        for (int i = 0; i < 100; i++) {
            EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, i), i);
        }
        vector_destroy(&vec);
    }
}

/**
 * Test: vector_pop_back
 * Dependencies: vector_init, vector_push_back, vector_size
 * Description: Tests removing the last element from the vector.
 */
UTEST_TEST_CASE(vector_pop_back){
    // Test 1: Pop from empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        EXPECT_EQUAL_INT(vector_pop_back(&vec, NULL), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(vector_size(&vec), 0);
        vector_destroy(&vec);
    }

    // Test 2: Pop without retrieving data
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        EXPECT_EQUAL_INT(vector_pop_back(&vec, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 1);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 0), 1);
        vector_destroy(&vec);
    }

    // Test 3: Pop with retrieving data
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        void* popped = NULL;
        EXPECT_EQUAL_INT(vector_pop_back(&vec, &popped), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)popped, 2);
        EXPECT_EQUAL_UINT(vector_size(&vec), 1);
        vector_destroy(&vec);
    }

    // Test 4: Pop with destroy function
    {
        Vector vec;
        vector_init(&vec, destroy_counter);
        int a = 1, b = 2;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        destroy_count = 0;
        EXPECT_EQUAL_INT(vector_pop_back(&vec, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(destroy_count, 1);
        EXPECT_EQUAL_UINT(vector_size(&vec), 1);
        vector_destroy(&vec);
    }

    // Test 5: Pop all elements
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2, c = 3;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        vector_push_back(&vec, &c);
        EXPECT_EQUAL_INT(vector_pop_back(&vec, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(vector_pop_back(&vec, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(vector_pop_back(&vec, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 0);
        EXPECT_TRUE(vector_empty(&vec));
        vector_destroy(&vec);
    }
}

/**
 * Test: vector_insert
 * Dependencies: vector_init, vector_push_back, vector_size, vector_at
 * Description: Tests inserting elements at specific positions.
 */
UTEST_TEST_CASE(vector_insert){
    // Test 1: Insert at beginning
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2;
        vector_push_back(&vec, &a);
        EXPECT_EQUAL_INT(vector_insert(&vec, 0, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 2);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 0), 2);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 1), 1);
        vector_destroy(&vec);
    }

    // Test 2: Insert at middle
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2, c = 3;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &c);
        EXPECT_EQUAL_INT(vector_insert(&vec, 1, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 3);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 0), 1);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 1), 2);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 2), 3);
        vector_destroy(&vec);
    }

    // Test 3: Insert at end (size)
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2;
        vector_push_back(&vec, &a);
        EXPECT_EQUAL_INT(vector_insert(&vec, vector_size(&vec), &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 2);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 1), 2);
        vector_destroy(&vec);
    }

    // Test 4: Insert at invalid index
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1;
        EXPECT_EQUAL_INT(vector_insert(&vec, 10, &a), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(vector_size(&vec), 0);
        vector_destroy(&vec);
    }

    // Test 5: Insert NULL element
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1;
        vector_push_back(&vec, &a);
        EXPECT_EQUAL_INT(vector_insert(&vec, 0, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 2);
        EXPECT_NULL(vector_at(&vec, 0));
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 1), 1);
        vector_destroy(&vec);
    }

    // Test 6: Insert multiple elements
    {
        Vector vec;
        vector_init(&vec, NULL);
        int values[5] = {1, 2, 3, 4, 5};
        for (int i = 0; i < 5; i++) {
            EXPECT_EQUAL_INT(vector_insert(&vec, i, &values[i]), COLLECTION_SUCCESS);
        }
        EXPECT_EQUAL_UINT(vector_size(&vec), 5);
        for (int i = 0; i < 5; i++) {
            EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, i), values[i]);
        }
        vector_destroy(&vec);
    }
}

/**
 * Test: vector_remove
 * Dependencies: vector_init, vector_push_back, vector_size, vector_at
 * Description: Tests removing elements at specific positions.
 */
UTEST_TEST_CASE(vector_remove){
    // Test 1: Remove at beginning
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2, c = 3;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        vector_push_back(&vec, &c);
        EXPECT_EQUAL_INT(vector_remove(&vec, 0, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 2);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 0), 2);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 1), 3);
        vector_destroy(&vec);
    }

    // Test 2: Remove at middle
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2, c = 3;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        vector_push_back(&vec, &c);
        EXPECT_EQUAL_INT(vector_remove(&vec, 1, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 2);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 0), 1);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 1), 3);
        vector_destroy(&vec);
    }

    // Test 3: Remove at end
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2, c = 3;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        vector_push_back(&vec, &c);
        EXPECT_EQUAL_INT(vector_remove(&vec, 2, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 2);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 0), 1);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 1), 2);
        vector_destroy(&vec);
    }

    // Test 4: Remove with retrieving data
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2, c = 3;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        vector_push_back(&vec, &c);
        void* removed = NULL;
        EXPECT_EQUAL_INT(vector_remove(&vec, 1, &removed), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)removed, 2);
        EXPECT_EQUAL_UINT(vector_size(&vec), 2);
        vector_destroy(&vec);
    }

    // Test 5: Remove with destroy function
    {
        Vector vec;
        vector_init(&vec, destroy_counter);
        int a = 1, b = 2, c = 3;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        vector_push_back(&vec, &c);
        destroy_count = 0;
        EXPECT_EQUAL_INT(vector_remove(&vec, 1, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(destroy_count, 1);
        EXPECT_EQUAL_UINT(vector_size(&vec), 2);
        vector_destroy(&vec);
    }

    // Test 6: Remove at invalid index
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1;
        vector_push_back(&vec, &a);
        EXPECT_EQUAL_INT(vector_remove(&vec, 10, NULL), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(vector_size(&vec), 1);
        vector_destroy(&vec);
    }

    // Test 7: Remove all elements
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2, c = 3;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        vector_push_back(&vec, &c);
        EXPECT_EQUAL_INT(vector_remove(&vec, 2, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(vector_remove(&vec, 1, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(vector_remove(&vec, 0, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 0);
        EXPECT_TRUE(vector_empty(&vec));
        vector_destroy(&vec);
    }
}

/**
 * Test: vector_clear
 * Dependencies: vector_init, vector_push_back, vector_size
 * Description: Tests removing all elements from the vector.
 */
UTEST_TEST_CASE(vector_clear){
    // Test 1: Clear empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        vector_clear(&vec);
        EXPECT_EQUAL_UINT(vector_size(&vec), 0);
        EXPECT_TRUE(vector_empty(&vec));
        vector_destroy(&vec);
    }

    // Test 2: Clear vector with elements
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2, c = 3;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        vector_push_back(&vec, &c);
        vector_clear(&vec);
        EXPECT_EQUAL_UINT(vector_size(&vec), 0);
        EXPECT_TRUE(vector_empty(&vec));
        vector_destroy(&vec);
    }

    // Test 3: Clear with destroy function
    {
        Vector vec;
        vector_init(&vec, destroy_counter);
        int a = 1, b = 2, c = 3;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        vector_push_back(&vec, &c);
        destroy_count = 0;
        vector_clear(&vec);
        EXPECT_EQUAL_INT(destroy_count, 3);
        EXPECT_EQUAL_UINT(vector_size(&vec), 0);
        vector_destroy(&vec);
    }

    // Test 4: Clear preserves capacity
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2, c = 3;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        vector_push_back(&vec, &c);
        size_t old_capacity = vector_capacity(&vec);
        vector_clear(&vec);
        EXPECT_EQUAL_UINT(vector_capacity(&vec), old_capacity);
        EXPECT_EQUAL_UINT(vector_size(&vec), 0);
        vector_destroy(&vec);
    }

    // Test 5: Use after clear
    {
        Vector vec;
        vector_init(&vec, NULL);
        int a = 1, b = 2;
        vector_push_back(&vec, &a);
        vector_clear(&vec);
        vector_push_back(&vec, &b);
        EXPECT_EQUAL_UINT(vector_size(&vec), 1);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 0), 2);
        vector_destroy(&vec);
    }
}

/**
 * Test: vector_memory_leak
 * Dependencies: vector_init, vector_destroy, vector_push_back, vector_pop_back, vector_insert, vector_remove, vector_clear, vector_resize
 * Description: Tests for memory leaks by tracking destroy function calls.
 */
UTEST_TEST_CASE(vector_memory_leak){
    // Test 1: All elements destroyed on vector_destroy
    {
        Vector vec;
        vector_init(&vec, destroy_counter);
        int values[10];
        for (int i = 0; i < 10; i++) {
            values[i] = i;
            vector_push_back(&vec, &values[i]);
        }
        destroy_count = 0;
        vector_destroy(&vec);
        EXPECT_EQUAL_INT(destroy_count, 10);
    }

    // Test 2: Elements destroyed on resize down
    {
        Vector vec;
        vector_init(&vec, destroy_counter);
        int values[10];
        for (int i = 0; i < 10; i++) {
            values[i] = i;
            vector_push_back(&vec, &values[i]);
        }
        destroy_count = 0;
        vector_resize(&vec, 5);
        EXPECT_EQUAL_INT(destroy_count, 5);
        vector_destroy(&vec);
        EXPECT_EQUAL_INT(destroy_count, 10);
    }

    // Test 3: Elements destroyed on pop_back
    {
        Vector vec;
        vector_init(&vec, destroy_counter);
        int values[5];
        for (int i = 0; i < 5; i++) {
            values[i] = i;
            vector_push_back(&vec, &values[i]);
        }
        destroy_count = 0;
        for (int i = 0; i < 5; i++) {
            vector_pop_back(&vec, NULL);
        }
        EXPECT_EQUAL_INT(destroy_count, 5);
        vector_destroy(&vec);
        EXPECT_EQUAL_INT(destroy_count, 5);
    }

    // Test 4: Elements destroyed on remove
    {
        Vector vec;
        vector_init(&vec, destroy_counter);
        int values[5];
        for (int i = 0; i < 5; i++) {
            values[i] = i;
            vector_push_back(&vec, &values[i]);
        }
        destroy_count = 0;
        vector_remove(&vec, 2, NULL);
        EXPECT_EQUAL_INT(destroy_count, 1);
        vector_remove(&vec, 1, NULL);
        EXPECT_EQUAL_INT(destroy_count, 2);
        vector_destroy(&vec);
        EXPECT_EQUAL_INT(destroy_count, 5);
    }

    // Test 5: Elements destroyed on clear
    {
        Vector vec;
        vector_init(&vec, destroy_counter);
        int values[10];
        for (int i = 0; i < 10; i++) {
            values[i] = i;
            vector_push_back(&vec, &values[i]);
        }
        destroy_count = 0;
        vector_clear(&vec);
        EXPECT_EQUAL_INT(destroy_count, 10);
        vector_destroy(&vec);
        EXPECT_EQUAL_INT(destroy_count, 10);
    }
}

/**
 * Test: vector_integration
 * Dependencies: All vector functions
 * Description: Integration test combining multiple operations.
 */
UTEST_TEST_CASE(vector_integration){
    // Test 1: Complex sequence of operations
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        int values[10];
        for (int i = 0; i < 10; i++) {
            values[i] = i;
            EXPECT_EQUAL_INT(vector_push_back(&vec, &values[i]), COLLECTION_SUCCESS);
        }
        EXPECT_EQUAL_UINT(vector_size(&vec), 10);
        
        void* popped = NULL;
        EXPECT_EQUAL_INT(vector_pop_back(&vec, &popped), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)popped, 9);
        EXPECT_EQUAL_UINT(vector_size(&vec), 9);
        
        int new_val = 99;
        EXPECT_EQUAL_INT(vector_insert(&vec, 5, &new_val), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 10);
        EXPECT_EQUAL_UINT(*(int*)vector_at(&vec, 5), 99);
        
        void* removed = NULL;
        EXPECT_EQUAL_INT(vector_remove(&vec, 0, &removed), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)removed, 0);
        EXPECT_EQUAL_UINT(vector_size(&vec), 9);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 20), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 20);
        
        EXPECT_EQUAL_INT(vector_shrink_to_fit(&vec), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_capacity(&vec), 20);
        
        vector_clear(&vec);
        EXPECT_EQUAL_UINT(vector_size(&vec), 0);
        EXPECT_TRUE(vector_empty(&vec));
        
        vector_destroy(&vec);
    }

    // Test 2: Resize and shrink operations
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_resize(&vec, 100);
        EXPECT_EQUAL_UINT(vector_size(&vec), 100);
        EXPECT_GREATER_EQUAL_UINT(vector_capacity(&vec), 100);
        
        vector_resize(&vec, 50);
        EXPECT_EQUAL_UINT(vector_size(&vec), 50);
        EXPECT_GREATER_EQUAL_UINT(vector_capacity(&vec), 100);
        
        EXPECT_EQUAL_INT(vector_shrink_to_fit(&vec), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_capacity(&vec), 50);
        
        vector_destroy(&vec);
    }

    // Test 3: Insert and remove at various positions
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        int a = 1, b = 2, c = 3, d = 4, e = 5;
        vector_push_back(&vec, &a);
        vector_push_back(&vec, &b);
        vector_push_back(&vec, &c);
        
        EXPECT_EQUAL_INT(vector_insert(&vec, 0, &d), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)vector_front(&vec), 4);
        
        EXPECT_EQUAL_INT(vector_insert(&vec, vector_size(&vec), &e), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)vector_back(&vec), 5);
        
        EXPECT_EQUAL_INT(vector_remove(&vec, 2, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(vector_size(&vec), 4);
        
        vector_destroy(&vec);
    }

    // Test 4: Edge cases combination
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_TRUE(vector_empty(&vec));
        EXPECT_NULL(vector_front(&vec));
        EXPECT_NULL(vector_back(&vec));
        
        int a = 1;
        vector_push_back(&vec, &a);
        EXPECT_FALSE(vector_empty(&vec));
        EXPECT_EQUAL_UINT(*(int*)vector_front(&vec), 1);
        EXPECT_EQUAL_UINT(*(int*)vector_back(&vec), 1);
        
        vector_pop_back(&vec, NULL);
        EXPECT_TRUE(vector_empty(&vec));
        EXPECT_NULL(vector_front(&vec));
        EXPECT_NULL(vector_back(&vec));
        
        vector_destroy(&vec);
    }
}

/**
 * Test suite: vector
 * Description: Test suite for vector data structure
 */
UTEST_TEST_SUITE(vector){
    UTEST_RUN_TEST_CASE(vector_init);
    UTEST_RUN_TEST_CASE(vector_size);
    UTEST_RUN_TEST_CASE(vector_empty);
    UTEST_RUN_TEST_CASE(vector_capacity);
    UTEST_RUN_TEST_CASE(vector_data);
    UTEST_RUN_TEST_CASE(vector_destroy);
    UTEST_RUN_TEST_CASE(vector_at);
    UTEST_RUN_TEST_CASE(vector_front);
    UTEST_RUN_TEST_CASE(vector_back);
    UTEST_RUN_TEST_CASE(vector_resize);
    UTEST_RUN_TEST_CASE(vector_shrink_to_fit);
    UTEST_RUN_TEST_CASE(vector_push_back);
    UTEST_RUN_TEST_CASE(vector_pop_back);
    UTEST_RUN_TEST_CASE(vector_insert);
    UTEST_RUN_TEST_CASE(vector_remove);
    UTEST_RUN_TEST_CASE(vector_clear);
    UTEST_RUN_TEST_CASE(vector_memory_leak);
    UTEST_RUN_TEST_CASE(vector_integration);
}

