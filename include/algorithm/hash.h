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

#ifndef ALGORITHM_HASH_H
#define ALGORITHM_HASH_H

/**
 * This file contains the interface for hash functions.
 */
#include <collection/common.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * Hash a string.
 * @param key The string to hash.
 * @return The hash of the string.
 * @complexity O(n)
 */
extern uint32_t hash_str(const char * key);

/**
 * Hash an integer.
 * @param key The integer to hash.
 * @return The hash of the integer.
 * @complexity O(1)
 */
extern uint32_t hash_int(const int key);

#endif