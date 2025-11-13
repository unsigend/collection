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
#include <collection/slist.h>


void slist_init(SList* slist, void (*destroy)(void *)){
    slist->size = 0;
    slist->head = NULL;
    slist->tail = NULL;
    slist->destroy = destroy;
}

void slist_destroy(SList* slist){
    SListNode* node = slist->head;
    while (node){
        SListNode* next = node->next;
        if (slist->destroy){
            slist->destroy(node->data);
        }
        free(node);
        node = next;
    }
    
    slist->size = 0;
    slist->head = NULL;
    slist->tail = NULL;
    slist->destroy = NULL;
}

void slist_push_front(SList* slist, void* data){
    SListNode* node = malloc(sizeof(SListNode));
    if (!node)
        return;
    node->data = data;
    node->next = slist->head;
    slist->head = node;
    if (slist->tail == NULL){
        slist->tail = node;
    }
    slist->size++;
}

void slist_push_back(SList* slist, void* data){
    SListNode* node = malloc(sizeof(SListNode));
    if (!node)
        return;
    node->data = data;
    node->next = NULL;
    if (slist->head == NULL){
        slist->head = node;
    }else {
        slist->tail->next = node;
    }
    slist->tail = node;
    slist->size++;
}

int slist_pop_front(SList* slist, void** data){
    if (slist->size == 0){
        return COLLECTION_FAILURE;
    }
    SListNode* node = slist->head;
    slist->head = node->next;
    if (slist->head == NULL){
        slist->tail = NULL;
    }
    if (data){
        *data = node->data;
    }else if (slist->destroy){
        slist->destroy(node->data);
    }
    free(node);
    slist->size--;
    return COLLECTION_SUCCESS;
}

int slist_insert_after(SList* slist, SListNode* node, void* data){
    if (node == NULL){
        return COLLECTION_FAILURE;
    }
    SListNode* new_node = malloc(sizeof(SListNode));
    if (!new_node)
        return COLLECTION_FAILURE;
    new_node->data = data;
    new_node->next = node->next;

    node->next = new_node;
    if (slist->tail == node){
        slist->tail = new_node;
    }
    slist->size++;
    return COLLECTION_SUCCESS;
}

int slist_remove_after(SList* slist, SListNode* node, void** data){
    if (!node || !node->next){
        return COLLECTION_FAILURE;
    }
    SListNode* removed_node = node->next;
    node->next = removed_node->next;
    if (slist->tail == removed_node){
        slist->tail = node;
    }
    if (data){
        *data = removed_node->data;
    }else if (slist->destroy){
        slist->destroy(removed_node->data);
    }
    free(removed_node);
    slist->size--;
    return COLLECTION_SUCCESS;
}