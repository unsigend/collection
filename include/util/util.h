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

#ifndef UTIL_UTIL_H
#define UTIL_UTIL_H

#include <collection/common.h>

/**
 * Swap two elements.
 * @param a The first element.
 * @param b The second element.
 * @param size The size of the elements.
 * @return 0 if successful, -1 if failed.
 * @complexity O(1)
 */
extern int util_swap(void * a, void * b, size_t size);

/**
 * Generate a random integer between min and max.
 * @param min The minimum value.
 * @param max The maximum value.
 * @return The random integer.
 * @complexity O(1)
 */
extern int util_random(int min, int max);

#endif