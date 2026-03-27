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

#include <stddef.h>
#include <util.h>

#define GET(base, idx, sz) ((void *)((char *)(base) + (idx) * (sz)))

int sortqs(void *base, size_t n, size_t sz, int (*cmp)(void *, void *))
{
  if (!base || !sz || !cmp)
    return -1;
  if (n <= 1)
    return 0;

  /* Median of three pivot selection: MIN[[min, mid], max], MAX[mid, max]*/
  size_t min = 0, mid = n / 2, max = n - 1;
  if (cmp(GET(base, min, sz), GET(base, mid, sz)) > 0)
    swap(GET(base, min, sz), GET(base, mid, sz), sz);
  if (cmp(GET(base, min, sz), GET(base, max, sz)) > 0)
    swap(GET(base, min, sz), GET(base, max, sz), sz);
  if (cmp(GET(base, mid, sz), GET(base, max, sz)) > 0)
    swap(GET(base, mid, sz), GET(base, max, sz), sz);

  if (n <= 3)
    return 0;

  size_t pivot = max - 1;
  swap(GET(base, mid, sz), GET(base, pivot, sz), sz);

  size_t l = 1, r = n - 3;
  while (l < r) {
    while (l < r && cmp(GET(base, l, sz), GET(base, pivot, sz)) < 0)
      l++;
    while (l < r && cmp(GET(base, r, sz), GET(base, pivot, sz)) > 0)
      r--;
    if (l < r) {
      swap(GET(base, l, sz), GET(base, r, sz), sz);
      l++, r--;
    }
  }
  size_t pos = l;
  if (cmp(GET(base, pos, sz), GET(base, pivot, sz)) <= 0)
    pos++;
  swap(GET(base, pos, sz), GET(base, pivot, sz), sz);

  return sortqs(base, pos, sz, cmp) ||
         sortqs(GET(base, pos + 1, sz), n - pos - 1, sz, cmp);
}