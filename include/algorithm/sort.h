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

#ifndef ALGORITHM_SORT_H
#define ALGORITHM_SORT_H

/**
 * This file contains collection for sorting algorithms.
 */
#include <collection/common.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * Sort the data using insertion sort.
 * @param data The data to sort.
 * @param n The number of elements to sort.
 * @param size The size of the element.
 * @param compare The function to compare two elements.
 * @return 0 if successful, -1 if failed.
 * @complexity O(n^2)
 */
extern int sort_insertion(void * data, size_t n, size_t size, 
    int (*compare)(const void *, const void *));

/**
 * Sort the data using selection sort.
 * @param data The data to sort.
 * @param n The number of elements to sort.
 * @param size The size of the element.
 * @param compare The function to compare two elements.
 * @return 0 if successful, -1 if failed.
 * @complexity O(n^2)
 */
extern int sort_selection(void * data, size_t n, size_t size, 
    int (*compare)(const void *, const void *));

/**
 * Sort the data using bubble sort.
 * @param data The data to sort.
 * @param n The number of elements to sort.
 * @param size The size of the element.
 * @param compare The function to compare two elements.
 * @return 0 if successful, -1 if failed.
 * @complexity O(n^2)
 */
extern int sort_bubble(void * data, size_t n, size_t size, 
    int (*compare)(const void *, const void *));

/**
 * Sort the data using quick sort.
 * @param data The data to sort.
 * @param n The number of elements to sort.
 * @param size The size of the element.
 * @param compare The function to compare two elements.
 * @return 0 if successful, -1 if failed.
 * @complexity O(n log n)
 */
extern int sort_quick(void * data, size_t n, size_t size, 
    int (*compare)(const void *, const void *));

#endif