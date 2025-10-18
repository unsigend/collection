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

#endif