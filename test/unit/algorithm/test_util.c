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

#include <util/util.h>
#include <utest.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

/**
 * Test: util_swap
 * Dependencies: None
 * Description: Tests the util_swap function for swapping elements of various types and sizes.
 */
UTEST_TEST_CASE(util_swap) {
    // Test 1: Swap two integers
    {
        int a = 10, b = 20;
        int result = util_swap(&a, &b, sizeof(int));
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(a, 20);
        EXPECT_EQUAL_INT(b, 10);
    }

    // Test 2: Swap two doubles
    {
        double a = 3.14, b = 2.71;
        int result = util_swap(&a, &b, sizeof(double));
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_EQUAL_DOUBLE(a, 2.71);
        EXPECT_EQUAL_DOUBLE(b, 3.14);
    }

    // Test 3: Swap two characters
    {
        char a = 'A', b = 'B';
        int result = util_swap(&a, &b, sizeof(char));
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(a, 'B');
        EXPECT_EQUAL_INT(b, 'A');
    }

    // Test 4: Swap two strings (char pointers)
    {
        char *a = "hello";
        char *b = "world";
        char *temp_a = a;
        char *temp_b = b;
        int result = util_swap(&a, &b, sizeof(char *));
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(a == temp_b);
        EXPECT_TRUE(b == temp_a);
    }

    // Test 5: Swap arrays of integers
    {
        int arr1[] = {1, 2, 3, 4, 5};
        int arr2[] = {6, 7, 8, 9, 10};
        int result = util_swap(arr1, arr2, sizeof(arr1));
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(arr1[0], 6);
        EXPECT_EQUAL_INT(arr1[4], 10);
        EXPECT_EQUAL_INT(arr2[0], 1);
        EXPECT_EQUAL_INT(arr2[4], 5);
    }

    // Test 6: Swap same pointer (should succeed)
    {
        int a = 42;
        int original = a;
        int result = util_swap(&a, &a, sizeof(int));
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(a, original);
    }

    // Test 7: Swap with size 0 (should fail)
    {
        int a = 10, b = 20;
        int result = util_swap(&a, &b, 0);
        EXPECT_EQUAL_INT(result, COLLECTION_FAILURE);
        EXPECT_EQUAL_INT(a, 10);
        EXPECT_EQUAL_INT(b, 20);
    }

    // Test 8: Swap with NULL first pointer (should fail)
    {
        int b = 20;
        int result = util_swap(NULL, &b, sizeof(int));
        EXPECT_EQUAL_INT(result, COLLECTION_FAILURE);
        EXPECT_EQUAL_INT(b, 20);
    }

    // Test 9: Swap with NULL second pointer (should fail)
    {
        int a = 10;
        int result = util_swap(&a, NULL, sizeof(int));
        EXPECT_EQUAL_INT(result, COLLECTION_FAILURE);
        EXPECT_EQUAL_INT(a, 10);
    }

    // Test 10: Swap with both NULL pointers (should fail)
    {
        int result = util_swap(NULL, NULL, sizeof(int));
        EXPECT_EQUAL_INT(result, COLLECTION_FAILURE);
    }

    // Test 11: Swap large structures
    {
        struct {
            int id;
            char name[100];
            double value;
        } struct1 = {1, "test1", 1.5};
        struct {
            int id;
            char name[100];
            double value;
        } struct2 = {2, "test2", 2.5};
        
        int result = util_swap(&struct1, &struct2, sizeof(struct1));
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(struct1.id, 2);
        EXPECT_EQUAL_INT(struct2.id, 1);
        EXPECT_EQUAL_INT(strcmp(struct1.name, "test2"), 0);
        EXPECT_EQUAL_INT(strcmp(struct2.name, "test1"), 0);
        EXPECT_EQUAL_DOUBLE(struct1.value, 2.5);
        EXPECT_EQUAL_DOUBLE(struct2.value, 1.5);
    }

    // Test 12: Swap very small size (1 byte)
    {
        unsigned char a = 0xAA, b = 0x55;
        int result = util_swap(&a, &b, 1);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(a, 0x55);
        EXPECT_EQUAL_INT(b, 0xAA);
    }

    // Test 13: Swap with overlapping memory (different objects, should work)
    {
        int arr[] = {1, 2, 3, 4, 5};
        int original[] = {1, 2, 3, 4, 5};
        int result = util_swap(&arr[0], &arr[2], sizeof(int));
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(arr[0], 3);
        EXPECT_EQUAL_INT(arr[2], 1);
        EXPECT_EQUAL_INT(arr[1], original[1]);
        EXPECT_EQUAL_INT(arr[3], original[3]);
        EXPECT_EQUAL_INT(arr[4], original[4]);
    }

    // Test 14: Swap long long integers
    {
        long long a = LLONG_MAX, b = LLONG_MIN;
        int result = util_swap(&a, &b, sizeof(long long));
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT64(a, LLONG_MIN);
        EXPECT_EQUAL_INT64(b, LLONG_MAX);
    }

    // Test 15: Swap float values
    {
        float a = 1.5f, b = 2.5f;
        int result = util_swap(&a, &b, sizeof(float));
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_EQUAL_FLOAT(a, 2.5f);
        EXPECT_EQUAL_FLOAT(b, 1.5f);
    }

    // Test 16: Swap multiple times
    {
        int a = 10, b = 20;
        util_swap(&a, &b, sizeof(int));
        EXPECT_EQUAL_INT(a, 20);
        EXPECT_EQUAL_INT(b, 10);
        util_swap(&a, &b, sizeof(int));
        EXPECT_EQUAL_INT(a, 10);
        EXPECT_EQUAL_INT(b, 20);
        util_swap(&a, &b, sizeof(int));
        EXPECT_EQUAL_INT(a, 20);
        EXPECT_EQUAL_INT(b, 10);
    }

    // Test 17: Swap with maximum size (large buffer)
    {
        size_t large_size = 1024;
        char *buf1 = malloc(large_size);
        char *buf2 = malloc(large_size);
        
        if (buf1 && buf2) {
            memset(buf1, 0xAA, large_size);
            memset(buf2, 0x55, large_size);
            
            int result = util_swap(buf1, buf2, large_size);
            EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
            
            for (size_t i = 0; i < large_size; i++) {
                EXPECT_EQUAL_INT((unsigned char)buf1[i], 0x55);
                EXPECT_EQUAL_INT((unsigned char)buf2[i], 0xAA);
            }
        }
        
        free(buf1);
        free(buf2);
    }
}

/**
 * Test: util_random
 * Dependencies: None
 * Description: Tests the util_random function for generating random integers within a range.
 */
UTEST_TEST_CASE(util_random) {
    // Test 1: Random number in positive range
    {
        int min = 1, max = 10;
        int result = util_random(min, max);
        EXPECT_TRUE(result >= min);
        EXPECT_TRUE(result <= max);
    }

    // Test 2: Random number in larger range
    {
        int min = 0, max = 100;
        int result = util_random(min, max);
        EXPECT_TRUE(result >= min);
        EXPECT_TRUE(result <= max);
    }

    // Test 3: Single value range (min == max)
    {
        int min = 5, max = 5;
        int result = util_random(min, max);
        EXPECT_EQUAL_INT(result, 5);
    }

    // Test 4: Zero range
    {
        int min = 0, max = 0;
        int result = util_random(min, max);
        EXPECT_EQUAL_INT(result, 0);
    }

    // Test 5: Negative range
    {
        int min = -10, max = -1;
        int result = util_random(min, max);
        EXPECT_TRUE(result >= min);
        EXPECT_TRUE(result <= max);
    }

    // Test 6: Range spanning zero
    {
        int min = -5, max = 5;
        int result = util_random(min, max);
        EXPECT_TRUE(result >= min);
        EXPECT_TRUE(result <= max);
    }

    // Test 7: Large positive range
    {
        int min = 1000, max = 9999;
        int result = util_random(min, max);
        EXPECT_TRUE(result >= min);
        EXPECT_TRUE(result <= max);
    }

    // Test 8: Large negative range
    {
        int min = -9999, max = -1000;
        int result = util_random(min, max);
        EXPECT_TRUE(result >= min);
        EXPECT_TRUE(result <= max);
    }

    // Test 9: Range with INT_MAX
    {
        int min = INT_MAX - 100, max = INT_MAX;
        int result = util_random(min, max);
        EXPECT_TRUE(result >= min);
        EXPECT_TRUE(result <= max);
    }

    // Test 10: Range with INT_MIN
    {
        int min = INT_MIN, max = INT_MIN + 100;
        int result = util_random(min, max);
        EXPECT_TRUE(result >= min);
        EXPECT_TRUE(result <= max);
    }

    // Test 11: Full integer range
    {
        int min = INT_MIN, max = INT_MAX;
        int result = util_random(min, max);
        EXPECT_TRUE(result >= min);
        EXPECT_TRUE(result <= max);
    }

    // Test 12: Multiple calls produce values in range
    {
        int min = 1, max = 100;
        int results[1000];
        for (int i = 0; i < 1000; i++) {
            results[i] = util_random(min, max);
            EXPECT_TRUE(results[i] >= min);
            EXPECT_TRUE(results[i] <= max);
        }
    }

    // Test 13: Small range
    {
        int min = 10, max = 11;
        int result = util_random(min, max);
        EXPECT_TRUE(result == 10 || result == 11);
    }

    // Test 14: Range with one negative and one positive
    {
        int min = -1, max = 1;
        int result = util_random(min, max);
        EXPECT_TRUE(result >= min);
        EXPECT_TRUE(result <= max);
    }

    // Test 15: Very small range near zero
    {
        int min = -2, max = 2;
        int result = util_random(min, max);
        EXPECT_TRUE(result >= min);
        EXPECT_TRUE(result <= max);
    }

    // Test 16: Range with min > max (edge case - behavior depends on implementation)
    {
        int min = 10, max = 5;
        int result = util_random(min, max);
        (void)result;
    }
}

/**
 * Test suite: util
 * Description: Test suite for util utility functions
 */
UTEST_TEST_SUITE(util) {
    UTEST_RUN_TEST_CASE(util_swap);
    UTEST_RUN_TEST_CASE(util_random);
}

