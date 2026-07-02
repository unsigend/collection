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

#ifndef COLLECTION_HASH_H
#define COLLECTION_HASH_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Compute a 32 bit hash of key.
 *
 * @param key input unsigned 32 bit key
 * @return    hash value
 */
extern uint32_t hash_u32(uint32_t key);

/**
 * Compute a 32 bit hash of key.
 *
 * @param key input unsigned 64 bit key
 * @return    hash value
 */
extern uint32_t hash_u64(uint64_t key);

/**
 * Compute a 32 bit hash of a null terminated string.
 *
 * @param str pointer to the string to hash, must not be NULL
 * @return    hash value
 */
extern uint32_t hash_str(const char *str);

#ifdef __cplusplus
}
#endif

#endif
