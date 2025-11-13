#include <utest.h>
#include <collection/vector.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/**
 * Global helper functions for testing destroy callbacks.
 * These functions are reused across multiple test cases.
 * Each test case maintains its own local counter for isolation.
 */

// Simple destroy counter - increments a counter pointer
static void destroy_counter(void *data) {
    int *counter = (int *)data;
    if (counter != NULL) {
        (*counter)++;
    }
}

// Destroy function that does nothing
static void destroy_noop(void *data) {
    (void)data;  // Unused
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
        
        EXPECT_EQUAL_UINT64(vec.size, 0);
        EXPECT_EQUAL_UINT64(vec.capacity, 0);
        EXPECT_NULL(vec.data);
        EXPECT_NULL(vec.destroy);
    }
    
    // Test 2: Initialize with non-NULL destroy function
    {
        Vector vec;
        vector_init(&vec, destroy_noop);
        
        EXPECT_EQUAL_UINT64(vec.size, 0);
        EXPECT_EQUAL_UINT64(vec.capacity, 0);
        EXPECT_NULL(vec.data);
        EXPECT_NOT_NULL(vec.destroy);
        EXPECT_TRUE(vec.destroy == destroy_noop);
    }
    
    // Test 3: Multiple initializations
    {
        Vector vec1, vec2;
        vector_init(&vec1, NULL);
        vector_init(&vec2, destroy_noop);
        
        EXPECT_NULL(vec1.destroy);
        EXPECT_NOT_NULL(vec2.destroy);
    }
}

/**
 * Test: vector_size
 * Dependencies: vector_init
 * Description: Tests the size function on initialized vectors.
 */
UTEST_TEST_CASE(vector_size){
    // Test 1: Size of newly initialized vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 0);
    }
    
    // Test 2: Direct size member access consistency
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), vec.size);
    }
}

/**
 * Test: vector_capacity
 * Dependencies: vector_init
 * Description: Tests the capacity query function on initialized vectors.
 */
UTEST_TEST_CASE(vector_capacity){
    // Test 1: Capacity of newly initialized vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_UINT64(vector_capacity(&vec), 0);
    }
    
    // Test 2: Direct capacity member access consistency
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_UINT64(vector_capacity(&vec), vec.capacity);
    }
}

/**
 * Test: vector_destroy
 * Dependencies: vector_init
 * Description: Tests the destruction of vectors.
 */
UTEST_TEST_CASE(vector_destroy){
    // Test 1: Destroy empty vector without destroy function
    {
        Vector vec;
        vector_init(&vec, NULL);
        vector_destroy(&vec);
        
        EXPECT_EQUAL_UINT64(vec.size, 0);
        EXPECT_EQUAL_UINT64(vec.capacity, 0);
        EXPECT_NULL(vec.data);
        EXPECT_NULL(vec.destroy);
    }
    
    // Test 2: Destroy empty vector with destroy function
    {
        Vector vec;
        vector_init(&vec, destroy_noop);
        vector_destroy(&vec);
        
        EXPECT_EQUAL_UINT64(vec.size, 0);
        EXPECT_EQUAL_UINT64(vec.capacity, 0);
        EXPECT_NULL(vec.data);
        EXPECT_NULL(vec.destroy);
    }
    
    // Test 3: Multiple destroy calls
    {
        Vector vec;
        vector_init(&vec, NULL);
        vector_destroy(&vec);
        vector_destroy(&vec);
        
        EXPECT_EQUAL_UINT64(vec.size, 0);
        EXPECT_NULL(vec.data);
    }
}

/**
 * Test: vector_resize
 * Dependencies: vector_init, vector_size, vector_capacity
 * Description: Tests resizing operations including expansion and shrinking.
 */
UTEST_TEST_CASE(vector_resize){
    // Test 1: Resize from zero to positive size
    {
        Vector vec;
        vector_init(&vec, NULL);

        int result = vector_resize(&vec, 10);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 10);
        EXPECT_EQUAL_UINT64(vector_capacity(&vec), 10);
        EXPECT_NOT_NULL(vec.data);
        
        // All new elements should be NULL
        for (size_t i = 0; i < 10; i++) {
            EXPECT_NULL(vec.data[i]);
        }
        
        vector_destroy(&vec);
    }
    
    // Test 2: Resize to same size
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        int result = vector_resize(&vec, 5);
        EXPECT_EQUAL_INT(result, 0);
        void *original_data = vec.data;
        
        result = vector_resize(&vec, 5);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 5);
        EXPECT_TRUE(vec.data == original_data);
        
        vector_destroy(&vec);
    }
    
    // Test 3: Resize to zero
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        int result = vector_resize(&vec, 5);
        EXPECT_EQUAL_INT(result, 0);
        result = vector_resize(&vec, 0);
        EXPECT_EQUAL_INT(result, 0);
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 0);
        EXPECT_EQUAL_UINT64(vector_capacity(&vec), 5);
        
        vector_destroy(&vec);
    }
    
    // Test 4: Expand multiple times
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 5), 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 5);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 10), 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 10);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 20), 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 20);
        
        vector_destroy(&vec);
    }
    
    // Test 5: Shrink and verify capacity unchanged
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 100), 0);
        size_t cap_after_expand = vector_capacity(&vec);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 10), 0);
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 10);
        EXPECT_EQUAL_UINT64(vector_capacity(&vec), cap_after_expand);
        
        vector_destroy(&vec);
    }
    
    // Test 6: Shrink preserves remaining elements
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 10), 0);
        const char *markers[5] = {"a", "b", "c", "d", "e"};
        for (size_t i = 0; i < 5; i++) {
            vec.data[i] = (void *)markers[i];
        }
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 5), 0);
        
        // First 5 elements should be preserved
        for (size_t i = 0; i < 5; i++) {
            EXPECT_TRUE(vec.data[i] == (void *)markers[i]);
        }
        
        vector_destroy(&vec);
    }
    
    // Test 7: Shrink clears removed elements
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 10), 0);
        for (size_t i = 0; i < 10; i++) {
            vec.data[i] = (void *)0xDEADBEEF;
        }
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 5), 0);
        
        // Elements [5, 10) should be NULL
        for (size_t i = 5; i < 10; i++) {
            EXPECT_NULL(vec.data[i]);
        }
        
        vector_destroy(&vec);
    }
    
    // Test 8: Expand within existing capacity
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 10), 0);
        EXPECT_EQUAL_INT(vector_resize(&vec, 5), 0);
        size_t cap = vector_capacity(&vec);
        EXPECT_EQUAL_UINT64(cap, 10);
        
        // Expand but within capacity
        EXPECT_EQUAL_INT(vector_resize(&vec, 7), 0);
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 7);
        EXPECT_EQUAL_UINT64(vector_capacity(&vec), cap);
        
        vector_destroy(&vec);
    }
    
    // Test 9: Resize with destroy function calls destroy on removed elements
    {
        int destroy_count = 0;
        Vector vec;
        vector_init(&vec, destroy_counter);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 10), 0);
        for (size_t i = 0; i < 10; i++) {
            vec.data[i] = &destroy_count;
        }
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 3), 0);  // Should call destroy 7 times
        
        EXPECT_EQUAL_INT(destroy_count, 7);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 3);
        
        vector_destroy(&vec);
    }
    
    // Test 10: Large resize
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 10000), 0);
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 10000);
        EXPECT_EQUAL_UINT64(vector_capacity(&vec), 10000);
        
        vector_destroy(&vec);
    }
}

/**
 * Test: vector_shrink_to_fit
 * Dependencies: vector_init, vector_resize, vector_size, vector_capacity
 * Description: Tests capacity reduction to match size.
 */
UTEST_TEST_CASE(vector_shrink_to_fit){
    // Test 1: Shrink when size equals capacity
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 10), 0);
        int result = vector_shrink_to_fit(&vec);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 10);
        EXPECT_EQUAL_UINT64(vector_capacity(&vec), 10);
        
        vector_destroy(&vec);
    }
    
    // Test 2: Shrink when capacity > size
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 100), 0);
        EXPECT_EQUAL_INT(vector_resize(&vec, 10), 0);
        
        EXPECT_EQUAL_UINT64(vector_capacity(&vec), 100);
        
        int result = vector_shrink_to_fit(&vec);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 10);
        EXPECT_EQUAL_UINT64(vector_capacity(&vec), 10);
        
        vector_destroy(&vec);
    }
    
    // Test 3: Shrink empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 50), 0);
        EXPECT_EQUAL_INT(vector_resize(&vec, 0), 0);
        
        int result = vector_shrink_to_fit(&vec);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 0);
        EXPECT_EQUAL_UINT64(vector_capacity(&vec), 0);
        
        vector_destroy(&vec);
    }
    
    // Test 4: Shrink preserves elements
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 100), 0);
        EXPECT_EQUAL_INT(vector_resize(&vec, 5), 0);
        
        const char *data = "preserved";
        for (size_t i = 0; i < 5; i++) {
            vec.data[i] = (void *)data;
        }
        
        int result = vector_shrink_to_fit(&vec);
        
        EXPECT_EQUAL_INT(result, 0);
        for (size_t i = 0; i < 5; i++) {
            EXPECT_TRUE(vec.data[i] == (void *)data);
        }
        
        vector_destroy(&vec);
    }
}

/**
 * Test: vector_push_back
 * Dependencies: vector_init, vector_size, vector_capacity
 * Description: Tests appending elements to the vector.
 */
UTEST_TEST_CASE(vector_push_back){
    // Test 1: Push to empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        const char *data = "first";
        int result = vector_push_back(&vec, (void *)data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 1);
        EXPECT_GREATER_EQUAL_UINT64(vector_capacity(&vec), 1);
        EXPECT_TRUE(vec.data[0] == (void *)data);
        
        vector_destroy(&vec);
    }
    
    // Test 2: Push multiple elements
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 10; i++) {
            int result = vector_push_back(&vec, (void *)(long)i);
            EXPECT_EQUAL_INT(result, 0);
        }
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 10);
        
        for (int i = 0; i < 10; i++) {
            EXPECT_TRUE(vec.data[i] == (void *)(long)i);
        }
        
        vector_destroy(&vec);
    }
    
    // Test 3: Push NULL pointer
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        int result = vector_push_back(&vec, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 1);
        EXPECT_NULL(vec.data[0]);
        
        vector_destroy(&vec);
    }
    
    // Test 4: Verify capacity growth
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        // First push: capacity should be 1
        int result = vector_push_back(&vec, (void *)1);
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_capacity(&vec), 1);
        
        // Second push: capacity should double to 2
        result = vector_push_back(&vec, (void *)2);
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_capacity(&vec), 2);
        
        // Third push: capacity should double to 4
        result = vector_push_back(&vec, (void *)3);
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_capacity(&vec), 4);
        
        vector_destroy(&vec);
    }
    
    // Test 5: Push after resize with spare capacity
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 5), 0);
        size_t cap = vector_capacity(&vec);
        
        int result = vector_push_back(&vec, (void *)0xABCD);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 6);
        EXPECT_GREATER_UINT64(vector_capacity(&vec), cap);
        
        vector_destroy(&vec);
    }
    
    // Test 6: Large number of pushes
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 1000; i++) {
            int result = vector_push_back(&vec, (void *)(long)i);
            EXPECT_EQUAL_INT(result, 0);
        }
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 1000);
        
        // Verify all elements
        for (int i = 0; i < 1000; i++) {
            EXPECT_TRUE(vec.data[i] == (void *)(long)i);
        }
        
        vector_destroy(&vec);
    }
    
    // Test 7: Push with different pointer values
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        const char *str1 = "hello";
        const char *str2 = "world";
        int num = 42;
        
        EXPECT_EQUAL_INT(vector_push_back(&vec, (void *)str1), 0);
        EXPECT_EQUAL_INT(vector_push_back(&vec, (void *)str2), 0);
        EXPECT_EQUAL_INT(vector_push_back(&vec, &num), 0);
        EXPECT_EQUAL_INT(vector_push_back(&vec, NULL), 0);
        
        EXPECT_TRUE(vec.data[0] == (void *)str1);
        EXPECT_TRUE(vec.data[1] == (void *)str2);
        EXPECT_TRUE(vec.data[2] == &num);
        EXPECT_NULL(vec.data[3]);
        
        vector_destroy(&vec);
    }
}

/**
 * Test: vector_at
 * Dependencies: vector_init, vector_push_back, vector_resize
 * Description: Tests indexed element access with bounds checking.
 */
UTEST_TEST_CASE(vector_at){
    // Test 1: Access elements in valid range
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        const char *data[5] = {"a", "b", "c", "d", "e"};
        for (int i = 0; i < 5; i++) {
            vector_push_back(&vec, (void *)data[i]);
        }
        
        for (int i = 0; i < 5; i++) {
            EXPECT_TRUE(vector_at(&vec, i) == (void *)data[i]);
        }
        
        vector_destroy(&vec);
    }
    
    // Test 2: Access first element
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)0x1234);
        
        EXPECT_TRUE(vector_at(&vec, 0) == (void *)0x1234);
        
        vector_destroy(&vec);
    }
    
    // Test 3: Access last element
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 10; i++) {
            vector_push_back(&vec, (void *)(long)i);
        }
        
        EXPECT_TRUE(vector_at(&vec, 9) == (void *)9);
        
        vector_destroy(&vec);
    }
    
    // Test 4: Access NULL element
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, NULL);
        
        EXPECT_NULL(vector_at(&vec, 0));
        
        vector_destroy(&vec);
    }
    
    // Test 5: Sequential access
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 100; i++) {
            vector_push_back(&vec, (void *)(long)(i * 2));
        }
        
        for (int i = 0; i < 100; i++) {
            EXPECT_TRUE(vector_at(&vec, i) == (void *)(long)(i * 2));
        }
        
        vector_destroy(&vec);
    }
    
    // Note: Out of bounds access causes program termination and cannot be tested
}

/**
 * Test: vector_front
 * Dependencies: vector_init, vector_push_back, vector_resize
 * Description: Tests access to the first element.
 */
UTEST_TEST_CASE(vector_front){
    // Test 1: Front of empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_NULL(vector_front(&vec));
        
        vector_destroy(&vec);
    }
    
    // Test 2: Front of single element vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        const char *data = "only";
        vector_push_back(&vec, (void *)data);
    
        EXPECT_TRUE(vector_front(&vec) == (void *)data);
        
        vector_destroy(&vec);
    }
    
    // Test 3: Front of multiple element vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        const char *first = "first";
        vector_push_back(&vec, (void *)first);
        vector_push_back(&vec, (void *)"second");
        vector_push_back(&vec, (void *)"third");
        
        EXPECT_TRUE(vector_front(&vec) == (void *)first);
        
        vector_destroy(&vec);
    }
    
    // Test 4: Front after resize
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 5), 0);
        vec.data[0] = (void *)0xBEEF;
        
        EXPECT_TRUE(vector_front(&vec) == (void *)0xBEEF);
        
        vector_destroy(&vec);
    }
    
    // Test 5: Front remains same after push_back
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        const char *first = "first";
        vector_push_back(&vec, (void *)first);
        
        for (int i = 0; i < 10; i++) {
            vector_push_back(&vec, (void *)(long)i);
        }
        
        EXPECT_TRUE(vector_front(&vec) == (void *)first);
        
        vector_destroy(&vec);
    }
}

/**
 * Test: vector_back
 * Dependencies: vector_init, vector_push_back, vector_resize
 * Description: Tests access to the last element.
 */
UTEST_TEST_CASE(vector_back){
    // Test 1: Back of empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_NULL(vector_back(&vec));
        
        vector_destroy(&vec);
    }
    
    // Test 2: Back of single element vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        const char *data = "only";
        vector_push_back(&vec, (void *)data);
        
        EXPECT_TRUE(vector_back(&vec) == (void *)data);
        
        vector_destroy(&vec);
    }
    
    // Test 3: Back of multiple element vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)"first");
        vector_push_back(&vec, (void *)"second");
        const char *last = "last";
        vector_push_back(&vec, (void *)last);
        
        EXPECT_TRUE(vector_back(&vec) == (void *)last);
        
        vector_destroy(&vec);
    }
    
    // Test 4: Back changes with push_back
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 10; i++) {
            vector_push_back(&vec, (void *)(long)i);
            EXPECT_TRUE(vector_back(&vec) == (void *)(long)i);
        }
        
        vector_destroy(&vec);
    }
    
    // Test 5: Back after resize
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 5), 0);
        vec.data[4] = (void *)0xDEAD;
        
        EXPECT_TRUE(vector_back(&vec) == (void *)0xDEAD);
        
        vector_destroy(&vec);
    }
    
    // Test 6: Back consistency with vector_at
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 20; i++) {
            vector_push_back(&vec, (void *)(long)i);
        }
        
        EXPECT_TRUE(vector_back(&vec) == vector_at(&vec, vector_size(&vec) - 1));
        
        vector_destroy(&vec);
    }
}

/**
 * Test: vector_pop_back
 * Dependencies: vector_init, vector_push_back, vector_size, vector_back
 * Description: Tests removal of the last element.
 */
UTEST_TEST_CASE(vector_pop_back){
    // Test 1: Pop from empty vector returns failure
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        void* data = NULL;
        int result = vector_pop_back(&vec, &data);
        
        EXPECT_EQUAL_INT(result, -1);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 0);
        
        vector_destroy(&vec);
    }
    
    // Test 2: Pop single element without retrieving data
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)0x1234);
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 1);
        
        int result = vector_pop_back(&vec, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 0);
        
        vector_destroy(&vec);
    }
    
    // Test 3: Pop single element and retrieve data
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        const char* test_data = "test";
        vector_push_back(&vec, (void *)test_data);
        
        void* data = NULL;
        int result = vector_pop_back(&vec, &data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_TRUE(data == (void *)test_data);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 0);
        
        vector_destroy(&vec);
    }
    
    // Test 4: Pop multiple elements
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 10; i++) {
            vector_push_back(&vec, (void *)(long)i);
        }
        
        for (int i = 9; i >= 0; i--) {
            EXPECT_EQUAL_UINT64(vector_size(&vec), (size_t)(i + 1));
            void* data = NULL;
            int result = vector_pop_back(&vec, &data);
            EXPECT_EQUAL_INT(result, 0);
            EXPECT_TRUE(data == (void *)(long)i);
        }
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 0);
        
        vector_destroy(&vec);
    }
    
    // Test 5: Pop updates back
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)1);
        vector_push_back(&vec, (void *)2);
        vector_push_back(&vec, (void *)3);
        
        vector_pop_back(&vec, NULL);
        EXPECT_TRUE(vector_back(&vec) == (void *)2);
        
        vector_pop_back(&vec, NULL);
        EXPECT_TRUE(vector_back(&vec) == (void *)1);
        
        vector_destroy(&vec);
    }
    
    // Test 6: Capacity unchanged after pop
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 10; i++) {
            vector_push_back(&vec, (void *)(long)i);
        }
        
        size_t cap = vector_capacity(&vec);
        
        vector_pop_back(&vec, NULL);
        vector_pop_back(&vec, NULL);
        
        EXPECT_EQUAL_UINT64(vector_capacity(&vec), cap);
        
        vector_destroy(&vec);
    }
    
    // Test 7: Pop clears element slot
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)0xBEEF);
        size_t old_capacity = vector_capacity(&vec);
        
        vector_pop_back(&vec, NULL);
        
        // Element should be NULL (within capacity range)
        if (old_capacity > 0) {
            EXPECT_NULL(vec.data[0]);
        }
        
        vector_destroy(&vec);
    }
    
    // Test 8: Push and pop repeatedly
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int cycle = 0; cycle < 5; cycle++) {
            vector_push_back(&vec, (void *)(long)cycle);
            EXPECT_EQUAL_UINT64(vector_size(&vec), 1);
            
            vector_pop_back(&vec, NULL);
            EXPECT_EQUAL_UINT64(vector_size(&vec), 0);
        }
        
        vector_destroy(&vec);
    }
    
    // Test 9: Pop with destroy function (data not retrieved)
    {
        int destroy_count = 0;
        Vector vec;
        vector_init(&vec, destroy_counter);
        
        vector_push_back(&vec, &destroy_count);
        vector_push_back(&vec, &destroy_count);
        vector_push_back(&vec, &destroy_count);
        
        vector_pop_back(&vec, NULL);
        EXPECT_EQUAL_INT(destroy_count, 1);
        
        vector_pop_back(&vec, NULL);
        EXPECT_EQUAL_INT(destroy_count, 2);
        
        vector_pop_back(&vec, NULL);
        EXPECT_EQUAL_INT(destroy_count, 3);
        
        vector_destroy(&vec);
    }
    
    // Test 10: Pop with data retrieval (destroy not called)
    {
        int destroy_count = 0;
        Vector vec;
        vector_init(&vec, destroy_counter);
        
        vector_push_back(&vec, &destroy_count);
        vector_push_back(&vec, &destroy_count);
        
        void* data = NULL;
        vector_pop_back(&vec, &data);
        
        // Destroy should NOT be called when data is retrieved
        EXPECT_EQUAL_INT(destroy_count, 0);
        EXPECT_NOT_NULL(data);
        
        vector_destroy(&vec);
    }
    
    // Test 11: Pop all then push again
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)1);
        vector_push_back(&vec, (void *)2);
        
        vector_pop_back(&vec, NULL);
        vector_pop_back(&vec, NULL);
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 0);
        
        vector_push_back(&vec, (void *)3);
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 1);
        EXPECT_TRUE(vector_at(&vec, 0) == (void *)3);
        
        vector_destroy(&vec);
    }
}

/**
 * Test: vector_insert
 * Dependencies: vector_init, vector_push_back, vector_at, vector_size
 * Description: Tests inserting elements at arbitrary positions.
 */
UTEST_TEST_CASE(vector_insert){
    // Test 1: Insert at beginning of empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        const char *data = "first";
        int result = vector_insert(&vec, 0, (void *)data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 1);
        EXPECT_TRUE(vector_at(&vec, 0) == (void *)data);
        
        vector_destroy(&vec);
    }
    
    // Test 2: Insert at beginning of non-empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)"second");
        vector_push_back(&vec, (void *)"third");
        
        int result = vector_insert(&vec, 0, (void *)"first");
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 3);
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 0), "first");
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 1), "second");
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 2), "third");
        
        vector_destroy(&vec);
    }
    
    // Test 3: Insert in the middle
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)"first");
        vector_push_back(&vec, (void *)"third");
        
        int result = vector_insert(&vec, 1, (void *)"second");
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 3);
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 0), "first");
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 1), "second");
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 2), "third");
        
        vector_destroy(&vec);
    }
    
    // Test 4: Insert at end (equivalent to push_back)
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)"first");
        vector_push_back(&vec, (void *)"second");
        
        int result = vector_insert(&vec, vector_size(&vec), (void *)"third");
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 3);
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 2), "third");
        
        vector_destroy(&vec);
    }
    
    // Test 5: Multiple insertions at same position
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)"end");
        
        int r1 = vector_insert(&vec, 0, (void *)"c");
        int r2 = vector_insert(&vec, 0, (void *)"b");
        int r3 = vector_insert(&vec, 0, (void *)"a");
        
        EXPECT_EQUAL_INT(r1, 0);
        EXPECT_EQUAL_INT(r2, 0);
        EXPECT_EQUAL_INT(r3, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 4);
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 0), "a");
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 1), "b");
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 2), "c");
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 3), "end");
        
        vector_destroy(&vec);
    }
    
    // Test 6: Insert NULL pointer
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)"first");
        int result = vector_insert(&vec, 1, NULL);
        vector_push_back(&vec, (void *)"third");
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 3);
        EXPECT_NULL(vector_at(&vec, 1));
        
        vector_destroy(&vec);
    }
    
    // Test 7: Insert with capacity growth
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        // Fill to capacity
        vector_push_back(&vec, (void *)1);
        size_t initial_cap = vector_capacity(&vec);
        
        // Insert should trigger growth
        int result = vector_insert(&vec, 0, (void *)0);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 2);
        EXPECT_GREATER_UINT64(vector_capacity(&vec), initial_cap);
        EXPECT_TRUE(vector_at(&vec, 0) == (void *)0);
        EXPECT_TRUE(vector_at(&vec, 1) == (void *)1);
        
        vector_destroy(&vec);
    }
    
    // Test 8: Insert multiple elements sequentially
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 10; i++) {
            int result = vector_insert(&vec, i, (void *)(long)i);
            EXPECT_EQUAL_INT(result, 0);
        }
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 10);
        
        for (int i = 0; i < 10; i++) {
            EXPECT_TRUE(vector_at(&vec, i) == (void *)(long)i);
        }
        
        vector_destroy(&vec);
    }
    
    // Test 9: Insert preserves existing elements
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        const char *elements[] = {"a", "b", "d", "e"};
        for (int i = 0; i < 4; i++) {
            vector_push_back(&vec, (void *)elements[i]);
        }
        
        int result = vector_insert(&vec, 2, (void *)"c");
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 5);
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 0), "a");
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 1), "b");
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 2), "c");
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 3), "d");
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 4), "e");
        
        vector_destroy(&vec);
    }
    
    // Note: Out of bounds insert (index > size) causes termination and cannot be tested
}

/**
 * Test: vector_remove
 * Dependencies: vector_init, vector_push_back, vector_at, vector_size
 * Description: Tests removing elements from arbitrary positions.
 */
UTEST_TEST_CASE(vector_remove){
    // Test 1: Remove only element without retrieving data
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)"only");
        int result = vector_remove(&vec, 0, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 0);
        
        vector_destroy(&vec);
    }
    
    // Test 2: Remove element and retrieve data
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        const char* test_data = "test";
        vector_push_back(&vec, (void *)test_data);
        
        void* data = NULL;
        int result = vector_remove(&vec, 0, &data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_TRUE(data == (void *)test_data);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 0);
        
        vector_destroy(&vec);
    }
    
    // Test 3: Remove first element
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)"first");
        vector_push_back(&vec, (void *)"second");
        vector_push_back(&vec, (void *)"third");
        
        int result = vector_remove(&vec, 0, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 2);
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 0), "second");
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 1), "third");
        
        vector_destroy(&vec);
    }
    
    // Test 4: Remove middle element
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)"first");
        vector_push_back(&vec, (void *)"second");
        vector_push_back(&vec, (void *)"third");
        
        int result = vector_remove(&vec, 1, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 2);
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 0), "first");
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 1), "third");
        
        vector_destroy(&vec);
    }
    
    // Test 5: Remove last element
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)"first");
        vector_push_back(&vec, (void *)"second");
        vector_push_back(&vec, (void *)"third");
        
        int result = vector_remove(&vec, 2, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 2);
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 0), "first");
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 1), "second");
        
        vector_destroy(&vec);
    }
    
    // Test 6: Remove multiple elements sequentially
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 10; i++) {
            vector_push_back(&vec, (void *)(long)i);
        }
        
        // Remove every other element from the front
        for (int i = 0; i < 5; i++) {
            int result = vector_remove(&vec, 0, NULL);
            EXPECT_EQUAL_INT(result, 0);
        }
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 5);
        
        // Remaining elements should be 5-9
        for (int i = 0; i < 5; i++) {
            EXPECT_TRUE(vector_at(&vec, i) == (void *)(long)(i + 5));
        }
        
        vector_destroy(&vec);
    }
    
    // Test 7: Capacity unchanged after remove
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 10; i++) {
            vector_push_back(&vec, (void *)(long)i);
        }
        
        size_t cap = vector_capacity(&vec);
        
        vector_remove(&vec, 5, NULL);
        vector_remove(&vec, 3, NULL);
        
        EXPECT_EQUAL_UINT64(vector_capacity(&vec), cap);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 8);
        
        vector_destroy(&vec);
    }
    
    // Test 8: Remove with destroy function (data not retrieved)
    {
        int destroy_count = 0;
        Vector vec;
        vector_init(&vec, destroy_counter);
        
        vector_push_back(&vec, &destroy_count);
        vector_push_back(&vec, &destroy_count);
        vector_push_back(&vec, &destroy_count);
        
        int result = vector_remove(&vec, 1, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_INT(destroy_count, 1);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 2);
        
        vector_destroy(&vec);
    }
    
    // Test 9: Remove with data retrieval (destroy not called)
    {
        int destroy_count = 0;
        Vector vec;
        vector_init(&vec, destroy_counter);
        
        vector_push_back(&vec, &destroy_count);
        vector_push_back(&vec, &destroy_count);
        
        void* data = NULL;
        int result = vector_remove(&vec, 0, &data);
        
        EXPECT_EQUAL_INT(result, 0);
        // Destroy should NOT be called when data is retrieved
        EXPECT_EQUAL_INT(destroy_count, 0);
        EXPECT_NOT_NULL(data);
        
        vector_destroy(&vec);
    }
    
    // Test 10: Remove all elements one by one from front
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 5; i++) {
            vector_push_back(&vec, (void *)(long)i);
        }
        
        for (int i = 0; i < 5; i++) {
            void* data = NULL;
            int result = vector_remove(&vec, 0, &data);
            EXPECT_EQUAL_INT(result, 0);
            EXPECT_TRUE(data == (void *)(long)i);
        }
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 0);
        
        vector_destroy(&vec);
    }
    
    // Test 11: Remove all elements one by one from back
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 5; i++) {
            vector_push_back(&vec, (void *)(long)i);
        }
        
        // Always remove last element
        for (int i = 4; i >= 0; i--) {
            int result = vector_remove(&vec, vector_size(&vec) - 1, NULL);
            EXPECT_EQUAL_INT(result, 0);
        }
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 0);
        
        vector_destroy(&vec);
    }
    
    // Test 12: Remove then insert at same position
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)"a");
        vector_push_back(&vec, (void *)"b");
        vector_push_back(&vec, (void *)"c");
        
        vector_remove(&vec, 1, NULL);
        vector_insert(&vec, 1, (void *)"new");
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 3);
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 0), "a");
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 1), "new");
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 2), "c");
        
        vector_destroy(&vec);
    }
    
    // Note: Out of bounds remove (index >= size) causes termination and cannot be tested
}

/**
 * Test: vector_clear
 * Dependencies: vector_init, vector_push_back, vector_size, vector_capacity
 * Description: Tests clearing all elements from the vector.
 */
UTEST_TEST_CASE(vector_clear){
    // Test 1: Clear empty vector
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_clear(&vec);
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 0);
        
        vector_destroy(&vec);
    }
    
    // Test 2: Clear single element
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)"element");
        vector_clear(&vec);
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 0);
        
        vector_destroy(&vec);
    }
    
    // Test 3: Clear multiple elements
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 10; i++) {
            vector_push_back(&vec, (void *)(long)i);
        }
        
        vector_clear(&vec);
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 0);
        
        vector_destroy(&vec);
    }
    
    // Test 4: Capacity unchanged after clear
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 10; i++) {
            vector_push_back(&vec, (void *)(long)i);
        }
        
        size_t cap = vector_capacity(&vec);
        
        vector_clear(&vec);
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 0);
        EXPECT_EQUAL_UINT64(vector_capacity(&vec), cap);
        
        vector_destroy(&vec);
    }
    
    // Test 5: Clear with destroy function
    {
        int destroy_count = 0;
        Vector vec;
        vector_init(&vec, destroy_counter);
        
        for (int i = 0; i < 100; i++) {
            vector_push_back(&vec, &destroy_count);
        }
        
        vector_clear(&vec);
        
        EXPECT_EQUAL_INT(destroy_count, 100);
        EXPECT_EQUAL_UINT64(vector_size(&vec), 0);
        
        vector_destroy(&vec);
    }
    
    // Test 6: Push after clear
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)"before");
        vector_clear(&vec);
        
        vector_push_back(&vec, (void *)"after");
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 1);
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 0), "after");
        
        vector_destroy(&vec);
    }
    
    // Test 7: Multiple clear operations
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int cycle = 0; cycle < 3; cycle++) {
            for (int i = 0; i < 5; i++) {
                vector_push_back(&vec, (void *)(long)i);
            }
            
            EXPECT_EQUAL_UINT64(vector_size(&vec), 5);
            
            vector_clear(&vec);
            
            EXPECT_EQUAL_UINT64(vector_size(&vec), 0);
        }
        
        vector_destroy(&vec);
    }
    
    // Test 8: Clear and shrink_to_fit
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 100; i++) {
            vector_push_back(&vec, (void *)(long)i);
        }
        
        vector_clear(&vec);
        vector_shrink_to_fit(&vec);
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 0);
        EXPECT_EQUAL_UINT64(vector_capacity(&vec), 0);
        
        vector_destroy(&vec);
    }
    
    // Test 9: Clear preserves capacity for reuse
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 50; i++) {
            vector_push_back(&vec, (void *)(long)i);
        }
        
        size_t cap_before = vector_capacity(&vec);
        vector_clear(&vec);
        size_t cap_after = vector_capacity(&vec);
        
        EXPECT_EQUAL_UINT64(cap_before, cap_after);
        
        // Should be able to push without reallocation
        for (int i = 0; i < 50; i++) {
            vector_push_back(&vec, (void *)(long)i);
        }
        
        EXPECT_EQUAL_UINT64(vector_capacity(&vec), cap_after);
        
        vector_destroy(&vec);
    }
    
    // Test 10: Clear then resize
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 10; i++) {
            EXPECT_EQUAL_INT(vector_push_back(&vec, (void *)(long)i), 0);
        }
        
        vector_clear(&vec);
        EXPECT_EQUAL_INT(vector_resize(&vec, 5), 0);
        
        EXPECT_EQUAL_UINT64(vector_size(&vec), 5);
        
        // All elements should be NULL
        for (size_t i = 0; i < 5; i++) {
            EXPECT_NULL(vector_at(&vec, i));
        }
        
        vector_destroy(&vec);
    }
}

/**
 * Test: vector_empty
 * Dependencies: vector_init, vector_push_back, vector_pop_back
 * Description: Tests checking if the vector is empty.
 */
UTEST_TEST_CASE(vector_empty){
    // Test 1: Empty after initialization
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_TRUE(vector_empty(&vec));
        
        vector_destroy(&vec);
    }
    
    // Test 2: Not empty after push
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)"element");
        
        EXPECT_FALSE(vector_empty(&vec));
        
        vector_destroy(&vec);
    }
    
    // Test 3: Empty after push and pop
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)"element");
        EXPECT_FALSE(vector_empty(&vec));
        
        vector_pop_back(&vec, NULL);
        EXPECT_TRUE(vector_empty(&vec));
        
        vector_destroy(&vec);
    }
    
    // Test 4: Empty after clear
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 10; i++) {
            vector_push_back(&vec, (void *)(long)i);
        }
        
        EXPECT_FALSE(vector_empty(&vec));
        
        vector_clear(&vec);
        
        EXPECT_TRUE(vector_empty(&vec));
        
        vector_destroy(&vec);
    }
    
    // Test 5: Empty after resize to zero
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 10), 0);
        EXPECT_FALSE(vector_empty(&vec));
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 0), 0);
        EXPECT_TRUE(vector_empty(&vec));
        
        vector_destroy(&vec);
    }
    
    // Test 6: Not empty after resize from zero
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_TRUE(vector_empty(&vec));
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 5), 0);
        
        EXPECT_FALSE(vector_empty(&vec));
        
        vector_destroy(&vec);
    }
    
    // Test 7: Empty consistency with size
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_TRUE(vector_empty(&vec) == (vector_size(&vec) == 0));
        
        vector_push_back(&vec, (void *)1);
        EXPECT_TRUE(vector_empty(&vec) == (vector_size(&vec) == 0));
        
        vector_push_back(&vec, (void *)2);
        EXPECT_TRUE(vector_empty(&vec) == (vector_size(&vec) == 0));
        
        vector_destroy(&vec);
    }
    
    // Test 8: Empty after multiple operations
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        // Start empty
        EXPECT_TRUE(vector_empty(&vec));
        
        // Add elements
        for (int i = 0; i < 5; i++) {
            vector_push_back(&vec, (void *)(long)i);
        }
        EXPECT_FALSE(vector_empty(&vec));
        
        // Remove all
        for (int i = 0; i < 5; i++) {
            vector_pop_back(&vec, NULL);
        }
        EXPECT_TRUE(vector_empty(&vec));
        
        vector_destroy(&vec);
    }
}

/**
 * Test: vector_data
 * Dependencies: vector_init, vector_push_back, vector_resize
 * Description: Tests getting the internal data pointer.
 */
UTEST_TEST_CASE(vector_data){
    // Test 1: Data of empty vector is NULL
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_NULL(vector_data(&vec));
        
        vector_destroy(&vec);
    }
    
    // Test 2: Data pointer after push
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        const char *elem = "test";
        vector_push_back(&vec, (void *)elem);
        
        void** data = vector_data(&vec);
        EXPECT_NOT_NULL(data);
        EXPECT_TRUE(data[0] == (void *)elem);
        
        vector_destroy(&vec);
    }
    
    // Test 3: Data consistency with direct member access
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)"test");
        
        EXPECT_TRUE(vector_data(&vec) == vec.data);
        
        vector_destroy(&vec);
    }
    
    // Test 4: Direct access via data pointer
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        const char *elements[] = {"a", "b", "c", "d", "e"};
        for (int i = 0; i < 5; i++) {
            vector_push_back(&vec, (void *)elements[i]);
        }
        
        void** data = vector_data(&vec);
        
        for (int i = 0; i < 5; i++) {
            EXPECT_TRUE(data[i] == (void *)elements[i]);
        }
        
        vector_destroy(&vec);
    }
    
    // Test 5: Data pointer with resize
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 10), 0);
        
        void** data = vector_data(&vec);
        EXPECT_NOT_NULL(data);
        
        // All should be NULL
        for (size_t i = 0; i < 10; i++) {
            EXPECT_NULL(data[i]);
        }
        
        vector_destroy(&vec);
    }
    
    // Test 6: Modify through data pointer
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 3), 0);
        
        void** data = vector_data(&vec);
        data[0] = (void *)"first";
        data[1] = (void *)"second";
        data[2] = (void *)"third";
        
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 0), "first");
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 1), "second");
        EXPECT_EQUAL_STRING((char*)vector_at(&vec, 2), "third");
        
        vector_destroy(&vec);
    }
    
    // Test 7: Data pointer changes after reallocation
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        vector_push_back(&vec, (void *)1);
        void** data_before = vector_data(&vec);
        
        // Trigger reallocation
        for (int i = 0; i < 100; i++) {
            vector_push_back(&vec, (void *)(long)i);
        }
        
        void** data_after = vector_data(&vec);
        
        // Pointer should have changed due to reallocation
        EXPECT_TRUE(data_before != data_after);
        
        vector_destroy(&vec);
    }
    
    // Test 8: Iterate using data pointer
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 20; i++) {
            vector_push_back(&vec, (void *)(long)(i * 2));
        }
        
        void** data = vector_data(&vec);
        
        for (int i = 0; i < 20; i++) {
            EXPECT_TRUE(data[i] == (void *)(long)(i * 2));
        }
        
        vector_destroy(&vec);
    }
    
    // Test 9: Data pointer after clear still valid but size is zero
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        for (int i = 0; i < 10; i++) {
            vector_push_back(&vec, (void *)(long)i);
        }
        
        void** data_before = vector_data(&vec);
        
        vector_clear(&vec);
        
        void** data_after = vector_data(&vec);
        
        // Pointer should be same (capacity unchanged)
        EXPECT_TRUE(data_before == data_after);
        EXPECT_NOT_NULL(data_after);
        
        vector_destroy(&vec);
    }
    
    // Test 10: Data pointer with shrink_to_fit on empty
    {
        Vector vec;
        vector_init(&vec, NULL);
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 10), 0);
        EXPECT_NOT_NULL(vector_data(&vec));
        
        EXPECT_EQUAL_INT(vector_resize(&vec, 0), 0);
        EXPECT_EQUAL_INT(vector_shrink_to_fit(&vec), 0);
        
        EXPECT_NULL(vector_data(&vec));
        
        vector_destroy(&vec);
    }
}

/**
 * Test Suite: vector
 * Runs all vector test cases in dependency order.
 */
UTEST_TEST_SUITE(vector){
    UTEST_RUN_TEST_CASE(vector_init);
    UTEST_RUN_TEST_CASE(vector_size);
    UTEST_RUN_TEST_CASE(vector_capacity);
    UTEST_RUN_TEST_CASE(vector_destroy);
    UTEST_RUN_TEST_CASE(vector_resize);
    UTEST_RUN_TEST_CASE(vector_shrink_to_fit);
    UTEST_RUN_TEST_CASE(vector_push_back);
    UTEST_RUN_TEST_CASE(vector_at);
    UTEST_RUN_TEST_CASE(vector_front);
    UTEST_RUN_TEST_CASE(vector_back);
    UTEST_RUN_TEST_CASE(vector_pop_back);
    UTEST_RUN_TEST_CASE(vector_empty);
    UTEST_RUN_TEST_CASE(vector_data);
    UTEST_RUN_TEST_CASE(vector_insert);
    UTEST_RUN_TEST_CASE(vector_remove);
    UTEST_RUN_TEST_CASE(vector_clear);
}

