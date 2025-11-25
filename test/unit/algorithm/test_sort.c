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
    int (*sort_func)(void *, size_t, size_t, int (*)(const void *, const void *))){
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
        
        int result1 = sort_func(arr, 3, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result1, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 3));

        int result2 = sort_func(arr + 6, 3, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result2, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr + 6, 3));
    }

    // Test 32: Edge case combinations
    {
        int empty[] = {};
        sort_func(empty, 0, sizeof(int), compare_int);
        
        int single[] = {42};
        sort_func(single, 1, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(single[0], 42);
        
        int two[] = {2, 1};
        sort_func(two, 2, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(two[0], 1);
        EXPECT_EQUAL_INT(two[1], 2);
        
        int same[] = {5, 5, 5};
        sort_func(same, 3, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(same[0], 5);
        EXPECT_EQUAL_INT(same[2], 5);
    }

    // Test 33: Array with only two distinct values (binary pattern)
    {
        int arr[] = {1, 0, 1, 0, 1, 0, 1, 0};
        int original[] = {1, 0, 1, 0, 1, 0, 1, 0};
        int result = sort_func(arr, 8, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 8));
        EXPECT_TRUE(arrays_equal_int(arr, original, 8));
    }

    // Test 34: Alternating pattern (worst case for some algorithms)
    {
        int arr[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7};
        int original[] = {1, 3, 2, 4, 3, 5, 4, 6, 5, 7};
        int result = sort_func(arr, 10, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 10));
        EXPECT_TRUE(arrays_equal_int(arr, original, 10));
    }

    // Test 35: Sawtooth pattern
    {
        int arr[] = {1, 2, 3, 1, 2, 3, 1, 2, 3};
        int original[] = {1, 2, 3, 1, 2, 3, 1, 2, 3};
        int result = sort_func(arr, 9, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 9));
        EXPECT_TRUE(arrays_equal_int(arr, original, 9));
    }

    // Test 36: Moderate sized array (not too large for unit tests)
    {
        const size_t moderate_size = 500;
        int *arr = malloc(moderate_size * sizeof(int));
        int *original = malloc(moderate_size * sizeof(int));
        
        if (arr && original) {
            for (size_t i = 0; i < moderate_size; i++) {
                arr[i] = (int)(moderate_size - i);
                original[i] = (int)(moderate_size - i);
            }
            
            int result = sort_func(arr, moderate_size, sizeof(int), compare_int);
            EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
            EXPECT_TRUE(is_sorted_int(arr, moderate_size));
            EXPECT_TRUE(arrays_equal_int(arr, original, moderate_size));
        }
        
        free(arr);
        free(original);
    }

    // Test 37: Power-of-two sized arrays (common edge case)
    {
        for (int pow = 1; pow <= 8; pow++) {
            size_t size = 1U << pow; // 2, 4, 8, 16, 32, 64, 128, 256
            int *arr = malloc(size * sizeof(int));
            if (!arr) continue;
            
            for (size_t i = 0; i < size; i++) {
                arr[i] = (int)(size - i);
            }
            
            int result = sort_func(arr, size, sizeof(int), compare_int);
            EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
            EXPECT_TRUE(is_sorted_int(arr, size));
            
            free(arr);
        }
    }

    // Test 38: Odd-sized arrays
    {
        for (int size = 3; size <= 21; size += 2) {
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

    // Test 39: Even-sized arrays
    {
        for (int size = 2; size <= 20; size += 2) {
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

    // Test 40: Array with many duplicates (few unique values)
    {
        int arr[] = {5, 2, 5, 2, 5, 2, 5, 2, 5, 2};
        int original[] = {5, 2, 5, 2, 5, 2, 5, 2, 5, 2};
        int result = sort_func(arr, 10, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 10));
        EXPECT_TRUE(arrays_equal_int(arr, original, 10));
    }

    // Test 41: Array with three distinct values
    {
        int arr[] = {3, 1, 2, 3, 1, 2, 3, 1, 2};
        int original[] = {3, 1, 2, 3, 1, 2, 3, 1, 2};
        int result = sort_func(arr, 9, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 9));
        EXPECT_TRUE(arrays_equal_int(arr, original, 9));
    }

    // Test 42: Extreme value distribution (most values are same)
    {
        int arr[] = {1, 100, 1, 1, 1, 1, 1, 1, 1, 1};
        int original[] = {1, 100, 1, 1, 1, 1, 1, 1, 1, 1};
        int result = sort_func(arr, 10, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 10));
        EXPECT_TRUE(arrays_equal_int(arr, original, 10));
    }

    // Test 43: Array with zeros
    {
        int arr[] = {0, 0, 0, 5, 0, 3, 0, 1, 0};
        int original[] = {0, 0, 0, 5, 0, 3, 0, 1, 0};
        int result = sort_func(arr, 9, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 9));
        EXPECT_TRUE(arrays_equal_int(arr, original, 9));
    }

    // Test 44: Array with all zeros
    {
        int arr[] = {0, 0, 0, 0, 0};
        int result = sort_func(arr, 5, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 5));
        for (int i = 0; i < 5; i++) {
            EXPECT_EQUAL_INT(arr[i], 0);
        }
    }

    // Test 45: Array with one unique value and rest duplicates
    {
        int arr[] = {42, 1, 1, 1, 1, 1, 1, 1};
        int original[] = {42, 1, 1, 1, 1, 1, 1, 1};
        int result = sort_func(arr, 8, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 8));
        EXPECT_TRUE(arrays_equal_int(arr, original, 8));
    }

    // Test 46: Multiple consecutive sorts (idempotency)
    {
        int arr[] = {5, 2, 8, 1, 3};
        int original[] = {5, 2, 8, 1, 3};
        
        for (int i = 0; i < 5; i++) {
            int result = sort_func(arr, 5, sizeof(int), compare_int);
            EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
            EXPECT_TRUE(is_sorted_int(arr, 5));
        }
        
        EXPECT_TRUE(arrays_equal_int(arr, original, 5));
    }

    // Test 47: Sort already sorted array multiple times
    {
        int arr[] = {1, 2, 3, 4, 5};
        
        for (int i = 0; i < 10; i++) {
            int result = sort_func(arr, 5, sizeof(int), compare_int);
            EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
            EXPECT_TRUE(is_sorted_int(arr, 5));
            EXPECT_EQUAL_INT(arr[0], 1);
            EXPECT_EQUAL_INT(arr[4], 5);
        }
    }

    // Test 48: String array with empty strings
    {
        const char *arr[] = {"", "banana", "", "apple", "cherry", ""};
        int result = sort_func(arr, 6, sizeof(char *), compare_string);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_EQUAL_STRING(arr[0], "");
        EXPECT_EQUAL_STRING(arr[1], "");
        EXPECT_EQUAL_STRING(arr[2], "");
    }

    // Test 49: String array with single character strings
    {
        const char *arr[] = {"z", "a", "m", "b", "y"};
        int result = sort_func(arr, 5, sizeof(char *), compare_string);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_EQUAL_STRING(arr[0], "a");
        EXPECT_EQUAL_STRING(arr[1], "b");
        EXPECT_EQUAL_STRING(arr[4], "z");
    }

    // Test 50: String array with duplicates
    {
        const char *arr[] = {"banana", "apple", "banana", "cherry", "apple"};
        int result = sort_func(arr, 5, sizeof(char *), compare_string);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_EQUAL_STRING(arr[0], "apple");
        EXPECT_EQUAL_STRING(arr[1], "apple");
        EXPECT_EQUAL_STRING(arr[2], "banana");
        EXPECT_EQUAL_STRING(arr[3], "banana");
        EXPECT_EQUAL_STRING(arr[4], "cherry");
    }

    // Test 51: Double array with very small values
    {
        double arr[] = {1e-10, 1e-20, 1e-30, 1e-15, 1e-25};
        int result = sort_func(arr, 5, sizeof(double), compare_double);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        for (size_t i = 1; i < 5; i++) {
            EXPECT_TRUE(arr[i - 1] <= arr[i]);
        }
    }

    // Test 52: Double array with very large values
    {
        double arr[] = {1e10, 1e20, 1e15, 1e25, 1e5};
        int result = sort_func(arr, 5, sizeof(double), compare_double);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        for (size_t i = 1; i < 5; i++) {
            EXPECT_TRUE(arr[i - 1] <= arr[i]);
        }
    }

    // Test 53: Double array with mixed magnitudes
    {
        double arr[] = {1e10, 1e-10, 1.0, 1e5, 1e-5};
        int result = sort_func(arr, 5, sizeof(double), compare_double);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        for (size_t i = 1; i < 5; i++) {
            EXPECT_TRUE(arr[i - 1] <= arr[i]);
        }
    }

    // Test 54: Array near MAX_STACK_SIZE boundary (512 bytes)
    {
        // Test with element size just below MAX_STACK_SIZE
        struct {
            int key;
            char padding[500]; 
        } arr[3];
        
        int keys[] = {3, 1, 2};
        for (int i = 0; i < 3; i++) {
            arr[i].key = keys[i];
            memset(arr[i].padding, 0, 500);
        }
        
        int result = sort_func(arr, 3, sizeof(arr[0]), compare_struct_large);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        for (int i = 1; i < 3; i++) {
            EXPECT_TRUE(arr[i - 1].key <= arr[i].key);
        }
    }

    // Test 55: Array at MAX_STACK_SIZE boundary (512 bytes)
    {
        struct {
            int key;
            char padding[508];
        } arr[3];
        
        int keys[] = {3, 1, 2};
        for (int i = 0; i < 3; i++) {
            arr[i].key = keys[i];
            memset(arr[i].padding, 0, 508);
        }
        
        int result = sort_func(arr, 3, sizeof(arr[0]), compare_struct_large);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        for (int i = 1; i < 3; i++) {
            EXPECT_TRUE(arr[i - 1].key <= arr[i].key);
        }
    }

    // Test 56: Array above MAX_STACK_SIZE boundary (triggers heap allocation)
    {
        struct {
            int key;
            char padding[520];
        } arr[3];
        
        int keys[] = {3, 1, 2};
        for (int i = 0; i < 3; i++) {
            arr[i].key = keys[i];
            memset(arr[i].padding, 0, 520);
        }
        
        int result = sort_func(arr, 3, sizeof(arr[0]), compare_struct_large);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        for (int i = 1; i < 3; i++) {
            EXPECT_TRUE(arr[i - 1].key <= arr[i].key);
        }
    }

    // Test 57: Array with sequential duplicates
    {
        int arr[] = {1, 1, 1, 2, 2, 2, 3, 3, 3};
        int result = sort_func(arr, 9, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 9));
        EXPECT_EQUAL_INT(arr[0], 1);
        EXPECT_EQUAL_INT(arr[2], 1);
        EXPECT_EQUAL_INT(arr[3], 2);
        EXPECT_EQUAL_INT(arr[5], 2);
        EXPECT_EQUAL_INT(arr[6], 3);
        EXPECT_EQUAL_INT(arr[8], 3);
    }

    // Test 58: Array with scattered duplicates
    {
        int arr[] = {3, 1, 2, 3, 1, 2, 3, 1, 2, 3};
        int original[] = {3, 1, 2, 3, 1, 2, 3, 1, 2, 3};
        int result = sort_func(arr, 10, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 10));
        EXPECT_TRUE(arrays_equal_int(arr, original, 10));
    }

    // Test 59: Array with extreme negative values
    {
        int arr[] = {INT_MIN, INT_MIN + 1, -1000000, -1000, -1};
        int original[] = {INT_MIN, INT_MIN + 1, -1000000, -1000, -1};
        int result = sort_func(arr, 5, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 5));
        EXPECT_TRUE(arrays_equal_int(arr, original, 5));
        EXPECT_EQUAL_INT(arr[0], INT_MIN);
    }

    // Test 60: Array with extreme positive values
    {
        int arr[] = {1, 1000, 1000000, INT_MAX - 1, INT_MAX};
        int original[] = {1, 1000, 1000000, INT_MAX - 1, INT_MAX};
        int result = sort_func(arr, 5, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 5));
        EXPECT_TRUE(arrays_equal_int(arr, original, 5));
        EXPECT_EQUAL_INT(arr[4], INT_MAX);
    }

    // Test 61: Array with values spanning full int range
    {
        int arr[] = {INT_MIN, -1000, 0, 1000, INT_MAX};
        int original[] = {INT_MIN, -1000, 0, 1000, INT_MAX};
        int result = sort_func(arr, 5, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 5));
        EXPECT_TRUE(arrays_equal_int(arr, original, 5));
    }

    // Test 62: Descending sort with duplicates
    {
        int arr[] = {5, 2, 5, 2, 1, 1, 3, 3};
        int result = sort_func(arr, 8, sizeof(int), compare_int_desc);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int_desc(arr, 8));
        EXPECT_EQUAL_INT(arr[0], 5);
        EXPECT_EQUAL_INT(arr[7], 1);
    }

    // Test 63: Array sorted in reverse, then sorted again
    {
        int arr[] = {5, 4, 3, 2, 1};
        int result1 = sort_func(arr, 5, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result1, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 5));
        
        // Reverse it manually
        for (int i = 0; i < 2; i++) {
            int temp = arr[i];
            arr[i] = arr[4 - i];
            arr[4 - i] = temp;
        }
        
        int result2 = sort_func(arr, 5, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result2, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 5));
    }

    // Test 64: Array with pattern that requires many swaps
    {
        int arr[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
        int original[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
        int result = sort_func(arr, 10, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 10));
        EXPECT_TRUE(arrays_equal_int(arr, original, 10));
    }

    // Test 65: Array with almost sorted pattern (one element out of place)
    {
        int arr[] = {1, 2, 3, 4, 0, 5, 6, 7, 8, 9};
        int original[] = {1, 2, 3, 4, 0, 5, 6, 7, 8, 9};
        int result = sort_func(arr, 10, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 10));
        EXPECT_TRUE(arrays_equal_int(arr, original, 10));
    }

    // Test 66: Array with almost sorted pattern (last element out of place)
    {
        int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
        int original[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
        int result = sort_func(arr, 10, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 10));
        EXPECT_TRUE(arrays_equal_int(arr, original, 10));
    }

    // Test 67: Array with almost sorted pattern (first element out of place)
    {
        int arr[] = {10, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        int original[] = {10, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        int result = sort_func(arr, 10, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 10));
        EXPECT_TRUE(arrays_equal_int(arr, original, 10));
    }

    // Test 68: Array with values in specific range
    {
        int arr[100];
        for (int i = 0; i < 100; i++) {
            arr[i] = (i * 7) % 100; // Pseudo-random but deterministic
        }
        int result = sort_func(arr, 100, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 100));
    }

    // Test 69: Array with specific pattern (hill pattern)
    {
        int arr[] = {1, 2, 3, 4, 5, 4, 3, 2, 1};
        int original[] = {1, 2, 3, 4, 5, 4, 3, 2, 1};
        int result = sort_func(arr, 9, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 9));
        EXPECT_TRUE(arrays_equal_int(arr, original, 9));
    }

    // Test 70: Array with specific pattern (valley pattern)
    {
        int arr[] = {5, 4, 3, 2, 1, 2, 3, 4, 5};
        int original[] = {5, 4, 3, 2, 1, 2, 3, 4, 5};
        int result = sort_func(arr, 9, sizeof(int), compare_int);
        EXPECT_EQUAL_INT(result, COLLECTION_SUCCESS);
        EXPECT_TRUE(is_sorted_int(arr, 9));
        EXPECT_TRUE(arrays_equal_int(arr, original, 9));
    }
}

/**
 * Test: sort_insertion
 * Dependencies: None
 * Description: Tests the sort_insertion function using generalized sort tests.
 */
UTEST_TEST_CASE(sort_insertion) {
    test_sort_generic(sort_insertion);
}

/**
 * Test: sort_selection
 * Dependencies: None
 * Description: Tests the sort_selection function using generalized sort tests.
 */
UTEST_TEST_CASE(sort_selection) {
    test_sort_generic(sort_selection);
}

/**
 * Test: sort_bubble
 * Dependencies: None
 * Description: Tests the sort_bubble function using generalized sort tests.
 */
UTEST_TEST_CASE(sort_bubble) {
    test_sort_generic(sort_bubble);
}

/**
 * Test: sort_quick
 * Dependencies: None
 * Description: Tests the sort_quick function using generalized sort tests.
 */
UTEST_TEST_CASE(sort_quick) {
    test_sort_generic(sort_quick);
}

/**
 * Test: sort_merge
 * Dependencies: None
 * Description: Tests the sort_merge function using generalized sort tests.
 */
UTEST_TEST_CASE(sort_merge) {
    test_sort_generic(sort_merge);
}
/**
 * Test suite: sort
 * Description: Test suite for sort algorithm function
 */
UTEST_TEST_SUITE(sort) {
    UTEST_RUN_TEST_CASE(sort_insertion);
    UTEST_RUN_TEST_CASE(sort_selection);
    UTEST_RUN_TEST_CASE(sort_bubble);
    UTEST_RUN_TEST_CASE(sort_quick);
    UTEST_RUN_TEST_CASE(sort_merge);
}
