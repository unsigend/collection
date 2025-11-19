#include <utest.h>
#include <collection/chtbl.h>
#include <algorithm/hash.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * Global helper functions for testing destroy callbacks.
 */

static int key_destroy_count = 0;
static int value_destroy_count = 0;

static void destroy_key_counter(void *key) {
    if (key != NULL) {
        key_destroy_count++;
        free(key);
    }
}

static void destroy_value_counter(void *value) {
    if (value != NULL) {
        value_destroy_count++;
        free(value);
    }
}

static void destroy_key_noop(void *key) {
    (void)key;
}

static void destroy_value_noop(void *value) {
    (void)value;
}

static void reset_destroy_counters(void) {
    key_destroy_count = 0;
    value_destroy_count = 0;
}

/**
 * Helper hash function for strings (wrapper for hash_str).
 */
static uint32_t hash_str_wrapper(const void *key) {
    return hash_str((const char *)key);
}

/**
 * Helper match function for strings (wrapper for strcmp).
 */
static bool match_str_wrapper(const void *key1, const void *key2) {
    return strcmp((const char *)key1, (const char *)key2) == 0;
}

/**
 * Helper hash function for integers.
 */
static uint32_t hash_int_ptr(const void *key) {
    return hash_int(*(int*)key);
}

/**
 * Helper match function for integers.
 */
static bool match_int_ptr(const void *key1, const void *key2) {
    return *(int*)key1 == *(int*)key2;
}

/**
 * Test: chtbl_init
 * Dependencies: None
 * Description: Tests basic initialization of the chained hash table.
 */
UTEST_TEST_CASE(chtbl_init){
    // Test 1: Initialize with NULL functions
    {
        Chtbl chtbl;
        int result = chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
        EXPECT_TRUE(chtbl_buckets(&chtbl) > 0);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 2: Initialize with custom hash and match functions
    {
        Chtbl chtbl;
        int result = chtbl_init(&chtbl, hash_str_wrapper, match_str_wrapper, NULL, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 3: Initialize with destroy functions
    {
        Chtbl chtbl;
        int result = chtbl_init(&chtbl, NULL, NULL, destroy_key_noop, destroy_value_noop);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 4: Initialize with NULL pointer (should fail)
    {
        int result = chtbl_init(NULL, NULL, NULL, NULL, NULL);
        EXPECT_EQUAL_INT(result, -1);
    }
}

/**
 * Test: chtbl_init_capacity
 * Dependencies: None
 * Description: Tests initialization with a given capacity.
 */
UTEST_TEST_CASE(chtbl_init_capacity){
    // Test 1: Initialize with valid capacity
    {
        Chtbl chtbl;
        int result = chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 32);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
        EXPECT_EQUAL_UINT64(chtbl_buckets(&chtbl), 32);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 2: Initialize with small capacity (should be adjusted to minimum)
    {
        Chtbl chtbl;
        int result = chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 4);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
        EXPECT_TRUE(chtbl_buckets(&chtbl) >= 4);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 3: Initialize with NULL pointer (should fail)
    {
        int result = chtbl_init_capacity(NULL, NULL, NULL, NULL, NULL, 16);
        EXPECT_EQUAL_INT(result, -1);
    }
    
    // Test 4: Initialize with large capacity
    {
        Chtbl chtbl;
        int result = chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 1024);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_buckets(&chtbl), 1024);
        
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_size
 * Dependencies: chtbl_init
 */
UTEST_TEST_CASE(chtbl_size){
    // Test 1: Size of newly initialized table
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 2: Size with NULL pointer
    {
        Chtbl *null_chtbl = NULL;
        EXPECT_EQUAL_UINT64(chtbl_size(null_chtbl), 0);
    }
    
    // Test 3: Size after insertions
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_insert(&chtbl, "key1", "value1");
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 1);
        
        chtbl_insert(&chtbl, "key2", "value2");
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 2);
        
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_buckets
 * Dependencies: chtbl_init, chtbl_init_capacity
 */
UTEST_TEST_CASE(chtbl_buckets){
    // Test 1: Buckets of default initialized table
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        EXPECT_TRUE(chtbl_buckets(&chtbl) > 0);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 2: Buckets with NULL pointer
    {
        Chtbl *null_chtbl = NULL;
        EXPECT_EQUAL_UINT64(chtbl_buckets(null_chtbl), 0);
    }
    
    // Test 3: Buckets with custom capacity
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 64);
        
        EXPECT_EQUAL_UINT64(chtbl_buckets(&chtbl), 64);
        
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_set_load_factor
 * Dependencies: chtbl_init
 */
UTEST_TEST_CASE(chtbl_set_load_factor){
    // Test 1: Set valid load factor
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_set_load_factor(&chtbl, 0.5f);
        // Load factor threshold is internal, but we can verify it doesn't crash
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 2: Set load factor with NULL pointer
    {
        chtbl_set_load_factor(NULL, 0.75f);
    }
    
    // Test 3: Set negative load factor
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_set_load_factor(&chtbl, -1.0f);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 4: Set load factor greater than maximum
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_set_load_factor(&chtbl, 2.0f);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 5: Set load factor to edge values
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_set_load_factor(&chtbl, 0.0f);
        EXPECT_NOT_EQUAL_FLOAT(chtbl.load_factor_threshold, 0.0f);
        
        chtbl_set_load_factor(&chtbl, 1.0f);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 6: Set load factor multiple times
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_set_load_factor(&chtbl, 0.5f);
        chtbl_set_load_factor(&chtbl, 0.75f);
        chtbl_set_load_factor(&chtbl, 0.9f);
        // Should not crash and should accept all values
        
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_load_factor
 * Dependencies: chtbl_init, chtbl_insert, chtbl_remove, chtbl_clear
 */
UTEST_TEST_CASE(chtbl_load_factor){
    // Test 1: Load factor of empty table
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        float load_factor = chtbl_load_factor(&chtbl);
        EXPECT_EQUAL_FLOAT(load_factor, 0.0f);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 2: Load factor with NULL pointer
    {
        float load_factor = chtbl_load_factor(NULL);
        EXPECT_EQUAL_FLOAT(load_factor, 0.0f);
    }
    
    // Test 3: Load factor after insertions
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 10);
        
        chtbl_insert(&chtbl, "key1", "value1");
        float load_factor = chtbl_load_factor(&chtbl);
        EXPECT_TRUE(load_factor > 0.0f);
        EXPECT_TRUE(load_factor <= 1.0f);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 4: Load factor increases with more insertions
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 10);
        
        float load_factor1 = chtbl_load_factor(&chtbl);
        EXPECT_EQUAL_FLOAT(load_factor1, 0.0f);
        
        chtbl_insert(&chtbl, "key1", "value1");
        float load_factor2 = chtbl_load_factor(&chtbl);
        EXPECT_TRUE(load_factor2 > load_factor1);
        
        chtbl_insert(&chtbl, "key2", "value2");
        float load_factor3 = chtbl_load_factor(&chtbl);
        EXPECT_TRUE(load_factor3 > load_factor2);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 5: Load factor decreases after removals
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 10);
        
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_insert(&chtbl, "key3", "value3");
        
        float load_factor1 = chtbl_load_factor(&chtbl);
        EXPECT_TRUE(load_factor1 > 0.0f);
        
        EXPECT_EQUAL_INT(chtbl_remove(&chtbl, "key1", NULL), COLLECTION_SUCCESS);
        float load_factor2 = chtbl_load_factor(&chtbl);
        EXPECT_TRUE(load_factor2 < load_factor1);
        
        EXPECT_EQUAL_INT(chtbl_remove(&chtbl, "key2", NULL), COLLECTION_SUCCESS);
        float load_factor3 = chtbl_load_factor(&chtbl);
        EXPECT_TRUE(load_factor3 < load_factor2);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 6: Load factor becomes zero after clear
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 10);
        
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        float load_factor1 = chtbl_load_factor(&chtbl);
        EXPECT_TRUE(load_factor1 > 0.0f);
        
        chtbl_clear(&chtbl);
        float load_factor2 = chtbl_load_factor(&chtbl);
        EXPECT_EQUAL_FLOAT(load_factor2, 0.0f);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 7: Load factor calculation accuracy
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 100);
        
        // Insert 50 entries, load factor should be 0.5
        for (int i = 0; i < 50; i++) {
            char key[32];
            snprintf(key, sizeof(key), "key%d", i);
            EXPECT_EQUAL_INT(chtbl_insert(&chtbl, key, "value"), COLLECTION_SUCCESS);
        }
        
        float load_factor = chtbl_load_factor(&chtbl);
        EXPECT_TRUE(load_factor >= 0.49f && load_factor <= 0.51f);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 8: Load factor edge case - exactly at threshold
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 4);
        chtbl_set_load_factor(&chtbl, 1.0f);
        
        // Insert 4 entries to reach load factor of 1.0
        for (int i = 0; i < 4; i++) {
            char key[32];
            snprintf(key, sizeof(key), "key%d", i);
            chtbl_insert(&chtbl, key, "value");
        }
        
        float load_factor = chtbl_load_factor(&chtbl);
        EXPECT_EQUAL_FLOAT(load_factor, 1.0f);
        
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_destroy
 * Dependencies: chtbl_init, chtbl_insert
 */
UTEST_TEST_CASE(chtbl_destroy){
    // Test 1: Destroy empty table
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_destroy(&chtbl);
        
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
    }
    
    // Test 2: Destroy table with elements (no destroy functions)
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        
        chtbl_destroy(&chtbl);
        
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
    }
    
    // Test 3: Destroy table with destroy functions
    {
        reset_destroy_counters();
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, destroy_value_counter);
        
        char *key1 = strdup("key1");
        char *value1 = strdup("value1");
        char *key2 = strdup("key2");
        char *value2 = strdup("value2");
        
        chtbl_insert(&chtbl, key1, value1);
        chtbl_insert(&chtbl, key2, value2);
        
        chtbl_destroy(&chtbl);
        
        EXPECT_EQUAL_INT(key_destroy_count, 2);
        EXPECT_EQUAL_INT(value_destroy_count, 2);
    }
    
    // Test 4: Multiple destroy calls (should not crash)
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        chtbl_destroy(&chtbl);
        chtbl_destroy(&chtbl);
        
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
    }
    
    // Test 5: Destroy with NULL pointer (should not crash)
    {
        chtbl_destroy(NULL);
    }
}

/**
 * Test: chtbl_insert
 * Dependencies: chtbl_init, chtbl_size, chtbl_find
 */
UTEST_TEST_CASE(chtbl_insert){
    // Test 1: Insert single key-value pair
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        int result = chtbl_insert(&chtbl, "key1", "value1");
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 1);
        EXPECT_NOT_NULL(chtbl_find(&chtbl, "key1"));
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 2: Insert multiple key-value pairs
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        for (int i = 0; i < 10; i++) {
            char key[32];
            char value[32];
            snprintf(key, sizeof(key), "key%d", i);
            snprintf(value, sizeof(value), "value%d", i);
            
            int result = chtbl_insert(&chtbl, key, value);
            EXPECT_EQUAL_INT(result, 0);
        }
        
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 10);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 3: Insert with NULL key (should fail or handle gracefully)
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        int result = chtbl_insert(&chtbl, NULL, "value");
        // Behavior depends on implementation, but should not crash
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 4: Insert with NULL value (should be allowed)
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        int result = chtbl_insert(&chtbl, "key", NULL);
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 1);
        
        void *value = chtbl_find(&chtbl, "key");
        EXPECT_NULL(value);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 5: Insert duplicate key (should update value)
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_insert(&chtbl, "key", "value1");
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 1);
        EXPECT_EQUAL_STRING((char*)chtbl_find(&chtbl, "key"), "value1");
        
        // Update with same key
        int result = chtbl_insert(&chtbl, "key", "value2");
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 1); // Size should not increase
        EXPECT_EQUAL_STRING((char*)chtbl_find(&chtbl, "key"), "value2");
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 5a: Multiple updates of the same key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_insert(&chtbl, "key", "value1");
        chtbl_insert(&chtbl, "key", "value2");
        chtbl_insert(&chtbl, "key", "value3");
        chtbl_insert(&chtbl, "key", "value4");
        
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 1);
        EXPECT_EQUAL_STRING((char*)chtbl_find(&chtbl, "key"), "value4");
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 5b: Update existing key with NULL value
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_insert(&chtbl, "key", "value1");
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 1);
        EXPECT_EQUAL_STRING((char*)chtbl_find(&chtbl, "key"), "value1");
        
        // Update with NULL value
        int result = chtbl_insert(&chtbl, "key", NULL);
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 1);
        EXPECT_NULL(chtbl_find(&chtbl, "key"));
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 5c: Update existing key with destroy functions (old value should be destroyed)
    {
        reset_destroy_counters();
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, destroy_value_counter);
        
        char *value1 = strdup("value1");
        char *value2 = strdup("value2");
        
        chtbl_insert(&chtbl, "key", value1);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 1);
        
        // Update with new value - old value should be destroyed
        reset_destroy_counters();
        chtbl_insert(&chtbl, "key", value2);
        EXPECT_EQUAL_INT(value_destroy_count, 1); // Old value should be destroyed
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 1);
        EXPECT_EQUAL_STRING((char*)chtbl_find(&chtbl, "key"), "value2");
        
        // Clean up - remove will destroy value2
        chtbl_remove(&chtbl, "key", NULL);
        EXPECT_EQUAL_INT(value_destroy_count, 2); // value2 should be destroyed
        // Note: value1 was already freed when updating, value2 was freed by remove
        chtbl_destroy(&chtbl);
    }
    
    // Test 5d: Update existing key with destroy key function (old key should NOT be destroyed)
    {
        reset_destroy_counters();
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, NULL);
        
        char *key1 = strdup("key");
        char *key2 = strdup("key"); // Same content, different pointer
        
        chtbl_insert(&chtbl, key1, "value1");
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 1);
        
        // Update with same key content but different pointer
        // The key pointer should be updated, but old key should NOT be destroyed
        // (because we're updating, not removing)
        reset_destroy_counters();
        chtbl_insert(&chtbl, key2, "value2");
        EXPECT_EQUAL_INT(key_destroy_count, 0); // Key should NOT be destroyed on update
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 1);
        EXPECT_EQUAL_STRING((char*)chtbl_find(&chtbl, "key"), "value2");
        
        // Clean up
        free(key1);
        free(key2);
        chtbl_destroy(&chtbl);
    }
    
    // Test 5e: Insert, update, insert new key, update again
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key1", "value1_updated");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_insert(&chtbl, "key1", "value1_updated_again");
        
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 2);
        EXPECT_EQUAL_STRING((char*)chtbl_find(&chtbl, "key1"), "value1_updated_again");
        EXPECT_EQUAL_STRING((char*)chtbl_find(&chtbl, "key2"), "value2");
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 6: Insert with NULL table pointer (should fail)
    {
        int result = chtbl_insert(NULL, "key", "value");
        EXPECT_EQUAL_INT(result, -1);
    }
    
    // Test 7: Insert with integer keys
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, hash_int_ptr, match_int_ptr, NULL, NULL);
        
        int key1 = 1, key2 = 2, key3 = 3;
        int result1 = chtbl_insert(&chtbl, &key1, "value1");
        int result2 = chtbl_insert(&chtbl, &key2, "value2");
        int result3 = chtbl_insert(&chtbl, &key3, "value3");
        
        EXPECT_EQUAL_INT(result1, 0);
        EXPECT_EQUAL_INT(result2, 0);
        EXPECT_EQUAL_INT(result3, 0);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 3);
        
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_find
 * Dependencies: chtbl_init, chtbl_insert
 */
UTEST_TEST_CASE(chtbl_find){
    // Test 1: Find in empty table
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        void *value = chtbl_find(&chtbl, "nonexistent");
        EXPECT_NULL(value);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 2: Find existing key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        
        void *value1 = chtbl_find(&chtbl, "key1");
        void *value2 = chtbl_find(&chtbl, "key2");
        
        EXPECT_NOT_NULL(value1);
        EXPECT_NOT_NULL(value2);
        EXPECT_EQUAL_STRING((char*)value1, "value1");
        EXPECT_EQUAL_STRING((char*)value2, "value2");
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 3: Find non-existent key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_insert(&chtbl, "key1", "value1");
        
        void *value = chtbl_find(&chtbl, "nonexistent");
        EXPECT_NULL(value);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 4: Find with NULL table pointer
    {
        void *value = chtbl_find(NULL, "key");
        EXPECT_NULL(value);
    }
    
    // Test 5: Find with NULL key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        void *value = chtbl_find(&chtbl, NULL);
        EXPECT_NULL(value);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 6: Find after update
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_insert(&chtbl, "key", "value1");
        chtbl_insert(&chtbl, "key", "value2");
        
        void *value = chtbl_find(&chtbl, "key");
        EXPECT_EQUAL_STRING((char*)value, "value2");
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 7: Find with integer keys
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, hash_int_ptr, match_int_ptr, NULL, NULL);
        
        int key1 = 1, key2 = 2, key3 = 999;
        chtbl_insert(&chtbl, &key1, "value1");
        chtbl_insert(&chtbl, &key2, "value2");
        
        void *value1 = chtbl_find(&chtbl, &key1);
        void *value2 = chtbl_find(&chtbl, &key2);
        void *value3 = chtbl_find(&chtbl, &key3);
        
        EXPECT_NOT_NULL(value1);
        EXPECT_NOT_NULL(value2);
        EXPECT_NULL(value3);
        EXPECT_EQUAL_STRING((char*)value1, "value1");
        EXPECT_EQUAL_STRING((char*)value2, "value2");
        
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_find_entry
 * Dependencies: chtbl_init, chtbl_insert
 */
UTEST_TEST_CASE(chtbl_find_entry){
    // Test 1: Find entry in empty table
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        ChtblEntry *entry = chtbl_find_entry(&chtbl, "nonexistent");
        EXPECT_NULL(entry);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 2: Find existing entry
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_insert(&chtbl, "key1", "value1");
        
        ChtblEntry *entry = chtbl_find_entry(&chtbl, "key1");
        EXPECT_NOT_NULL(entry);
        EXPECT_EQUAL_STRING((char*)entry->key, "key1");
        EXPECT_EQUAL_STRING((char*)entry->value, "value1");
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 3: Find non-existent entry
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_insert(&chtbl, "key1", "value1");
        
        ChtblEntry *entry = chtbl_find_entry(&chtbl, "nonexistent");
        EXPECT_NULL(entry);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 4: Find entry with NULL table pointer
    {
        ChtblEntry *entry = chtbl_find_entry(NULL, "key");
        EXPECT_NULL(entry);
    }
    
    // Test 5: Find entry with NULL key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        ChtblEntry *entry = chtbl_find_entry(&chtbl, NULL);
        EXPECT_NULL(entry);
        
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_in
 * Dependencies: chtbl_init, chtbl_insert, chtbl_remove
 */
UTEST_TEST_CASE(chtbl_in){
    // Test 1: Check key in empty table
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        EXPECT_FALSE(chtbl_in(&chtbl, "key"));
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 2: Check existing key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        
        EXPECT_TRUE(chtbl_in(&chtbl, "key1"));
        EXPECT_TRUE(chtbl_in(&chtbl, "key2"));
        EXPECT_FALSE(chtbl_in(&chtbl, "nonexistent"));
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 3: Check after removal
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_remove(&chtbl, "key1", NULL);
        
        EXPECT_FALSE(chtbl_in(&chtbl, "key1"));
        
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_remove
 * Dependencies: chtbl_init, chtbl_insert, chtbl_size, chtbl_find
 */
UTEST_TEST_CASE(chtbl_remove){
    // Test 1: Remove from empty table
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        int result = chtbl_remove(&chtbl, "nonexistent", NULL);
        EXPECT_EQUAL_INT(result, -1);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 2: Remove existing key without retrieving entry
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        
        int result = chtbl_remove(&chtbl, "key1", NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 1);
        EXPECT_NULL(chtbl_find(&chtbl, "key1"));
        EXPECT_NOT_NULL(chtbl_find(&chtbl, "key2"));
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 3: Remove existing key and retrieve entry
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_insert(&chtbl, "key1", "value1");
        
        ChtblEntry *entry = NULL;
        int result = chtbl_remove(&chtbl, "key1", &entry);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_NOT_NULL(entry);
        EXPECT_EQUAL_STRING((char*)entry->key, "key1");
        EXPECT_EQUAL_STRING((char*)entry->value, "value1");
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
        
        free(entry);
        chtbl_destroy(&chtbl);
    }
    
    // Test 4: Remove with destroy functions (entry not retrieved)
    {
        reset_destroy_counters();
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, destroy_value_counter);
        
        char *key1 = strdup("key1");
        char *value1 = strdup("value1");
        chtbl_insert(&chtbl, key1, value1);
        
        int result = chtbl_remove(&chtbl, "key1", NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_INT(key_destroy_count, 1);
        EXPECT_EQUAL_INT(value_destroy_count, 1);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 5: Remove with destroy functions (entry retrieved, destroy not called)
    {
        reset_destroy_counters();
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, destroy_value_counter);
        
        char *key1 = strdup("key1");
        char *value1 = strdup("value1");
        chtbl_insert(&chtbl, key1, value1);
        
        ChtblEntry *entry = NULL;
        int result = chtbl_remove(&chtbl, "key1", &entry);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_INT(key_destroy_count, 0);
        EXPECT_EQUAL_INT(value_destroy_count, 0);
        EXPECT_NOT_NULL(entry);
        
        free(entry->key);
        free(entry->value);
        free(entry);
        chtbl_destroy(&chtbl);
    }
    
    // Test 6: Remove non-existent key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_insert(&chtbl, "key1", "value1");
        
        int result = chtbl_remove(&chtbl, "nonexistent", NULL);
        EXPECT_EQUAL_INT(result, -1);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 1);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 7: Remove with NULL table pointer
    {
        int result = chtbl_remove(NULL, "key", NULL);
        EXPECT_EQUAL_INT(result, -1);
    }
    
    // Test 8: Remove with NULL key
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        int result = chtbl_remove(&chtbl, NULL, NULL);
        // Behavior depends on implementation
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 9: Remove multiple keys
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        for (int i = 0; i < 10; i++) {
            char key[32];
            snprintf(key, sizeof(key), "key%d", i);
            chtbl_insert(&chtbl, key, "value");
        }
        
        for (int i = 0; i < 10; i++) {
            char key[32];
            snprintf(key, sizeof(key), "key%d", i);
            int result = chtbl_remove(&chtbl, key, NULL);
            EXPECT_EQUAL_INT(result, 0);
        }
        
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
        
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: chtbl_clear
 * Dependencies: chtbl_init, chtbl_insert, chtbl_size
 */
UTEST_TEST_CASE(chtbl_clear){
    // Test 1: Clear empty table
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_clear(&chtbl);
        
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 2: Clear table with elements
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_insert(&chtbl, "key3", "value3");
        
        chtbl_clear(&chtbl);
        
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
        EXPECT_NULL(chtbl_find(&chtbl, "key1"));
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 3: Clear with destroy functions
    {
        reset_destroy_counters();
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, destroy_value_counter);
        
        char *key1 = strdup("key1");
        char *value1 = strdup("value1");
        char *key2 = strdup("key2");
        char *value2 = strdup("value2");
        
        chtbl_insert(&chtbl, key1, value1);
        chtbl_insert(&chtbl, key2, value2);
        
        chtbl_clear(&chtbl);
        
        EXPECT_EQUAL_INT(key_destroy_count, 2);
        EXPECT_EQUAL_INT(value_destroy_count, 2);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 4: Clear and reuse table
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        
        chtbl_clear(&chtbl);
        
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
        
        chtbl_insert(&chtbl, "newkey", "newvalue");
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 1);
        EXPECT_NOT_NULL(chtbl_find(&chtbl, "newkey"));
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 5: Multiple clears (should not call destroy functions multiple times)
    {
        reset_destroy_counters();
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, destroy_value_counter);
        
        char *key1 = strdup("key1");
        char *value1 = strdup("value1");
        chtbl_insert(&chtbl, key1, value1);
        
        // First clear should call destroy functions
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_INT(key_destroy_count, 1);
        EXPECT_EQUAL_INT(value_destroy_count, 1);
        
        // Second clear on empty table should not call destroy functions again
        reset_destroy_counters();
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_INT(key_destroy_count, 0);
        EXPECT_EQUAL_INT(value_destroy_count, 0);
        
        // Third clear should also be safe
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_INT(key_destroy_count, 0);
        EXPECT_EQUAL_INT(value_destroy_count, 0);
        
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 6: Multiple clears with insertions between
    {
        reset_destroy_counters();
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, destroy_key_counter, destroy_value_counter);
        
        char *key1 = strdup("key1");
        char *value1 = strdup("value1");
        chtbl_insert(&chtbl, key1, value1);
        
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_INT(key_destroy_count, 1);
        EXPECT_EQUAL_INT(value_destroy_count, 1);
        
        // Insert new elements and clear again
        char *key2 = strdup("key2");
        char *value2 = strdup("value2");
        chtbl_insert(&chtbl, key2, value2);
        
        reset_destroy_counters();
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_INT(key_destroy_count, 1);
        EXPECT_EQUAL_INT(value_destroy_count, 1);
        
        // Note: key1, value1, key2, value2 were already freed by destroy functions
        chtbl_destroy(&chtbl);
    }
    
    // Test 7: Clear with NULL pointer (should not crash)
    {
        chtbl_clear(NULL);
    }
}

/**
 * Test: chtbl_resize
 * Dependencies: chtbl_init, chtbl_insert, chtbl_buckets, chtbl_find
 */
UTEST_TEST_CASE(chtbl_resize){
    // Test 1: Resize empty table
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        
        int result = chtbl_resize(&chtbl, 32);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_buckets(&chtbl), 32);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 2: Resize table with elements (should preserve data)
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_insert(&chtbl, "key3", "value3");
        
        int result = chtbl_resize(&chtbl, 32);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_buckets(&chtbl), 32);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 3);
        EXPECT_EQUAL_STRING((char*)chtbl_find(&chtbl, "key1"), "value1");
        EXPECT_EQUAL_STRING((char*)chtbl_find(&chtbl, "key2"), "value2");
        EXPECT_EQUAL_STRING((char*)chtbl_find(&chtbl, "key3"), "value3");
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 3: Resize to smaller size (should preserve data)
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 32);
        
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        
        int result = chtbl_resize(&chtbl, 16);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_buckets(&chtbl), 16);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 2);
        EXPECT_EQUAL_STRING((char*)chtbl_find(&chtbl, "key1"), "value1");
        EXPECT_EQUAL_STRING((char*)chtbl_find(&chtbl, "key2"), "value2");
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 4: Resize with NULL pointer (should fail)
    {
        int result = chtbl_resize(NULL, 32);
        EXPECT_EQUAL_INT(result, -1);
    }
    
    // Test 5: Resize with many elements
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 8);
        
        for (int i = 0; i < 50; i++) {
            char key[32];
            char value[32];
            snprintf(key, sizeof(key), "key%d", i);
            snprintf(value, sizeof(value), "value%d", i);
            chtbl_insert(&chtbl, key, value);
        }
        
        int result = chtbl_resize(&chtbl, 64);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_buckets(&chtbl), 64);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 50);
        
        // Verify some random keys still exist
        EXPECT_EQUAL_STRING((char*)chtbl_find(&chtbl, "key0"), "value0");
        EXPECT_EQUAL_STRING((char*)chtbl_find(&chtbl, "key25"), "value25");
        EXPECT_EQUAL_STRING((char*)chtbl_find(&chtbl, "key49"), "value49");
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 6: Resize changes load factor (decreases when buckets increase)
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 10);
        
        // Insert 10 entries, load factor = 1.0
        for (int i = 0; i < 10; i++) {
            char key[32];
            snprintf(key, sizeof(key), "key%d", i);
            chtbl_insert(&chtbl, key, "value");
        }
        
        float load_factor_before = chtbl_load_factor(&chtbl);
        EXPECT_EQUAL_FLOAT(load_factor_before, 1.0f);
        
        // Resize to 20 buckets, load factor should decrease to 0.5
        int result = chtbl_resize(&chtbl, 20);
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_buckets(&chtbl), 20);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 10);
        
        float load_factor_after = chtbl_load_factor(&chtbl);
        EXPECT_TRUE(load_factor_after < load_factor_before);
        EXPECT_TRUE(load_factor_after >= 0.49f && load_factor_after <= 0.51f);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 7: Resize changes load factor (increases when buckets decrease)
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 20);
        
        // Insert 10 entries, load factor = 0.5
        for (int i = 0; i < 10; i++) {
            char key[32];
            snprintf(key, sizeof(key), "key%d", i);
            chtbl_insert(&chtbl, key, "value");
        }
        
        float load_factor_before = chtbl_load_factor(&chtbl);
        EXPECT_TRUE(load_factor_before >= 0.49f && load_factor_before <= 0.51f);
        
        // Resize to 10 buckets, load factor should increase to 1.0
        int result = chtbl_resize(&chtbl, 10);
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_buckets(&chtbl), 10);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 10);
        
        float load_factor_after = chtbl_load_factor(&chtbl);
        EXPECT_TRUE(load_factor_after > load_factor_before);
        EXPECT_EQUAL_FLOAT(load_factor_after, 1.0f);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 8: Resize preserves load factor threshold setting
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        chtbl_set_load_factor(&chtbl, 0.8f);
        
        // Insert some entries
        for (int i = 0; i < 8; i++) {
            char key[32];
            snprintf(key, sizeof(key), "key%d", i);
            chtbl_insert(&chtbl, key, "value");
        }
        
        // Resize should not change the threshold setting
        int result = chtbl_resize(&chtbl, 32);
        EXPECT_EQUAL_INT(result, 0);
        
        // Threshold should still be 0.8 (we can't directly check, but resize shouldn't affect it)
        // The load factor should be recalculated based on new bucket count
        float load_factor = chtbl_load_factor(&chtbl);
        EXPECT_TRUE(load_factor >= 0.24f && load_factor <= 0.26f); // 8/32 = 0.25
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 9: Multiple resize operations
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 8);
        
        // Insert entries
        for (int i = 0; i < 16; i++) {
            char key[32];
            snprintf(key, sizeof(key), "key%d", i);
            chtbl_insert(&chtbl, key, "value");
        }
        
        // First resize
        int result1 = chtbl_resize(&chtbl, 16);
        EXPECT_EQUAL_INT(result1, 0);
        EXPECT_EQUAL_UINT64(chtbl_buckets(&chtbl), 16);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 16);
        
        // Second resize
        int result2 = chtbl_resize(&chtbl, 32);
        EXPECT_EQUAL_INT(result2, 0);
        EXPECT_EQUAL_UINT64(chtbl_buckets(&chtbl), 32);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 16);
        
        // Third resize back to smaller
        int result3 = chtbl_resize(&chtbl, 8);
        EXPECT_EQUAL_INT(result3, 0);
        EXPECT_EQUAL_UINT64(chtbl_buckets(&chtbl), 8);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 16);
        
        // Verify all entries still exist
        for (int i = 0; i < 16; i++) {
            char key[32];
            snprintf(key, sizeof(key), "key%d", i);
            EXPECT_NOT_NULL(chtbl_find(&chtbl, key));
        }
        
        chtbl_destroy(&chtbl);
    }
    
    // Test 10: Resize to same size (should succeed but no change)
    {
        Chtbl chtbl;
        chtbl_init_capacity(&chtbl, NULL, NULL, NULL, NULL, 16);
        
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        
        size_t buckets_before = chtbl_buckets(&chtbl);
        size_t size_before = chtbl_size(&chtbl);
        float load_factor_before = chtbl_load_factor(&chtbl);
        
        int result = chtbl_resize(&chtbl, 16);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_buckets(&chtbl), buckets_before);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), size_before);
        
        float load_factor_after = chtbl_load_factor(&chtbl);
        EXPECT_EQUAL_FLOAT(load_factor_after, load_factor_before);
        
        // Verify entries still exist
        EXPECT_NOT_NULL(chtbl_find(&chtbl, "key1"));
        EXPECT_NOT_NULL(chtbl_find(&chtbl, "key2"));
        
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test: integration_chtbl
 * Dependencies: All chtbl functions
 * Description: Integration test that exercises all hash table functions together.
 */
UTEST_TEST_CASE(integration_chtbl){
    // Comprehensive integration test using all functions
    {
        Chtbl chtbl;
        
        // Initialize with custom capacity
        int result = chtbl_init_capacity(&chtbl, hash_str_wrapper, match_str_wrapper, 
            destroy_key_noop, destroy_value_noop, 16);
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
        EXPECT_EQUAL_UINT64(chtbl_buckets(&chtbl), 16);
        
        // Set load factor threshold
        chtbl_set_load_factor(&chtbl, 0.75f);
        
        // Insert multiple entries
        for (int i = 0; i < 10; i++) {
            char key[32];
            char value[32];
            snprintf(key, sizeof(key), "key%d", i);
            snprintf(value, sizeof(value), "value%d", i);
            result = chtbl_insert(&chtbl, key, value);
            EXPECT_EQUAL_INT(result, 0);
        }
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 10);
        
        // Check load factor
        float load_factor = chtbl_load_factor(&chtbl);
        EXPECT_TRUE(load_factor > 0.0f);
        
        // Find entries
        void *value1 = chtbl_find(&chtbl, "key1");
        EXPECT_NOT_NULL(value1);
        EXPECT_EQUAL_STRING((char*)value1, "value1");
        
        // Find entry
        ChtblEntry *entry = chtbl_find_entry(&chtbl, "key5");
        EXPECT_NOT_NULL(entry);
        EXPECT_EQUAL_STRING((char*)entry->key, "key5");
        EXPECT_EQUAL_STRING((char*)entry->value, "value5");
        
        // Check if key exists
        EXPECT_TRUE(chtbl_in(&chtbl, "key2"));
        EXPECT_FALSE(chtbl_in(&chtbl, "nonexistent"));
        
        // Update existing key
        chtbl_insert(&chtbl, "key3", "newvalue3");
        void *updated_value = chtbl_find(&chtbl, "key3");
        EXPECT_EQUAL_STRING((char*)updated_value, "newvalue3");
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 10); // Size should not increase
        
        // Remove entry without retrieving
        result = chtbl_remove(&chtbl, "key0", NULL);
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 9);
        EXPECT_FALSE(chtbl_in(&chtbl, "key0"));
        
        // Remove entry and retrieve
        ChtblEntry *removed_entry = NULL;
        result = chtbl_remove(&chtbl, "key1", &removed_entry);
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_NOT_NULL(removed_entry);
        EXPECT_EQUAL_STRING((char*)removed_entry->key, "key1");
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 8);
        free(removed_entry);
        
        // Resize the table
        result = chtbl_resize(&chtbl, 32);
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(chtbl_buckets(&chtbl), 32);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 8);
        
        // Verify entries still exist after resize
        EXPECT_TRUE(chtbl_in(&chtbl, "key2"));
        EXPECT_TRUE(chtbl_in(&chtbl, "key4"));
        void *value_after_resize = chtbl_find(&chtbl, "key7");
        EXPECT_NOT_NULL(value_after_resize);
        
        // Check load factor after resize
        float load_factor_after = chtbl_load_factor(&chtbl);
        EXPECT_TRUE(load_factor_after < load_factor);
        
        // Clear the table
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
        EXPECT_EQUAL_UINT64(chtbl_buckets(&chtbl), 32); // Buckets should remain
        EXPECT_EQUAL_FLOAT(chtbl_load_factor(&chtbl), 0.0f);
        
        // Insert after clear
        chtbl_insert(&chtbl, "newkey", "newvalue");
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 1);
        EXPECT_TRUE(chtbl_in(&chtbl, "newkey"));
        
        // Multiple resize operations
        chtbl_resize(&chtbl, 16);
        chtbl_resize(&chtbl, 64);
        EXPECT_EQUAL_UINT64(chtbl_buckets(&chtbl), 64);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 1);
        EXPECT_TRUE(chtbl_in(&chtbl, "newkey"));
        
        // Destroy the table
        chtbl_destroy(&chtbl);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
    }
    
    // Test with integer keys
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, hash_int_ptr, match_int_ptr, NULL, NULL);
        
        int key1 = 1, key2 = 2, key3 = 3;
        chtbl_insert(&chtbl, &key1, "value1");
        chtbl_insert(&chtbl, &key2, "value2");
        chtbl_insert(&chtbl, &key3, "value3");
        
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 3);
        EXPECT_TRUE(chtbl_in(&chtbl, &key2));
        
        void *value = chtbl_find(&chtbl, &key1);
        EXPECT_EQUAL_STRING((char*)value, "value1");
        
        chtbl_remove(&chtbl, &key2, NULL);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 2);
        
        chtbl_clear(&chtbl);
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 0);
        
        chtbl_destroy(&chtbl);
    }
    
    // Test load factor threshold persistence
    {
        Chtbl chtbl;
        chtbl_init(&chtbl, NULL, NULL, NULL, NULL);
        
        chtbl_set_load_factor(&chtbl, 0.8f);
        
        // Perform various operations
        chtbl_insert(&chtbl, "key1", "value1");
        chtbl_insert(&chtbl, "key2", "value2");
        chtbl_find(&chtbl, "key1");
        chtbl_remove(&chtbl, "key1", NULL);
        chtbl_clear(&chtbl);
        
        // Threshold should still be set (operations should work)
        chtbl_insert(&chtbl, "key3", "value3");
        EXPECT_EQUAL_UINT64(chtbl_size(&chtbl), 1);
        
        chtbl_destroy(&chtbl);
    }
}

/**
 * Test Suite: chtbl
 */
UTEST_TEST_SUITE(chtbl){
    UTEST_RUN_TEST_CASE(chtbl_init);
    UTEST_RUN_TEST_CASE(chtbl_init_capacity);
    UTEST_RUN_TEST_CASE(chtbl_size);
    UTEST_RUN_TEST_CASE(chtbl_buckets);
    UTEST_RUN_TEST_CASE(chtbl_set_load_factor);
    UTEST_RUN_TEST_CASE(chtbl_load_factor);
    UTEST_RUN_TEST_CASE(chtbl_destroy);
    UTEST_RUN_TEST_CASE(chtbl_insert);
    UTEST_RUN_TEST_CASE(chtbl_find);
    UTEST_RUN_TEST_CASE(chtbl_find_entry);
    UTEST_RUN_TEST_CASE(chtbl_in);
    UTEST_RUN_TEST_CASE(chtbl_remove);
    UTEST_RUN_TEST_CASE(chtbl_clear);
    UTEST_RUN_TEST_CASE(chtbl_resize);
    UTEST_RUN_TEST_CASE(integration_chtbl);
}

