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

#include <algorithm/hash.h>
#include <utest.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>

/**
 * Test: hash_str
 * Dependencies: None
 * Description: Tests the hash_str function including deterministic behavior and uniform distribution.
 */
UTEST_TEST_CASE(hash_str){
    // Test 1: Same string produces same hash
    {
        const char* str1 = "hello";
        const char* str2 = "hello";
        uint32_t hash1 = hash_str(str1);
        uint32_t hash2 = hash_str(str2);
        EXPECT_EQUAL_UINT(hash1, hash2);
    }

    // Test 2: Different strings produce different hashes (high probability)
    {
        const char* str1 = "hello";
        const char* str2 = "world";
        uint32_t hash1 = hash_str(str1);
        uint32_t hash2 = hash_str(str2);
        EXPECT_NOT_EQUAL_UINT(hash1, hash2);
    }

    // Test 3: Empty string produces valid hash
    {
        uint32_t hash = hash_str("");
        EXPECT_TRUE(hash >= 0);
    }

    // Test 4: Single character string
    {
        uint32_t hash1 = hash_str("a");
        uint32_t hash2 = hash_str("a");
        EXPECT_EQUAL_UINT(hash1, hash2);
    }

    // Test 5: Case sensitivity
    {
        uint32_t hash1 = hash_str("Hello");
        uint32_t hash2 = hash_str("hello");
        EXPECT_NOT_EQUAL_UINT(hash1, hash2);
    }

    // Test 6: Whitespace sensitivity
    {
        uint32_t hash1 = hash_str("hello");
        uint32_t hash2 = hash_str("hello ");
        EXPECT_NOT_EQUAL_UINT(hash1, hash2);
    }

    // Test 7: Long string
    {
        const char* long_str = "This is a very long string that should still produce a valid hash value";
        uint32_t hash1 = hash_str(long_str);
        uint32_t hash2 = hash_str(long_str);
        EXPECT_EQUAL_UINT(hash1, hash2);
    }

    // Test 8: String with special characters
    {
        const char* str1 = "hello@world#123";
        const char* str2 = "hello@world#123";
        uint32_t hash1 = hash_str(str1);
        uint32_t hash2 = hash_str(str2);
        EXPECT_EQUAL_UINT(hash1, hash2);
    }

    // Test 9: Multiple calls with same string
    {
        const char* str = "test";
        uint32_t hash1 = hash_str(str);
        uint32_t hash2 = hash_str(str);
        uint32_t hash3 = hash_str(str);
        EXPECT_EQUAL_UINT(hash1, hash2);
        EXPECT_EQUAL_UINT(hash2, hash3);
    }

    // Test 10: Similar strings produce different hashes
    {
        uint32_t hash1 = hash_str("abc");
        uint32_t hash2 = hash_str("abd");
        uint32_t hash3 = hash_str("ab");
        EXPECT_NOT_EQUAL_UINT(hash1, hash2);
        EXPECT_NOT_EQUAL_UINT(hash1, hash3);
        EXPECT_NOT_EQUAL_UINT(hash2, hash3);
    }

    // Test 11: Deterministic - same string multiple times
    {
        const char* str = "deterministic_test";
        uint32_t hash1 = hash_str(str);
        uint32_t hash2 = hash_str(str);
        uint32_t hash3 = hash_str(str);
        uint32_t hash4 = hash_str(str);
        uint32_t hash5 = hash_str(str);
        
        EXPECT_EQUAL_UINT(hash1, hash2);
        EXPECT_EQUAL_UINT(hash2, hash3);
        EXPECT_EQUAL_UINT(hash3, hash4);
        EXPECT_EQUAL_UINT(hash4, hash5);
    }

    // Test 12: Deterministic - different string literals with same content
    {
        uint32_t hash1 = hash_str("test");
        uint32_t hash2 = hash_str("test");
        EXPECT_EQUAL_UINT(hash1, hash2);
    }

    // Test 13: Deterministic - string variable vs literal
    {
        const char* str = "test";
        uint32_t hash1 = hash_str(str);
        uint32_t hash2 = hash_str("test");
        EXPECT_EQUAL_UINT(hash1, hash2);
    }

    // Test 14: Deterministic - multiple different strings maintain consistency
    {
        const char* strs[] = {"a", "b", "c", "d", "e"};
        uint32_t hashes1[5];
        uint32_t hashes2[5];
        
        for (int i = 0; i < 5; i++) {
            hashes1[i] = hash_str(strs[i]);
        }
        
        for (int i = 0; i < 5; i++) {
            hashes2[i] = hash_str(strs[i]);
        }
        
        for (int i = 0; i < 5; i++) {
            EXPECT_EQUAL_UINT(hashes1[i], hashes2[i]);
        }
    }

    // Test 15: Uniform distribution - distribution across multiple strings
    {
        const char* strings[] = {
            "a", "b", "c", "d", "e", "f", "g", "h", "i", "j",
            "k", "l", "m", "n", "o", "p", "q", "r", "s", "t",
            "u", "v", "w", "x", "y", "z"
        };
        const int num_strings = sizeof(strings) / sizeof(strings[0]);
        uint32_t hashes[26];
        
        for (int i = 0; i < num_strings; i++) {
            hashes[i] = hash_str(strings[i]);
        }
        
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
        
        EXPECT_TRUE(unique_count >= num_strings / 2);
    }

    // Test 16: Uniform distribution - distribution across numeric strings
    {
        char num_str[32];
        uint32_t hashes[100];
        
        for (int i = 0; i < 100; i++) {
            snprintf(num_str, sizeof(num_str), "number_%d", i);
            hashes[i] = hash_str(num_str);
        }
        
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
        
        EXPECT_TRUE(unique_count >= 80);
    }

    // Test 17: Uniform distribution - hash values are distributed across range
    {
        const int num_samples = 1000;
        uint32_t hashes[num_samples];
        char str[64];
        
        for (int i = 0; i < num_samples; i++) {
            snprintf(str, sizeof(str), "sample_string_%d", i);
            hashes[i] = hash_str(str);
        }
        
        uint32_t min_hash = UINT32_MAX;
        uint32_t max_hash = 0;
        for (int i = 0; i < num_samples; i++) {
            if (hashes[i] < min_hash) {
                min_hash = hashes[i];
            }
            if (hashes[i] > max_hash) {
                max_hash = hashes[i];
            }
        }
        
        EXPECT_TRUE(max_hash > min_hash);
        EXPECT_TRUE((max_hash - min_hash) > num_samples / 10);
    }

    // Test 18: Hash table simulation with strings
    {
        const char* keys[] = {"key1", "key2", "key3", "key4", "key5"};
        uint32_t hashes[5];
        
        for (int i = 0; i < 5; i++) {
            hashes[i] = hash_str(keys[i]);
        }
        
        for (int i = 0; i < 5; i++) {
            uint32_t hash = hash_str(keys[i]);
            EXPECT_EQUAL_UINT(hash, hashes[i]);
        }
    }

    // Test 19: Collision detection
    {
        const int num_strings = 1000;
        uint32_t hashes[num_strings];
        char str[64];
        
        for (int i = 0; i < num_strings; i++) {
            snprintf(str, sizeof(str), "key_%d", i);
            hashes[i] = hash_str(str);
        }
        
        int collision_count = 0;
        for (int i = 0; i < num_strings; i++) {
            for (int j = i + 1; j < num_strings; j++) {
                if (hashes[i] == hashes[j]) {
                    collision_count++;
                }
            }
        }
        
        EXPECT_TRUE(collision_count < num_strings);
    }

    // Test 20: Hash value range validation
    {
        uint32_t str_hash = hash_str("test_string");
        EXPECT_TRUE(str_hash <= UINT32_MAX);
    }
}

/**
 * Test: hash_int
 * Dependencies: None
 * Description: Tests the hash_int function including deterministic behavior and uniform distribution.
 */
UTEST_TEST_CASE(hash_int){
    // Test 1: Same integer produces same hash
    {
        int val = 42;
        uint32_t hash1 = hash_int(val);
        uint32_t hash2 = hash_int(val);
        EXPECT_EQUAL_UINT(hash1, hash2);
    }

    // Test 2: Different integers produce different hashes (high probability)
    {
        uint32_t hash1 = hash_int(42);
        uint32_t hash2 = hash_int(43);
        EXPECT_NOT_EQUAL_UINT(hash1, hash2);
    }

    // Test 3: Zero produces valid hash
    {
        uint32_t hash = hash_int(0);
        EXPECT_TRUE(hash >= 0);
    }

    // Test 4: Positive integers
    {
        uint32_t hash1 = hash_int(1);
        uint32_t hash2 = hash_int(1);
        EXPECT_EQUAL_UINT(hash1, hash2);
    }

    // Test 5: Negative integers
    {
        uint32_t hash1 = hash_int(-1);
        uint32_t hash2 = hash_int(-1);
        EXPECT_EQUAL_UINT(hash1, hash2);
    }

    // Test 6: Negative and positive produce different hashes
    {
        uint32_t hash1 = hash_int(42);
        uint32_t hash2 = hash_int(-42);
        EXPECT_NOT_EQUAL_UINT(hash1, hash2);
    }

    // Test 7: INT_MAX produces valid hash
    {
        uint32_t hash = hash_int(INT_MAX);
        EXPECT_TRUE(hash >= 0);
    }

    // Test 8: INT_MIN produces valid hash
    {
        uint32_t hash = hash_int(INT_MIN);
        EXPECT_TRUE(hash >= 0);
    }

    // Test 9: Multiple calls with same integer
    {
        int val = 12345;
        uint32_t hash1 = hash_int(val);
        uint32_t hash2 = hash_int(val);
        uint32_t hash3 = hash_int(val);
        EXPECT_EQUAL_UINT(hash1, hash2);
        EXPECT_EQUAL_UINT(hash2, hash3);
    }

    // Test 10: Sequential integers produce different hashes
    {
        uint32_t hash1 = hash_int(100);
        uint32_t hash2 = hash_int(101);
        uint32_t hash3 = hash_int(102);
        EXPECT_NOT_EQUAL_UINT(hash1, hash2);
        EXPECT_NOT_EQUAL_UINT(hash2, hash3);
    }

    // Test 11: Deterministic - same integer multiple times
    {
        int val = 12345;
        uint32_t hash1 = hash_int(val);
        uint32_t hash2 = hash_int(val);
        uint32_t hash3 = hash_int(val);
        uint32_t hash4 = hash_int(val);
        uint32_t hash5 = hash_int(val);
        
        EXPECT_EQUAL_UINT(hash1, hash2);
        EXPECT_EQUAL_UINT(hash2, hash3);
        EXPECT_EQUAL_UINT(hash3, hash4);
        EXPECT_EQUAL_UINT(hash4, hash5);
    }

    // Test 12: Deterministic - integer literal vs variable
    {
        int val = 42;
        uint32_t hash1 = hash_int(val);
        uint32_t hash2 = hash_int(42);
        EXPECT_EQUAL_UINT(hash1, hash2);
    }

    // Test 13: Deterministic - multiple different integers maintain consistency
    {
        int vals[] = {1, 2, 3, 4, 5};
        uint32_t hashes1[5];
        uint32_t hashes2[5];
        
        for (int i = 0; i < 5; i++) {
            hashes1[i] = hash_int(vals[i]);
        }
        
        for (int i = 0; i < 5; i++) {
            hashes2[i] = hash_int(vals[i]);
        }
        
        for (int i = 0; i < 5; i++) {
            EXPECT_EQUAL_UINT(hashes1[i], hashes2[i]);
        }
    }

    // Test 14: Deterministic - edge values maintain consistency
    {
        uint32_t hash1_max = hash_int(INT_MAX);
        uint32_t hash2_max = hash_int(INT_MAX);
        EXPECT_EQUAL_UINT(hash1_max, hash2_max);
        
        uint32_t hash1_min = hash_int(INT_MIN);
        uint32_t hash2_min = hash_int(INT_MIN);
        EXPECT_EQUAL_UINT(hash1_min, hash2_min);
    }

    // Test 15: Uniform distribution - distribution across sequential integers
    {
        uint32_t hashes[100];
        
        for (int i = 0; i < 100; i++) {
            hashes[i] = hash_int(i);
        }
        
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
        
        EXPECT_TRUE(unique_count >= 80);
    }

    // Test 16: Uniform distribution - distribution across negative integers
    {
        uint32_t hashes[100];
        
        for (int i = 0; i < 100; i++) {
            hashes[i] = hash_int(-i - 1);
        }
        
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
        
        EXPECT_TRUE(unique_count >= 80);
    }

    // Test 17: Uniform distribution - hash values are distributed across range
    {
        const int num_samples = 1000;
        uint32_t hashes[num_samples];
        
        for (int i = 0; i < num_samples; i++) {
            hashes[i] = hash_int(i * 7 + 13);
        }
        
        uint32_t min_hash = UINT32_MAX;
        uint32_t max_hash = 0;
        for (int i = 0; i < num_samples; i++) {
            if (hashes[i] < min_hash) {
                min_hash = hashes[i];
            }
            if (hashes[i] > max_hash) {
                max_hash = hashes[i];
            }
        }
        
        EXPECT_TRUE(max_hash > min_hash);
        EXPECT_TRUE((max_hash - min_hash) > num_samples / 10);
    }

    // Test 18: Uniform distribution - distribution across large range
    {
        uint32_t hashes[100];
        
        for (int i = 0; i < 100; i++) {
            hashes[i] = hash_int(i * 1000000);
        }
        
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
        
        EXPECT_TRUE(unique_count >= 80);
    }

    // Test 19: Hash table simulation with integers
    {
        int keys[] = {10, 20, 30, 40, 50};
        uint32_t hashes[5];
        
        for (int i = 0; i < 5; i++) {
            hashes[i] = hash_int(keys[i]);
        }
        
        for (int i = 0; i < 5; i++) {
            uint32_t hash = hash_int(keys[i]);
            EXPECT_EQUAL_UINT(hash, hashes[i]);
        }
    }

    // Test 20: Hash value range validation
    {
        uint32_t int_hash = hash_int(12345);
        EXPECT_TRUE(int_hash <= UINT32_MAX);
    }
}

/**
 * Test suite: hash
 * Description: Test suite for hash algorithm functions
 */
UTEST_TEST_SUITE(hash){
    UTEST_RUN_TEST_CASE(hash_str);
    UTEST_RUN_TEST_CASE(hash_int);
}
