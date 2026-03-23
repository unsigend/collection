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
#include <vector.h>

#define PARENT(idx) (((idx) - 1) / 2)
#define LEFT(idx) (2 * (idx) + 1)
#define RIGHT(idx) (2 * (idx) + 2)

static int shiftup(struct heap *heap, size_t idx)
{
  if (!heap || idx >= heap->vec.sz)
    return -1;
  size_t p = PARENT(idx);
}
static void shiftdown(struct heap *heap, size_t idx) {}

int heap_init(struct heap *heap, size_t elesz, int (*cmp)(void *, void *),
              void (*destroy)(void *))
{
  if (!heap || !elesz || !cmp)
    return -1;
  heap->cmp = cmp;
  return vec_init(&heap->vec, elesz, destroy);
}

void heap_fini(struct heap *heap)
{
  if (!heap)
    return;
  vec_fini(&heap->vec);
}

int heap_push(struct heap *heap, void *ele) {}

int heap_pop(struct heap *heap, void *dest) {}

void *heap_peek(struct heap *heap)
{
  if (!heap)
    return NULL;
  return vec_front(&heap->vec);
}

void heap_clear(struct heap *heap)
{
  if (!heap)
    return;
  vec_clear(&heap->vec);
}