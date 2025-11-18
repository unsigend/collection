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
#include <collection/common.h>
#include <collection/vector.h>
#include <stdbool.h>
#include <stdint.h>

// internal struct for Chtbl
typedef struct {
    Vector table;
    uint32_t (*hash)(const void *key);
    int (*match)(const void *key1, const void *key2);
    void (*destroy)(void *data);
    size_t size;
    float load_factor_threshold;
} Chtbl;

/**
 * Initialize the chained hash table.
 * @param htbl The chained hash table to initialize.
 * @param hash The hash function to use. Pass NULL to use default string hash (hash_str).
 * @param match The function to use to match keys. Pass NULL for string keys (uses strcmp internally).
 * @param destroy The function to use to destroy data. Pass NULL if no cleanup is needed.
 * @return 0 if successful, -1 if failed.
 * @complexity O(1)
 */
extern int chtbl_init(Chtbl *htbl, uint32_t (*hash)(const void *key), 
    int (*match)(const void *key1, const void *key2), 
    void (*destroy)(void *data));

/**
 * Initialize the chained hash table with a specific number of buckets.
 * @param htbl The chained hash table to initialize.
 * @param nbuckets The number of buckets to allocate.
 * @param hash The hash function to use. Pass NULL to use default string hash (hash_str).
 * @param match The function to use to match keys. Pass NULL for string keys (uses strcmp internally).
 * @param destroy The function to use to destroy data. Pass NULL if no cleanup is needed.
 * @return 0 if successful, -1 if failed.
 * @complexity O(1)
 */
extern int chtbl_init_with_nbuckets(Chtbl *htbl, size_t nbuckets, uint32_t 
    (*hash)(const void *key), 
    int (*match)(const void *key1, const void *key2), 
    void (*destroy)(void *data));

/**
 * Set the load factor for the chained hash table.
 * @param htbl The chained hash table to set the load factor of.
 * @param threshold The load factor threshold to set.
 * @complexity O(1)
 */
extern void chtbl_set_load_factor(Chtbl *htbl, float threshold);

/**
 * Get the load factor for the chained hash table.
 * @param htbl The chained hash table to get the load factor of.
 * @return The load factor of the chained hash table.
 * @complexity O(1)
 */
#define chtbl_load_factor(htbl) ((htbl) ? (htbl)->load_factor_threshold : 0.0)

/**
 * Destroy the chained hash table.
 * @param htbl The chained hash table to destroy.
 * @complexity O(n)
 */
extern void chtbl_destroy(Chtbl *htbl);

/**
 * Get the size of the chained hash table.
 * @param htbl The chained hash table to get the size of.
 * @return The size of the chained hash table.
 * @complexity O(1)
 */
#define chtbl_size(htbl) ((htbl) ? (htbl)->size : 0)

/**
 * Get the number of buckets in the chained hash table.
 * @param htbl The chained hash table to get the number of buckets of.
 * @return The number of buckets in the chained hash table.
 * @complexity O(1)
 */
#define chtbl_buckets(htbl) ((htbl) ? (htbl)->table.size : 0)

/**
 * Check if a key exists in the chained hash table.
 * @param htbl The chained hash table to check.
 * @param key The key to check for.
 * @return true if the key exists, false otherwise.
 * @complexity O(1)
 */
#define chtbl_in(htbl, key) (chtbl_find((htbl), (key)) != NULL)

/**
 * Insert an element into the chained hash table.
 * @param htbl The chained hash table to insert the element into.
 * @param data The element to insert.
 * @return 0 if successful, -1 if failed.
 * @complexity O(1)
 */
extern int chtbl_insert(Chtbl *htbl, const void *key);

/**
 * Remove an element from the chained hash table.
 * @param htbl The chained hash table to remove the element from.
 * @param key The key of the element to remove.
 * @param data The element to remove.
 * @return 0 if successful, -1 if failed.
 * @complexity O(1)
 */
extern int chtbl_remove(Chtbl *htbl, const void *key, void **data);

/**
 * Find an element in the chained hash table by a specific key.
 * @param htbl The chained hash table to find the element in.
 * @param key The key to find the element by.
 * @return The element if found, NULL otherwise.
 * @complexity O(1)
 */
extern void* chtbl_find(Chtbl *htbl, void *key);

/**
 * Clear the chained hash table.
 * @param htbl The chained hash table to clear.
 * @complexity O(n)
 */
extern void chtbl_clear(Chtbl *htbl);

/**
 * Resize the chained hash table to a specific number of buckets.
 * @param htbl The chained hash table to resize.
 * @param nbuckets The number of buckets to resize to.
 * @return 0 if successful, -1 if failed.
 * @complexity O(n)
 */
extern int chtbl_resize(Chtbl *htbl, size_t nbuckets);


#endif