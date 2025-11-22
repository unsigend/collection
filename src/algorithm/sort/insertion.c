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
#include <string.h>

#if defined(__linux__)
#include <sys/types.h>
#endif

#define ARRAY_INDEX(data, i, size) ((char *)data + (i) * (size))

/**
 * @note: swap based heavy IO 
 */
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
            if (util_swap(ARRAY_INDEX(data, j - 1, size), 
            ARRAY_INDEX(data, j, size), size)
                != COLLECTION_SUCCESS)
                return COLLECTION_FAILURE;
            j--;
        }
    }
    return COLLECTION_SUCCESS;
}