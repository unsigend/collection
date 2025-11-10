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
 */
extern void vector_init(Vector* vector, void (*destroy)(void *));

/**
 * Destroy the vector.
 * @param vector The vector to destroy.
 */
extern void vector_destroy(Vector* vector);


/**
 * Get the size of the vector.
 * @param vector The vector to get the size of.
 * @return The size of the vector.
 */
extern size_t vector_size(Vector* vector);

/**
 * Get the capacity of the vector.
 * @param vector The vector to get the capacity of.
 * @return The capacity of the vector.
 */
extern size_t vector_capacity(Vector* vector);

/**
 * Get the element at the given index.
 * @param vector The vector to get the element from.
 * @param index The index of the element to get.
 * @return The element at the given index.
 */
extern void* vector_at(Vector* vector, size_t index);

/**
 * Get the last element of the vector.
 * @param vector The vector to get the last element from.
 * @return The last element of the vector, or NULL if the vector is empty.
 */
extern void* vector_back(Vector* vector);

/**
 * Get the first element of the vector.
 * @param vector The vector to get the first element from.
 * @return The first element of the vector, or NULL if the vector is empty.
 */
extern void* vector_front(Vector* vector);

/**
 * Resize the vector to the given size.
 * @param vector The vector to resize.
 * @param new_size The new size of the vector.
 */
extern void vector_resize(Vector* vector, size_t new_size);

/**
 * Shrink the vector to the size of the elements.
 * @param vector The vector to shrink.
 */
extern void vector_shrink_to_fit(Vector* vector);

/**
 * Push an element to the back of the vector.
 * @param vector The vector to push the element to.
 * @param element The element to push.
 */
extern void vector_push_back(Vector* vector, void* element);

/**
 * Pop an element from the back of the vector.
 * @param vector The vector to pop the element from.
 */
extern void vector_pop_back(Vector* vector);

#endif