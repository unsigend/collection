#include <utest.h>
#include <algorithm/hash.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

/**
 * Test: hash_str
 * Dependencies: None
 * Description: Tests string hashing function.
 */
UTEST_TEST_CASE(hash_str){
    // Test 1: Same string produces same hash
    {
        const char* str1 = "hello";
        const char* str2 = "hello";
        
        uint32_t hash1 = hash_str(str1);
        uint32_t hash2 = hash_str(str2);
        
        EXPECT_EQUAL_UINT32(hash1, hash2);
    }
    
    // Test 2: Multiple calls produce same hash
    {
        const char* str = "test_string";
        
        uint32_t hash1 = hash_str(str);
        uint32_t hash2 = hash_str(str);
        uint32_t hash3 = hash_str(str);
        
        EXPECT_EQUAL_UINT32(hash1, hash2);
        EXPECT_EQUAL_UINT32(hash2, hash3);
    }
    
    // Test 3: Different strings produce different hashes (high probability)
    {
        const char* str1 = "hello";
        const char* str2 = "world";
        
        uint32_t hash1 = hash_str(str1);
        uint32_t hash2 = hash_str(str2);
        
        // Different strings should produce different hashes (collision is possible but unlikely)
        EXPECT_NOT_EQUAL_UINT32(hash1, hash2);
    }
    
    // Test 4: Empty string
    {
        const char* str = "";
        uint32_t hash = hash_str(str);
        
        // Should produce a valid hash (not crash)
        (void)hash;
    }
    
    // Test 5: Single character
    {
        const char* str = "a";
        uint32_t hash = hash_str(str);
        
        // Should produce a valid hash
        (void)hash;
    }
    
    // Test 6: Single character repeated
    {
        const char* str1 = "a";
        const char* str2 = "aa";
        
        uint32_t hash1 = hash_str(str1);
        uint32_t hash2 = hash_str(str2);
        
        // Should produce different hashes
        EXPECT_NOT_EQUAL_UINT32(hash1, hash2);
    }
    
    // Test 7: Very long string
    {
        char long_str[1000];
        memset(long_str, 'a', 999);
        long_str[999] = '\0';
        
        uint32_t hash = hash_str(long_str);
        
        // Should produce a valid hash
        (void)hash;
    }
    
    // Test 8: String with special characters
    {
        const char* str = "!@#$%^&*()_+-=[]{}|;':\",./<>?";
        uint32_t hash = hash_str(str);
        
        // Should produce a valid hash
        (void)hash;
    }
    
    // Test 9: String with null bytes (should hash up to first null)
    {
        char str_with_null[10];
        strcpy(str_with_null, "hello");
        str_with_null[5] = '\0';
        str_with_null[6] = 'x'; // This should be ignored
        
        const char* normal_str = "hello";
        
        uint32_t hash1 = hash_str(str_with_null);
        uint32_t hash2 = hash_str(normal_str);
        
        // Should produce same hash (hash stops at null terminator)
        EXPECT_EQUAL_UINT32(hash1, hash2);
    }
    
    // Test 10: String with only whitespace
    {
        const char* str = "   \t\n\r   ";
        uint32_t hash = hash_str(str);
        
        // Should produce a valid hash
        (void)hash;
    }
    
    // Test 11: String with unicode-like characters
    {
        const char* str = "café";
        uint32_t hash = hash_str(str);
        
        // Should produce a valid hash
        (void)hash;
    }
    
    // Test 12: Different strings produce different hashes (distribution test)
    {
        const char* strings[] = {
            "a", "b", "c", "d", "e",
            "aa", "ab", "ac", "ad", "ae",
            "hello", "world", "test", "hash", "function"
        };
        const int num_strings = sizeof(strings) / sizeof(strings[0]);
        
        uint32_t hashes[15];
        for (int i = 0; i < num_strings; i++) {
            hashes[i] = hash_str(strings[i]);
        }
        
        // Check that most hashes are different (allow some collisions)
        int unique_count = 0;
        for (int i = 0; i < num_strings; i++) {
            int is_unique = 1;
            for (int j = 0; j < i; j++) {
                if (hashes[i] == hashes[j]) {
                    is_unique = 0;
                    break;
                }
            }
            if (is_unique) {
                unique_count++;
            }
        }
        
        // At least 80% should be unique (reasonable expectation)
        EXPECT_TRUE(unique_count >= (num_strings * 8 / 10));
    }
    
    // Test 13: Similar strings produce different hashes
    {
        const char* str1 = "hello";
        const char* str2 = "Hello";
        const char* str3 = "HELLO";
        const char* str4 = "hello ";
        const char* str5 = " hello";
        
        uint32_t hash1 = hash_str(str1);
        uint32_t hash2 = hash_str(str2);
        uint32_t hash3 = hash_str(str3);
        uint32_t hash4 = hash_str(str4);
        uint32_t hash5 = hash_str(str5);
        
        // All should be different (case and whitespace sensitive)
        EXPECT_NOT_EQUAL_UINT32(hash1, hash2);
        EXPECT_NOT_EQUAL_UINT32(hash1, hash3);
        EXPECT_NOT_EQUAL_UINT32(hash1, hash4);
        EXPECT_NOT_EQUAL_UINT32(hash1, hash5);
    }
    
    // Test 14: Reversed strings produce different hashes
    {
        const char* str1 = "abc";
        const char* str2 = "cba";
        
        uint32_t hash1 = hash_str(str1);
        uint32_t hash2 = hash_str(str2);
        
        EXPECT_NOT_EQUAL_UINT32(hash1, hash2);
    }
    
    // Test 15: Prefix vs suffix
    {
        const char* str1 = "prefix_suffix";
        const char* str2 = "suffix_prefix";
        
        uint32_t hash1 = hash_str(str1);
        uint32_t hash2 = hash_str(str2);
        
        EXPECT_NOT_EQUAL_UINT32(hash1, hash2);
    }
}

/**
 * Test: hash_int
 * Dependencies: None
 * Description: Tests integer hashing function.
 */
UTEST_TEST_CASE(hash_int){
    // Test 1: Same integer produces same hash
    {
        int val1 = 42;
        int val2 = 42;
        
        uint32_t hash1 = hash_int(val1);
        uint32_t hash2 = hash_int(val2);
        
        EXPECT_EQUAL_UINT32(hash1, hash2);
    }
    
    // Test 2: Multiple calls produce same hash
    {
        int val = 12345;
        
        uint32_t hash1 = hash_int(val);
        uint32_t hash2 = hash_int(val);
        uint32_t hash3 = hash_int(val);
        
        EXPECT_EQUAL_UINT32(hash1, hash2);
        EXPECT_EQUAL_UINT32(hash2, hash3);
    }
    
    // Test 3: Different integers produce different hashes (high probability)
    {
        int val1 = 42;
        int val2 = 43;
        
        uint32_t hash1 = hash_int(val1);
        uint32_t hash2 = hash_int(val2);
        
        // Different integers should produce different hashes
        EXPECT_NOT_EQUAL_UINT32(hash1, hash2);
    }
    
    // Test 4: Zero
    {
        int val = 0;
        uint32_t hash = hash_int(val);
        
        // Should produce a valid hash
        (void)hash;
    }
    
    // Test 5: Positive one
    {
        int val = 1;
        uint32_t hash = hash_int(val);
        
        // Should produce a valid hash
        (void)hash;
    }
    
    // Test 6: Negative one
    {
        int val = -1;
        uint32_t hash = hash_int(val);
        
        // Should produce a valid hash
        (void)hash;
    }
    
    // Test 7: Maximum positive integer
    {
        int val = INT_MAX;
        uint32_t hash = hash_int(val);
        
        // Should produce a valid hash
        (void)hash;
    }
    
    // Test 8: Minimum negative integer
    {
        int val = INT_MIN;
        uint32_t hash = hash_int(val);
        
        // Should produce a valid hash
        (void)hash;
    }
    
    // Test 9: Small positive numbers
    {
        for (int i = 0; i < 10; i++) {
            uint32_t hash = hash_int(i);
            (void)hash;
        }
    }
    
    // Test 10: Small negative numbers
    {
        for (int i = -10; i < 0; i++) {
            uint32_t hash = hash_int(i);
            (void)hash;
        }
    }
    
    // Test 11: Large positive numbers
    {
        int val1 = 1000000;
        int val2 = 1000001;
        
        uint32_t hash1 = hash_int(val1);
        uint32_t hash2 = hash_int(val2);
        
        // Should produce different hashes
        EXPECT_NOT_EQUAL_UINT32(hash1, hash2);
    }
    
    // Test 12: Large negative numbers
    {
        int val1 = -1000000;
        int val2 = -1000001;
        
        uint32_t hash1 = hash_int(val1);
        uint32_t hash2 = hash_int(val2);
        
        // Should produce different hashes
        EXPECT_NOT_EQUAL_UINT32(hash1, hash2);
    }
    
    // Test 13: Sequential integers produce different hashes
    {
        uint32_t hashes[100];
        for (int i = 0; i < 100; i++) {
            hashes[i] = hash_int(i);
        }
        
        // Check that most hashes are different
        int unique_count = 0;
        for (int i = 0; i < 100; i++) {
            int is_unique = 1;
            for (int j = 0; j < i; j++) {
                if (hashes[i] == hashes[j]) {
                    is_unique = 0;
                    break;
                }
            }
            if (is_unique) {
                unique_count++;
            }
        }
        
        // At least 90% should be unique for sequential integers
        EXPECT_TRUE(unique_count >= 90);
    }
    
    // Test 14: Negative and positive produce different hashes
    {
        int val1 = 42;
        int val2 = -42;
        
        uint32_t hash1 = hash_int(val1);
        uint32_t hash2 = hash_int(val2);
        
        // Should produce different hashes
        EXPECT_NOT_EQUAL_UINT32(hash1, hash2);
    }
    
    // Test 15: Sparse integers
    {
        int values[] = {1, 100, 1000, 10000, 100000, 1000000};
        const int num_values = sizeof(values) / sizeof(values[0]);
        
        uint32_t hashes[6];
        for (int i = 0; i < num_values; i++) {
            hashes[i] = hash_int(values[i]);
        }
        
        // All should be different
        for (int i = 0; i < num_values; i++) {
            for (int j = i + 1; j < num_values; j++) {
                EXPECT_NOT_EQUAL_UINT32(hashes[i], hashes[j]);
            }
        }
    }
    
    // Test 16: Powers of two
    {
        int powers[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
        const int num_powers = sizeof(powers) / sizeof(powers[0]);
        
        uint32_t hashes[11];
        for (int i = 0; i < num_powers; i++) {
            hashes[i] = hash_int(powers[i]);
        }
        
        // Check uniqueness
        int unique_count = 0;
        for (int i = 0; i < num_powers; i++) {
            int is_unique = 1;
            for (int j = 0; j < i; j++) {
                if (hashes[i] == hashes[j]) {
                    is_unique = 0;
                    break;
                }
            }
            if (is_unique) {
                unique_count++;
            }
        }
        
        // Most should be unique
        EXPECT_TRUE(unique_count >= (num_powers * 8 / 10));
    }
    
    // Test 17: Negative powers of two
    {
        int val1 = 256;
        int val2 = -256;
        
        uint32_t hash1 = hash_int(val1);
        uint32_t hash2 = hash_int(val2);
        
        EXPECT_NOT_EQUAL_UINT32(hash1, hash2);
    }
}

/**
 * Test Suite: hash
 */
UTEST_TEST_SUITE(hash){
    UTEST_RUN_TEST_CASE(hash_str);
    UTEST_RUN_TEST_CASE(hash_int);
}
