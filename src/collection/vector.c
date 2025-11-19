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
#include <stdbool.h>
#include <stdio.h>
#include <collection/vector.h>

// the factor to resize the capacity of the vector
#define VECTOR_RESIZE_FACTOR 2

/**
 * Destroy the elements in the range [index, index + count).
 * @param vector The vector to destroy the elements in.
 * @param index The index of the first element to destroy.
 * @param count The number of elements to destroy.
 * @complexity O(n)
 */
static void _vector_destroy_range(Vector* vector, size_t index, size_t count){
    if (vector->destroy) {
        for (size_t i = index; i < index + count; i++) {
            vector->destroy(vector->data[i]);
        }
    }
}

/**
 * Resize the capacity of the vector to the new capacity.
 * @param vector The vector to resize.
 * @param new_capacity The new capacity of the vector.
 * @return 0 if successful, -1 if failed (memory allocation failure)
 * @complexity O(1)
 */
static int _vector_resize_capacity(Vector* vector, size_t new_capacity){
    if (!new_capacity){
        free(vector->data);
        vector->data = NULL;
        vector->capacity = 0;
        return COLLECTION_SUCCESS;
    }
    // allocate or deallocate memory for the vector data
    void **new_data = realloc(vector->data, new_capacity * sizeof(void *));
    if (new_data == NULL){
        return COLLECTION_FAILURE;
    }
    vector->data = new_data;
    vector->capacity = new_capacity;
    return COLLECTION_SUCCESS;
}

bool vector_empty(const Vector* vector){
    return vector ? vector->size == 0 : false;
}

void** vector_data(const Vector* vector){
    return vector ? vector->data : NULL;
}

size_t vector_size(const Vector* vector){
    return vector ? vector->size : 0;
}

size_t vector_capacity(const Vector* vector){
    return vector ? vector->capacity : 0;
}

int vector_shrink_to_fit(Vector* vector){
    return _vector_resize_capacity(vector, vector->size);
}

int vector_resize(Vector* vector, size_t new_size){
    if (new_size == vector->size) {
        return COLLECTION_SUCCESS;
    }
    int result;
    // expand the capacity
    if (new_size > vector->capacity) {
        result = _vector_resize_capacity(vector, new_size);
        if (result != COLLECTION_SUCCESS){
            return COLLECTION_FAILURE;
        }
        memset(vector->data + vector->size, 0, (new_size - vector->size) * sizeof(void *));
    }
    // shrink the size
    else if (new_size < vector->size) {
        _vector_destroy_range(vector, new_size, vector->size - new_size);
        memset(vector->data + new_size, 0, (vector->size - new_size) * sizeof(void *));
    }
    // expand the size
    else if (new_size > vector->size) {
        memset(vector->data + vector->size, 0, (new_size - vector->size) * sizeof(void *));
    }
    vector->size = new_size;
    return COLLECTION_SUCCESS;
}

void vector_init(Vector* vector, void (*destroy)(void *)){
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
    vector->destroy = destroy;
}

void vector_destroy(Vector* vector){
    if (vector->data){
        _vector_destroy_range(vector, 0, vector->size);
        free(vector->data);
        vector->data = NULL;
    }
    vector->size = 0;
    vector->capacity = 0;
    vector->destroy = NULL;
}

void* vector_at(const Vector* vector, size_t index){
    if (index >= vector->size){
        fprintf(stderr, "FATAL: Index out of bounds\n");
        exit(COLLECTION_FAILURE);
    }
    return vector->data[index];
}

void* vector_back(const Vector* vector){
    if (vector->size == 0){
        return NULL;
    }
    return vector->data[vector->size - 1];
}

void* vector_front(const Vector* vector){
    if (vector->size == 0){
        return NULL;
    }
    return vector->data[0];
}

int vector_push_back(Vector* vector, void* element){
    if (vector->size >= vector->capacity) {
        const size_t new_capacity = vector->capacity ? 
        vector->capacity * VECTOR_RESIZE_FACTOR : 1;
        if (_vector_resize_capacity(vector, new_capacity) != COLLECTION_SUCCESS){
            return COLLECTION_FAILURE;
        }
    }
    vector->data[vector->size++] = element;
    return COLLECTION_SUCCESS;
}

int vector_pop_back(Vector* vector, void** data){
    if (vector->size == 0){
        return COLLECTION_FAILURE;
    }
    vector->size--;
    if (data){
        *data = vector->data[vector->size];
    }
    if (!data && vector->destroy){
        vector->destroy(vector->data[vector->size]);
    }
    vector->data[vector->size] = NULL;
    return COLLECTION_SUCCESS;
}

void vector_clear(Vector* vector){
    if (vector->destroy) {
        _vector_destroy_range(vector, 0, vector->size);
    }
    memset(vector->data, 0, vector->size * sizeof(void *));
    vector->size = 0;
}

int vector_insert(Vector* vector, size_t index, void* element){
    if (index > vector->size){
        return COLLECTION_FAILURE;
    }
    if (index == vector->size){
        vector_push_back(vector, element);
        return COLLECTION_SUCCESS;
    }
    // expand the capacity
    if (vector->size >= vector->capacity){
        const size_t new_capacity = vector->capacity ? 
        vector->capacity * VECTOR_RESIZE_FACTOR : 1;
        _vector_resize_capacity(vector, new_capacity);
    }
    memmove(vector->data + index + 1, vector->data + index, (vector->size - index) * sizeof(void *));
    vector->data[index] = element;
    vector->size++;
    return COLLECTION_SUCCESS;
}

int vector_remove(Vector* vector, size_t index, void** data){
    if (index >= vector->size){
        return COLLECTION_FAILURE;
    }
    if (data){
        *data = vector->data[index];
    }
    if (!data && vector->destroy){
        vector->destroy(vector->data[index]);
    }
    memmove(vector->data + index, vector->data + index + 1, (vector->size - index - 1) * sizeof(void *));
    vector->size--;
    vector->data[vector->size] = NULL;
    return COLLECTION_SUCCESS;
}