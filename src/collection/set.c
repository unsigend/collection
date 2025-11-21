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

#include <collection/common.h>
#include <collection/slist.h>
#include <collection/set.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int set_init(Set* set, uint32_t (*hash)(const void * key), 
    bool (*match)(const void * key1, const void * key2), 
    void (*destroy)(void *)){
    if (!set)
        return COLLECTION_FAILURE;
    memset(set, 0, sizeof(Set));
    return chtbl_init(&set->chtbl, hash, match, destroy, NULL);
}

void set_destroy(Set* set){
    if (!set)
        return;
    chtbl_destroy(&set->chtbl);
    memset(set, 0, sizeof(Set));
}

bool set_empty(const Set* set){
    if (!set)
        return true;
    return chtbl_size(&set->chtbl) == 0;
}

size_t set_size(const Set* set){
    if (!set)
        return 0;
    return chtbl_size(&set->chtbl);
}

int set_insert(Set* set, const void * data){
    if (!set || !data)
        return COLLECTION_FAILURE;
    return chtbl_insert(&set->chtbl, data, NULL);
}

int set_remove(Set* set, const void * data){
    if (!set || !data)
        return COLLECTION_FAILURE;
    return chtbl_remove(&set->chtbl, data, NULL);
}

void set_clear(Set* set){
    if (!set)
        return;
    chtbl_clear(&set->chtbl);
}

bool set_contains(const Set* set, const void * data){
    if (!set || !data)
        return false;
    return chtbl_in(&set->chtbl, data);
}

int set_intersection(Set* result, const Set* set1, const Set* set2){
    if (!result || !set1 || !set2)
        return COLLECTION_FAILURE;
    if (!set_empty(result))
        set_clear(result);
    result->chtbl.destroy_key = NULL;
    result->chtbl.hash = set1->chtbl.hash;
    result->chtbl.match = set1->chtbl.match;

    // optimize the intersection operation with the smaller set
    const Set* smaller = set1;
    const Set* larger = set2;
    if (set_size(set1) > set_size(set2)){
        smaller = set2;
        larger = set1;
    }

    // iterate through the smaller set
    for (size_t i = 0; i < chtbl_buckets(&smaller->chtbl); i++){
        SList* bucket = (SList*)vector_at(&smaller->chtbl.table, i);
        if (!bucket) continue;
        SListNode* node = slist_front(bucket);
        while (node) {
            ChtblEntry* entry = (ChtblEntry*)slist_data(node);
            if (set_contains(larger, entry->key)){
                set_insert(result, entry->key);
            }
            node = slist_next(node);
        }
    }
    
    return COLLECTION_SUCCESS;
}

int set_union(Set* result, const Set* set1, const Set* set2){
    if (!result || !set1 || !set2)
        return COLLECTION_FAILURE;
    if (!set_empty(result))
        set_clear(result);
    result->chtbl.destroy_key = NULL;
    result->chtbl.hash = set1->chtbl.hash;
    result->chtbl.match = set1->chtbl.match;

    for (size_t i = 0; i < chtbl_buckets(&set1->chtbl); i++){
        SList* bucket = (SList*)vector_at(&set1->chtbl.table, i);
        if (!bucket) continue;
        SListNode* node = slist_front(bucket);
        while (node) {
            ChtblEntry* entry = (ChtblEntry*)slist_data(node);
            set_insert(result, entry->key);
            node = slist_next(node);
        }
    }

    for (size_t i = 0; i < chtbl_buckets(&set2->chtbl); i++){
        SList* bucket = (SList*)vector_at(&set2->chtbl.table, i);
        if (!bucket) continue;
        SListNode* node = slist_front(bucket);
        while (node) {
            ChtblEntry* entry = (ChtblEntry*)slist_data(node);
            set_insert(result, entry->key);
            node = slist_next(node);
        }
    }

    return COLLECTION_SUCCESS;
}

int set_difference(Set* result, const Set* set1, const Set* set2){
    if (!result || !set1 || !set2)
        return COLLECTION_FAILURE;
    if (!set_empty(result))
        set_clear(result);
    result->chtbl.destroy_key = NULL;
    result->chtbl.hash = set1->chtbl.hash;
    result->chtbl.match = set1->chtbl.match;

    // iterate through the first set
    for (size_t i = 0; i < chtbl_buckets(&set1->chtbl); i++){
        SList* bucket = (SList*)vector_at(&set1->chtbl.table, i);
        if (!bucket) continue;
        SListNode* node = slist_front(bucket);
        while (node) {
            ChtblEntry* entry = (ChtblEntry*)slist_data(node);
            if (!set_contains(set2, entry->key)){
                set_insert(result, entry->key);
            }
            node = slist_next(node);
        }
    }

    return COLLECTION_SUCCESS;
}

bool set_equal(const Set* set1, const Set* set2){
    if (set1 == set2 && set1)
        return true;
    if (!set1 || !set2)
        return false;

    return set_size(set1) == set_size(set2) && set_subset(set1, set2);
}

bool set_subset(const Set* set1, const Set* set2){
    if (!set1 || !set2)
        return false;

    if (set_size(set1) > set_size(set2))
        return false;

    for (size_t i = 0; i < chtbl_buckets(&set1->chtbl); i++){
        SList* bucket = (SList*)vector_at(&set1->chtbl.table, i);
        if (!bucket) continue;
        SListNode* node = slist_front(bucket);
        while (node) {
            ChtblEntry* entry = (ChtblEntry*)slist_data(node);
            if (!set_contains(set2, entry->key))
                return false;
            node = slist_next(node);
        }
    }

    return true;
}