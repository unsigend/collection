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

#include <algorithm/hash.h>
#include <stdint.h>

/**
 * Hash a string. This hash function is based on 
 * "Compilers: Principles, Techniques, and Tools" by Aho, Sethi, and Ullman.
 * @note: PWJ hash
 */
uint32_t hash_str(const char * key){
    uint32_t hash = 0;

    while (*key) {
        uint32_t c;
        hash = (hash << 4) + (*key++);
        if ((c = (hash & 0xF0000000)) != 0) {
            hash ^= c >> 24;
            hash ^= c;
        }
    }

    return hash;
}