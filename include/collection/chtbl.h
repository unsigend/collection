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

// internal struct for Chtbl
typedef struct {
    Vector table;
    int (*hash)(const void *key);
    int (*match)(const void *key1, const void *key2);
    void (*destroy)(void *data);
    size_t size;
} Chtbl;

/**
 * Initialize the chained hash table.
 * @param htbl The chained hash table to initialize.
 * @param nbuckets The number of buckets in the hash table.
 * @param hash The hash function to use.
 * @param match The function to use to match keys.
 * @param destroy The function to use to destroy data.
 * @return 0 if successful, -1 if failed.
 * @complexity O(n)
 */
extern int chtbl_init(Chtbl *htbl, size_t nbuckets, int (*hash)(const void *key), 
    int (*match)(const void *key1, const void *key2), 
    void (*destroy)(void *data));

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
 * Insert an element into the chained hash table.
 * @param htbl The chained hash table to insert the element into.
 * @param data The element to insert.
 * @return 0 if successful, -1 if failed.
 * @complexity O(1)
 */
extern int chtbl_insert(Chtbl *htbl, const void *data);

/**
 * Remove an element from the chained hash table.
 * @param htbl The chained hash table to remove the element from.
 * @param data The element to remove.
 * @return 0 if successful, -1 if failed.
 * @complexity O(1)
 */
extern int chtbl_remove(Chtbl *htbl, void **data);

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


#endif