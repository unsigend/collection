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

#ifndef COLLECTION_VECTOR_H
#define COLLECTION_VECTOR_H

/**
 * This file contains the interface for dynamic arrays.
 * API reference and inspired by C++ std::vector.
 */
#include <collection/common.h>
#include <stdbool.h>

// internal struct for Vector
typedef struct {
    void **data;
    size_t size;
    size_t capacity;
    void (*destroy)(void *);
} Vector;

/**
 * Initialize the vector.
 * @param vector The vector to initialize.
 * @param destroy The function to destroy the elements.
 * @complexity O(1)
 */
extern void vector_init(Vector* vector, void (*destroy)(void *));

/**
 * Destroy the vector.
 * @param vector The vector to destroy.
 * @complexity O(n)
 */
extern void vector_destroy(Vector* vector);

/**
 * Check if the vector is empty.
 * @param vector The vector to check.
 * @return True if the vector is empty, false otherwise.
 * @complexity O(1)
 */
#define vector_empty(vector) ((vector)->size == 0)

/**
 * Get the data of the vector.
 * @param vector The vector to get the data from.
 * @return The data of the vector.
 * @complexity O(1)
 */
#define vector_data(vector) ((vector)->data)

/**
 * Get the size of the vector.
 * @param vector The vector to get the size of.
 * @return The size of the vector.
 * @complexity O(1)
 */
#define vector_size(vector) ((vector)->size)

/**
 * Get the capacity of the vector.
 * @param vector The vector to get the capacity of.
 * @return The capacity of the vector.
 * @complexity O(1)
 */
#define vector_capacity(vector) ((vector)->capacity)

/**
 * Get the element at the given index.
 * @param vector The vector to get the element from.
 * @param index The index of the element to get.
 * @return The element at the given index.
 * @complexity O(1)
 */
extern void* vector_at(Vector* vector, size_t index);

/**
 * Get the last element of the vector.
 * @param vector The vector to get the last element from.
 * @return The last element of the vector, or NULL if the vector is empty.
 * @complexity O(1)
 */
extern void* vector_back(Vector* vector);

/**
 * Get the first element of the vector.
 * @param vector The vector to get the first element from.
 * @return The first element of the vector, or NULL if the vector is empty.
 * @complexity O(1)
 */
extern void* vector_front(Vector* vector);

/**
 * Resize the vector to the given size.
 * @param vector The vector to resize.
 * @param new_size The new size of the vector.
 * @return 0 if successful, -1 if failed
 * @complexity O(n)
 */
extern int vector_resize(Vector* vector, size_t new_size);

/**
 * Shrink the vector to the size of the elements.
 * @param vector The vector to shrink.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int vector_shrink_to_fit(Vector* vector);

/**
 * Push an element to the back of the vector.
 * @param vector The vector to push the element to.
 * @param element The element to push.
 * @return 0 if successful, -1 if failed
 * @complexity O(1) amortized, O(n) worst case when reallocation occurs
 */
extern int vector_push_back(Vector* vector, void* element);

/**
 * Pop an element from the back of the vector.
 * @param vector The vector to pop the element from.
 * @param data The pointer to store the popped element.
 * @return 0 if successful, -1 if failed
 * @complexity O(1)
 */
extern int vector_pop_back(Vector* vector, void** data);

/**
 * Insert an element at the given index.
 * @param vector The vector to insert the element into.
 * @param index The index of the element to insert.
 * @param element The element to insert.
 * @return 0 if successful, -1 if failed
 * @complexity O(n)
 */
extern int vector_insert(Vector* vector, size_t index, void* element);

/**
 * Remove an element from the vector at the given index.
 * @param vector The vector to remove the element from.
 * @param index The index of the element to remove.
 * @param data The pointer to store the removed element.
 * @return 0 if successful, -1 if failed
 * @complexity O(n)
 */
extern int vector_remove(Vector* vector, size_t index, void** data);

/**
 * Remove all elements from the vector.
 * @param vector The vector to remove the elements from.
 * @complexity O(n)
 */
extern void vector_clear(Vector* vector);

#endif