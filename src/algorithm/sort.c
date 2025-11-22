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

#include <algorithm/sort.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(__linux__)
#include <sys/types.h>
#endif

#define MAX_STACK_SIZE 512
#define ARRAY_INDEX(data, i, size) ((char *)data + (i) * (size))

/**
 * Swap two elements.
 * @param a The first element.
 * @param b The second element.
 * @param size The size of the elements.
 * @return 0 if successful, -1 if failed.
 * @complexity O(1)
 * @note: If the size of the elements is less than MAX_STACK_SIZE,
 *        the elements are swapped using a temporary array in stack.
 *        Otherwise, the elements are swapped by a buffer in heap.
 */
static int swap(void * a, void * b, size_t size){
    if (!a || !b || size == 0) return COLLECTION_FAILURE;
    if (a == b) return COLLECTION_SUCCESS;
    if (size < MAX_STACK_SIZE){
        char temp[size];
        memcpy(temp, a, size);
        memcpy(a, b, size);
        memcpy(b, temp, size);
        return COLLECTION_SUCCESS;
    }
    void * temp = malloc(size);
    if (!temp) return COLLECTION_FAILURE;
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
    free(temp);
    return COLLECTION_SUCCESS;
}


int sort_insertion(void * data, size_t n, size_t size,
    int (*compare)(const void *, const void *)){
    if (!data || !compare || size == 0)
        return COLLECTION_FAILURE;

    if (n == 1 || n == 0)
        return COLLECTION_SUCCESS;

    for (size_t i = 1; i < n; i++){
        ssize_t j = (ssize_t)i;
        while (j > 0 && compare(ARRAY_INDEX(data, j - 1, size), 
            ARRAY_INDEX(data, j, size)) > 0){
            if (swap(ARRAY_INDEX(data, j - 1, size), ARRAY_INDEX(data, j, size), size)
                != COLLECTION_SUCCESS)
                return COLLECTION_FAILURE;
            j--;
        }
    }

    return COLLECTION_SUCCESS;
}

int sort_selection(void * data, size_t n, size_t size,
    int (*compare)(const void *, const void *)){
    if (!data || !compare || size == 0)
        return COLLECTION_FAILURE;

    if (n == 1 || n == 0)
        return COLLECTION_SUCCESS;

    for (size_t i = 0; i < n - 1; i++){
        size_t min_index = i;
        for (size_t j = i + 1; j < n; j++){
            if (compare(ARRAY_INDEX(data, j, size), ARRAY_INDEX(data, min_index, size)) < 0){
                min_index = j;
            }
        }
        if (min_index != i){
            swap(ARRAY_INDEX(data, i, size), ARRAY_INDEX(data, min_index, size), size);
        }
    }

    return COLLECTION_SUCCESS;
}