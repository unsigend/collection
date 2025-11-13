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

#ifndef COLLECTION_DLIST_H
#define COLLECTION_DLIST_H

/**
 * This file contains the interface for doubly linked list.
 */
#include <collection/common.h>
#include <stdbool.h>

// internal struct for DListNode
typedef struct DListNode {
    void* data;
    struct DListNode* prev;
    struct DListNode* next;
} DListNode;

// internal struct for DList
typedef struct {
    size_t size;
    DListNode* head;
    DListNode* tail;
    void (*destroy)(void *);
} DList;

/**
 * Initialize the doubly linked list.
 * @param dlist The doubly linked list to initialize.
 * @param destroy The function to destroy the elements.
 * @complexity O(1)
 */
extern void dlist_init(DList* dlist, void (*destroy)(void *));

/**
 * Destroy the doubly linked list.
 * @param dlist The doubly linked list to destroy.
 * @complexity O(n)
 */
extern void dlist_destroy(DList* dlist);

/**
 * Get the next node of a given node.
 * @param node The node to get the next node of.
 * @return The next node of the given node.
 * @complexity O(1)
 */
#define dlist_next(node) ((node) ? (node)->next : NULL)

/**
 * Get the previous node of a given node.
 * @param node The node to get the previous node of.
 * @return The previous node of the given node.
 * @complexity O(1)
 */
#define dlist_prev(node) ((node) ? (node)->prev : NULL)

/**
 * Get the data of a given node.
 * @param node The node to get the data of.
 * @return The data of the given node.
 * @complexity O(1)
 */
#define dlist_data(node) ((node) ? (node)->data : NULL)

/**
 * Get the head node of a given list.
 * @param dlist The list to get the head node of.
 * @return The head node of the given list.
 * @complexity O(1)
 */
#define dlist_head(dlist) ((dlist) ? (dlist)->head : NULL)

/**
 * Get the tail node of a given list.
 * @param dlist The list to get the tail node of.
 * @return The tail node of the given list.
 * @complexity O(1)
 */
#define dlist_tail(dlist) ((dlist) ? (dlist)->tail : NULL)

/**
 * Check if the doubly linked list is empty.
 * @param dlist The doubly linked list to check.
 * @return True if the doubly linked list is empty, false otherwise.
 * @complexity O(1)
 */
#define dlist_empty(dlist) ((dlist) ? (dlist)->size == 0 : true)

/**
 * Get the size of the doubly linked list.
 * @param dlist The doubly linked list to get the size of.
 * @return The size of the doubly linked list.
 * @complexity O(1)
 */
#define dlist_size(dlist) ((dlist) ? (dlist)->size : 0)

/**
 * Insert an element after a given node.
 * @param dlist The doubly linked list to insert the element into.
 * @param node The node to insert the element after.
 * @param data The element to insert.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int dlist_insert_after(DList* dlist, DListNode* node, void* data);

/**
 * Insert an element before a given node.
 * @param dlist The doubly linked list to insert the element into.
 * @param node The node to insert the element before.
 * @param data The element to insert.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int dlist_insert_before(DList* dlist, DListNode* node, void* data);

/**
 * Remove an element from the doubly linked list.
 * @param dlist The doubly linked list to remove the element from.
 * @param node The node to remove.
 * @param data The pointer to store the removed element.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int dlist_remove(DList* dlist, DListNode* node, void** data);

/**
 * Pop an element from the front of the doubly linked list.
 * @param dlist The doubly linked list to pop the element from.
 * @param data The pointer to store the popped element.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int dlist_pop_front(DList* dlist, void** data);

/**
 * Pop an element from the back of the doubly linked list.
 * @param dlist The doubly linked list to pop the element from.
 * @param data The pointer to store the popped element.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int dlist_pop_back(DList* dlist, void** data);

/**
 * Push an element to the front of the doubly linked list.
 * @param dlist The doubly linked list to push the element to.
 * @param data The element to push.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int dlist_push_front(DList* dlist, void* data);

/**
 * Push an element to the back of the doubly linked list.
 * @param dlist The doubly linked list to push the element to.
 * @param data The element to push.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int dlist_push_back(DList* dlist, void* data);

/**
 * Clear the doubly linked list.
 * @param dlist The doubly linked list to clear.
 * @complexity O(n)
 */
extern void dlist_clear(DList* dlist);

#endif