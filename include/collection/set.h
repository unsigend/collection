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

#ifndef COLLECTION_SET_H
#define COLLECTION_SET_H

/**
 * This file contains the interface for set.
 * Which is a unordered collection of unique elements.
 */
#include <collection/common.h>
#include <collection/chtbl.h>
#include <stdbool.h>

// internal struct for Set
typedef struct {
    /* hash table */ 
    Chtbl chtbl;
} Set;

/**
 * Initialize the set.
 * @param set The set to initialize.
 * @param hash The hash function. (default: hash_str)
 * @param match The match function. (default: strcmp)
 * @param destroy The function to destroy the elements.
 * @complexity O(1)
 */
extern int set_init(Set* set, uint32_t (*hash)(const void * key), 
    bool (*match)(const void * key1, const void * key2), 
    void (*destroy)(void *));

/**
 * Destroy the set.
 * @param set The set to destroy.
 * @complexity O(n)
 */
extern void set_destroy(Set* set);

/**
 * Check if the set is empty.
 * @param set The set to check.
 * @return True if the set is empty, false otherwise.
 * @complexity O(1)
 */
extern bool set_empty(const Set* set);

/**
 * Get the size of the set.
 * @param set The set to get the size of.
 * @return The size of the set.
 * @complexity O(1)
 */
extern size_t set_size(const Set* set);

/**
 * Insert an element into the set.
 * @param set The set to insert the element into.
 * @param data The element to insert.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int set_insert(Set* set, const void * data);

/**
 * Remove an element from the set.
 * @param set The set to remove the element from.
 * @param data The element to remove.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int set_remove(Set* set, const void * data);

/**
 * Clear the set.
 * @param set The set to clear.
 * @complexity O(n)
 */
extern void set_clear(Set* set);

/**
 * Check if the set contains an element.
 * @param set The set to check.
 * @param data The element to check.
 * @return True if the set contains the element, false otherwise.
 * @complexity O(1)
 */
extern bool set_contains(const Set* set, const void * data);

/**
 * Get the intersection of two sets.
 * @param set1 The first set.
 * @param set2 The second set.
 * @param result The result set.
 * @return 0 if successful, -1 if failed
 * @note The result set will be cleared before the operation and keep a shadow copy.
 * @complexity O(n)
 */
extern int set_intersection(Set* result, const Set* set1, const Set* set2);

/**
 * Get the union of two sets.
 * @param set1 The first set.
 * @param set2 The second set.
 * @param result The result set.
 * @return 0 if successful, -1 if failed
 * @note The result set will be cleared before the operation and keep a shadow copy.
 * @complexity O(n + m)
 */
extern int set_union(Set* result, const Set* set1, const Set* set2);

/**
 * Get the difference of two sets.
 * @param set1 The first set.
 * @param set2 The second set.
 * @param result The result set.
 * @return 0 if successful, -1 if failed
 * @note The result set will be cleared before the operation and keep a shadow copy.
 * @complexity O(n)
 */
extern int set_difference(Set* result, const Set* set1, const Set* set2);

/**
 * Check if two sets are equal.
 * @param set1 The first set.
 * @param set2 The second set.
 * @return True if the two sets are equal, false otherwise.
 * @complexity O(n)
 */
extern bool set_equal(const Set* set1, const Set* set2);

/**
 * Check if one set is a subset of another.
 * @param set1 The first set.
 * @param set2 The second set.
 * @return True if the first set is a subset of the second set, false otherwise.
 * @complexity O(n)
 */
extern bool set_subset(const Set* set1, const Set* set2);

#endif