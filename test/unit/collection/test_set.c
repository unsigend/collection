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

#include "utest/utest_types.h"
#include <collection/set.h>
#include <collection/common.h>
#include <algorithm/hash.h>
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

/* Hash and match functions for integer keys */
static uint32_t hash_int_ptr(const void* key) {
    return hash_int(*(int*)key);
}

static bool match_int_ptr(const void* key1, const void* key2) {
    return *(int*)key1 == *(int*)key2;
}

/* Hash and match functions for string keys */
static uint32_t hash_str_wrapper(const void* key) {
    return hash_str((const char*)key);
}

static bool match_str_wrapper(const void* key1, const void* key2) {
    return strcmp((const char*)key1, (const char*)key2) == 0;
}

/**
 * Test: set_init
 * Dependencies: None
 * Description: Tests basic initialization of the set structure.
 */
UTEST_TEST_CASE(set_init){
    // Test 1: Initialize with NULL functions
    {
        Set set;
        EXPECT_EQUAL_INT(set_init(&set, NULL, NULL, NULL), COLLECTION_SUCCESS);
        EXPECT_TRUE(set_empty(&set));
        EXPECT_EQUAL_UINT(set_size(&set), 0);
        set_destroy(&set);
    }

    // Test 2: Initialize with destroy function
    {
        Set set;
        EXPECT_EQUAL_INT(set_init(&set, NULL, NULL, free), COLLECTION_SUCCESS);
        EXPECT_TRUE(set_empty(&set));
        EXPECT_EQUAL_UINT(set_size(&set), 0);
        set_destroy(&set);
    }

    // Test 3: Initialize with NULL set pointer
    {
        EXPECT_EQUAL_INT(set_init(NULL, NULL, NULL, NULL), COLLECTION_FAILURE);
    }
}

/**
 * Test: set_destroy
 * Dependencies: set_init, set_insert
 * Description: Tests the set_destroy function to free all resources.
 */
UTEST_TEST_CASE(set_destroy){
    // Test 1: Destroy empty set
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        set_destroy(&set);
        EXPECT_TRUE(set_empty(&set));
    }

    // Test 2: Destroy set with elements
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        char* str1 = "test1";
        char* str2 = "test2";
        set_insert(&set, str1);
        set_insert(&set, str2);
        set_destroy(&set);
    }

    // Test 3: Destroy set with destroy function
    {
        Set set;
        set_init(&set, NULL, NULL, destroy_counter);
        int* val1 = malloc(sizeof(int));
        int* val2 = malloc(sizeof(int));
        *val1 = 1;
        *val2 = 2;
        set_insert(&set, val1);
        set_insert(&set, val2);
        destroy_count = 0;
        set_destroy(&set);
        EXPECT_EQUAL_INT(destroy_count, 2);
        free(val1);
        free(val2);
    }

    // Test 4: Destroy NULL set
    {
        set_destroy(NULL);
    }
}

/**
 * Test: set_empty
 * Dependencies: set_init, set_insert, set_clear
 * Description: Tests the set_empty function to check if set is empty.
 */
UTEST_TEST_CASE(set_empty){
    // Test 1: Empty set
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        EXPECT_TRUE(set_empty(&set));
        set_destroy(&set);
    }

    // Test 2: Non-empty set
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        char* str = "test";
        set_insert(&set, str);
        EXPECT_FALSE(set_empty(&set));
        set_destroy(&set);
    }

    // Test 3: Empty after clear
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        char* str1 = "test1";
        char* str2 = "test2";
        set_insert(&set, str1);
        set_insert(&set, str2);
        EXPECT_FALSE(set_empty(&set));
        set_clear(&set);
        EXPECT_TRUE(set_empty(&set));
        set_destroy(&set);
    }

    // Test 4: Empty with NULL set
    {
        EXPECT_TRUE(set_empty(NULL));
    }
}

/**
 * Test: set_size
 * Dependencies: set_init, set_insert, set_remove, set_clear
 * Description: Tests the set_size function to get the number of elements.
 */
UTEST_TEST_CASE(set_size){
    // Test 1: Size of empty set
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        EXPECT_EQUAL_UINT(set_size(&set), 0);
        set_destroy(&set);
    }

    // Test 2: Size after insertions
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        char* str1 = "test1";
        char* str2 = "test2";
        char* str3 = "test3";
        set_insert(&set, str1);
        EXPECT_EQUAL_UINT(set_size(&set), 1);
        set_insert(&set, str2);
        EXPECT_EQUAL_UINT(set_size(&set), 2);
        set_insert(&set, str3);
        EXPECT_EQUAL_UINT(set_size(&set), 3);
        set_destroy(&set);
    }

    // Test 3: Size after duplicate insertions
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        char* str = "test";
        set_insert(&set, str);
        EXPECT_EQUAL_UINT(set_size(&set), 1);
        set_insert(&set, str);
        EXPECT_EQUAL_UINT(set_size(&set), 1);
        set_destroy(&set);
    }

    // Test 4: Size after removal
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        char* str1 = "test1";
        char* str2 = "test2";
        set_insert(&set, str1);
        set_insert(&set, str2);
        EXPECT_EQUAL_UINT(set_size(&set), 2);
        set_remove(&set, str1);
        EXPECT_EQUAL_UINT(set_size(&set), 1);
        set_destroy(&set);
    }

    // Test 5: Size after clear
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        char* str1 = "test1";
        char* str2 = "test2";
        set_insert(&set, str1);
        set_insert(&set, str2);
        EXPECT_EQUAL_UINT(set_size(&set), 2);
        set_clear(&set);
        EXPECT_EQUAL_UINT(set_size(&set), 0);
        set_destroy(&set);
    }

    // Test 6: Size with NULL set
    {
        EXPECT_EQUAL_UINT(set_size(NULL), 0);
    }
}

/**
 * Test: set_insert
 * Dependencies: set_init, set_size, set_contains
 * Description: Tests the set_insert function to add elements.
 */
UTEST_TEST_CASE(set_insert){
    // Test 1: Insert single element
    {
        Set set;
        EXPECT_EQUAL_INT(set_init(&set, NULL, NULL, NULL), UTEST_RESULT_SUCCESS);
        char* str = "test";
        EXPECT_EQUAL_INT(set_insert(&set, str), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&set), 1);
        EXPECT_TRUE(set_contains(&set, str));
        set_destroy(&set);
    }

    // Test 2: Insert multiple elements
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        char* str1 = "test1";
        char* str2 = "test2";
        char* str3 = "test3";
        EXPECT_EQUAL_INT(set_insert(&set, str1), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(set_insert(&set, str2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(set_insert(&set, str3), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&set), 3);
        set_destroy(&set);
    }

    // Test 3: Insert duplicate element
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        char* str = "test";
        EXPECT_EQUAL_INT(set_insert(&set, str), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(set_insert(&set, str), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&set), 1);
        set_destroy(&set);
    }

    // Test 4: Insert with NULL set
    {
        char* str = "test";
        EXPECT_EQUAL_INT(set_insert(NULL, str), COLLECTION_FAILURE);
    }

    // Test 5: Insert NULL data
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        EXPECT_EQUAL_INT(set_insert(&set, NULL), COLLECTION_FAILURE);
        set_destroy(&set);
    }
}

/**
 * Test: set_remove
 * Dependencies: set_init, set_insert, set_size, set_contains
 * Description: Tests the set_remove function to remove elements.
 */
UTEST_TEST_CASE(set_remove){
    // Test 1: Remove existing element
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        char* str = "test";
        set_insert(&set, str);
        EXPECT_EQUAL_UINT(set_size(&set), 1);
        EXPECT_EQUAL_INT(set_remove(&set, str), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&set), 0);
        EXPECT_FALSE(set_contains(&set, str));
        set_destroy(&set);
    }

    // Test 2: Remove non-existing element
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        char* str1 = "test1";
        char* str2 = "test2";
        set_insert(&set, str1);
        EXPECT_EQUAL_INT(set_remove(&set, str2), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(set_size(&set), 1);
        set_destroy(&set);
    }

    // Test 3: Remove from empty set
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        char* str = "test";
        EXPECT_EQUAL_INT(set_remove(&set, str), COLLECTION_FAILURE);
        set_destroy(&set);
    }

    // Test 4: Remove multiple elements
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        char* str1 = "test1";
        char* str2 = "test2";
        char* str3 = "test3";
        set_insert(&set, str1);
        set_insert(&set, str2);
        set_insert(&set, str3);
        EXPECT_EQUAL_UINT(set_size(&set), 3);
        EXPECT_EQUAL_INT(set_remove(&set, str2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&set), 2);
        EXPECT_EQUAL_INT(set_remove(&set, str1), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&set), 1);
        set_destroy(&set);
    }

    // Test 5: Remove with NULL set
    {
        char* str = "test";
        EXPECT_EQUAL_INT(set_remove(NULL, str), COLLECTION_FAILURE);
    }

    // Test 6: Remove NULL data
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        EXPECT_EQUAL_INT(set_remove(&set, NULL), COLLECTION_FAILURE);
        set_destroy(&set);
    }
}

/**
 * Test: set_clear
 * Dependencies: set_init, set_insert, set_size, set_empty
 * Description: Tests the set_clear function to remove all elements.
 */
UTEST_TEST_CASE(set_clear){
    // Test 1: Clear empty set
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        set_clear(&set);
        EXPECT_TRUE(set_empty(&set));
        EXPECT_EQUAL_UINT(set_size(&set), 0);
        set_destroy(&set);
    }

    // Test 2: Clear set with elements
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        char* str1 = "test1";
        char* str2 = "test2";
        char* str3 = "test3";
        set_insert(&set, str1);
        set_insert(&set, str2);
        set_insert(&set, str3);
        EXPECT_EQUAL_UINT(set_size(&set), 3);
        set_clear(&set);
        EXPECT_TRUE(set_empty(&set));
        EXPECT_EQUAL_UINT(set_size(&set), 0);
        set_destroy(&set);
    }

    // Test 3: Clear and reuse set
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        char* str1 = "test1";
        char* str2 = "test2";
        set_insert(&set, str1);
        set_insert(&set, str2);
        set_clear(&set);
        char* str3 = "test3";
        set_insert(&set, str3);
        EXPECT_EQUAL_UINT(set_size(&set), 1);
        set_destroy(&set);
    }

    // Test 4: Clear with NULL set
    {
        set_clear(NULL);
    }
}

/**
 * Test: set_contains
 * Dependencies: set_init, set_insert, set_remove
 * Description: Tests the set_contains function to check membership.
 */
UTEST_TEST_CASE(set_contains){
    // Test 1: Contains existing element
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        char* str = "test";
        set_insert(&set, str);
        EXPECT_TRUE(set_contains(&set, str));
        set_destroy(&set);
    }

    // Test 2: Contains non-existing element
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        char* str1 = "test1";
        char* str2 = "test2";
        set_insert(&set, str1);
        EXPECT_FALSE(set_contains(&set, str2));
        set_destroy(&set);
    }

    // Test 3: Contains after removal
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        char* str = "test";
        set_insert(&set, str);
        EXPECT_TRUE(set_contains(&set, str));
        set_remove(&set, str);
        EXPECT_FALSE(set_contains(&set, str));
        set_destroy(&set);
    }

    // Test 4: Contains in empty set
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        char* str = "test";
        EXPECT_FALSE(set_contains(&set, str));
        set_destroy(&set);
    }

    // Test 5: Contains with NULL set
    {
        char* str = "test";
        EXPECT_FALSE(set_contains(NULL, str));
    }

    // Test 6: Contains NULL data
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        EXPECT_FALSE(set_contains(&set, NULL));
        set_destroy(&set);
    }
}

/**
 * Test: set_intersection
 * Dependencies: set_init, set_insert, set_size, set_contains, set_clear
 * Description: Tests the set_intersection function to compute intersection.
 */
UTEST_TEST_CASE(set_intersection){
    // Test 1: Intersection of two sets with common elements
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        char* str3 = "c";
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        set_insert(&set2, str2);
        set_insert(&set2, str3);
        
        EXPECT_EQUAL_INT(set_intersection(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        EXPECT_TRUE(set_contains(&result, str2));
        EXPECT_FALSE(set_contains(&result, str1));
        EXPECT_FALSE(set_contains(&result, str3));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 2: Intersection of disjoint sets
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        set_insert(&set1, str1);
        set_insert(&set2, str2);
        
        EXPECT_EQUAL_INT(set_intersection(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 0);
        EXPECT_TRUE(set_empty(&result));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 3: Intersection of identical sets
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        set_insert(&set2, str1);
        set_insert(&set2, str2);
        
        EXPECT_EQUAL_INT(set_intersection(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        EXPECT_TRUE(set_contains(&result, str1));
        EXPECT_TRUE(set_contains(&result, str2));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 4: Intersection with empty set
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        char* str1 = "a";
        set_insert(&set1, str1);
        
        EXPECT_EQUAL_INT(set_intersection(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 0);
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 5: Intersection clears result set first
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        char* str3 = "c";
        set_insert(&result, str3);
        set_insert(&set1, str1);
        set_insert(&set2, str1);
        
        EXPECT_EQUAL_INT(set_intersection(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        EXPECT_TRUE(set_contains(&result, str1));
        EXPECT_FALSE(set_contains(&result, str3));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 6: Intersection with NULL pointers
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        EXPECT_EQUAL_INT(set_intersection(NULL, &set1, &set2), COLLECTION_FAILURE);
        EXPECT_EQUAL_INT(set_intersection(&result, NULL, &set2), COLLECTION_FAILURE);
        EXPECT_EQUAL_INT(set_intersection(&result, &set1, NULL), COLLECTION_FAILURE);
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 7: Result set initially uses int hash, then switches to str hash after intersection
    {
        Set set1, set2, result;
        set_init(&set1, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&set2, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&result, hash_int_ptr, match_int_ptr, NULL);
        
        int val1 = 100;
        int val2 = 200;
        set_insert(&result, &val1);
        set_insert(&result, &val2);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        
        char* str1 = "common1";
        char* str2 = "common2";
        char* str3 = "unique";
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        set_insert(&set1, str3);
        set_insert(&set2, str1);
        set_insert(&set2, str2);
        
        EXPECT_EQUAL_INT(set_intersection(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        EXPECT_TRUE(set_contains(&result, str1));
        EXPECT_TRUE(set_contains(&result, str2));
        EXPECT_FALSE(set_contains(&result, str3));
        EXPECT_FALSE(set_contains(&result, &val1));
        EXPECT_FALSE(set_contains(&result, &val2));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 8: Result set with int hash, intersection of disjoint sets using str hash
    {
        Set set1, set2, result;
        set_init(&set1, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&set2, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&result, hash_int_ptr, match_int_ptr, NULL);
        
        int val1 = 42;
        set_insert(&result, &val1);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        
        char* str1 = "a";
        char* str2 = "b";
        set_insert(&set1, str1);
        set_insert(&set2, str2);
        
        EXPECT_EQUAL_INT(set_intersection(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 0);
        EXPECT_TRUE(set_empty(&result));
        EXPECT_FALSE(set_contains(&result, &val1));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 9: Multiple intersection operations with hash function changes
    {
        Set set1, set2, result;
        set_init(&set1, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&set2, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&result, hash_int_ptr, match_int_ptr, NULL);
        
        char* str1 = "shared1";
        char* str2 = "shared2";
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        set_insert(&set2, str1);
        set_insert(&set2, str2);
        EXPECT_EQUAL_INT(set_intersection(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        EXPECT_TRUE(set_contains(&result, str1));
        EXPECT_TRUE(set_contains(&result, str2));
        
        set_clear(&set1);
        set_clear(&set2);
        char* str3 = "new_shared";
        set_insert(&set1, str3);
        set_insert(&set2, str3);
        
        EXPECT_EQUAL_INT(set_intersection(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        EXPECT_TRUE(set_contains(&result, str3));
        EXPECT_FALSE(set_contains(&result, str1));
        EXPECT_FALSE(set_contains(&result, str2));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 10: No double free when set1 has heap-allocated keys and result has values
    {
        Set set1, set2, result;
        set_init(&set1, hash_str_wrapper, match_str_wrapper, free);
        set_init(&set2, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&result, hash_str_wrapper, match_str_wrapper, free);
        
        char* result_val1 = malloc(10);
        char* result_val2 = malloc(10);
        strcpy(result_val1, "result1");
        strcpy(result_val2, "result2");
        set_insert(&result, result_val1);
        set_insert(&result, result_val2);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        
        char* set1_key1 = malloc(10);
        char* set1_key2 = malloc(10);
        strcpy(set1_key1, "common1");
        strcpy(set1_key2, "common2");
        set_insert(&set1, set1_key1);
        set_insert(&set1, set1_key2);
        
        char* set2_key1 = malloc(10);
        char* set2_key2 = malloc(10);
        strcpy(set2_key1, "common1");
        strcpy(set2_key2, "common2");
        set_insert(&set2, set2_key1);
        set_insert(&set2, set2_key2);
        
        EXPECT_EQUAL_INT(set_intersection(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        EXPECT_TRUE(set_contains(&result, set1_key1));
        EXPECT_TRUE(set_contains(&result, set1_key2));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
        free(set2_key1);
        free(set2_key2);
    }
}

/**
 * Test: set_union
 * Dependencies: set_init, set_insert, set_size, set_contains, set_clear
 * Description: Tests the set_union function to compute union.
 */
UTEST_TEST_CASE(set_union){
    // Test 1: Union of two sets
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        char* str3 = "c";
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        set_insert(&set2, str2);
        set_insert(&set2, str3);
        
        EXPECT_EQUAL_INT(set_union(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 3);
        EXPECT_TRUE(set_contains(&result, str1));
        EXPECT_TRUE(set_contains(&result, str2));
        EXPECT_TRUE(set_contains(&result, str3));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 2: Union of disjoint sets
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        set_insert(&set1, str1);
        set_insert(&set2, str2);
        
        EXPECT_EQUAL_INT(set_union(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        EXPECT_TRUE(set_contains(&result, str1));
        EXPECT_TRUE(set_contains(&result, str2));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 3: Union of identical sets
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        set_insert(&set2, str1);
        set_insert(&set2, str2);
        
        EXPECT_EQUAL_INT(set_union(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        EXPECT_TRUE(set_contains(&result, str1));
        EXPECT_TRUE(set_contains(&result, str2));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 4: Union with empty set
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        char* str1 = "a";
        set_insert(&set1, str1);
        
        EXPECT_EQUAL_INT(set_union(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        EXPECT_TRUE(set_contains(&result, str1));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 5: Union clears result set first
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        char* str3 = "c";
        set_insert(&result, str3);
        set_insert(&set1, str1);
        set_insert(&set2, str2);
        
        EXPECT_EQUAL_INT(set_union(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        EXPECT_TRUE(set_contains(&result, str1));
        EXPECT_TRUE(set_contains(&result, str2));
        EXPECT_FALSE(set_contains(&result, str3));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 6: Union with NULL pointers
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        EXPECT_EQUAL_INT(set_union(NULL, &set1, &set2), COLLECTION_FAILURE);
        EXPECT_EQUAL_INT(set_union(&result, NULL, &set2), COLLECTION_FAILURE);
        EXPECT_EQUAL_INT(set_union(&result, &set1, NULL), COLLECTION_FAILURE);
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 7: Result set initially uses int hash, then switches to str hash after union
    {
        Set set1, set2, result;
        set_init(&set1, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&set2, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&result, hash_int_ptr, match_int_ptr, NULL);
        
        int val1 = 100;
        int val2 = 200;
        set_insert(&result, &val1);
        set_insert(&result, &val2);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        
        char* str1 = "alpha";
        char* str2 = "beta";
        char* str3 = "gamma";
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        set_insert(&set2, str2);
        set_insert(&set2, str3);
        
        EXPECT_EQUAL_INT(set_union(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 3);
        EXPECT_TRUE(set_contains(&result, str1));
        EXPECT_TRUE(set_contains(&result, str2));
        EXPECT_TRUE(set_contains(&result, str3));
        EXPECT_FALSE(set_contains(&result, &val1));
        EXPECT_FALSE(set_contains(&result, &val2));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 8: Result set with int hash, union with empty sets using str hash
    {
        Set set1, set2, result;
        set_init(&set1, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&set2, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&result, hash_int_ptr, match_int_ptr, NULL);
        
        int val1 = 42;
        set_insert(&result, &val1);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        
        EXPECT_EQUAL_INT(set_union(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 0);
        EXPECT_TRUE(set_empty(&result));
        EXPECT_FALSE(set_contains(&result, &val1));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 9: Multiple union operations with hash function changes
    {
        Set set1, set2, result;
        set_init(&set1, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&set2, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&result, hash_int_ptr, match_int_ptr, NULL);
        
        char* str1 = "x";
        char* str2 = "y";
        set_insert(&set1, str1);
        set_insert(&set2, str2);
        EXPECT_EQUAL_INT(set_union(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        EXPECT_TRUE(set_contains(&result, str1));
        EXPECT_TRUE(set_contains(&result, str2));
        
        set_clear(&set1);
        set_clear(&set2);
        char* str3 = "z";
        char* str4 = "w";
        set_insert(&set1, str3);
        set_insert(&set2, str4);
        
        EXPECT_EQUAL_INT(set_union(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        EXPECT_TRUE(set_contains(&result, str3));
        EXPECT_TRUE(set_contains(&result, str4));
        EXPECT_FALSE(set_contains(&result, str1));
        EXPECT_FALSE(set_contains(&result, str2));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 10: No double free when set1 has heap-allocated keys and result has values
    {
        Set set1, set2, result;
        set_init(&set1, hash_str_wrapper, match_str_wrapper, free);
        set_init(&set2, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&result, hash_str_wrapper, match_str_wrapper, free);
        
        char* result_val1 = malloc(10);
        char* result_val2 = malloc(10);
        strcpy(result_val1, "result1");
        strcpy(result_val2, "result2");
        set_insert(&result, result_val1);
        set_insert(&result, result_val2);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        
        char* set1_key1 = malloc(10);
        char* set1_key2 = malloc(10);
        strcpy(set1_key1, "set1_key1");
        strcpy(set1_key2, "set1_key2");
        set_insert(&set1, set1_key1);
        set_insert(&set1, set1_key2);
        
        char* set2_key1 = malloc(10);
        strcpy(set2_key1, "set1_key1");
        set_insert(&set2, set2_key1);
        
        EXPECT_EQUAL_INT(set_union(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        EXPECT_TRUE(set_contains(&result, set1_key1));
        EXPECT_TRUE(set_contains(&result, set1_key2));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
        free(set2_key1);
    }
}

/**
 * Test: set_difference
 * Dependencies: set_init, set_insert, set_size, set_contains, set_clear
 * Description: Tests the set_difference function to compute difference.
 */
UTEST_TEST_CASE(set_difference){
    // Test 1: Difference of two sets
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        char* str3 = "c";
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        set_insert(&set2, str2);
        set_insert(&set2, str3);
        
        EXPECT_EQUAL_INT(set_difference(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        EXPECT_TRUE(set_contains(&result, str1));
        EXPECT_FALSE(set_contains(&result, str2));
        EXPECT_FALSE(set_contains(&result, str3));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 2: Difference of disjoint sets
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        set_insert(&set1, str1);
        set_insert(&set2, str2);
        
        EXPECT_EQUAL_INT(set_difference(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        EXPECT_TRUE(set_contains(&result, str1));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 3: Difference of identical sets
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        set_insert(&set2, str1);
        set_insert(&set2, str2);
        
        EXPECT_EQUAL_INT(set_difference(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 0);
        EXPECT_TRUE(set_empty(&result));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 4: Difference with empty second set
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        
        EXPECT_EQUAL_INT(set_difference(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        EXPECT_TRUE(set_contains(&result, str1));
        EXPECT_TRUE(set_contains(&result, str2));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 5: Difference clears result set first
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        char* str3 = "c";
        set_insert(&result, str3);
        set_insert(&set1, str1);
        set_insert(&set2, str2);
        
        EXPECT_EQUAL_INT(set_difference(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        EXPECT_TRUE(set_contains(&result, str1));
        EXPECT_FALSE(set_contains(&result, str3));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 6: Difference with NULL pointers
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        EXPECT_EQUAL_INT(set_difference(NULL, &set1, &set2), COLLECTION_FAILURE);
        EXPECT_EQUAL_INT(set_difference(&result, NULL, &set2), COLLECTION_FAILURE);
        EXPECT_EQUAL_INT(set_difference(&result, &set1, NULL), COLLECTION_FAILURE);
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 7: Result set initially uses int hash, then switches to str hash after difference
    {
        Set set1, set2, result;
        set_init(&set1, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&set2, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&result, hash_int_ptr, match_int_ptr, NULL);
        
        int val1 = 100;
        int val2 = 200;
        set_insert(&result, &val1);
        set_insert(&result, &val2);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        
        char* str1 = "only_in_set1";
        char* str2 = "in_both";
        char* str3 = "only_in_set2";
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        set_insert(&set2, str2);
        set_insert(&set2, str3);
        
        EXPECT_EQUAL_INT(set_difference(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        EXPECT_TRUE(set_contains(&result, str1));
        EXPECT_FALSE(set_contains(&result, str2));
        EXPECT_FALSE(set_contains(&result, str3));
        EXPECT_FALSE(set_contains(&result, &val1));
        EXPECT_FALSE(set_contains(&result, &val2));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 8: Result set with int hash, difference when set1 is subset of set2
    {
        Set set1, set2, result;
        set_init(&set1, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&set2, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&result, hash_int_ptr, match_int_ptr, NULL);
        
        int val1 = 42;
        set_insert(&result, &val1);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        
        char* str1 = "a";
        char* str2 = "b";
        set_insert(&set1, str1);
        set_insert(&set2, str1);
        set_insert(&set2, str2);
        
        EXPECT_EQUAL_INT(set_difference(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 0);
        EXPECT_TRUE(set_empty(&result));
        EXPECT_FALSE(set_contains(&result, &val1));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 9: Multiple difference operations with hash function changes
    {
        Set set1, set2, result;
        set_init(&set1, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&set2, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&result, hash_int_ptr, match_int_ptr, NULL);
        
        char* str1 = "diff1";
        char* str2 = "common";
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        set_insert(&set2, str2);
        EXPECT_EQUAL_INT(set_difference(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        EXPECT_TRUE(set_contains(&result, str1));
        
        set_clear(&set1);
        set_clear(&set2);
        char* str3 = "diff2";
        char* str4 = "common2";
        set_insert(&set1, str3);
        set_insert(&set1, str4);
        set_insert(&set2, str4);
        
        EXPECT_EQUAL_INT(set_difference(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        EXPECT_TRUE(set_contains(&result, str3));
        EXPECT_FALSE(set_contains(&result, str1));
        EXPECT_FALSE(set_contains(&result, str4));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 10: No double free when set1 has heap-allocated keys and result has values
    {
        Set set1, set2, result;
        set_init(&set1, hash_str_wrapper, match_str_wrapper, free);
        set_init(&set2, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&result, hash_str_wrapper, match_str_wrapper, free);
        
        char* result_val1 = malloc(10);
        char* result_val2 = malloc(10);
        strcpy(result_val1, "result1");
        strcpy(result_val2, "result2");
        set_insert(&result, result_val1);
        set_insert(&result, result_val2);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        
        char* set1_key1 = malloc(10);
        char* set1_key2 = malloc(10);
        strcpy(set1_key1, "only_set1");
        strcpy(set1_key2, "in_both");
        set_insert(&set1, set1_key1);
        set_insert(&set1, set1_key2);
        
        char* set2_key1 = malloc(10);
        strcpy(set2_key1, "in_both");
        set_insert(&set2, set2_key1);
        
        EXPECT_EQUAL_INT(set_difference(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        EXPECT_TRUE(set_contains(&result, set1_key1));
        EXPECT_FALSE(set_contains(&result, set1_key2));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
        free(set2_key1);
    }
}

/**
 * Test: set_equal
 * Dependencies: set_init, set_insert, set_size
 * Description: Tests the set_equal function to check equality.
 */
UTEST_TEST_CASE(set_equal){
    // Test 1: Equal sets
    {
        Set set1, set2;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        set_insert(&set2, str1);
        set_insert(&set2, str2);
        
        EXPECT_TRUE(set_equal(&set1, &set2));
        set_destroy(&set1);
        set_destroy(&set2);
    }

    // Test 2: Equal empty sets
    {
        Set set1, set2;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        
        EXPECT_TRUE(set_equal(&set1, &set2));
        set_destroy(&set1);
        set_destroy(&set2);
    }

    // Test 3: Unequal sets with different sizes
    {
        Set set1, set2;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        set_insert(&set2, str1);
        
        EXPECT_FALSE(set_equal(&set1, &set2));
        set_destroy(&set1);
        set_destroy(&set2);
    }

    // Test 4: Unequal sets with same size
    {
        Set set1, set2;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        char* str3 = "c";
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        set_insert(&set2, str1);
        set_insert(&set2, str3);
        
        EXPECT_FALSE(set_equal(&set1, &set2));
        set_destroy(&set1);
        set_destroy(&set2);
    }

    // Test 5: Same set pointer
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        
        EXPECT_TRUE(set_equal(&set, &set));
        set_destroy(&set);
    }

    // Test 6: Equal with NULL pointers
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        
        EXPECT_FALSE(set_equal(NULL, &set));
        EXPECT_FALSE(set_equal(&set, NULL));
        EXPECT_FALSE(set_equal(NULL, NULL));
        
        set_destroy(&set);
    }
}

/**
 * Test: set_subset
 * Dependencies: set_init, set_insert, set_size
 * Description: Tests the set_subset function to check subset relationship.
 */
UTEST_TEST_CASE(set_subset){
    // Test 1: Proper subset
    {
        Set set1, set2;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        char* str3 = "c";
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        set_insert(&set2, str1);
        set_insert(&set2, str2);
        set_insert(&set2, str3);
        
        EXPECT_TRUE(set_subset(&set1, &set2));
        set_destroy(&set1);
        set_destroy(&set2);
    }

    // Test 2: Equal sets (subset)
    {
        Set set1, set2;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        set_insert(&set2, str1);
        set_insert(&set2, str2);
        
        EXPECT_TRUE(set_subset(&set1, &set2));
        set_destroy(&set1);
        set_destroy(&set2);
    }

    // Test 3: Empty set is subset
    {
        Set set1, set2;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        
        char* str1 = "a";
        set_insert(&set2, str1);
        
        EXPECT_TRUE(set_subset(&set1, &set2));
        set_destroy(&set1);
        set_destroy(&set2);
    }

    // Test 4: Not a subset
    {
        Set set1, set2;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        char* str3 = "c";
        set_insert(&set1, str1);
        set_insert(&set1, str3);
        set_insert(&set2, str1);
        set_insert(&set2, str2);
        
        EXPECT_FALSE(set_subset(&set1, &set2));
        set_destroy(&set1);
        set_destroy(&set2);
    }

    // Test 5: Larger set is not subset
    {
        Set set1, set2;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        char* str3 = "c";
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        set_insert(&set1, str3);
        set_insert(&set2, str1);
        set_insert(&set2, str2);
        
        EXPECT_FALSE(set_subset(&set1, &set2));
        set_destroy(&set1);
        set_destroy(&set2);
    }

    // Test 6: Subset with NULL pointers
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        
        EXPECT_FALSE(set_subset(NULL, &set));
        EXPECT_FALSE(set_subset(&set, NULL));
        EXPECT_FALSE(set_subset(NULL, NULL));
        
        set_destroy(&set);
    }
}

/**
 * Test: set_memory_leak
 * Dependencies: set_init, set_destroy, set_insert, set_remove, set_clear, set_intersection, set_union, set_difference
 * Description: Tests for memory leaks by tracking destroy function calls and shadow copy behavior.
 */
UTEST_TEST_CASE(set_memory_leak){
    // Test 1: All elements destroyed on set_destroy
    {
        Set set;
        set_init(&set, NULL, NULL, destroy_counter);
        int* val1 = malloc(sizeof(int));
        int* val2 = malloc(sizeof(int));
        int* val3 = malloc(sizeof(int));
        *val1 = 1;
        *val2 = 2;
        *val3 = 3;
        set_insert(&set, val1);
        set_insert(&set, val2);
        set_insert(&set, val3);
        destroy_count = 0;
        set_destroy(&set);
        EXPECT_EQUAL_INT(destroy_count, 3);
        free(val1);
        free(val2);
        free(val3);
    }

    // Test 2: Elements destroyed on set_remove
    {
        Set set;
        set_init(&set, NULL, NULL, destroy_counter);
        int* val1 = malloc(sizeof(int));
        int* val2 = malloc(sizeof(int));
        *val1 = 1;
        *val2 = 2;
        set_insert(&set, val1);
        set_insert(&set, val2);
        destroy_count = 0;
        set_remove(&set, val1);
        EXPECT_EQUAL_INT(destroy_count, 1);
        set_destroy(&set);
        EXPECT_EQUAL_INT(destroy_count, 2);
        free(val1);
        free(val2);
    }

    // Test 3: Elements destroyed on set_clear
    {
        Set set;
        set_init(&set, NULL, NULL, destroy_counter);
        int* val1 = malloc(sizeof(int));
        int* val2 = malloc(sizeof(int));
        int* val3 = malloc(sizeof(int));
        *val1 = 1;
        *val2 = 2;
        *val3 = 3;
        set_insert(&set, val1);
        set_insert(&set, val2);
        set_insert(&set, val3);
        destroy_count = 0;
        set_clear(&set);
        EXPECT_EQUAL_INT(destroy_count, 3);
        set_destroy(&set);
        EXPECT_EQUAL_INT(destroy_count, 3);
        free(val1);
        free(val2);
        free(val3);
    }

    // Test 4: Shadow copy in set_intersection - original data not destroyed
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, destroy_counter);
        set_init(&set2, NULL, NULL, destroy_counter);
        set_init(&result, NULL, NULL, NULL);
        
        int* val1 = malloc(sizeof(int));
        int* val2 = malloc(sizeof(int));
        int* val3 = malloc(sizeof(int));
        *val1 = 1;
        *val2 = 2;
        *val3 = 3;
        set_insert(&set1, val1);
        set_insert(&set1, val2);
        set_insert(&set2, val2);
        set_insert(&set2, val3);
        
        destroy_count = 0;
        EXPECT_EQUAL_INT(set_intersection(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(destroy_count, 0);
        
        set_destroy(&result);
        EXPECT_EQUAL_INT(destroy_count, 0);
        
        set_destroy(&set1);
        EXPECT_EQUAL_INT(destroy_count, 2);
        set_destroy(&set2);
        EXPECT_EQUAL_INT(destroy_count, 4);
        
        free(val1);
        free(val2);
        free(val3);
    }

    // Test 5: Shadow copy in set_union - original data not destroyed
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, destroy_counter);
        set_init(&set2, NULL, NULL, destroy_counter);
        set_init(&result, NULL, NULL, NULL);
        
        int* val1 = malloc(sizeof(int));
        int* val2 = malloc(sizeof(int));
        int* val3 = malloc(sizeof(int));
        *val1 = 1;
        *val2 = 2;
        *val3 = 3;
        set_insert(&set1, val1);
        set_insert(&set1, val2);
        set_insert(&set2, val2);
        set_insert(&set2, val3);
        
        destroy_count = 0;
        EXPECT_EQUAL_INT(set_union(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(destroy_count, 0);
        
        set_destroy(&result);
        EXPECT_EQUAL_INT(destroy_count, 0);
        
        set_destroy(&set1);
        EXPECT_EQUAL_INT(destroy_count, 2);
        set_destroy(&set2);
        EXPECT_EQUAL_INT(destroy_count, 4);
        
        free(val1);
        free(val2);
        free(val3);
    }

    // Test 6: Shadow copy in set_difference - original data not destroyed
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, destroy_counter);
        set_init(&set2, NULL, NULL, destroy_counter);
        set_init(&result, NULL, NULL, NULL);
        
        int* val1 = malloc(sizeof(int));
        int* val2 = malloc(sizeof(int));
        int* val3 = malloc(sizeof(int));
        *val1 = 1;
        *val2 = 2;
        *val3 = 3;
        set_insert(&set1, val1);
        set_insert(&set1, val2);
        set_insert(&set2, val2);
        set_insert(&set2, val3);
        
        destroy_count = 0;
        EXPECT_EQUAL_INT(set_difference(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(destroy_count, 0);
        
        set_destroy(&result);
        EXPECT_EQUAL_INT(destroy_count, 0);
        
        set_destroy(&set1);
        EXPECT_EQUAL_INT(destroy_count, 2);
        set_destroy(&set2);
        EXPECT_EQUAL_INT(destroy_count, 4);
        
        free(val1);
        free(val2);
        free(val3);
    }

    // Test 7: Elements freed by free
    {
        Set set;
        set_init(&set, NULL, NULL, free);
        for (int i = 0; i < 10; i++) {
            int* value = malloc(sizeof(int));
            *value = i;
            set_insert(&set, value);
        }
        EXPECT_EQUAL_UINT(set_size(&set), 10);
        destroy_count = 0;
        set_destroy(&set);
        EXPECT_EQUAL_INT(destroy_count, 0);
    }
}

/**
 * Test: set_integration
 * Dependencies: All set functions
 * Description: Integration test combining multiple operations.
 */
UTEST_TEST_CASE(set_integration){
    // Test 1: Complex sequence of operations
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        
        char* strs[10] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};
        for (int i = 0; i < 10; i++) {
            EXPECT_EQUAL_INT(set_insert(&set, strs[i]), COLLECTION_SUCCESS);
        }
        EXPECT_EQUAL_UINT(set_size(&set), 10);
        
        EXPECT_EQUAL_INT(set_remove(&set, strs[5]), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&set), 9);
        EXPECT_FALSE(set_contains(&set, strs[5]));
        
        set_clear(&set);
        EXPECT_EQUAL_UINT(set_size(&set), 0);
        EXPECT_TRUE(set_empty(&set));
        
        set_destroy(&set);
    }

    // Test 2: Set operations combination
    {
        Set set1, set2, set3, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&set3, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        char* str3 = "c";
        char* str4 = "d";
        char* str5 = "e";
        
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        set_insert(&set1, str3);
        set_insert(&set2, str2);
        set_insert(&set2, str3);
        set_insert(&set2, str4);
        set_insert(&set3, str4);
        set_insert(&set3, str5);
        
        EXPECT_EQUAL_INT(set_intersection(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        EXPECT_TRUE(set_contains(&result, str2));
        EXPECT_TRUE(set_contains(&result, str3));
        
        EXPECT_EQUAL_INT(set_union(&result, &set1, &set3), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 5);
        
        EXPECT_EQUAL_INT(set_difference(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        EXPECT_TRUE(set_contains(&result, str1));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&set3);
        set_destroy(&result);
    }

    // Test 3: Equality and subset operations
    {
        Set set1, set2, set3;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&set3, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        char* str3 = "c";
        
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        set_insert(&set2, str1);
        set_insert(&set2, str2);
        set_insert(&set3, str1);
        set_insert(&set3, str2);
        set_insert(&set3, str3);
        
        EXPECT_TRUE(set_equal(&set1, &set2));
        EXPECT_FALSE(set_equal(&set1, &set3));
        
        EXPECT_TRUE(set_subset(&set1, &set3));
        EXPECT_FALSE(set_subset(&set3, &set1));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&set3);
    }

    // Test 4: Multiple operations on same set
    {
        Set set;
        set_init(&set, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        char* str3 = "c";
        
        set_insert(&set, str1);
        set_insert(&set, str2);
        EXPECT_EQUAL_UINT(set_size(&set), 2);
        
        set_insert(&set, str1);
        EXPECT_EQUAL_UINT(set_size(&set), 2);
        
        set_remove(&set, str2);
        EXPECT_EQUAL_UINT(set_size(&set), 1);
        EXPECT_TRUE(set_contains(&set, str1));
        
        set_insert(&set, str3);
        EXPECT_EQUAL_UINT(set_size(&set), 2);
        
        set_clear(&set);
        EXPECT_TRUE(set_empty(&set));
        
        set_insert(&set, str1);
        EXPECT_EQUAL_UINT(set_size(&set), 1);
        
        set_destroy(&set);
    }

    // Test 5: Nested set operations
    {
        Set set1, set2, set3, temp, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&set3, NULL, NULL, NULL);
        set_init(&temp, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        char* str1 = "a";
        char* str2 = "b";
        char* str3 = "c";
        char* str4 = "d";
        
        set_insert(&set1, str1);
        set_insert(&set1, str2);
        set_insert(&set2, str2);
        set_insert(&set2, str3);
        set_insert(&set3, str3);
        set_insert(&set3, str4);
        
        EXPECT_EQUAL_INT(set_intersection(&temp, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(set_union(&result, &temp, &set3), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 3);
        EXPECT_TRUE(set_contains(&result, str2));
        EXPECT_TRUE(set_contains(&result, str3));
        EXPECT_TRUE(set_contains(&result, str4));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&set3);
        set_destroy(&temp);
        set_destroy(&result);
    }

    // Test 6: Edge cases combination
    {
        Set set1, set2, result;
        set_init(&set1, NULL, NULL, NULL);
        set_init(&set2, NULL, NULL, NULL);
        set_init(&result, NULL, NULL, NULL);
        
        EXPECT_TRUE(set_empty(&set1));
        EXPECT_TRUE(set_empty(&set2));
        EXPECT_TRUE(set_equal(&set1, &set2));
        EXPECT_TRUE(set_subset(&set1, &set2));
        
        char* str = "a";
        set_insert(&set1, str);
        EXPECT_FALSE(set_equal(&set1, &set2));
        EXPECT_FALSE(set_subset(&set1, &set2));
        EXPECT_TRUE(set_subset(&set2, &set1));
        
        EXPECT_EQUAL_INT(set_intersection(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_TRUE(set_empty(&result));
        
        EXPECT_EQUAL_INT(set_union(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        
        EXPECT_EQUAL_INT(set_difference(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 7: Chain of operations with hash function switching
    {
        Set set1, set2, result;
        set_init(&set1, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&set2, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&result, hash_int_ptr, match_int_ptr, NULL);
        
        int val1 = 999;
        set_insert(&result, &val1);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        
        char* str1 = "u1";
        char* str2 = "u2";
        set_insert(&set1, str1);
        set_insert(&set2, str2);
        EXPECT_EQUAL_INT(set_union(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        EXPECT_TRUE(set_contains(&result, str1));
        EXPECT_TRUE(set_contains(&result, str2));
        EXPECT_FALSE(set_contains(&result, &val1));
        
        set_clear(&set1);
        set_clear(&set2);
        char* str3 = "i1";
        char* str4 = "i2";
        set_insert(&set1, str3);
        set_insert(&set1, str4);
        set_insert(&set2, str3);
        EXPECT_EQUAL_INT(set_intersection(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        EXPECT_TRUE(set_contains(&result, str3));
        EXPECT_FALSE(set_contains(&result, str1));
        EXPECT_FALSE(set_contains(&result, str2));
        
        set_clear(&set1);
        set_clear(&set2);
        char* str5 = "d1";
        char* str6 = "d2";
        set_insert(&set1, str5);
        set_insert(&set1, str6);
        set_insert(&set2, str6);
        EXPECT_EQUAL_INT(set_difference(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        EXPECT_TRUE(set_contains(&result, str5));
        EXPECT_FALSE(set_contains(&result, str3));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 8: Result set with pre-existing elements cleared before each operation
    {
        Set set1, set2, result;
        set_init(&set1, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&set2, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&result, hash_str_wrapper, match_str_wrapper, NULL);
        
        char* str_old1 = "old1";
        char* str_old2 = "old2";
        set_insert(&result, str_old1);
        set_insert(&result, str_old2);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        
        char* str_new1 = "new1";
        char* str_new2 = "new2";
        set_insert(&set1, str_new1);
        set_insert(&set2, str_new2);
        EXPECT_EQUAL_INT(set_union(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        EXPECT_TRUE(set_contains(&result, str_new1));
        EXPECT_TRUE(set_contains(&result, str_new2));
        EXPECT_FALSE(set_contains(&result, str_old1));
        EXPECT_FALSE(set_contains(&result, str_old2));
        
        char* str_old3 = "old3";
        set_insert(&result, str_old3);
        EXPECT_EQUAL_UINT(set_size(&result), 3);
        
        set_clear(&set1);
        set_clear(&set2);
        char* str_new3 = "new3";
        set_insert(&set1, str_new3);
        set_insert(&set2, str_new3);
        EXPECT_EQUAL_INT(set_intersection(&result, &set1, &set2), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(set_size(&result), 1);
        EXPECT_TRUE(set_contains(&result, str_new3));
        EXPECT_FALSE(set_contains(&result, str_old3));
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }

    // Test 9: Verify hash function is correctly inherited from input sets
    {
        Set set1, set2, result;
        set_init(&set1, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&set2, hash_str_wrapper, match_str_wrapper, NULL);
        set_init(&result, hash_int_ptr, match_int_ptr, NULL);
        
        int val = 123;
        set_insert(&result, &val);
        
        char* str = "test_string";
        set_insert(&set1, str);
        EXPECT_EQUAL_INT(set_union(&result, &set1, &set2), COLLECTION_SUCCESS);
        
        EXPECT_TRUE(set_contains(&result, str));
        EXPECT_FALSE(set_contains(&result, &val));
        
        char* str2 = "another_string";
        EXPECT_EQUAL_INT(set_insert(&result, str2), COLLECTION_SUCCESS);
        EXPECT_TRUE(set_contains(&result, str2));
        EXPECT_EQUAL_UINT(set_size(&result), 2);
        
        set_destroy(&set1);
        set_destroy(&set2);
        set_destroy(&result);
    }
}

/**
 * Test suite: set
 * Description: Test suite for set data structure
 */
UTEST_TEST_SUITE(set){
    UTEST_RUN_TEST_CASE(set_init);
    UTEST_RUN_TEST_CASE(set_destroy);
    UTEST_RUN_TEST_CASE(set_empty);
    UTEST_RUN_TEST_CASE(set_size);
    UTEST_RUN_TEST_CASE(set_insert);
    UTEST_RUN_TEST_CASE(set_remove);
    UTEST_RUN_TEST_CASE(set_clear);
    UTEST_RUN_TEST_CASE(set_contains);
    UTEST_RUN_TEST_CASE(set_intersection);
    UTEST_RUN_TEST_CASE(set_union);
    UTEST_RUN_TEST_CASE(set_difference);
    UTEST_RUN_TEST_CASE(set_equal);
    UTEST_RUN_TEST_CASE(set_subset);
    UTEST_RUN_TEST_CASE(set_memory_leak);
    UTEST_RUN_TEST_CASE(set_integration);
}

