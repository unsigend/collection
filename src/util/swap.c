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

#include <util/util.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 512

/**
 * @note: If the size of the elements is less than MAX_STACK_SIZE,
 *        the elements are swapped using a temporary array in stack.
 *        Otherwise, the elements are swapped by a buffer in heap.
 */
int util_swap(void * a, void * b, size_t size){
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