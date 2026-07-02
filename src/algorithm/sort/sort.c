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

#include "sort.h"

#include <math.h>
#include <util.h>

#define THRESHOLD 16

static int sort_rec(void *base, size_t n, size_t sz, int (*cmp)(void *, void *),
                    int depth)
{
    if (n <= THRESHOLD)
        return sort_insertion(base, n, sz, cmp);
    if (!depth)
        return sort_heap(base, n, sz, cmp);

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

    return sort_rec(base, pos, sz, cmp, depth - 1) ||
           sort_rec(GET(base, pos + 1, sz), n - pos - 1, sz, cmp, depth - 1);
}

int sort(void *base, size_t n, size_t sz, int (*cmp)(void *, void *))
{
    if (!base || !sz || !cmp)
        return -1;
    if (n <= 1)
        return 0;
    int depth = 2 * (int)log2(n);
    return sort_rec(base, n, sz, cmp, depth);
}