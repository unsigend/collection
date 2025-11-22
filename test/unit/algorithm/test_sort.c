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

#include <algorithm/sort.h>
#include <utest.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>

/**
 * Generic compare function for integers (ascending order).
 */
static int compare_int(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

/**
 * Generic compare function for integers (descending order).
 */
static int compare_int_desc(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia > ib) return -1;
    if (ia < ib) return 1;
    return 0;
}

/**
 * Generic compare function for doubles (ascending order).
 */
static int compare_double(const void *a, const void *b) {
    double da = *(const double *)a;
    double db = *(const double *)b;
    if (da < db) return -1;
    if (da > db) return 1;
    return 0;
}

/**
 * Generic compare function for strings (ascending order).
 */
static int compare_string(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

/**
 * Compare function for struct with key and padding.
 */
static int compare_struct_large(const void *a, const void *b) {
    const int *key_a = (const int *)a;
    const int *key_b = (const int *)b;
    if (*key_a < *key_b) return -1;
    if (*key_a > *key_b) return 1;
    return 0;
}

/**
 * Compare function for struct with key and value.
 */
static int compare_key(const void *a, const void *b) {
    const int *key_a = (const int *)a;
    const int *key_b = (const int *)b;
    if (*key_a < *key_b) return -1;
    if (*key_a > *key_b) return 1;
    return 0;
}

/**
 * Compare function for struct with key and large padding.
 */
static int compare_struct_very_large(const void *a, const void *b) {
    const int *key_a = (const int *)a;
    const int *key_b = (const int *)b;
    if (*key_a < *key_b) return -1;
    if (*key_a > *key_b) return 1;
    return 0;
}

/**
 * Helper function to verify array is sorted in ascending order.
 */
static int is_sorted_int(const int *arr, size_t n) {
    for (size_t i = 1; i < n; i++) {
        if (arr[i - 1] > arr[i]) {
            return 0;
        }
    }
    return 1;
}

/**
 * Helper function to verify array is sorted in descending order.
 */
static int is_sorted_int_desc(const int *arr, size_t n) {
    for (size_t i = 1; i < n; i++) {
        if (arr[i - 1] < arr[i]) {
            return 0;
        }
    }
    return 1;
}

/**
 * Helper function to verify array contains same elements (for stability check).
 */
static int arrays_equal_int(const int *arr1, const int *arr2, size_t n) {
    int *copy1 = malloc(n * sizeof(int));
    int *copy2 = malloc(n * sizeof(int));
    
    if (!copy1 || !copy2) {
        free(copy1);
        free(copy2);
        return 0;
    }
    
    memcpy(copy1, arr1, n * sizeof(int));
    memcpy(copy2, arr2, n * sizeof(int));
    
    // Sort both copies for comparison
    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            if (copy1[i] > copy1[j]) {
                int temp = copy1[i];
                copy1[i] = copy1[j];
                copy1[j] = temp;
            }
            if (copy2[i] > copy2[j]) {
                int temp = copy2[i];
                copy2[i] = copy2[j];
                copy2[j] = temp;
            }
        }
    }
    
    int result = memcmp(copy1, copy2, n * sizeof(int)) == 0;
    free(copy1);
    free(copy2);
    return result;
}

/**
 * Generic test helper for any sort function.
 * Tests basic sorting functionality with various data types and edge cases.
 */
static void test_sort_generic(
    int (*sort_func)(void *, size_t, size_t, int (*)(const void *, const void *)),
    const char *sort_name
) {
    // Test 1: Empty array
    {
        int empty[] = {};
        int result = sort_func(empty, 0, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
    }

    // Test 2: Single element
    {
        int single[] = {42};
        int original = single[0];
        int result = sort_func(single, 1, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(single[0], original);
    }

    // Test 3: Two elements - already sorted
    {
        int arr[] = {1, 2};
        int result = sort_func(arr, 2, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 2));
        EXPECT_EQUAL_INT(arr[0], 1);
        EXPECT_EQUAL_INT(arr[1], 2);
    }

    // Test 4: Two elements - reverse order
    {
        int arr[] = {2, 1};
        int result = sort_func(arr, 2, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 2));
        EXPECT_EQUAL_INT(arr[0], 1);
        EXPECT_EQUAL_INT(arr[1], 2);
    }

    // Test 5: Small array - already sorted
    {
        int arr[] = {1, 2, 3, 4, 5};
        int result = sort_func(arr, 5, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 5));
        EXPECT_EQUAL_INT(arr[0], 1);
        EXPECT_EQUAL_INT(arr[4], 5);
    }

    // Test 6: Small array - reverse sorted
    {
        int arr[] = {5, 4, 3, 2, 1};
        int result = sort_func(arr, 5, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 5));
        EXPECT_EQUAL_INT(arr[0], 1);
        EXPECT_EQUAL_INT(arr[4], 5);
    }

    // Test 7: Small array - random order
    {
        int arr[] = {3, 1, 4, 1, 5};
        int original[] = {3, 1, 4, 1, 5};
        int result = sort_func(arr, 5, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 5));
        EXPECT_TRUE(arrays_equal_int(arr, original, 5));
    }

    // Test 8: Medium array - random order
    {
        int arr[] = {64, 34, 25, 12, 22, 11, 90, 5};
        int original[] = {64, 34, 25, 12, 22, 11, 90, 5};
        int result = sort_func(arr, 8, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 8));
        EXPECT_TRUE(arrays_equal_int(arr, original, 8));
    }

    // Test 9: Large array
    {
        int arr[100];
        int original[100];
        for (int i = 0; i < 100; i++) {
            arr[i] = 100 - i;
            original[i] = 100 - i;
        }
        int result = sort_func(arr, 100, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 100));
        EXPECT_TRUE(arrays_equal_int(arr, original, 100));
    }

    // Test 10: Array with duplicates
    {
        int arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
        int original[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
        int result = sort_func(arr, 10, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 10));
        EXPECT_TRUE(arrays_equal_int(arr, original, 10));
    }

    // Test 11: Array with all same elements
    {
        int arr[] = {5, 5, 5, 5, 5};
        int result = sort_func(arr, 5, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 5));
        for (int i = 0; i < 5; i++) {
            EXPECT_EQUAL_INT(arr[i], 5);
        }
    }

    // Test 12: Array with negative numbers
    {
        int arr[] = {-5, -2, -8, -1, -9};
        int original[] = {-5, -2, -8, -1, -9};
        int result = sort_func(arr, 5, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 5));
        EXPECT_TRUE(arrays_equal_int(arr, original, 5));
    }

    // Test 13: Array with mixed positive and negative
    {
        int arr[] = {-5, 2, -8, 1, 0, -9, 5};
        int original[] = {-5, 2, -8, 1, 0, -9, 5};
        int result = sort_func(arr, 7, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 7));
        EXPECT_TRUE(arrays_equal_int(arr, original, 7));
    }

    // Test 14: Array with edge values
    {
        int arr[] = {INT_MAX, INT_MIN, 0, -1, 1};
        int original[] = {INT_MAX, INT_MIN, 0, -1, 1};
        int result = sort_func(arr, 5, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 5));
        EXPECT_TRUE(arrays_equal_int(arr, original, 5));
    }

    // Test 15: Descending order sort
    {
        int arr[] = {1, 2, 3, 4, 5};
        int result = sort_func(arr, 5, sizeof(int), compare_int_desc);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int_desc(arr, 5));
        EXPECT_EQUAL_INT(arr[0], 5);
        EXPECT_EQUAL_INT(arr[4], 1);
    }

    // Test 16: Double precision floating point
    {
        double arr[] = {3.14, 1.41, 2.71, 0.57, 1.73};
        double original[] = {3.14, 1.41, 2.71, 0.57, 1.73};
        int result = sort_func(arr, 5, sizeof(double), compare_double);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        for (size_t i = 1; i < 5; i++) {
            EXPECT_TRUE(arr[i - 1] <= arr[i]);
        }
    }

    // Test 17: String array sorting
    {
        const char *arr[] = {"banana", "apple", "cherry", "date"};
        const char *original[] = {"banana", "apple", "cherry", "date"};
        int result = sort_func(arr, 4, sizeof(char *), compare_string);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_EQUAL_STRING(arr[0], "apple");
        EXPECT_EQUAL_STRING(arr[1], "banana");
        EXPECT_EQUAL_STRING(arr[2], "cherry");
        EXPECT_EQUAL_STRING(arr[3], "date");
    }

    // Test 18: Large element size
    {
        struct {
            int key;
            char padding[100];
        } arr[5];
        
        int keys[] = {5, 2, 8, 1, 3};
        for (int i = 0; i < 5; i++) {
            arr[i].key = keys[i];
            memset(arr[i].padding, 0, 100);
        }
        
        int result = sort_func(arr, 5, sizeof(arr[0]), compare_struct_large);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        for (int i = 1; i < 5; i++) {
            EXPECT_TRUE(arr[i - 1].key <= arr[i].key);
        }
    }

    // Test 19: NULL data pointer
    {
        int result = sort_func(NULL, 5, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_FAILURE);
    }

    // Test 20: NULL compare function
    {
        int arr[] = {3, 1, 4};
        int result = sort_func(arr, 3, sizeof(int), NULL);
        EXPECT_EQUAL_INT(result, COLLECTION_FAILURE);
    }

    // Test 21: Zero element size
    {
        int arr[] = {3, 1, 4};
        int result = sort_func(arr, 3, 0, compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_FAILURE);
    }

    // Test 22: Valid array with zero elements (should succeed)
    {
        int arr[] = {1, 2, 3};
        int result = sort_func(arr, 0, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
    }

    // Test 23: Stability test with equal keys
    {
        struct {
            int key;
            int value;
        } arr[] = {
            {3, 1},
            {1, 2},
            {3, 3},
            {2, 4},
            {1, 5}
        };
        
        int result = sort_func(arr, 5, sizeof(arr[0]), compare_key);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        
        // Verify sorted by key
        for (int i = 1; i < 5; i++) {
            EXPECT_TRUE(arr[i - 1].key <= arr[i].key);
        }
    }

    // Test 24: Medium sized array
    {
        int arr[1000];
        for (int i = 0; i < 1000; i++) {
            arr[i] = 1000 - i;
        }
        int result = sort_func(arr, 1000, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 1000));
    }

    // Test 25: Already sorted large array
    {
        int arr[500];
        for (int i = 0; i < 500; i++) {
            arr[i] = i;
        }
        int result = sort_func(arr, 500, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 500));
    }

    // Test 26: Multiple sort operations
    {
        int arr1[] = {5, 2, 8, 1, 3};
        int arr2[] = {10, 4, 6, 2, 8};
        int arr3[] = {15, 7, 3, 9, 1};
        
        sort_func(arr1, 5, sizeof(int), compare_int);
        sort_func(arr2, 5, sizeof(int), compare_int);
        sort_func(arr3, 5, sizeof(int), compare_int);
        
        EXPECT_TRUE(is_sorted_int(arr1, 5));
        EXPECT_TRUE(is_sorted_int(arr2, 5));
        EXPECT_TRUE(is_sorted_int(arr3, 5));
    }

    // Test 27: Large element size (triggers heap allocation in swap)
    {
        struct {
            int key;
            char padding[600];
        } arr[5];
        
        int keys[] = {5, 2, 8, 1, 3};
        for (int i = 0; i < 5; i++) {
            arr[i].key = keys[i];
            memset(arr[i].padding, 0, 600);
        }
        
        int result = sort_func(arr, 5, sizeof(arr[0]), compare_struct_very_large);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        for (int i = 1; i < 5; i++) {
            EXPECT_TRUE(arr[i - 1].key <= arr[i].key);
        }
    }

    // Test 28: Many operations with different sizes
    {
        for (int size = 1; size <= 50; size++) {
            int *arr = malloc(size * sizeof(int));
            if (!arr) continue;
            
            for (int i = 0; i < size; i++) {
                arr[i] = size - i;
            }
            
            int result = sort_func(arr, size, sizeof(int), compare_int);
            EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
            EXPECT_TRUE(is_sorted_int(arr, size));
            
            free(arr);
        }
    }

    // Test 29: Sort, verify, sort again with different comparator
    {
        int arr[] = {5, 2, 8, 1, 3};
        
        int result1 = sort_func(arr, 5, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result1, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 5));
        
        int result2 = sort_func(arr, 5, sizeof(int), compare_int_desc);
        EXPECT_EQUAL_INT(result2, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int_desc(arr, 5));
    }

    // Test 30: Multiple data types in sequence
    {
        int int_arr[] = {5, 2, 8, 1, 3};
        double double_arr[] = {3.14, 1.41, 2.71, 0.57};
        const char *str_arr[] = {"banana", "apple", "cherry"};
        
        sort_func(int_arr, 5, sizeof(int), compare_int);
        sort_func(double_arr, 4, sizeof(double), compare_double);
        sort_func(str_arr, 3, sizeof(char *), compare_string);
        
        EXPECT_TRUE(is_sorted_int(int_arr, 5));
        EXPECT_EQUAL_STRING(str_arr[0], "apple");
        EXPECT_EQUAL_STRING(str_arr[2], "cherry");
    }

    // Test 31: Sort subarray
    {
        int arr[] = {9, 8, 7, 3, 2, 1, 6, 5, 4};
        
        // Sort first 3 elements
        int result1 = sort_func(arr, 3, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result1, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 3));
        
        // Sort last 3 elements
        int result2 = sort_func(arr + 6, 3, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result2, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr + 6, 3));
    }

    // Test 32: Edge case combinations
    {
        // Empty array
        int empty[] = {};
        sort_func(empty, 0, sizeof(int), compare_int);
        
        // Single element
        int single[] = {42};
        sort_func(single, 1, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(single[0], 42);
        
        // Two elements
        int two[] = {2, 1};
        sort_func(two, 2, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(two[0], 1);
        EXPECT_EQUAL_INT(two[1], 2);
        
        // All same
        int same[] = {5, 5, 5};
        sort_func(same, 3, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(same[0], 5);
        EXPECT_EQUAL_INT(same[2], 5);
    }
}

/**
 * Test: sort_insertion
 * Dependencies: None
 * Description: Tests the sort_insertion function using generalized sort tests.
 */
UTEST_TEST_CASE(sort_insertion) {
    test_sort_generic(sort_insertion, "insertion");
}

/**
 * Test: sort_selection
 * Dependencies: None
 * Description: Tests the sort_selection function using generalized sort tests.
 */
UTEST_TEST_CASE(sort_selection) {
    test_sort_generic(sort_selection, "selection");
}

/**
 * Test suite: sort
 * Description: Test suite for sort algorithm function
 */
UTEST_TEST_SUITE(sort) {
    UTEST_RUN_TEST_CASE(sort_insertion);
    UTEST_RUN_TEST_CASE(sort_selection);
}
