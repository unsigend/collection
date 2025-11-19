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
#include <collection/clist.h>

CListNode* clist_next(const CListNode* node){
    return node ? node->next : NULL;
}

CListNode* clist_prev(const CListNode* node){
    return node ? node->prev : NULL;
}

void* clist_data(const CListNode* node){
    return node ? node->data : NULL;
}

CListNode* clist_head(const CList* clist){
    return clist ? clist->head : NULL;
}

CListNode* clist_tail(const CList* clist){
    return (clist && clist->head) ? clist->head->prev : NULL;
}

bool clist_empty(const CList* clist){
    return clist ? clist->size == 0 : false;
}

size_t clist_size(const CList* clist){
    return clist ? clist->size : 0;
}

void clist_init(CList* clist, void (*destroy)(void *)){
    clist->size = 0;
    clist->head = NULL;
    clist->destroy = destroy;
}

void clist_destroy(CList* clist){
    clist_clear(clist);
    clist->destroy = NULL;
}

void clist_clear(CList* clist){
    if (!clist || clist_empty(clist)){
        return;
    }
    CListNode* node = clist->head;
    for (size_t i = 0; i < clist->size; i++){
        CListNode* next = node->next;
        if (clist->destroy){
            clist->destroy(node->data);
        }
        free(node);
        node = next;
    }
    clist->size = 0;
    clist->head = NULL;
}

int clist_push_front(CList* clist, void* data){
    if (!clist)
        return COLLECTION_FAILURE;
    CListNode* node = malloc(sizeof(CListNode));
    if (!node)
        return COLLECTION_FAILURE;
    node->data = data;
    
    CListNode* head = clist_head(clist);

    if (head) {
        CListNode* tail = head->prev;
        node->next = head;
        node->prev = tail;
        head->prev = node;
        tail->next = node;
    }else {
        node->next = node;
        node->prev = node;
    }

    clist->head = node;
    clist->size++;
    return COLLECTION_SUCCESS;
}

int clist_push_back(CList* clist, void* data){
    if (!clist)
        return COLLECTION_FAILURE;
    CListNode* node = malloc(sizeof(CListNode));
    if (!node)
        return COLLECTION_FAILURE;
    node->data = data;

    CListNode* head = clist_head(clist);
    if (head) {
        CListNode* tail = head->prev;
        node->next = head;
        node->prev = tail;
        head->prev = node;
        tail->next = node;
    }else {
        node->next = node;
        node->prev = node;
        clist->head = node;
    }
    clist->size++;
    return COLLECTION_SUCCESS;
}

int clist_pop_front(CList* clist, void** data){
    if (!clist || clist_empty(clist))
        return COLLECTION_FAILURE;

    if (clist_size(clist) == 1) {
        if (data) {
            *data = clist_data(clist->head);
        }else if (clist->destroy) {
            clist->destroy(clist_data(clist->head));
        }
        free(clist->head);
        clist->head = NULL;
        clist->size = 0;
        return COLLECTION_SUCCESS;
    }

    CListNode* head = clist_head(clist);
    CListNode* tail = clist_tail(clist);

    clist->head = head->next;
    head->next->prev = tail;
    tail->next = head->next;
    if (data) {
        *data = head->data;
    }else if (clist->destroy) {
        clist->destroy(head->data);
    }
    free(head);
    clist->size--;
    return COLLECTION_SUCCESS;
}

int clist_pop_back(CList* clist, void** data){
    if (!clist || clist_empty(clist))
        return COLLECTION_FAILURE;

    if (clist_size(clist) == 1) {
        if (data) {
            *data = clist_data(clist->head);
        }else if (clist->destroy) {
            clist->destroy(clist_data(clist->head));
        }
        free(clist->head);
        clist->head = NULL;
        clist->size = 0;
        return COLLECTION_SUCCESS;
    }

    CListNode* head = clist_head(clist);
    CListNode* tail = clist_tail(clist);

    tail->prev->next = head;
    head->prev = tail->prev;
    if (data) {
        *data = tail->data;
    }else if (clist->destroy) {
        clist->destroy(tail->data);
    }
    free(tail);
    clist->size--;
    return COLLECTION_SUCCESS;
}

int clist_insert_after(CList* clist, CListNode* node, void* data){
    if (!clist || !node)
        return COLLECTION_FAILURE;
    
    CListNode* new_node = malloc(sizeof(CListNode));
    if (!new_node)
        return COLLECTION_FAILURE;

    new_node->data = data;
    new_node->next = node->next;
    new_node->prev = node;

    node->next->prev = new_node;
    node->next = new_node;
    clist->size++;
    return COLLECTION_SUCCESS;
}

int clist_insert_before(CList* clist, CListNode* node, void* data){
    if (!clist || !node)
        return COLLECTION_FAILURE;
    
    CListNode* new_node = malloc(sizeof(CListNode));
    if (!new_node)
        return COLLECTION_FAILURE;

    new_node->data = data;
    new_node->next = node;
    new_node->prev = node->prev;

    node->prev->next = new_node;
    node->prev = new_node;
    clist->size++;

    if (node == clist_head(clist)){
        clist->head = new_node;
    }
    return COLLECTION_SUCCESS;
}

int clist_remove(CList* clist, CListNode* node, void** data){
    if (!clist || !node)
        return COLLECTION_FAILURE;

    if (clist_size(clist) == 1) {
        return clist_pop_back(clist, data);
    }

    CListNode* prev = node->prev;
    CListNode* next = node->next;

    prev->next = next;
    next->prev = prev;

    if (node == clist_head(clist)){
        clist->head = next;
    }
    if (data) {
        *data = node->data;
    }else if (clist->destroy) {
        clist->destroy(node->data);
    }
    free(node);
    clist->size--;
    return COLLECTION_SUCCESS;
}
