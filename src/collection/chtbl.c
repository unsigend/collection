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

#include <collection/chtbl.h>
#include <collection/slist.h>
#include <string.h>
#include <stdlib.h>

static void _destroy_bucket(void *data){
    slist_destroy((SList*)data);
    free(data);
}

int chtbl_init(Chtbl *htbl, size_t nbuckets, int (*hash)(const void *key),
    int (*match)(const void *key1, const void *key2), 
    void (*destroy)(void *data))
{
    if (!htbl || !nbuckets)
        return COLLECTION_FAILURE;

    memset(htbl, 0, sizeof(Chtbl));

    vector_init(&htbl->table, _destroy_bucket);
    for (size_t i = 0; i < nbuckets; i++){
        SList* slist = malloc(sizeof(SList));
        if (!slist){
            vector_destroy(&htbl->table);
            return COLLECTION_FAILURE;
        }
        slist_init(slist, destroy);
        if (vector_push_back(&htbl->table, slist) != COLLECTION_SUCCESS){
            vector_destroy(&htbl->table);
            free(slist);
            return COLLECTION_FAILURE;
        }
    }

    htbl->hash = hash;
    htbl->match = match;
    htbl->destroy = destroy;
    return COLLECTION_SUCCESS;
}

void chtbl_destroy(Chtbl *htbl){
    if (!htbl) return;
    vector_destroy(&htbl->table);
    htbl->hash = NULL;
    htbl->match = NULL;
    htbl->destroy = NULL;
    htbl->size = 0;
}