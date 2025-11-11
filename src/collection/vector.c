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
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <collection/vector.h>

#define VECTOR_RESIZE_FACTOR 2

// @brief: destroy the elements in the range [index, index + count)
static void _vector_destroy_range(Vector* vector, size_t index, size_t count){
    if (vector->destroy) {
        for (size_t i = index; i < index + count; i++) {
            vector->destroy(vector->data[i]);
        }
    }
}

// @brief: resize the capacity of the vector
static void _vector_resize_capacity(Vector* vector, size_t new_capacity){
    if (!new_capacity){
        free(vector->data);
        vector->data = NULL;
        vector->capacity = 0;
        return;
    }
    // allocate or deallocate memory for the vector data
    void **new_data = realloc(vector->data, new_capacity * sizeof(void *));
    if (new_data == NULL){
        fprintf(stderr, "FATAL: Failed to resize vector\n");
        exit(COLLECTION_FAILURE);
    }
    vector->data = new_data;
    vector->capacity = new_capacity;
}

void vector_shrink_to_fit(Vector* vector){
    _vector_resize_capacity(vector, vector->size);
}

void vector_resize(Vector* vector, size_t new_size){
    if (new_size == vector->size) {
        return;
    }
    // expand the capacity
    if (new_size > vector->capacity) {
        _vector_resize_capacity(vector, new_size);
        // fill the new elements with NULL
        memset(vector->data + vector->size, 0, (new_size - vector->size) * sizeof(void *));
    }
    // shrink the size
    else if (new_size < vector->size) {
        // destroy the elements in the range [new_size, vector->size)
        _vector_destroy_range(vector, new_size, vector->size - new_size);
        memset(vector->data + new_size, 0, (vector->size - new_size) * sizeof(void *));
    }
    // expand the size
    else if (new_size > vector->size) {
        // fill the new elements with NULL
        memset(vector->data + vector->size, 0, (new_size - vector->size) * sizeof(void *));
    }
    vector->size = new_size;
}

bool vector_empty(Vector* vector){
    return vector->size == 0;
}

void** vector_data(Vector* vector){
    return vector->data;
}

void vector_init(Vector* vector, void (*destroy)(void *)){
    // allocate memory for the vector stuct
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
    vector->destroy = destroy;
}


void vector_destroy(Vector* vector){
    if (vector->data){
        // destroy the elements if the destroy function is provided
        _vector_destroy_range(vector, 0, vector->size);

        free(vector->data);
        vector->data = NULL;
    }
    vector->size = 0;
    vector->capacity = 0;
    vector->destroy = NULL;
}

size_t vector_size(Vector* vector){
    return vector->size;
}

size_t vector_capacity(Vector* vector){
    return vector->capacity;
}

void* vector_at(Vector* vector, size_t index){
    // check if the index is out of bounds
    if (index >= vector->size){
        fprintf(stderr, "FATAL: Index out of bounds\n");
        exit(COLLECTION_FAILURE);
    }
    return vector->data[index];
}

void* vector_back(Vector* vector){
    if (vector->size == 0){
        return NULL;
    }
    return vector->data[vector->size - 1];
}

void* vector_front(Vector* vector){
    if (vector->size == 0){
        return NULL;
    }
    return vector->data[0];
}

void vector_push_back(Vector* vector, void* element){
    if (vector->size >= vector->capacity) {
        const size_t new_capacity = vector->capacity ? 
        vector->capacity * VECTOR_RESIZE_FACTOR : 1;
        _vector_resize_capacity(vector, new_capacity);
    }
    vector->data[vector->size++] = element;
}

void vector_pop_back(Vector* vector){
    if (vector->size == 0){
        return;
    }
    vector->size--;
    if (vector->destroy) {
        vector->destroy(vector->data[vector->size]);
    }
    vector->data[vector->size] = NULL;
}

void vector_clear(Vector* vector){
    if (vector->destroy) {
        _vector_destroy_range(vector, 0, vector->size);
    }
    memset(vector->data, 0, vector->size * sizeof(void *));
    vector->size = 0;
}

void vector_insert(Vector* vector, size_t index, void* element){
    if (index > vector->size){
        fprintf(stderr, "FATAL: Index out of bounds\n");
        exit(COLLECTION_FAILURE);
    }
    if (index == vector->size){
        vector_push_back(vector, element);
        return;
    }
    // expand the capacity
    if (vector->size >= vector->capacity){
        const size_t new_capacity = vector->capacity ? 
        vector->capacity * VECTOR_RESIZE_FACTOR : 1;
        _vector_resize_capacity(vector, new_capacity);
    }
    // shift the elements to the right
    memmove(vector->data + index + 1, vector->data + index, (vector->size - index) * sizeof(void *));
    vector->data[index] = element;
    vector->size++;
}

void vector_remove(Vector* vector, size_t index){
    if (index >= vector->size){
        fprintf(stderr, "FATAL: Index out of bounds\n");
        exit(COLLECTION_FAILURE);
    }
    if (vector->destroy) {
        vector->destroy(vector->data[index]);
    }
    // shift the elements to the left
    memmove(vector->data + index, vector->data + index + 1, (vector->size - index - 1) * sizeof(void *));
    vector->size--;
    vector->data[vector->size] = NULL;
}