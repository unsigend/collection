/* collection - A generic data structure and algorithms library
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

#ifndef COLLECTION_SORT_H
#define COLLECTION_SORT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Sort n elements of sz bytes each in place at base using cmp.
 *
 * cmp receives pointers to two elements and should return a value less than
 * zero, zero, or greater than zero if the first argument sorts before, equal
 * to, or after the second, in the usual style of strcmp.
 *
 * @param base pointer to the first element, must not be NULL when n > 0
 * @param n    number of elements
 * @param sz   byte size of each element, must be non zero when n > 0
 * @param cmp  element comparator, must not be NULL when n > 0
 * @return     0 on success, -1 on error
 */
extern int sort(void *base, size_t n, size_t sz, int (*cmp)(void *, void *));

#ifdef COL_ALL_SORTS

/**
 * Insertion sort. Same parameters and return values as sort.
 *
 * Declared only when COL_ALL_SORTS is defined before including this header.
 * Internal raw algorithm without the tuning of sort. Prefer sort for general
 * use.
 *
 * @param base pointer to the first element, must not be NULL when n > 0
 * @param n    number of elements
 * @param sz   byte size of each element, must be non zero when n > 0
 * @param cmp  element comparator, must not be NULL when n > 0
 * @return     0 on success, -1 on error
 */
extern int sort_insertion(void *base, size_t n, size_t sz,
                          int (*cmp)(void *, void *));

/**
 * Quicksort. Same parameters and return values as sort.
 *
 * Declared only when COL_ALL_SORTS is defined before including this header.
 * Internal raw algorithm without the tuning of sort. Prefer sort for general
 * use.
 *
 * @param base pointer to the first element, must not be NULL when n > 0
 * @param n    number of elements
 * @param sz   byte size of each element, must be non zero when n > 0
 * @param cmp  element comparator, must not be NULL when n > 0
 * @return     0 on success, -1 on error
 */
extern int sort_quick(void *base, size_t n, size_t sz,
                      int (*cmp)(void *, void *));

/**
 * Heapsort. Same parameters and return values as sort.
 *
 * Declared only when COL_ALL_SORTS is defined before including this header.
 * Internal raw algorithm without the tuning of sort. Prefer sort for general
 * use.
 *
 * @param base pointer to the first element, must not be NULL when n > 0
 * @param n    number of elements
 * @param sz   byte size of each element, must be non zero when n > 0
 * @param cmp  element comparator, must not be NULL when n > 0
 * @return     0 on success, -1 on error
 */
extern int sort_heap(void *base, size_t n, size_t sz,
                     int (*cmp)(void *, void *));

#endif

#ifdef __cplusplus
}
#endif

#endif
