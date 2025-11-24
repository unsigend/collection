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
#include <util/util.h>
#include <stddef.h>

#define ARRAY_INDEX(data, i, size) ((char *)data + (i) * (size))


/**
 * Partition the data into two parts.
 * @note: the pivot is based on median of three algorithm
 * @return the index of the pivot or -1 if failed
 */
static int _partition(void * data, size_t n, size_t size,
    int (*compare)(const void *, const void *))
{
    size_t left = 0;
    size_t right = n - 1;
    size_t mid = left + (right - left) / 2;

    // apply median of three algorithm
    if (compare(ARRAY_INDEX(data, left, size), 
    ARRAY_INDEX(data, mid, size)) > 0){
        if (util_swap(ARRAY_INDEX(data, left, size), 
        ARRAY_INDEX(data, mid, size), size))
        return -1;
    }
    if (compare(ARRAY_INDEX(data, mid, size), 
    ARRAY_INDEX(data, right, size)) > 0){
        if (util_swap(ARRAY_INDEX(data, mid, size), 
        ARRAY_INDEX(data, right, size), size))
        return -1;
    }
    if (compare(ARRAY_INDEX(data, left, size), 
    ARRAY_INDEX(data, mid, size)) > 0){
        if (util_swap(ARRAY_INDEX(data, left, size), 
        ARRAY_INDEX(data, mid, size), size))
        return -1;
    }

    // put the pivot to the right-1 position
    if (util_swap(ARRAY_INDEX(data, mid, size), 
        ARRAY_INDEX(data, right - 1, size), size))
        return -1;

    int pivot = right - 1;
    right-= 2;
    left++;

    while (left <= right){
        while (compare(ARRAY_INDEX(data, left, size), 
        ARRAY_INDEX(data, pivot, size)) < 0) left++;
        while (compare(ARRAY_INDEX(data, right, size), 
        ARRAY_INDEX(data, pivot, size)) > 0) right--;
        if (left < right){
            if (util_swap(ARRAY_INDEX(data, left++, size), 
            ARRAY_INDEX(data, right--, size), size) != COLLECTION_SUCCESS)
                return -1;
        }else break;
    }
    
    // restore the pivot
    if (util_swap(ARRAY_INDEX(data, left, size), 
    ARRAY_INDEX(data, pivot, size), size) != COLLECTION_SUCCESS)
        return -1;

    return left;
}

static int _qsort( void * data, size_t n, size_t size,
    size_t low, size_t high,
    int (*compare)(const void *, const void *)){
    if (low >= high)
        return COLLECTION_SUCCESS;

    if (high - low + 1 < 3){
        if (high - low + 1 == 2){
            if (compare(ARRAY_INDEX(data, low, size), 
            ARRAY_INDEX(data, high, size)) > 0){
                if (util_swap(ARRAY_INDEX(data, low, size), 
                ARRAY_INDEX(data, high, size), size) != COLLECTION_SUCCESS)
                    return COLLECTION_FAILURE;
            }
        }
        return COLLECTION_SUCCESS;
    }

    int pivot = _partition(ARRAY_INDEX(data, low, size), high - low + 1, size, compare);
    if (pivot == -1)
        return COLLECTION_FAILURE;
    pivot += low;
    if (_qsort(data, n, size, low, pivot - 1, compare) != COLLECTION_SUCCESS)
        return COLLECTION_FAILURE;
    if (_qsort(data, n, size, pivot + 1, high, compare) != COLLECTION_SUCCESS)
        return COLLECTION_FAILURE;
    return COLLECTION_SUCCESS;
}

int sort_quick(void * data, size_t n, size_t size,
    int (*compare)(const void *, const void *)){
    if (!data || !compare || size == 0)
        return COLLECTION_FAILURE;
    if (n == 1 || n == 0)
        return COLLECTION_SUCCESS;
    return _qsort(data, n, size, 0, n - 1, compare);
}