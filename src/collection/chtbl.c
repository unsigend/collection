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

#include <collection/vector.h>
#include <collection/common.h>
#include <algorithm/hash.h>
#include <collection/slist.h>
#include <collection/chtbl.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_LOAD_FACTOR_THRESHOLD 0.75f
#define MAX_LOAD_FACTOR_THRESHOLD 1.0f
#define DEFAULT_BUCKETS 16
#define MIN_BUCKETS 8
#define RESIZE_FACTOR 2

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
 * @param context The context of the chained hash table.
 * @complexity O(1)
 */
static void entry_destroy(void* entry, void* context){
    if (!entry) return;
    ChtblEntry* entry_ptr = (ChtblEntry*)entry;
    Chtbl* chtbl = (Chtbl*)context;
    if (chtbl->destroy_key && entry_ptr->key){
        chtbl->destroy_key(entry_ptr->key);
    }
    if (chtbl->destroy_value && entry_ptr->value){
        chtbl->destroy_value(entry_ptr->value);
    }
    free(entry_ptr);
}

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
        slist_init_context(bucket, chtbl, entry_destroy);
        if (vector_push_back(&chtbl->table, bucket) != COLLECTION_SUCCESS) {
            slist_destroy(bucket);
            free(bucket);
            vector_destroy(&chtbl->table);
            return COLLECTION_FAILURE;
        }
    }
    return COLLECTION_SUCCESS;
}

/**
 * Resize the buckets of the chained hash table.
 * @param chtbl The chained hash table to resize the buckets of.
 * @param nbuckets The number of buckets to resize to.
 * @return 0 if successful, -1 if failed.
 * @complexity O(n)
 */
static int _resize(Chtbl* chtbl, size_t nbuckets){
    if (chtbl_buckets(chtbl) == nbuckets)
        return COLLECTION_SUCCESS;

    // create a new table
    Vector new_table;
    vector_init(&new_table, bucket_destroy);

    // initialize the new buckets
    for (size_t i = 0; i < nbuckets; i++){
        SList* bucket = malloc(sizeof(SList));
        if (!bucket) {
            vector_destroy(&new_table);
            return COLLECTION_FAILURE;
        }
        slist_init_context(bucket, chtbl, entry_destroy);
        if (vector_push_back(&new_table, bucket) != COLLECTION_SUCCESS) {
            slist_destroy(bucket);
            free(bucket);
            vector_destroy(&new_table);
            return COLLECTION_FAILURE;
        }
    }

    // rehash all entries from the old table to the new table
    for (size_t i = 0; i < chtbl_buckets(chtbl); i++){
        SList* old_bucket = (SList*)vector_at(&chtbl->table, i);
        if (!old_bucket) continue;

        SListNode* node = slist_front(old_bucket);
        while (node){
            ChtblEntry* entry = (ChtblEntry*)slist_data(node);

            uint32_t hash = chtbl->hash(entry->key);
            size_t index = INDEX(nbuckets, hash);

            SList* new_bucket = (SList*)vector_at(&new_table, index);

            if (slist_push_back(new_bucket, entry) != COLLECTION_SUCCESS) {
                // explicitly set the destroy function to NULL to 
                // avoid destroying the entries in old table
                new_table.destroy = NULL;
                vector_destroy(&new_table);
                return COLLECTION_FAILURE;
            }
            node = slist_next(node);
        }
        old_bucket->destroy = NULL;
        old_bucket->destroy_context = NULL;
    }

    // destroy the old table without destroying the entries
    vector_destroy(&chtbl->table);
    chtbl->table = new_table;
    return COLLECTION_SUCCESS;
}

bool chtbl_in(const Chtbl* chtbl, const void * key){
    return chtbl_find((Chtbl*)chtbl, key) != NULL;
}

size_t chtbl_size(const Chtbl* chtbl){
    return chtbl ? chtbl->size : 0;
}

size_t chtbl_buckets(const Chtbl* chtbl){
    return chtbl ? chtbl->table.size : 0;
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
    if (!chtbl || threshold <= 0.0f)
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

void chtbl_clear(Chtbl* chtbl){
    if (!chtbl) return;
    for (size_t i = 0; i < chtbl_buckets(chtbl); i++){
        SList* bucket = (SList*)vector_at(&chtbl->table, i);
        if (bucket){
            slist_clear(bucket);
        }
    }
    chtbl->size = 0;
}

void* chtbl_find(Chtbl* chtbl, const void * key){
    if (!chtbl || !key) return NULL;

    uint32_t hash = chtbl->hash(key);
    size_t index = INDEX(chtbl_buckets(chtbl), hash);

    SList* bucket = (SList*)vector_at(&chtbl->table, index);
    if (!bucket) return NULL;

    SListNode* node = slist_front(bucket);
    while (node){
        ChtblEntry* entry = (ChtblEntry*)slist_data(node);
        if (chtbl->match(entry->key, key)){
            return entry->value;
        }
        node = slist_next(node);
    }
    return NULL;
}

ChtblEntry* chtbl_find_entry(Chtbl* chtbl, const void * key){
    if (!chtbl || !key) return NULL;

    uint32_t hash = chtbl->hash(key);
    size_t index = INDEX(chtbl_buckets(chtbl), hash);

    SList* bucket = (SList*)vector_at(&chtbl->table, index);
    if (!bucket) return NULL;

    SListNode* node = slist_front(bucket);
    while (node){
        ChtblEntry* entry = (ChtblEntry*)slist_data(node);
        if (chtbl->match(entry->key, key)){
            return entry;
        }
        node = slist_next(node);
    }
    return NULL;
}

int chtbl_resize(Chtbl* chtbl, size_t nbuckets){
    if (!chtbl)
        return COLLECTION_FAILURE;

    if (nbuckets < MIN_BUCKETS)
        nbuckets = MIN_BUCKETS;

    return _resize(chtbl, nbuckets);
}

int chtbl_insert(Chtbl* chtbl, const void * key, const void * value){
    if (!chtbl || !key) return COLLECTION_FAILURE;

    uint32_t hash = chtbl->hash(key);
    size_t index = INDEX(chtbl_buckets(chtbl), hash);

    SList* bucket = (SList*)vector_at(&chtbl->table, index);
    if (!bucket) return COLLECTION_FAILURE;

    SListNode* node = slist_front(bucket);

    // check if the key already exists
    while (node) {
        ChtblEntry* entry = (ChtblEntry*)slist_data(node);
        if (chtbl->match(entry->key, key)){
            // update the value
            if (chtbl->destroy_value && entry->value) {
                chtbl->destroy_value(entry->value);
            }
            entry->value = (void*)value;
            
            return COLLECTION_SUCCESS;
        }
        node = slist_next(node);
    }

    // insert the new key-value pair
    ChtblEntry* new_entry = malloc(sizeof(ChtblEntry));
    if (!new_entry) return COLLECTION_FAILURE;
    new_entry->key = (void*)key;
    new_entry->value = (void*)value;
    if (slist_push_back(bucket, new_entry) != COLLECTION_SUCCESS) {
        free(new_entry);
        return COLLECTION_FAILURE;
    }

    ++chtbl->size;

    // check if the load factor is exceeded
    float load_factor = chtbl_load_factor(chtbl);
    if (load_factor > chtbl->load_factor_threshold) {
        return _resize(chtbl, chtbl_buckets(chtbl) * RESIZE_FACTOR);
    }
    return COLLECTION_SUCCESS;
}

int chtbl_remove(Chtbl* chtbl, const void * key, ChtblEntry** entry){
    if (!chtbl || !key) return COLLECTION_FAILURE;

    uint32_t hash = chtbl->hash(key);
    size_t index = INDEX(chtbl_buckets(chtbl), hash);

    SList* bucket = (SList*)vector_at(&chtbl->table, index);
    if (!bucket) return COLLECTION_FAILURE;

    SListNode* prev = NULL;
    SListNode* node = slist_front(bucket);

    if (node && chtbl->match(((ChtblEntry*)slist_data(node))->key, key)){
        slist_pop_front(bucket, (void**)entry);
        --chtbl->size;
        return COLLECTION_SUCCESS;
    }

    prev = node;
    node = slist_next(node);
    
    while (node) {
        if (chtbl->match(((ChtblEntry*)slist_data(node))->key, key)){
            slist_remove_after(bucket, prev, (void**)entry);
            --chtbl->size;
            return COLLECTION_SUCCESS;
        }
        prev = node;
        node = slist_next(node);
    }

    return COLLECTION_FAILURE;
}