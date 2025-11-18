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

#ifndef COLLECTION_CHTBL_H
#define COLLECTION_CHTBL_H

/**
 * This file contains the interface for chained hash table.
 */
#include <collection/vector.h>
#include <collection/common.h>
#include <stdbool.h>
#include <stdint.h>

// internal structure for ChtblEntry
typedef struct ChtblEntry {
    void * key;
    void * value;
} ChtblEntry;

// internal structure for Chtbl
typedef struct Chtbl {
    /* hash table */ 
    Vector table;
    /* number of the entries in the table */ 
    size_t size;
    /* maximum load factor */ 
    float load_factor_threshold;
    /* hash function */ 
    uint32_t (*hash)(const void * key);
    /* match function */ 
    bool (*match)(const void * key1, const void * key2);
    /* destroy key function */ 
    void (*destroy_key)(void * key);
    /* destroy value function */ 
    void (*destroy_value)(void * value);
} Chtbl;

/**
 * Initialize the chained hash table.
 * @param chtbl The chained hash table to initialize.
 * @param hash The hash function. (default: hash_str)
 * @param match The match function. (default: strcmp)
 * @param destroy_key The function to destroy the key. Can be NULL.
 * @param destroy_value The function to destroy the value. Can be NULL.
 * @return 0 if successful, -1 if failed.
 * @complexity O(1)
 */
extern int chtbl_init(Chtbl* chtbl, 
    uint32_t (*hash)(const void * key),
    bool (*match)(const void * key1, const void * key2),
    void (*destroy_key)(void * key),
    void (*destroy_value)(void * value));

/**
 * Initialize the chained hash table with a given capacity.
 * @param chtbl The chained hash table to initialize.
 * @param hash The hash function. (default: hash_str)
 * @param match The match function. (default: strcmp)
 * @param destroy_key The function to destroy the key. Can be NULL.
 * @param destroy_value The function to destroy the value. Can be NULL.
 * @param capacity The capacity of the hash table.
 * @return 0 if successful, -1 if failed.
 * @complexity O(n)
 */
extern int chtbl_init_capacity(Chtbl* chtbl, 
    uint32_t (*hash)(const void * key),
    bool (*match)(const void * key1, const void * key2),
    void (*destroy_key)(void * key),
    void (*destroy_value)(void * value),
    size_t capacity);

/**
 * Set the load factor threshold of the chained hash table.
 * @param chtbl The chained hash table to set the load factor threshold.
 * @param threshold The load factor threshold.
 * @complexity O(1)
 */
extern void chtbl_set_load_factor(Chtbl* chtbl, float threshold);

/**
 * Get the current load factor of the chained hash table.
 * @param chtbl The chained hash table to get the load factor.
 * @return The load factor of the chained hash table.
 * @complexity O(1)
 */
extern float chtbl_load_factor(Chtbl* chtbl);

/**
 * Destroy the chained hash table.
 * @param chtbl The chained hash table to destroy.
 * @complexity O(n)
 */
extern void chtbl_destroy(Chtbl* chtbl);

/**
 * Get the size of the key-value pairs in the chained hash table.
 * @param chtbl The chained hash table to get the size of.
 * @return The size of the key-value pairs in the chained hash table.
 * @complexity O(1)
 */
#define chtbl_size(chtbl) ((chtbl) ? (chtbl)->size : 0)

/**
 * Get the number of buckets in the chained hash table.
 * @param chtbl The chained hash table to get the number of buckets of.
 * @return The number of buckets in the chained hash table.
 * @complexity O(1)
 */
#define chtbl_buckets(chtbl) ((chtbl) ? (chtbl)->table.size : 0)

/**
 * Check if a key is in the chained hash table.
 * @param chtbl The chained hash table to check if the key is in.
 * @param key The key to check.
 * @return True if the key is in the chained hash table, false otherwise.
 * @complexity O(1)
 */
#define chtbl_in(chtbl, key) (chtbl_find(chtbl, key) != NULL)

/**
 * Find a key-value pair in the chained hash table by key.
 * @param chtbl The chained hash table to find the key-value pair in.
 * @param key The key to find.
 * @return The value of the key-value pair, or NULL if not found.
 * @complexity O(1)
 */
extern void* chtbl_find(Chtbl* chtbl, const void * key);

/**
 * Find a key-value pair in the chained hash table by key.
 * @param chtbl The chained hash table to find the key-value pair in.
 * @param key The key to find.
 * @return The entry of the key-value pair, or NULL if not found.
 * @complexity O(1)
 */
extern ChtblEntry* chtbl_find_entry(Chtbl* chtbl, const void * key);

/**
 * Insert a key-value pair into the chained hash table.
 * @param chtbl The chained hash table to insert the key-value pair into.
 * @param key The key to insert.
 * @param value The value to insert.
 * @return 0 if successful, -1 if failed.
 * @complexity O(1)
 */
extern int chtbl_insert(Chtbl* chtbl, const void * key, const void * value);

/**
 * Remove a key-value pair from the chained hash table by key.
 * @param chtbl The chained hash table to remove the key-value pair from.
 * @param key The key to remove.
 * @param entry The pointer to store the removed entry. 
 * Or NULL will call the destroy_value and destroy_key functions.
 * @return 0 if successful, -1 if failed.
 * @complexity O(1)
 */
extern int chtbl_remove(Chtbl* chtbl, const void * key, ChtblEntry** entry);

/**
 * Clear the chained hash table.
 * @param chtbl The chained hash table to clear.
 * @complexity O(n)
 */
extern void chtbl_clear(Chtbl* chtbl);

/**
 * Resize the chained hash table to the given number of buckets.
 * @param chtbl The chained hash table to resize.
 * @param nbuckets The number of buckets to resize to.
 * @return 0 if successful, -1 if failed.
 * @complexity O(n)
 */
extern int chtbl_resize(Chtbl* chtbl, size_t nbuckets);

#endif