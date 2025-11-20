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

#include <collection/chtbl.h>
#include <collection/common.h>
#include <algorithm/hash.h>
#include <utest.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

/* Global destroy counters for testing */
static int destroy_key_count = 0;
static int destroy_value_count = 0;

static void destroy_key_counter(void* key) {
    if (key != NULL) {
        destroy_key_count++;
        free(key);
    }
}

static void destroy_value_counter(void* value) {
    if (value != NULL) {
        destroy_value_count++;
        free(value);
    }
}

static void destroy_both_counters(void* ptr) {
    destroy_key_counter(ptr);
    destroy_value_counter(ptr);
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
 * Test: chtbl_init
 * Dependencies: None
 * Description: Tests basic initialization of the chained hash table structure.
 */
UTEST_TEST_CASE(chtbl_init){
    // Test 1: Initialize with all NULL functions
    {
        Chtbl chtbl;
        EXPECT_EQUAL_INT(chtbl_init(&chtbl, NULL, NULL, NULL, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        EXPECT_TRUE(chtbl_buckets(&chtbl) > 0);
        chtbl_destroy(&chtbl);
    }

    // Test 2: Initialize with custom hash and match functions
    {
        Chtbl chtbl;
        EXPECT_EQUAL_INT(chtbl_init(&chtbl, hash_str_wrapper, match_str_wrapper, NULL, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 3: Initialize with destroy functions
    {
        Chtbl chtbl;
        EXPECT_EQUAL_INT(chtbl_init(&chtbl, NULL, NULL, free, free), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 4: Initialize with NULL chtbl
    {
        EXPECT_EQUAL_INT(chtbl_init(NULL, NULL, NULL, NULL, NULL), COLLECTION_FAILURE);
    }

    // Test 5: Initialize with only hash function
    {
        Chtbl chtbl;
        EXPECT_EQUAL_INT(chtbl_init(&chtbl, hash_str_wrapper, NULL, NULL, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 6: Initialize with only match function
    {
        Chtbl chtbl;
        EXPECT_EQUAL_INT(chtbl_init(&chtbl, NULL, match_str_wrapper, NULL, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 7: Initialize with only destroy_key function
    {
        Chtbl chtbl;
        EXPECT_EQUAL_INT(chtbl_init(&chtbl, NULL, NULL, free, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 8: Initialize with only destroy_value function
    {
        Chtbl chtbl;
        EXPECT_EQUAL_INT(chtbl_init(&chtbl, NULL, NULL, NULL, free), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_init_capacity
 * Dependencies: None
 * Description: Tests initialization with specified capacity.
 */
UTEST_TEST_CASE(chtbl_init_capacity){
    // Test 1: Initialize with valid capacity
    {
        Chtbl chtbl;
        EXPECT_EQUAL_INT(chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 32), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_buckets(&chtbl), 32);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 2: Initialize with capacity below minimum
    {
        Chtbl chtbl;
        EXPECT_EQUAL_INT(chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 4), COLLECTION_SUCCESS);
        EXPECT_TRUE(chtbl_buckets(&chtbl) >= 8);
        chtbl_destroy(&chtbl);
    }

    // Test 3: Initialize with large capacity
    {
        Chtbl chtbl;
        EXPECT_EQUAL_INT(chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 128), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_buckets(&chtbl), 128);
        chtbl_destroy(&chtbl);
    }

    // Test 4: Initialize with NULL chtbl
    {
        EXPECT_EQUAL_INT(chtbl_init_capacity(NULL, NULL, NULL, NULL, NULL, 16), COLLECTION_FAILURE);
    }

    // Test 5: Initialize with zero capacity (should clamp to minimum)
    {
        Chtbl chtbl;
        EXPECT_EQUAL_INT(chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 0), COLLECTION_SUCCESS);
        EXPECT_TRUE(chtbl_buckets(&chtbl) >= 8);
        chtbl_destroy(&chtbl);
    }

    // Test 6: Initialize with capacity exactly at minimum
    {
        Chtbl chtbl;
        EXPECT_EQUAL_INT(chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 8), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_buckets(&chtbl), 8);
        chtbl_destroy(&chtbl);
    }

    // Test 7: Initialize with very large capacity
    {
        Chtbl chtbl;
        EXPECT_EQUAL_INT(chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 1024), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_buckets(&chtbl), 1024);
        chtbl_destroy(&chtbl);
    }

    // Test 8: Initialize with capacity just below minimum
    {
        Chtbl chtbl;
        EXPECT_EQUAL_INT(chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 7), COLLECTION_SUCCESS);
        EXPECT_TRUE(chtbl_buckets(&chtbl) >= 8);
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_size
 * Dependencies: chtbl_init
 * Description: Tests the chtbl_size function to get the number of entries.
 */
UTEST_TEST_CASE(chtbl_size){
    // Test 1: Size of empty hash table
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 2: Size after insertions
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        chtbl_insert(&chtbl, "key2", "value2");
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 2);
        chtbl_insert(&chtbl, "key3", "value3");
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 3);
        chtbl_destroy(&chtbl);
    }

    // Test 3: Size after update
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        chtbl_insert(&chtbl, "key1", "value2");
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        chtbl_destroy(&chtbl);
    }

    // Test 4: Size after removal
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_remove(&chtbl, "key1", NULL);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        chtbl_destroy(&chtbl);
    }

    // Test 5: Size with NULL chtbl
    {
        EXPECT_EQUAL_UINT(chtbl_size(NULL), 0);
    }

    // Test 6: Size after clear then insert
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        chtbl_insert(&chtbl, "key3", "value3");
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        chtbl_destroy(&chtbl);
    }

    // Test 7: Size with many entries
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        char keys[100][32];
        for (int i = 0; i < 100; i++) {
            snprintf(keys[i], sizeof(keys[i]), "key%d", i);
            EXPECT_EQUAL_INT(chtbl_insert(&chtbl, keys[i], "value"), COLLECTION_SUCCESS);
        }
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 100);
        chtbl_destroy(&chtbl);
    }

    // Test 8: Size after multiple updates of same key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        chtbl_insert(&chtbl, "key1", "value2");
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        chtbl_insert(&chtbl, "key1", "value3");
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_buckets
 * Dependencies: chtbl_init, chtbl_init_capacity
 * Description: Tests the chtbl_buckets function to get the number of buckets.
 */
UTEST_TEST_CASE(chtbl_buckets){
    // Test 1: Default buckets
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        EXPECT_TRUE(chtbl_buckets(&chtbl) > 0);
        chtbl_destroy(&chtbl);
    }

    // Test 2: Custom capacity buckets
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 64);
        EXPECT_EQUAL_UINT(chtbl_buckets(&chtbl), 64);
        chtbl_destroy(&chtbl);
    }

    // Test 3: Buckets remain same after insertions
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 32);
        size_t initial_buckets = chtbl_buckets(&chtbl);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        EXPECT_EQUAL_UINT(chtbl_buckets(&chtbl), initial_buckets);
        chtbl_destroy(&chtbl);
    }

    // Test 4: Buckets change after resize
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        EXPECT_EQUAL_UINT(chtbl_buckets(&chtbl), 16);
        chtbl_resize(&chtbl, 32);
        EXPECT_EQUAL_UINT(chtbl_buckets(&chtbl), 32);
        chtbl_destroy(&chtbl);
    }

    // Test 5: Buckets with NULL chtbl
    {
        EXPECT_EQUAL_UINT(chtbl_buckets(NULL), 0);
    }

    // Test 6: Buckets after multiple resizes
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        EXPECT_EQUAL_UINT(chtbl_buckets(&chtbl), 16);
        chtbl_resize(&chtbl, 32);
        EXPECT_EQUAL_UINT(chtbl_buckets(&chtbl), 32);
        chtbl_resize(&chtbl, 64);
        EXPECT_EQUAL_UINT(chtbl_buckets(&chtbl), 64);
        chtbl_resize(&chtbl, 128);
        EXPECT_EQUAL_UINT(chtbl_buckets(&chtbl), 128);
        chtbl_destroy(&chtbl);
    }

    // Test 7: Buckets with very large initial capacity
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 2048);
        EXPECT_EQUAL_UINT(chtbl_buckets(&chtbl), 2048);
        chtbl_destroy(&chtbl);
    }

    // Test 8: Buckets remain same after clear
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 32);
        size_t buckets_before = chtbl_buckets(&chtbl);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_UINT(chtbl_buckets(&chtbl), buckets_before);
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_in
 * Dependencies: chtbl_init, chtbl_insert
 * Description: Tests the chtbl_in function to check if a key exists.
 */
UTEST_TEST_CASE(chtbl_in){
    // Test 1: Key not in empty hash table
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        EXPECT_FALSE(chtbl_in(&chtbl, "nonexistent"));
        chtbl_destroy(&chtbl);
    }

    // Test 2: Key exists after insertion
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        EXPECT_TRUE(chtbl_in(&chtbl, "key1"));
        chtbl_destroy(&chtbl);
    }

    // Test 3: Key doesn't exist
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        EXPECT_FALSE(chtbl_in(&chtbl, "key2"));
        chtbl_destroy(&chtbl);
    }

    // Test 4: Key removed
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_remove(&chtbl, "key1", NULL);
        EXPECT_FALSE(chtbl_in(&chtbl, "key1"));
        chtbl_destroy(&chtbl);
    }

    // Test 5: Multiple keys
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_insert(&chtbl, "key3", "value3");
        EXPECT_TRUE(chtbl_in(&chtbl, "key1"));
        EXPECT_TRUE(chtbl_in(&chtbl, "key2"));
        EXPECT_TRUE(chtbl_in(&chtbl, "key3"));
        EXPECT_FALSE(chtbl_in(&chtbl, "key4"));
        chtbl_destroy(&chtbl);
    }

    // Test 6: Key with NULL chtbl
    {
        EXPECT_FALSE(chtbl_in(NULL, "key"));
    }

    // Test 7: Empty string key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "", "empty_key_value");
        EXPECT_TRUE(chtbl_in(&chtbl, ""));
        chtbl_destroy(&chtbl);
    }

    // Test 8: Key after resize
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 8);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_resize(&chtbl, 32);
        EXPECT_TRUE(chtbl_in(&chtbl, "key1"));
        EXPECT_TRUE(chtbl_in(&chtbl, "key2"));
        chtbl_destroy(&chtbl);
    }

    // Test 9: Keys that hash to same bucket (collision)
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 4);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_insert(&chtbl, "key3", "value3");
        EXPECT_TRUE(chtbl_in(&chtbl, "key1"));
        EXPECT_TRUE(chtbl_in(&chtbl, "key2"));
        EXPECT_TRUE(chtbl_in(&chtbl, "key3"));
        chtbl_destroy(&chtbl);
    }

    // Test 10: Key with NULL pointer
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        EXPECT_FALSE(chtbl_in(&chtbl, NULL));
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_insert
 * Dependencies: chtbl_init, chtbl_size
 * Description: Tests inserting key-value pairs into the hash table.
 */
UTEST_TEST_CASE(chtbl_insert){
    // Test 1: Insert into empty hash table
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        EXPECT_EQUAL_INT(chtbl_insert(&chtbl, "key1", "value1"), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        EXPECT_TRUE(chtbl_in(&chtbl, "key1"));
        chtbl_destroy(&chtbl);
    }

    // Test 2: Insert multiple entries
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        EXPECT_EQUAL_INT(chtbl_insert(&chtbl, "key1", "value1"), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(chtbl_insert(&chtbl, "key2", "value2"), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(chtbl_insert(&chtbl, "key3", "value3"), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 3);
        chtbl_destroy(&chtbl);
    }

    // Test 3: Update existing key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        EXPECT_EQUAL_INT(chtbl_insert(&chtbl, "key1", "value2"), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        EXPECT_EQUAL_UINT(strcmp((char*)chtbl_find(&chtbl, "key1"), "value2"), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 4: Insert NULL value
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        EXPECT_EQUAL_INT(chtbl_insert(&chtbl, "key1", NULL), COLLECTION_SUCCESS);
        EXPECT_NULL(chtbl_find(&chtbl, "key1"));
        chtbl_destroy(&chtbl);
    }

    // Test 5: Insert with NULL chtbl
    {
        EXPECT_EQUAL_INT(chtbl_insert(NULL, "key", "value"), COLLECTION_FAILURE);
    }

    // Test 6: Insert with NULL key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        EXPECT_EQUAL_INT(chtbl_insert(&chtbl, NULL, "value"), COLLECTION_FAILURE);
        chtbl_destroy(&chtbl);
    }

    // Test 7: Insert with integer keys
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, hash_int_ptr, match_int_ptr, NULL, NULL);
        int key1 = 1, key2 = 2, key3 = 3;
        EXPECT_EQUAL_INT(chtbl_insert(&chtbl, &key1, "value1"), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(chtbl_insert(&chtbl, &key2, "value2"), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(chtbl_insert(&chtbl, &key3, "value3"), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 3);
        chtbl_destroy(&chtbl);
    }

    // Test 8: Insert empty string key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        EXPECT_EQUAL_INT(chtbl_insert(&chtbl, "", "empty_key"), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        EXPECT_TRUE(chtbl_in(&chtbl, ""));
        chtbl_destroy(&chtbl);
    }

    // Test 9: Insert many entries to trigger automatic resize
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 8);
        chtbl_set_load_factor(&chtbl, 0.75f);
        size_t initial_buckets = chtbl_buckets(&chtbl);
        char keys[20][32];
        for (int i = 0; i < 20; i++) {
            snprintf(keys[i], sizeof(keys[i]), "key%d", i);
            EXPECT_EQUAL_INT(chtbl_insert(&chtbl, keys[i], "value"), COLLECTION_SUCCESS);
        }
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 20);
        chtbl_destroy(&chtbl);
    }

    // Test 10: Insert same key multiple times (should update)
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        EXPECT_EQUAL_INT(chtbl_insert(&chtbl, "key1", "value1"), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(chtbl_insert(&chtbl, "key1", "value2"), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(chtbl_insert(&chtbl, "key1", "value3"), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        EXPECT_EQUAL_UINT(strcmp((char*)chtbl_find(&chtbl, "key1"), "value3"), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 11: Insert after clear
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_INT(chtbl_insert(&chtbl, "key2", "value2"), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        chtbl_destroy(&chtbl);
    }

    // Test 12: Insert with collision keys
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 4);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_insert(&chtbl, "key3", "value3");
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 3);
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_find
 * Dependencies: chtbl_init, chtbl_insert
 * Description: Tests finding values by key.
 */
UTEST_TEST_CASE(chtbl_find){
    // Test 1: Find in empty hash table
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        EXPECT_NULL(chtbl_find(&chtbl, "nonexistent"));
        chtbl_destroy(&chtbl);
    }

    // Test 2: Find existing key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        void* value = chtbl_find(&chtbl, "key1");
        EXPECT_NOT_NULL(value);
        EXPECT_EQUAL_UINT(strcmp((char*)value, "value1"), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 3: Find non-existent key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        EXPECT_NULL(chtbl_find(&chtbl, "key2"));
        chtbl_destroy(&chtbl);
    }

    // Test 4: Find after update
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key1", "value2");
        void* value = chtbl_find(&chtbl, "key1");
        EXPECT_EQUAL_UINT(strcmp((char*)value, "value2"), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 5: Find with NULL chtbl
    {
        EXPECT_NULL(chtbl_find(NULL, "key"));
    }

    // Test 6: Find with NULL key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        EXPECT_NULL(chtbl_find(&chtbl, NULL));
        chtbl_destroy(&chtbl);
    }

    // Test 7: Find after resize
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 8);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_resize(&chtbl, 32);
        void* value1 = chtbl_find(&chtbl, "key1");
        void* value2 = chtbl_find(&chtbl, "key2");
        EXPECT_NOT_NULL(value1);
        EXPECT_NOT_NULL(value2);
        EXPECT_EQUAL_UINT(strcmp((char*)value1, "value1"), 0);
        EXPECT_EQUAL_UINT(strcmp((char*)value2, "value2"), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 8: Find with collision keys
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 4);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_insert(&chtbl, "key3", "value3");
        void* value1 = chtbl_find(&chtbl, "key1");
        void* value2 = chtbl_find(&chtbl, "key2");
        void* value3 = chtbl_find(&chtbl, "key3");
        EXPECT_NOT_NULL(value1);
        EXPECT_NOT_NULL(value2);
        EXPECT_NOT_NULL(value3);
        chtbl_destroy(&chtbl);
    }

    // Test 9: Find empty string key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "", "empty_key_value");
        void* value = chtbl_find(&chtbl, "");
        EXPECT_NOT_NULL(value);
        EXPECT_EQUAL_UINT(strcmp((char*)value, "empty_key_value"), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 10: Find after multiple updates
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key1", "value2");
        chtbl_insert(&chtbl, "key1", "value3");
        void* value = chtbl_find(&chtbl, "key1");
        EXPECT_NOT_NULL(value);
        EXPECT_EQUAL_UINT(strcmp((char*)value, "value3"), 0);
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_find_entry
 * Dependencies: chtbl_init, chtbl_insert
 * Description: Tests finding entries by key.
 */
UTEST_TEST_CASE(chtbl_find_entry){
    // Test 1: Find entry in empty hash table
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        EXPECT_NULL(chtbl_find_entry(&chtbl, "nonexistent"));
        chtbl_destroy(&chtbl);
    }

    // Test 2: Find existing entry
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        ChtblEntry* entry = chtbl_find_entry(&chtbl, "key1");
        EXPECT_NOT_NULL(entry);
        EXPECT_EQUAL_UINT(strcmp((char*)entry->key, "key1"), 0);
        EXPECT_EQUAL_UINT(strcmp((char*)entry->value, "value1"), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 3: Find non-existent entry
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        EXPECT_NULL(chtbl_find_entry(&chtbl, "key2"));
        chtbl_destroy(&chtbl);
    }

    // Test 4: Entry pointer remains valid until removal
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        ChtblEntry* entry1 = chtbl_find_entry(&chtbl, "key1");
        chtbl_insert(&chtbl, "key2", "value2");
        ChtblEntry* entry2 = chtbl_find_entry(&chtbl, "key1");
        EXPECT_EQUAL_UINT(entry1, entry2);
        chtbl_destroy(&chtbl);
    }

    // Test 5: Find with NULL chtbl
    {
        EXPECT_NULL(chtbl_find_entry(NULL, "key"));
    }

    // Test 6: Find with NULL key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        EXPECT_NULL(chtbl_find_entry(&chtbl, NULL));
        chtbl_destroy(&chtbl);
    }

    // Test 7: Find entry after resize
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 8);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_resize(&chtbl, 32);
        ChtblEntry* entry = chtbl_find_entry(&chtbl, "key1");
        EXPECT_NOT_NULL(entry);
        EXPECT_EQUAL_UINT(strcmp((char*)entry->key, "key1"), 0);
        EXPECT_EQUAL_UINT(strcmp((char*)entry->value, "value1"), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 8: Find entry with collision keys
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 4);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        ChtblEntry* entry1 = chtbl_find_entry(&chtbl, "key1");
        ChtblEntry* entry2 = chtbl_find_entry(&chtbl, "key2");
        EXPECT_NOT_NULL(entry1);
        EXPECT_NOT_NULL(entry2);
        EXPECT_NOT_EQUAL_UINT(entry1, entry2);
        chtbl_destroy(&chtbl);
    }

    // Test 9: Find entry with empty string key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "", "empty_key_value");
        ChtblEntry* entry = chtbl_find_entry(&chtbl, "");
        EXPECT_NOT_NULL(entry);
        EXPECT_EQUAL_UINT(strcmp((char*)entry->key, ""), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 10: Entry pointer invalidated after removal
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        ChtblEntry* entry = chtbl_find_entry(&chtbl, "key1");
        EXPECT_NOT_NULL(entry);
        chtbl_remove(&chtbl, "key1", NULL);
        EXPECT_NULL(chtbl_find_entry(&chtbl, "key1"));
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_remove
 * Dependencies: chtbl_init, chtbl_insert, chtbl_size
 * Description: Tests removing key-value pairs from the hash table.
 */
UTEST_TEST_CASE(chtbl_remove){
    // Test 1: Remove from empty hash table
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        EXPECT_EQUAL_INT(chtbl_remove(&chtbl, "nonexistent", NULL), COLLECTION_FAILURE);
        chtbl_destroy(&chtbl);
    }

    // Test 2: Remove without retrieving entry
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        EXPECT_EQUAL_INT(chtbl_remove(&chtbl, "key1", NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        EXPECT_FALSE(chtbl_in(&chtbl, "key1"));
        chtbl_destroy(&chtbl);
    }

    // Test 3: Remove with retrieving entry
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        ChtblEntry* entry = NULL;
        EXPECT_EQUAL_INT(chtbl_remove(&chtbl, "key1", &entry), COLLECTION_SUCCESS);
        EXPECT_NOT_NULL(entry);
        EXPECT_EQUAL_UINT(strcmp((char*)entry->key, "key1"), 0);
        EXPECT_EQUAL_UINT(strcmp((char*)entry->value, "value1"), 0);
        free(entry);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 4: Remove non-existent key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        EXPECT_EQUAL_INT(chtbl_remove(&chtbl, "key2", NULL), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        chtbl_destroy(&chtbl);
    }

    // Test 5: Remove with NULL chtbl
    {
        EXPECT_EQUAL_INT(chtbl_remove(NULL, "key", NULL), COLLECTION_FAILURE);
    }

    // Test 6: Remove with NULL key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        EXPECT_EQUAL_INT(chtbl_remove(&chtbl, NULL, NULL), COLLECTION_FAILURE);
        chtbl_destroy(&chtbl);
    }

    // Test 7: Remove multiple entries
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_insert(&chtbl, "key3", "value3");
        chtbl_remove(&chtbl, "key1", NULL);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 2);
        chtbl_remove(&chtbl, "key2", NULL);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        chtbl_remove(&chtbl, "key3", NULL);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 8: Remove from collision chain (middle)
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 4);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_insert(&chtbl, "key3", "value3");
        EXPECT_EQUAL_INT(chtbl_remove(&chtbl, "key2", NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 2);
        EXPECT_TRUE(chtbl_in(&chtbl, "key1"));
        EXPECT_FALSE(chtbl_in(&chtbl, "key2"));
        EXPECT_TRUE(chtbl_in(&chtbl, "key3"));
        chtbl_destroy(&chtbl);
    }

    // Test 9: Remove from collision chain (end)
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 4);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_insert(&chtbl, "key3", "value3");
        EXPECT_EQUAL_INT(chtbl_remove(&chtbl, "key3", NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 2);
        EXPECT_TRUE(chtbl_in(&chtbl, "key1"));
        EXPECT_TRUE(chtbl_in(&chtbl, "key2"));
        EXPECT_FALSE(chtbl_in(&chtbl, "key3"));
        chtbl_destroy(&chtbl);
    }

    // Test 10: Remove all entries one by one
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        char keys[10][32];
        for (int i = 0; i < 10; i++) {
            snprintf(keys[i], sizeof(keys[i]), "key%d", i);
            chtbl_insert(&chtbl, keys[i], "value");
        }
        for (int i = 0; i < 10; i++) {
            EXPECT_EQUAL_INT(chtbl_remove(&chtbl, keys[i], NULL), COLLECTION_SUCCESS);
            EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 9 - i);
        }
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 11: Remove after resize
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 8);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_resize(&chtbl, 32);
        EXPECT_EQUAL_INT(chtbl_remove(&chtbl, "key1", NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        EXPECT_FALSE(chtbl_in(&chtbl, "key1"));
        EXPECT_TRUE(chtbl_in(&chtbl, "key2"));
        chtbl_destroy(&chtbl);
    }

    // Test 12: Remove empty string key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "", "empty_key_value");
        EXPECT_EQUAL_INT(chtbl_remove(&chtbl, "", NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        EXPECT_FALSE(chtbl_in(&chtbl, ""));
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_clear
 * Dependencies: chtbl_init, chtbl_insert, chtbl_size
 * Description: Tests clearing all entries from the hash table.
 */
UTEST_TEST_CASE(chtbl_clear){
    // Test 1: Clear empty hash table
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 2: Clear hash table with entries
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_insert(&chtbl, "key3", "value3");
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        EXPECT_FALSE(chtbl_in(&chtbl, "key1"));
        chtbl_destroy(&chtbl);
    }

    // Test 3: Buckets remain after clear
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 32);
        size_t buckets_before = chtbl_buckets(&chtbl);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_UINT(chtbl_buckets(&chtbl), buckets_before);
        chtbl_destroy(&chtbl);
    }

    // Test 4: Use after clear
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_clear(&chtbl);
        chtbl_insert(&chtbl, "key2", "value2");
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        EXPECT_TRUE(chtbl_in(&chtbl, "key2"));
        chtbl_destroy(&chtbl);
    }

    // Test 5: Clear multiple times
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_clear(&chtbl);
        chtbl_clear(&chtbl);
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 6: Clear then insert many entries
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_clear(&chtbl);
        char keys[20][32];
        for (int i = 0; i < 20; i++) {
            snprintf(keys[i], sizeof(keys[i]), "key%d", i);
            chtbl_insert(&chtbl, keys[i], "value");
        }
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 20);
        chtbl_destroy(&chtbl);
    }

    // Test 7: Clear with many entries
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        char keys[100][32];
        for (int i = 0; i < 100; i++) {
            snprintf(keys[i], sizeof(keys[i]), "key%d", i);
            chtbl_insert(&chtbl, keys[i], "value");
        }
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 8: Clear empty hash table
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_set_load_factor
 * Dependencies: chtbl_init
 * Description: Tests setting the load factor threshold.
 */
UTEST_TEST_CASE(chtbl_set_load_factor){
    // Test 1: Set valid load factor
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_set_load_factor(&chtbl, 0.5f);
        chtbl_destroy(&chtbl);
    }

    // Test 2: Set load factor to 1.0
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_set_load_factor(&chtbl, 1.0f);
        chtbl_destroy(&chtbl);
    }

    // Test 3: Set load factor above maximum (should clamp)
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_set_load_factor(&chtbl, 2.0f);
        chtbl_destroy(&chtbl);
    }

    // Test 4: Set load factor with NULL chtbl
    {
        chtbl_set_load_factor(NULL, 0.75f);
    }

    // Test 5: Set negative load factor (should be ignored)
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_set_load_factor(&chtbl, -1.0f);
        chtbl_destroy(&chtbl);
    }

    // Test 6: Set load factor to exactly 0.0 (edge case)
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_set_load_factor(&chtbl, 0.0f);
        chtbl_destroy(&chtbl);
    }

    // Test 7: Set load factor to exactly 1.0
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_set_load_factor(&chtbl, 1.0f);
        chtbl_destroy(&chtbl);
    }

    // Test 8: Set load factor multiple times
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_set_load_factor(&chtbl, 0.5f);
        chtbl_set_load_factor(&chtbl, 0.75f);
        chtbl_set_load_factor(&chtbl, 0.9f);
        chtbl_destroy(&chtbl);
    }

    // Test 9: Set load factor with very small value
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_set_load_factor(&chtbl, 0.001f);
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_load_factor
 * Dependencies: chtbl_init, chtbl_insert
 * Description: Tests getting the current load factor.
 */
UTEST_TEST_CASE(chtbl_load_factor){
    // Test 1: Load factor of empty hash table
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        EXPECT_EQUAL_UINT(chtbl_load_factor(&chtbl), 0.0f);
        chtbl_destroy(&chtbl);
    }

    // Test 2: Load factor after insertions
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        chtbl_insert(&chtbl, "key1", "value1");
        float load = chtbl_load_factor(&chtbl);
        EXPECT_TRUE(load > 0.0f);
        EXPECT_TRUE(load <= 1.0f);
        chtbl_destroy(&chtbl);
    }

    // Test 3: Load factor increases with more entries
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        float load1 = chtbl_load_factor(&chtbl);
        chtbl_insert(&chtbl, "key1", "value1");
        float load2 = chtbl_load_factor(&chtbl);
        chtbl_insert(&chtbl, "key2", "value2");
        float load3 = chtbl_load_factor(&chtbl);
        EXPECT_TRUE(load1 < load2);
        EXPECT_TRUE(load2 < load3);
        chtbl_destroy(&chtbl);
    }

    // Test 4: Load factor with NULL chtbl
    {
        EXPECT_EQUAL_UINT(chtbl_load_factor(NULL), 0.0f);
    }

    // Test 5: Load factor after resize
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 8);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        float load_before = chtbl_load_factor(&chtbl);
        chtbl_resize(&chtbl, 32);
        float load_after = chtbl_load_factor(&chtbl);
        EXPECT_TRUE(load_after < load_before);
        chtbl_destroy(&chtbl);
    }

    // Test 6: Load factor with many entries
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        char keys[50][32];
        for (int i = 0; i < 50; i++) {
            snprintf(keys[i], sizeof(keys[i]), "key%d", i);
            chtbl_insert(&chtbl, keys[i], "value");
        }
        float load = chtbl_load_factor(&chtbl);
        EXPECT_TRUE(load > 0.0f);
        EXPECT_TRUE(load <= 1.0f);
        chtbl_destroy(&chtbl);
    }

    // Test 7: Load factor after clear
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_UINT(chtbl_load_factor(&chtbl), 0.0f);
        chtbl_destroy(&chtbl);
    }

    // Test 8: Load factor with single bucket
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 8);
        chtbl_insert(&chtbl, "key1", "value1");
        float load = chtbl_load_factor(&chtbl);
        EXPECT_TRUE(load > 0.0f);
        EXPECT_TRUE(load <= 1.0f);
        chtbl_destroy(&chtbl);
    }

    // Test 9: Load factor increases after insert
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        EXPECT_EQUAL_UINT(chtbl_load_factor(&chtbl), 0.0f);
        
        chtbl_insert(&chtbl, "key1", "value1");
        float load1 = chtbl_load_factor(&chtbl);
        EXPECT_EQUAL_UINT(load1, 1.0f / 16.0f);
        
        chtbl_insert(&chtbl, "key2", "value2");
        float load2 = chtbl_load_factor(&chtbl);
        EXPECT_EQUAL_UINT(load2, 2.0f / 16.0f);
        
        chtbl_insert(&chtbl, "key3", "value3");
        float load3 = chtbl_load_factor(&chtbl);
        EXPECT_EQUAL_UINT(load3, 3.0f / 16.0f);
        
        chtbl_destroy(&chtbl);
    }

    // Test 10: Load factor decreases after remove
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_insert(&chtbl, "key3", "value3");
        float load_before = chtbl_load_factor(&chtbl);
        EXPECT_EQUAL_UINT(load_before, 3.0f / 16.0f);
        
        chtbl_remove(&chtbl, "key1", NULL);
        float load_after1 = chtbl_load_factor(&chtbl);
        EXPECT_EQUAL_UINT(load_after1, 2.0f / 16.0f);
        
        chtbl_remove(&chtbl, "key2", NULL);
        float load_after2 = chtbl_load_factor(&chtbl);
        EXPECT_EQUAL_UINT(load_after2, 1.0f / 16.0f);
        
        chtbl_remove(&chtbl, "key3", NULL);
        float load_after3 = chtbl_load_factor(&chtbl);
        EXPECT_EQUAL_UINT(load_after3, 0.0f);
        
        chtbl_destroy(&chtbl);
    }

    // Test 11: Load factor stays same after update
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        float load_before = chtbl_load_factor(&chtbl);
        EXPECT_EQUAL_UINT(load_before, 2.0f / 16.0f);
        
        chtbl_insert(&chtbl, "key1", "newvalue1");
        float load_after = chtbl_load_factor(&chtbl);
        EXPECT_EQUAL_UINT(load_after, 2.0f / 16.0f);
        EXPECT_EQUAL_UINT(load_before, load_after);
        
        chtbl_destroy(&chtbl);
    }

    // Test 12: Load factor after insert then remove then insert
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        chtbl_insert(&chtbl, "key1", "value1");
        float load1 = chtbl_load_factor(&chtbl);
        EXPECT_EQUAL_UINT(load1, 1.0f / 16.0f);
        
        chtbl_remove(&chtbl, "key1", NULL);
        float load2 = chtbl_load_factor(&chtbl);
        EXPECT_EQUAL_UINT(load2, 0.0f);
        
        chtbl_insert(&chtbl, "key2", "value2");
        float load3 = chtbl_load_factor(&chtbl);
        EXPECT_EQUAL_UINT(load3, 1.0f / 16.0f);
        
        chtbl_destroy(&chtbl);
    }

    // Test 13: Load factor with exact bucket count
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 8);
        // Set threshold to 1.0 to prevent automatic resize
        chtbl_set_load_factor(&chtbl, 1.0f);
        char keys[8][32];
        for (int i = 0; i < 8; i++) {
            snprintf(keys[i], sizeof(keys[i]), "key%d", i);
            chtbl_insert(&chtbl, keys[i], "value");
        }
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 8);
        float load = chtbl_load_factor(&chtbl);
        EXPECT_EQUAL_UINT(load, (float)(8.0f / 8.0f));
        EXPECT_EQUAL_UINT(load, 1.0f);
        
        chtbl_insert(&chtbl, "key9", "value");
        float load_after = chtbl_load_factor(&chtbl);
        // After inserting 9th entry, load factor > 1.0 triggers resize to 16 buckets
        // So load factor becomes 9/16 = 0.5625
        EXPECT_TRUE(load_after > 0.0f);
        EXPECT_TRUE(load_after <= 1.0f);
        
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_resize
 * Dependencies: chtbl_init, chtbl_insert
 * Description: Tests resizing the hash table.
 */
UTEST_TEST_CASE(chtbl_resize){
    // Test 1: Resize to larger capacity
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        EXPECT_EQUAL_INT(chtbl_resize(&chtbl, 32), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_buckets(&chtbl), 32);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 2);
        EXPECT_TRUE(chtbl_in(&chtbl, "key1"));
        EXPECT_TRUE(chtbl_in(&chtbl, "key2"));
        chtbl_destroy(&chtbl);
    }

    // Test 2: Resize to smaller capacity
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 32);
        chtbl_insert(&chtbl, "key1", "value1");
        EXPECT_EQUAL_INT(chtbl_resize(&chtbl, 16), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_buckets(&chtbl), 16);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        chtbl_destroy(&chtbl);
    }

    // Test 3: Resize to same capacity
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        EXPECT_EQUAL_INT(chtbl_resize(&chtbl, 16), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_buckets(&chtbl), 16);
        chtbl_destroy(&chtbl);
    }

    // Test 4: Resize below minimum
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        EXPECT_EQUAL_INT(chtbl_resize(&chtbl, 4), COLLECTION_SUCCESS);
        EXPECT_TRUE(chtbl_buckets(&chtbl) >= 8);
        chtbl_destroy(&chtbl);
    }

    // Test 5: Resize with NULL chtbl
    {
        EXPECT_EQUAL_INT(chtbl_resize(NULL, 32), COLLECTION_FAILURE);
    }

    // Test 6: Resize preserves all entries
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 8);
        char keys[10][32];
        for (int i = 0; i < 10; i++) {
            snprintf(keys[i], sizeof(keys[i]), "key%d", i);
            chtbl_insert(&chtbl, keys[i], "value");
        }
        EXPECT_EQUAL_INT(chtbl_resize(&chtbl, 32), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 10);
        for (int i = 0; i < 10; i++) {
            EXPECT_TRUE(chtbl_in(&chtbl, keys[i]));
        }
        chtbl_destroy(&chtbl);
    }

    // Test 7: Multiple resizes preserve all entries
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 8);
        char keys[20][32];
        for (int i = 0; i < 20; i++) {
            snprintf(keys[i], sizeof(keys[i]), "key%d", i);
            chtbl_insert(&chtbl, keys[i], "value");
        }
        chtbl_resize(&chtbl, 16);
        chtbl_resize(&chtbl, 32);
        chtbl_resize(&chtbl, 64);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 20);
        for (int i = 0; i < 20; i++) {
            EXPECT_TRUE(chtbl_in(&chtbl, keys[i]));
        }
        chtbl_destroy(&chtbl);
    }

    // Test 8: Resize with collision keys
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 4);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_insert(&chtbl, "key3", "value3");
        EXPECT_EQUAL_INT(chtbl_resize(&chtbl, 32), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 3);
        EXPECT_TRUE(chtbl_in(&chtbl, "key1"));
        EXPECT_TRUE(chtbl_in(&chtbl, "key2"));
        EXPECT_TRUE(chtbl_in(&chtbl, "key3"));
        chtbl_destroy(&chtbl);
    }

    // Test 9: Resize empty hash table
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        EXPECT_EQUAL_INT(chtbl_resize(&chtbl, 32), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_buckets(&chtbl), 32);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        chtbl_destroy(&chtbl);
    }

    // Test 10: Resize to very large capacity
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        chtbl_insert(&chtbl, "key1", "value1");
        EXPECT_EQUAL_INT(chtbl_resize(&chtbl, 2048), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_buckets(&chtbl), 2048);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        EXPECT_TRUE(chtbl_in(&chtbl, "key1"));
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_destroy
 * Dependencies: chtbl_init
 * Description: Tests destruction of the hash table and cleanup.
 */
UTEST_TEST_CASE(chtbl_destroy){
    // Test 1: Destroy empty hash table
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
    }

    // Test 2: Destroy hash table with entries
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
    }

    // Test 3: Destroy with NULL chtbl
    {
        chtbl_destroy(NULL);
    }

    // Test 4: Destroy after clear
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_clear(&chtbl);
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
    }

    // Test 5: Destroy after resize
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 8);
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_resize(&chtbl, 32);
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
    }

    // Test 6: Destroy with many entries
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        char keys[100][32];
        for (int i = 0; i < 100; i++) {
            snprintf(keys[i], sizeof(keys[i]), "key%d", i);
            chtbl_insert(&chtbl, keys[i], "value");
        }
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
    }
}

/**
 * Test: chtbl_destroy_key
 * Dependencies: chtbl_init, chtbl_insert, chtbl_destroy
 * Description: Tests that destroy_key function is called properly.
 */
UTEST_TEST_CASE(chtbl_destroy_key){
    // Test 1: Destroy key on remove
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, NULL);
        char* key1 = strdup("key1");
        chtbl_insert(&chtbl, key1, "value1");
        destroy_key_count = 0;
        chtbl_remove(&chtbl, key1, NULL);
        EXPECT_EQUAL_INT(destroy_key_count, 1);
        chtbl_destroy(&chtbl);
    }

    // Test 2: Will not destroy key on update
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, NULL);
        char* key1 = strdup("key1");
        char* key2 = strdup("key1");
        chtbl_insert(&chtbl, key1, "value1");
        destroy_key_count = 0;
        chtbl_insert(&chtbl, key2, "value2");
        EXPECT_EQUAL_INT(destroy_key_count, 0);
        free(key2);
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_INT(destroy_key_count, 1);
    }

    // Test 3: Destroy key on clear
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, NULL);
        char* key1 = strdup("key1");
        char* key2 = strdup("key2");
        chtbl_insert(&chtbl, key1, "value1");
        chtbl_insert(&chtbl, key2, "value2");
        destroy_key_count = 0;
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_INT(destroy_key_count, 2);
        chtbl_destroy(&chtbl);
    }

    // Test 4: Destroy key on destroy
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, NULL);
        char* key1 = strdup("key1");
        char* key2 = strdup("key2");
        chtbl_insert(&chtbl, key1, "value1");
        chtbl_insert(&chtbl, key2, "value2");
        destroy_key_count = 0;
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_INT(destroy_key_count, 2);
    }

    // Test 5: No destroy when retrieving entry
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, NULL);
        char* key1 = strdup("key1");
        chtbl_insert(&chtbl, key1, "value1");
        destroy_key_count = 0;
        ChtblEntry* entry = NULL;
        chtbl_remove(&chtbl, key1, &entry);
        EXPECT_EQUAL_INT(destroy_key_count, 0);
        free(entry);
        free(key1);
        chtbl_destroy(&chtbl);
    }

    // Test 6: Destroy key on resize will reserve the key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, NULL);
        char* key1 = strdup("key1");
        char* key2 = strdup("key2");
        chtbl_insert(&chtbl, key1, "value1");
        chtbl_insert(&chtbl, key2, "value2");
        destroy_key_count = 0;
        chtbl_resize(&chtbl, 32);
        EXPECT_EQUAL_INT(destroy_key_count, 0);
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_INT(destroy_key_count, 2);
    }

    // Test 7: Destroy key called when removing with NULL entry pointer
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, NULL);
        char* key1 = strdup("key1");
        chtbl_insert(&chtbl, key1, "value1");
        destroy_key_count = 0;
        chtbl_remove(&chtbl, key1, NULL);
        EXPECT_EQUAL_INT(destroy_key_count, 1);
        chtbl_destroy(&chtbl);
    }

    // Test 8: Update value multiple times will reserve the key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, NULL);
        char* key1 = strdup("key");
        char* key2 = strdup("key");
        char* key3 = strdup("key");
        chtbl_insert(&chtbl, key1, "value1");
        destroy_key_count = 0;
        chtbl_insert(&chtbl, key2, "value2");
        EXPECT_EQUAL_INT(destroy_key_count, 0);
        chtbl_insert(&chtbl, key3, "value3");
        EXPECT_EQUAL_INT(destroy_key_count, 0);
        free(key2);
        free(key3);
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_INT(destroy_key_count, 1);
    }
}

/**
 * Test: chtbl_destroy_value
 * Dependencies: chtbl_init, chtbl_insert, chtbl_destroy
 * Description: Tests that destroy_value function is called properly.
 */
UTEST_TEST_CASE(chtbl_destroy_value){
    // Test 1: Destroy value on remove
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, destroy_value_counter);
        char* value1 = strdup("value1");
        chtbl_insert(&chtbl, "key1", value1);
        destroy_value_count = 0;
        chtbl_remove(&chtbl, "key1", NULL);
        EXPECT_EQUAL_INT(destroy_value_count, 1);
        chtbl_destroy(&chtbl);
    }

    // Test 2: Destroy value on update
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, destroy_value_counter);
        char* value1 = strdup("value1");
        char* value2 = strdup("value2");
        chtbl_insert(&chtbl, "key1", value1);
        destroy_value_count = 0;
        chtbl_insert(&chtbl, "key1", value2);
        EXPECT_EQUAL_INT(destroy_value_count, 1);
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_INT(destroy_value_count, 2);
    }

    // Test 3: Destroy value on clear
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, destroy_value_counter);
        char* value1 = strdup("value1");
        char* value2 = strdup("value2");
        chtbl_insert(&chtbl, "key1", value1);
        chtbl_insert(&chtbl, "key2", value2);
        destroy_value_count = 0;
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_INT(destroy_value_count, 2);
        chtbl_destroy(&chtbl);
    }

    // Test 4: Destroy value on destroy
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, destroy_value_counter);
        char* value1 = strdup("value1");
        char* value2 = strdup("value2");
        chtbl_insert(&chtbl, "key1", value1);
        chtbl_insert(&chtbl, "key2", value2);
        destroy_value_count = 0;
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_INT(destroy_value_count, 2);
    }

    // Test 5: No destroy when retrieving entry
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, destroy_value_counter);
        char* value1 = strdup("value1");
        chtbl_insert(&chtbl, "key1", value1);
        destroy_value_count = 0;
        ChtblEntry* entry = NULL;
        chtbl_remove(&chtbl, "key1", &entry);
        EXPECT_EQUAL_INT(destroy_value_count, 0);
        free(entry->value);
        free(entry);
        chtbl_destroy(&chtbl);
    }

    // Test 6: Destroy value on resize will reserve the value
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, destroy_value_counter);
        char* value1 = strdup("value1");
        char* value2 = strdup("value2");
        chtbl_insert(&chtbl, "key1", value1);
        chtbl_insert(&chtbl, "key2", value2);
        destroy_value_count = 0;
        chtbl_resize(&chtbl, 32);
        EXPECT_EQUAL_INT(destroy_value_count, 0);
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_INT(destroy_value_count, 2);
    }

    // Test 7: Destroy value with NULL value pointer in entry
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, destroy_value_counter);
        chtbl_insert(&chtbl, "key1", NULL);
        destroy_value_count = 0;
        chtbl_remove(&chtbl, "key1", NULL);
        EXPECT_EQUAL_INT(destroy_value_count, 0);
        chtbl_destroy(&chtbl);
    }

    // Test 8: Destroy value multiple times on same key update
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, destroy_value_counter);
        char* value1 = strdup("value1");
        char* value2 = strdup("value2");
        char* value3 = strdup("value3");
        chtbl_insert(&chtbl, "key1", value1);
        destroy_value_count = 0;
        chtbl_insert(&chtbl, "key1", value2);
        EXPECT_EQUAL_INT(destroy_value_count, 1);
        chtbl_insert(&chtbl, "key1", value3);
        EXPECT_EQUAL_INT(destroy_value_count, 2);
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_INT(destroy_value_count, 3);
    }
}

/**
 * Test: chtbl_memory_leak
 * Dependencies: chtbl_init, chtbl_destroy, chtbl_insert, chtbl_remove, chtbl_clear
 * Description: Tests for memory leaks by tracking destroy function calls.
 */
UTEST_TEST_CASE(chtbl_memory_leak){
    // Test 1: All keys and values destroyed on chtbl_destroy
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, destroy_value_counter);
        char* keys[10];
        char* values[10];
        for (int i = 0; i < 10; i++) {
            keys[i] = malloc(32);
            values[i] = malloc(32);
            snprintf(keys[i], 32, "key%d", i);
            snprintf(values[i], 32, "value%d", i);
            chtbl_insert(&chtbl, keys[i], values[i]);
        }
        destroy_key_count = 0;
        destroy_value_count = 0;
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_INT(destroy_key_count, 10);
        EXPECT_EQUAL_INT(destroy_value_count, 10);
    }

    // Test 2: Keys and values destroyed on remove
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, destroy_value_counter);
        char* keys[5];
        char* values[5];
        for (int i = 0; i < 5; i++) {
            keys[i] = malloc(32);
            values[i] = malloc(32);
            snprintf(keys[i], 32, "key%d", i);
            snprintf(values[i], 32, "value%d", i);
            chtbl_insert(&chtbl, keys[i], values[i]);
        }
        destroy_key_count = 0;
        destroy_value_count = 0;
        for (int i = 0; i < 5; i++) {
            chtbl_remove(&chtbl, keys[i], NULL);
        }
        EXPECT_EQUAL_INT(destroy_key_count, 5);
        EXPECT_EQUAL_INT(destroy_value_count, 5);
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_INT(destroy_key_count, 5);
        EXPECT_EQUAL_INT(destroy_value_count, 5);
    }

    // Test 3: Keys and values destroyed on clear
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, destroy_value_counter);
        char* keys[10];
        char* values[10];
        for (int i = 0; i < 10; i++) {
            keys[i] = malloc(32);
            values[i] = malloc(32);
            snprintf(keys[i], 32, "key%d", i);
            snprintf(values[i], 32, "value%d", i);
            chtbl_insert(&chtbl, keys[i], values[i]);
        }
        destroy_key_count = 0;
        destroy_value_count = 0;
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_INT(destroy_key_count, 10);
        EXPECT_EQUAL_INT(destroy_value_count, 10);
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_INT(destroy_key_count, 10);
        EXPECT_EQUAL_INT(destroy_value_count, 10);
    }

    // Test 4: Update key will reserve the key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, destroy_value_counter);
        char* key1 = malloc(32);
        char* key2 = malloc(32);
        char* value1 = malloc(32);
        char* value2 = malloc(32);
        strcpy(key1, "key");
        strcpy(key2, "key");
        strcpy(value1, "value1");
        strcpy(value2, "value2");
        chtbl_insert(&chtbl, key1, value1);
        destroy_key_count = 0;
        destroy_value_count = 0;
        chtbl_insert(&chtbl, key2, value2);
        EXPECT_EQUAL_INT(destroy_key_count, 0);
        EXPECT_EQUAL_INT(destroy_value_count, 1);
        free(key2);
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_INT(destroy_key_count, 1);
        EXPECT_EQUAL_INT(destroy_value_count, 2);
    }

    // Test 5: No destroy when retrieving entry
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, destroy_value_counter);
        char* key1 = malloc(32);
        char* value1 = malloc(32);
        strcpy(key1, "key1");
        strcpy(value1, "value1");
        chtbl_insert(&chtbl, key1, value1);
        destroy_key_count = 0;
        destroy_value_count = 0;
        ChtblEntry* entry = NULL;
        chtbl_remove(&chtbl, key1, &entry);
        EXPECT_EQUAL_INT(destroy_key_count, 0);
        EXPECT_EQUAL_INT(destroy_value_count, 0);
        free(entry->key);
        free(entry->value);
        free(entry);
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_INT(destroy_key_count, 0);
        EXPECT_EQUAL_INT(destroy_value_count, 0);
    }

    // Test 6: Memory leak test with resize
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, destroy_value_counter);
        char* keys[10];
        char* values[10];
        for (int i = 0; i < 10; i++) {
            keys[i] = malloc(32);
            values[i] = malloc(32);
            snprintf(keys[i], 32, "key%d", i);
            snprintf(values[i], 32, "value%d", i);
            chtbl_insert(&chtbl, keys[i], values[i]);
        }
        destroy_key_count = 0;
        destroy_value_count = 0;
        chtbl_resize(&chtbl, 32);
        EXPECT_EQUAL_INT(destroy_key_count, 0);
        EXPECT_EQUAL_INT(destroy_value_count, 0);
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_INT(destroy_key_count, 10);
        EXPECT_EQUAL_INT(destroy_value_count, 10);
    }

    // Test 7: Memory leak test with many updates
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, destroy_value_counter);
        char* key1 = malloc(32);
        strcpy(key1, "key");
        char* values[5];
        destroy_key_count = 0;
        destroy_value_count = 0;
        for (int i = 0; i < 5; i++) {
            values[i] = malloc(32);
            snprintf(values[i], 32, "value%d", i);
            chtbl_insert(&chtbl, key1, values[i]);
        }
        EXPECT_EQUAL_INT(destroy_key_count, 0);
        EXPECT_EQUAL_INT(destroy_value_count, 4);
        
        destroy_key_count = 0;
        destroy_value_count = 0;
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_INT(destroy_key_count, 1);
        EXPECT_EQUAL_INT(destroy_value_count, 1);
    }

    // Test 8: Memory leak test with clear then insert
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, destroy_value_counter);
        char* keys1[5];
        char* values1[5];
        for (int i = 0; i < 5; i++) {
            keys1[i] = malloc(32);
            values1[i] = malloc(32);
            snprintf(keys1[i], 32, "key%d", i);
            snprintf(values1[i], 32, "value%d", i);
            chtbl_insert(&chtbl, keys1[i], values1[i]);
        }
        destroy_key_count = 0;
        destroy_value_count = 0;
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_INT(destroy_key_count, 5);
        EXPECT_EQUAL_INT(destroy_value_count, 5);
        char* keys2[3];
        char* values2[3];
        for (int i = 0; i < 3; i++) {
            keys2[i] = malloc(32);
            values2[i] = malloc(32);
            snprintf(keys2[i], 32, "newkey%d", i);
            snprintf(values2[i], 32, "newvalue%d", i);
            chtbl_insert(&chtbl, keys2[i], values2[i]);
        }
        destroy_key_count = 0;
        destroy_value_count = 0;
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_INT(destroy_key_count, 3);
        EXPECT_EQUAL_INT(destroy_value_count, 3);
    }
    // Test 9: Elements freed by free
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, free, free);
        for (int i = 0; i < 10; i++) {
            char* key = malloc(32);
            char* value = malloc(32);
            snprintf(key, 32, "key%d", i);
            snprintf(value, 32, "value%d", i);
            chtbl_insert(&chtbl, key, value);
        }
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 10);
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_integration
 * Dependencies: All chtbl functions
 * Description: Integration test combining multiple operations.
 */
UTEST_TEST_CASE(chtbl_integration){
    // Test 1: Complex sequence of operations
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        char keys[20][32];
        for (int i = 0; i < 20; i++) {
            snprintf(keys[i], sizeof(keys[i]), "key%d", i);
            EXPECT_EQUAL_INT(chtbl_insert(&chtbl, keys[i], "value"), COLLECTION_SUCCESS);
        }
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 20);
        
        for (int i = 0; i < 10; i++) {
            EXPECT_TRUE(chtbl_in(&chtbl, keys[i]));
            void* value = chtbl_find(&chtbl, keys[i]);
            EXPECT_NOT_NULL(value);
        }
        
        for (int i = 0; i < 5; i++) {
            chtbl_remove(&chtbl, keys[i], NULL);
        }
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 15);
        
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        
        chtbl_destroy(&chtbl);
    }

    // Test 2: Load factor and automatic resize
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        chtbl_set_load_factor(&chtbl, 0.75f);
        
        size_t initial_buckets = chtbl_buckets(&chtbl);
        char keys[20][32];
        for (int i = 0; i < 20; i++) {
            snprintf(keys[i], sizeof(keys[i]), "key%d", i);
            chtbl_insert(&chtbl, keys[i], "value");
        }
        
        float load = chtbl_load_factor(&chtbl);
        EXPECT_TRUE(load >= 0.0f);
        EXPECT_TRUE(load <= 1.0f);
        
        chtbl_destroy(&chtbl);
    }

    // Test 3: Collision handling
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 4);
        
        char keys[10][32];
        for (int i = 0; i < 10; i++) {
            snprintf(keys[i], sizeof(keys[i]), "key%d", i);
            chtbl_insert(&chtbl, keys[i], "value");
        }
        
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 10);
        for (int i = 0; i < 10; i++) {
            EXPECT_TRUE(chtbl_in(&chtbl, keys[i]));
        }
        
        chtbl_destroy(&chtbl);
    }

    // Test 4: Integer keys with custom hash and match
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, hash_int_ptr, match_int_ptr, NULL, NULL);
        
        int keys[10];
        for (int i = 0; i < 10; i++) {
            keys[i] = i;
            char value[32];
            snprintf(value, sizeof(value), "value%d", i);
            chtbl_insert(&chtbl, &keys[i], value);
        }
        
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 10);
        for (int i = 0; i < 10; i++) {
            EXPECT_TRUE(chtbl_in(&chtbl, &keys[i]));
            void* value = chtbl_find(&chtbl, &keys[i]);
            EXPECT_NOT_NULL(value);
        }
        
        chtbl_destroy(&chtbl);
    }

    // Test 5: Resize preserves all entries
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 8);
        
        char keys[50][32];
        for (int i = 0; i < 50; i++) {
            snprintf(keys[i], sizeof(keys[i]), "key%d", i);
            chtbl_insert(&chtbl, keys[i], "value");
        }
        
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 50);
        EXPECT_EQUAL_INT(chtbl_resize(&chtbl, 64), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 50);
        
        for (int i = 0; i < 50; i++) {
            EXPECT_TRUE(chtbl_in(&chtbl, keys[i]));
        }
        
        chtbl_destroy(&chtbl);
    }

    // Test 6: Find entry and modify value pointer
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_insert(&chtbl, "key1", "value1");
        ChtblEntry* entry = chtbl_find_entry(&chtbl, "key1");
        EXPECT_NOT_NULL(entry);
        EXPECT_EQUAL_UINT(strcmp((char*)entry->key, "key1"), 0);
        EXPECT_EQUAL_UINT(strcmp((char*)entry->value, "value1"), 0);
        
        chtbl_destroy(&chtbl);
    }

    // Test 7: Edge cases combination
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        EXPECT_FALSE(chtbl_in(&chtbl, "nonexistent"));
        EXPECT_NULL(chtbl_find(&chtbl, "nonexistent"));
        
        chtbl_insert(&chtbl, "key1", NULL);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        EXPECT_NULL(chtbl_find(&chtbl, "key1"));
        
        chtbl_insert(&chtbl, "key1", "value1");
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 1);
        EXPECT_NOT_NULL(chtbl_find(&chtbl, "key1"));
        
        chtbl_destroy(&chtbl);
    }

    // Test 8: Complex resize and collision scenario
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 4);
        char keys[20][32];
        for (int i = 0; i < 20; i++) {
            snprintf(keys[i], sizeof(keys[i]), "key%d", i);
            chtbl_insert(&chtbl, keys[i], "value");
        }
        chtbl_resize(&chtbl, 8);
        chtbl_resize(&chtbl, 16);
        chtbl_resize(&chtbl, 32);
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 20);
        for (int i = 0; i < 20; i++) {
            EXPECT_TRUE(chtbl_in(&chtbl, keys[i]));
        }
        chtbl_destroy(&chtbl);
    }

    // Test 9: Load factor threshold triggering resize
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        chtbl_set_load_factor(&chtbl, 0.75f);
        size_t initial_buckets = chtbl_buckets(&chtbl);
        char keys[20][32];
        for (int i = 0; i < 20; i++) {
            snprintf(keys[i], sizeof(keys[i]), "key%d", i);
            chtbl_insert(&chtbl, keys[i], "value");
        }
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 20);
        for (int i = 0; i < 20; i++) {
            EXPECT_TRUE(chtbl_in(&chtbl, keys[i]));
        }
        chtbl_destroy(&chtbl);
    }

    // Test 10: Remove all entries then reinsert
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        char keys[10][32];
        for (int i = 0; i < 10; i++) {
            snprintf(keys[i], sizeof(keys[i]), "key%d", i);
            chtbl_insert(&chtbl, keys[i], "value");
        }
        for (int i = 0; i < 10; i++) {
            chtbl_remove(&chtbl, keys[i], NULL);
        }
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 0);
        for (int i = 0; i < 10; i++) {
            snprintf(keys[i], sizeof(keys[i]), "newkey%d", i);
            chtbl_insert(&chtbl, keys[i], "newvalue");
        }
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl), 10);
        chtbl_destroy(&chtbl);
    }

    // Test 11: Empty string key operations
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl, "", "empty_key");
        EXPECT_TRUE(chtbl_in(&chtbl, ""));
        EXPECT_NOT_NULL(chtbl_find(&chtbl, ""));
        ChtblEntry* entry = chtbl_find_entry(&chtbl, "");
        EXPECT_NOT_NULL(entry);
        chtbl_remove(&chtbl, "", NULL);
        EXPECT_FALSE(chtbl_in(&chtbl, ""));
        chtbl_destroy(&chtbl);
    }

    // Test 12: Mixed operations with integer and string keys
    {
        Chtbl chtbl_str;
        chtbl_init(&chtbl_str, NULL, NULL, NULL, NULL);
        chtbl_insert(&chtbl_str, "key1", "value1");
        chtbl_insert(&chtbl_str, "key2", "value2");
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl_str), 2);
        chtbl_destroy(&chtbl_str);

        Chtbl chtbl_int;
        chtbl_init(&chtbl_int, hash_int_ptr, match_int_ptr, NULL, NULL);
        int keys[5] = {1, 2, 3, 4, 5};
        for (int i = 0; i < 5; i++) {
            chtbl_insert(&chtbl_int, &keys[i], "value");
        }
        EXPECT_EQUAL_UINT(chtbl_size(&chtbl_int), 5);
        chtbl_destroy(&chtbl_int);
    }
}

/**
 * Test suite: chtbl
 * Description: Test suite for chained hash table data structure
 */
UTEST_TEST_SUITE(chtbl){
    UTEST_RUN_TEST_CASE(chtbl_init);
    UTEST_RUN_TEST_CASE(chtbl_init_capacity);
    UTEST_RUN_TEST_CASE(chtbl_size);
    UTEST_RUN_TEST_CASE(chtbl_buckets);
    UTEST_RUN_TEST_CASE(chtbl_in);
    UTEST_RUN_TEST_CASE(chtbl_insert);
    UTEST_RUN_TEST_CASE(chtbl_find);
    UTEST_RUN_TEST_CASE(chtbl_find_entry);
    UTEST_RUN_TEST_CASE(chtbl_remove);
    UTEST_RUN_TEST_CASE(chtbl_clear);
    UTEST_RUN_TEST_CASE(chtbl_set_load_factor);
    UTEST_RUN_TEST_CASE(chtbl_load_factor);
    UTEST_RUN_TEST_CASE(chtbl_resize);
    UTEST_RUN_TEST_CASE(chtbl_destroy);
    UTEST_RUN_TEST_CASE(chtbl_destroy_key);
    UTEST_RUN_TEST_CASE(chtbl_destroy_value);
    UTEST_RUN_TEST_CASE(chtbl_memory_leak);
    UTEST_RUN_TEST_CASE(chtbl_integration);
}

