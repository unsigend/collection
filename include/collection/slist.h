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

#ifndef COLLECTION_SLIST_H
#define COLLECTION_SLIST_H

/**
 * This file contains the interface for singly linked list.
 */
#include <collection/common.h>
#include <stdbool.h>

// internal struct for SListNode
typedef struct SListNode {
    void* data;
    struct SListNode* next;
} SListNode;

// internal struct for SList
typedef struct {
    size_t size;
    SListNode* head;
    SListNode* tail;
    void (*destroy)(void *);
} SList;

/**
 * Initialize the singly linked list.
 * @param slist The singly linked list to initialize.
 * @param destroy The function to destroy the elements.
 * @complexity O(1)
 */
extern void slist_init(SList* slist, void (*destroy)(void *));

/**
 * Destroy the singly linked list.
 * @param slist The singly linked list to destroy.
 * @complexity O(n)
 */
extern void slist_destroy(SList* slist);

/**
 * Check if the singly linked list is empty.
 * @param slist The singly linked list to check.
 * @return True if the singly linked list is empty, false otherwise.
 * @complexity O(1)
 */
extern bool slist_empty(SList* slist);

/**
 * Get the size of the singly linked list.
 * @param slist The singly linked list to get the size of.
 * @return The size of the singly linked list.
 * @complexity O(1)
 */
extern size_t slist_size(SList* slist);

/**
 * Get the first element of the singly linked list.
 * @param slist The singly linked list to get the first element of.
 * @return The first element of the singly linked list.
 * @complexity O(1)
 */
extern SListNode* slist_front(SList* slist);

/**
 * Get the last element of the singly linked list.
 * @param slist The singly linked list to get the last element of.
 * @return The last element of the singly linked list.
 * @complexity O(1)
 */
extern SListNode* slist_back(SList* slist);

/**
 * Push an element to the front of the singly linked list.
 * @param slist The singly linked list to push the element to.
 * @param data The element to push.
 * @complexity O(1)
 */
extern void slist_push_front(SList* slist, void* data);

/**
 * Push an element to the back of the singly linked list.
 * @param slist The singly linked list to push the element to.
 * @param data The element to push.
 * @complexity O(1)
 */
extern void slist_push_back(SList* slist, void* data);

/**
 * Pop an element from the front of the singly linked list.
 * @param slist The singly linked list to pop the element from.
 * @param data The pointer to store the popped element.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int slist_pop_front(SList* slist, void** data);

/**
 * Pop an element from the back of the singly linked list.
 * @param slist The singly linked list to pop the element from.
 * @param data The pointer to store the popped element.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int slist_pop_back(SList* slist, void** data);

/**
 * Insert an element after a given node.
 * @param slist The singly linked list to insert the element into.
 * @param node The node to insert the element after.
 * @param data The element to insert.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int slist_insert_after(SList* slist, SListNode* node, void* data);

/**
 * Remove the element after a given node and return it.
 * @param slist The singly linked list to remove the element from.
 * @param node The node to remove the element after.
 * @param data The pointer to store the removed element.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int slist_remove_after(SList* slist, SListNode* node, void** data);

/**
 * Get the next node of a given node.
 * @param node The node to get the next node of.
 * @return The next node of the given node.
 * @complexity O(1)
 */
extern SListNode* slist_next(SListNode* node);

/**
 * Get the data of a given node.
 * @param node The node to get the data of.
 * @return The data of the given node.
 * @complexity O(1)
 */
extern void* slist_data(SListNode* node);

/**
 * Get the head node of a given list.
 * @param slist The list to get the head node of.
 * @return The head node of the given list.
 * @complexity O(1)
 */
extern SListNode* slist_head(SList* slist);

/**
 * Get the tail node of a given list.
 * @param slist The list to get the tail node of.
 * @return The tail node of the given list.
 * @complexity O(1)
 */
extern SListNode* slist_tail(SList* slist);


#endif