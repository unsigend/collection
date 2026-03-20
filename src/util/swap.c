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

#include <stdlib.h>
#include <string.h>
#include <util/util.h>

#define BUFFSZ 512

/* If the size of the elements is less than MAX_STACK_SIZE, the elements are
   swapped using a temporary array in stack. Otherwise, the elements are swapped
   by a buffer in heap. */

int swap(void *a, void *b, size_t sz)
{
  if (!a || !b || sz == 0)
    return COLLECTION_FAILURE;
  if (a == b)
    return COLLECTION_SUCCESS;
  if (sz < BUFFSZ) {
    char temp[sz];
    memcpy(temp, a, sz);
    memcpy(a, b, sz);
    memcpy(b, temp, sz);
    return COLLECTION_SUCCESS;
  }
  void *temp = malloc(sz);
  if (!temp)
    return COLLECTION_FAILURE;
  memcpy(temp, a, sz);
  memcpy(a, b, sz);
  memcpy(b, temp, sz);
  free(temp);
  return COLLECTION_SUCCESS;
}