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

#include <collection/common.h>
#include <algorithm/sort.h>
#include <util/util.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#define ARRAY_INDEX(data, i, size) ((char *)data + (i) * (size))

/**
 * Merge two sorted arrays into one sorted array.
 * @note: the data buffer should not overlap with the input arrays.
 */
static int _merge(void* data, size_t size, void* arr1, size_t n1, 
    void* arr2, size_t n2, int (*compare)(const void *, const void *))
{
    if (!data || !arr1 || !arr2 || !compare)
        return COLLECTION_FAILURE;

    size_t write = 0, read1 = 0, read2 = 0;
    while (read1 < n1 && read2 < n2) {
        if (compare(ARRAY_INDEX(arr1, read1, size), 
        ARRAY_INDEX(arr2, read2, size)) < 0){
            memcpy(ARRAY_INDEX(data, write, size), 
            ARRAY_INDEX(arr1, read1, size), size);
            read1++;
        }else{
            memcpy(ARRAY_INDEX(data, write, size), 
            ARRAY_INDEX(arr2, read2, size), size);
            read2++;
        }
        write++;
    }
    while (read1 < n1){
        memcpy(ARRAY_INDEX(data, write, size), 
        ARRAY_INDEX(arr1, read1, size), size);
        read1++;
        write++;
    }
    while (read2 < n2){
        memcpy(ARRAY_INDEX(data, write, size), 
        ARRAY_INDEX(arr2, read2, size), size);
        read2++;
        write++;
    }
    return COLLECTION_SUCCESS;
}

static int _merge_sort(void* data, size_t n, size_t size, 
    size_t low, size_t high,
    int (*compare)(const void *, const void *)){
    if (low >= high)
        return COLLECTION_SUCCESS;
    
    size_t mid = low + (high - low) / 2;
    void* buff = malloc((high - low + 1) * size);
    if (!buff)
        return COLLECTION_FAILURE;

    if (_merge_sort(data, n, size, low, mid, compare) 
        != COLLECTION_SUCCESS){
        free(buff);
        return COLLECTION_FAILURE;
    }
    if (_merge_sort(data, n, size, mid + 1, high, compare) 
        != COLLECTION_SUCCESS){
        free(buff);
        return COLLECTION_FAILURE;
    }
    
    if (_merge(buff, size, 
        ARRAY_INDEX(data, low, size), mid - low + 1, 
        ARRAY_INDEX(data, mid + 1, size), high - mid, compare)
        != COLLECTION_SUCCESS){
        free(buff);
        return COLLECTION_FAILURE;
    }
    memcpy(ARRAY_INDEX(data, low, size), buff, (high - low + 1) * size);
    free(buff);
    return COLLECTION_SUCCESS;
}


int sort_merge(void* data, size_t n, size_t size,
    int (*compare)(const void *, const void *)){
    if (!data || !compare || size == 0)
        return COLLECTION_FAILURE;

    if (n == 1 || n == 0)
        return COLLECTION_SUCCESS;

    return _merge_sort(data, n, size, 0, n - 1, compare);
}