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

int dlist_push_front(DList* dlist, void* data){
    if (!dlist)
        return COLLECTION_FAILURE;
    DListNode* node = malloc(sizeof(DListNode));
    if (!node)
        return COLLECTION_FAILURE;
    node->data = data;
    node->prev = NULL;
    node->next = dlist->head;
    if (dlist->head)
        dlist->head->prev = node;
    dlist->head = node;
    if (!dlist->tail)
        dlist->tail = node;
    dlist->size++;
    return COLLECTION_SUCCESS;
}

int dlist_push_back(DList* dlist, void* data){
    if (!dlist)
        return COLLECTION_FAILURE;
    DListNode* node = malloc(sizeof(DListNode));
    if (!node)
        return COLLECTION_FAILURE;
    node->data = data;
    node->prev = dlist->tail;
    node->next = NULL;
    if (dlist->tail)
        dlist->tail->next = node;
    dlist->tail = node;
    if (!dlist->head)
        dlist->head = node;
    dlist->size++;
    return COLLECTION_SUCCESS;
}

int dlist_pop_front(DList* dlist, void** data){
    if (!dlist || dlist_empty(dlist))
        return COLLECTION_FAILURE;
    DListNode* node = dlist->head;
    dlist->head = node->next;
    if (dlist->head){
        dlist->head->prev = NULL;
    }else{
        dlist->tail = NULL;
    }
    if (data){
        *data = node->data;
    }else if (dlist->destroy){
        dlist->destroy(node->data);
    }
    free(node);
    dlist->size--;
    return COLLECTION_SUCCESS;
}

int dlist_pop_back(DList* dlist, void** data){
    if (!dlist || dlist_empty(dlist))
        return COLLECTION_FAILURE;
    DListNode* node = dlist->tail;
    dlist->tail = node->prev;
    if (dlist->tail){
        dlist->tail->next = NULL;
    }else{
        dlist->head = NULL;
    }
    if (data){
        *data = node->data;
    }else if (dlist->destroy){
        dlist->destroy(node->data);
    }
    free(node);
    dlist->size--;
    return COLLECTION_SUCCESS;
}

int dlist_insert_after(DList* dlist, DListNode* node, void* data){
    if (!dlist || !node)
        return COLLECTION_FAILURE;
    DListNode* new_node = malloc(sizeof(DListNode));
    if (!new_node)
        return COLLECTION_FAILURE;

    new_node->data = data;
    new_node->prev = node;
    new_node->next = node->next;

    if (node->next){
        node->next->prev = new_node;
    }else{
        dlist->tail = new_node;
    }
    node->next = new_node;
    dlist->size++;
    return COLLECTION_SUCCESS;
}

int dlist_insert_before(DList* dlist, DListNode* node, void* data){
    if (!dlist || !node)
        return COLLECTION_FAILURE;
    DListNode* new_node = malloc(sizeof(DListNode));
    if (!new_node)
        return COLLECTION_FAILURE;

    new_node->data = data;
    new_node->prev = node->prev;
    new_node->next = node;
    if (node->prev){
        node->prev->next = new_node;
    }else{
        dlist->head = new_node;
    }
    node->prev = new_node;
    dlist->size++;
    return COLLECTION_SUCCESS;
}

int dlist_remove(DList* dlist, DListNode* node, void** data){
    if (!dlist || !node)
        return COLLECTION_FAILURE;
    DListNode* prev = node->prev;
    DListNode* next = node->next;

    if (prev){
        prev->next = next;
    }else{
        dlist->head = next;
    }
    if (next){
        next->prev = prev;
    }else{
        dlist->tail = prev;
    }
    if (data){
        *data = node->data;
    }else if (dlist->destroy){
        dlist->destroy(node->data);
    }
    free(node);
    dlist->size--;
    return COLLECTION_SUCCESS;
}