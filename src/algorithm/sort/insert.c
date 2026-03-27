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

#include <errno.h>
#include <sort.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <util.h>

#define GET(base, idx, sz) ((void *)((char *)(base) + (idx) * (sz)))
#define THRESHOLD 256

int sortins(void *base, size_t n, size_t sz, int (*cmp)(void *, void *))
{
  if (!base || !sz || !cmp)
    return -1;

  char stack[THRESHOLD];
  char *buf = NULL;
  if (sz <= THRESHOLD)
    buf = stack;
  else {
    buf = malloc(sz);
    if (!buf) {
      errno = ENOMEM;
      return -1;
    }
  }

  for (size_t cur = 1; cur < n; cur++) {
    void *key = GET(base, cur, sz);
    memcpy(buf, key, sz);
    size_t pos = cur;
    while (pos && cmp(GET(base, pos - 1, sz), buf) > 0) {
      memcpy(GET(base, pos, sz), GET(base, pos - 1, sz), sz);
      pos--;
    }
    memcpy(GET(base, pos, sz), buf, sz);
  }

  if (buf != stack)
    free(buf);
  return 0;
}