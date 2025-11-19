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

#ifndef COLLECTION_CLIST_H
#define COLLECTION_CLIST_H

/**
 * This file contains the interface for circular linked list.
 */
#include <collection/common.h>
#include <stdbool.h>

// internal struct for CListNode
typedef struct CListNode {
    void* data;
    struct CListNode* next;
    struct CListNode* prev;
} CListNode;

// internal struct for CList
typedef struct {
    size_t size;
    CListNode* head;
    void (*destroy)(void *);
} CList;

/**
 * Initialize the circular linked list.
 * @param clist The circular linked list to initialize.
 * @param destroy The function to destroy the elements.
 * @complexity O(1)
 */
extern void clist_init(CList* clist, void (*destroy)(void *));

/**
 * Destroy the circular linked list.
 * @param clist The circular linked list to destroy.
 * @complexity O(n)
 */
extern void clist_destroy(CList* clist);

/**
 * Get the next node of a given node.
 * @param node The node to get the next node of.
 * @return The next node of the given node.
 * @complexity O(1)
 */
extern CListNode* clist_next(const CListNode* node);

/**
 * Get the previous node of a given node.
 * @param node The node to get the previous node of.
 * @return The previous node of the given node.
 * @complexity O(1)
 */
extern CListNode* clist_prev(const CListNode* node);

/**
 * Get the data of a given node.
 * @param node The node to get the data of.
 * @return The data of the given node.
 * @complexity O(1)
 */
extern void* clist_data(const CListNode* node);

/**
 * Get the head node of a given list.
 * @param clist The list to get the head node of.
 * @return The head node of the given list.
 * @complexity O(1)
 */
extern CListNode* clist_head(const CList* clist);

/**
 * Get the tail node of a given list.
 * @param clist The list to get the tail node of.
 * @return The tail node of the given list.
 * @complexity O(1)
 */
extern CListNode* clist_tail(const CList* clist);

/**
 * Check if the circular linked list is empty.
 * @param clist The circular linked list to check.
 * @return True if the circular linked list is empty, false otherwise.
 * @complexity O(1)
 */
extern bool clist_empty(const CList* clist);

/**
 * Get the size of the circular linked list.
 * @param clist The circular linked list to get the size of.
 * @return The size of the circular linked list.
 * @complexity O(1)
 */
extern size_t clist_size(const CList* clist);

/**
 * Insert an element after a given node.
 * @param clist The circular linked list to insert the element into.
 * @param node The node to insert the element after.
 * @param data The element to insert.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int clist_insert_after(CList* clist, CListNode* node, void* data);

/**
 * Insert an element before a given node.
 * @param clist The circular linked list to insert the element into.
 * @param node The node to insert the element before.
 * @param data The element to insert.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int clist_insert_before(CList* clist, CListNode* node, void* data);

/**
 * Remove an element from the circular linked list.
 * @param clist The circular linked list to remove the element from.
 * @param node The node to remove.
 * @param data The pointer to store the removed element.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int clist_remove(CList* clist, CListNode* node, void** data);

/**
 * Pop an element from the front of the circular linked list.
 * @param clist The circular linked list to pop the element from.
 * @param data The pointer to store the popped element.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int clist_pop_front(CList* clist, void** data);

/**
 * Pop an element from the back of the circular linked list.
 * @param clist The circular linked list to pop the element from.
 * @param data The pointer to store the popped element.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int clist_pop_back(CList* clist, void** data);

/**
 * Push an element to the front of the circular linked list.
 * @param clist The circular linked list to push the element to.
 * @param data The element to push.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int clist_push_front(CList* clist, void* data);

/**
 * Push an element to the back of the circular linked list.
 * @param clist The circular linked list to push the element to.
 * @param data The element to push.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int clist_push_back(CList* clist, void* data);

/**
 * Clear the circular linked list.
 * @param clist The circular linked list to clear.
 * @complexity O(n)
 */
extern void clist_clear(CList* clist);

#endif