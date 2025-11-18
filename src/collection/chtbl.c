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

#include "algorithm/hash.h"
#include "collection/slist.h"
#include <collection/chtbl.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>


#define DEFAULT_LOAD_FACTOR_THRESHOLD 0.75f
#define MAX_LOAD_FACTOR_THRESHOLD 1.0f
#define DEFAULT_BUCKETS 16
#define MIN_BUCKETS 8

#define INDEX(NBUCKETS, HASH) ((HASH) % (NBUCKETS))

/**
 * Default hash function for strings.
 * @param key The key to hash.
 * @return The hash of the key.
 * @complexity O(n)
 */
static uint32_t default_hash(const void * key){
    return hash_str((const char *)key);
}

/**
 * Default match function for strings.
 * @param key1 The first key to compare.
 * @param key2 The second key to compare.
 * @return True if the keys are equal, false otherwise.
 * @complexity O(n)
 */
static bool default_match(const void * key1, const void * key2){
    return strcmp((const char *)key1, (const char *)key2) == 0;
}

/**
 * Destroy an entry of the chained hash table.
 * @param entry The entry to destroy.
 * @param destroy_key The function to destroy the key.
 * @param destroy_value The function to destroy the value.
 * @complexity O(1)
 */
// static void entry_destroy(ChtblEntry* entry, 
//     void (*destroy_key)(void *), void (*destroy_value)(void *)){
//     if (!entry) return;

//     if (destroy_key && entry->key) {
//         destroy_key(entry->key);
//     }
//     if (destroy_value && entry->value) {
//         destroy_value(entry->value);
//     }
//     free(entry);
// }

/**
 * Destroy a bucket of the chained hash table.
 * @param bucket The bucket to destroy.
 * @complexity O(n)
 */
static void bucket_destroy(void* bucket){
    if (!bucket) return;
    slist_destroy((SList*)bucket);
    free(bucket);
}

/**
 * Initialize the buckets of the chained hash table.
 * @param chtbl The chained hash table to initialize the buckets of.
 * @param nbuckets The number of buckets to initialize.
 * @return 0 if successful, -1 if failed.
 * @complexity O(n)
 */
static int _init_buckets(Chtbl* chtbl, size_t nbuckets){
    for (size_t i = 0; i < nbuckets; i++){
        SList* bucket = malloc(sizeof(SList));
        if (!bucket) {
            vector_destroy(&chtbl->table);
            return COLLECTION_FAILURE;
        }
        slist_init(bucket, bucket_destroy);
        if (vector_push_back(&chtbl->table, bucket) != COLLECTION_SUCCESS) {
            slist_destroy(bucket);
            free(bucket);
            vector_destroy(&chtbl->table);
            return COLLECTION_FAILURE;
        }
    }
    return COLLECTION_SUCCESS;
}


int chtbl_init_capacity(Chtbl* chtbl, 
    uint32_t (*hash)(const void * key),
    bool (*match)(const void * key1, const void * key2),
    void (*destroy_key)(void * key),
    void (*destroy_value)(void * value),
    size_t capacity)
{
    if (!chtbl)
        return COLLECTION_FAILURE;

    if (capacity < MIN_BUCKETS)
        capacity = MIN_BUCKETS;

    memset(chtbl, 0, sizeof(Chtbl));

    chtbl->hash = hash ? hash : default_hash;
    chtbl->match = match ? match : default_match;
    chtbl->destroy_key = destroy_key;
    chtbl->destroy_value = destroy_value;
    chtbl->load_factor_threshold = DEFAULT_LOAD_FACTOR_THRESHOLD;
    chtbl->size = 0;

    vector_init(&chtbl->table, bucket_destroy);

    if (_init_buckets(chtbl, capacity) != COLLECTION_SUCCESS)
        return COLLECTION_FAILURE;
    return COLLECTION_SUCCESS;
}

int chtbl_init(Chtbl* chtbl, 
    uint32_t (*hash)(const void * key),
    bool (*match)(const void * key1, const void * key2),
    void (*destroy_key)(void * key),
    void (*destroy_value)(void * value))
{
    return chtbl_init_capacity(chtbl, hash, match, destroy_key, destroy_value, DEFAULT_BUCKETS);
}

void chtbl_set_load_factor(Chtbl* chtbl, float threshold){
    if (!chtbl || threshold < 0.0f)
        return;
    if (threshold > MAX_LOAD_FACTOR_THRESHOLD)
        threshold = MAX_LOAD_FACTOR_THRESHOLD;
    chtbl->load_factor_threshold = threshold;
}

float chtbl_load_factor(Chtbl* chtbl){
    if (!chtbl || chtbl_size(chtbl) == 0)
        return 0.0f;
    return (float)chtbl_size(chtbl) / (float)chtbl_buckets(chtbl);
}

void chtbl_destroy(Chtbl* chtbl){
    if (!chtbl) return;
    vector_destroy(&chtbl->table);
    memset(chtbl, 0, sizeof(Chtbl));
}

