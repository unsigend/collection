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

#include <stdlib.h>
#include <collection/dlist.h>

void dlist_init(DList* dlist, void (*destroy)(void *)){
    dlist->size = 0;
    dlist->head = NULL;
    dlist->tail = NULL;
    dlist->destroy = destroy;
}

void dlist_destroy(DList* dlist){
    dlist_clear(dlist);
    dlist->destroy = NULL;
}

void dlist_clear(DList* dlist){
    if (!dlist || dlist_empty(dlist)){
        return;
    }
    DListNode* node = dlist->head;
    while (node){
        DListNode* next = node->next;
        if (dlist->destroy){
            dlist->destroy(node->data);
        }
        free(node);
        node = next;
    }
    dlist->size = 0;
    dlist->head = NULL;
    dlist->tail = NULL;
}
