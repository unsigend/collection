/* collection - A high-performance C data structure library
 * Copyright (C) 2026-2027 Yixiang Qiu
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

#include <string.h>
#include <utils.h>

void swap(void *lhs, void *rhs, size_t size)
{
    unsigned long word;
    unsigned char byte;
    size_t i = 0;

    if (lhs == rhs)
        return;

    for (; i + sizeof(word) <= size; i += sizeof(word)) {
        memcpy(&word, lhs + i, sizeof(word));
        memcpy(lhs + i, rhs + i, sizeof(word));
        memcpy(rhs + i, &word, sizeof(word));
    }
    for (; i < size; i++) {
        memcpy(&byte, lhs + i, 1);
        memcpy(lhs + i, rhs + i, 1);
        memcpy(rhs + i, &byte, 1);
    }
}