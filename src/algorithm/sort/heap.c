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

#include <heap.h>
#include <stddef.h>

#define GET(base, idx, sz) ((void *)((char *)(base) + (idx) * (sz)))

int sortheap(void *base, size_t n, size_t sz, int (*cmp)(void *, void *))
{
  if (!base || !sz || !cmp)
    return -1;

  struct heap h;
  if (heap_init(&h, sz, cmp, NULL) == -1)
    return -1;

  for (size_t i = 0; i < n; i++) {
    if (heap_push(&h, GET(base, i, sz)) == -1) {
      heap_fini(&h);
      return -1;
    }
  }

  for (size_t i = 0; i < n; i++) {
    if (heap_pop(&h, GET(base, i, sz)) == -1) {
      heap_fini(&h);
      return -1;
    }
  }

  heap_fini(&h);
  return 0;
}