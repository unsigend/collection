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

#include <dvec.h>
#include <errno.h>
#include <heap.h>
#include <string.h>
#include <utils.h>

#define parent(i) (((i) - 1) / 2)
#define left(i) (((i) * 2) + 1)
#define right(i) (((i) * 2) + 2)

#define min(a, b) ((a) < (b) ? (a) : (b))

static void shift_up(struct heap *h, size_t i)
{
    while (i > 0) {
        size_t p = parent(i);
        if (h->cmp(dvec_at(&h->dvec, i), dvec_at(&h->dvec, p)) < 0) {
            swap(dvec_at(&h->dvec, i), dvec_at(&h->dvec, p), h->dvec.elesz);
            i = p;
        } else
            break;
    }
}

static void shift_down(struct heap *h, size_t i)
{
    while (1) {
        size_t l = left(i);
        size_t r = right(i);
        size_t best = i;

        if (l < h->dvec.len &&
            h->cmp(dvec_at(&h->dvec, l), dvec_at(&h->dvec, best)) < 0)
            best = l;
        if (r < h->dvec.len &&
            h->cmp(dvec_at(&h->dvec, r), dvec_at(&h->dvec, best)) < 0)
            best = r;
        if (best == i)
            break;
        swap(dvec_at(&h->dvec, i), dvec_at(&h->dvec, best), h->dvec.elesz);
        i = best;
    }
}

int heap_init(struct heap *h, size_t elesz, size_t cap,
              int (*cmp)(const void *, const void *))
{
    h->cmp = cmp;
    return dvec_init(&h->dvec, elesz, cap);
}

int heap_init_empty(struct heap *h, size_t elesz,
                    int (*cmp)(const void *, const void *))
{
    return heap_init(h, elesz, 0, cmp);
}

void heap_free(struct heap *h)
{
    if (!h)
        return;
    dvec_free(&h->dvec);
}

void heap_clear(struct heap *h) { dvec_clear(&h->dvec); }

void *heap_top(struct heap *h)
{
    if (heap_empty(h))
        return NULL;
    return dvec_at(&h->dvec, 0);
}

int heap_heapify(struct heap *h, const void *base, size_t n)
{
    const char *p = base;
    int err = 0;

    /* bulk-copy: replace current contents with input array */
    dvec_clear(&h->dvec);
    for (size_t i = 0; i < n; i++) {
        if ((err = dvec_pushb(&h->dvec, p + i * h->dvec.elesz)))
            return err;
    }

    /* Floyd's: shift-down every internal node, bottom-up */
    if (n < 2)
        return 0;
    for (size_t i = n / 2; i-- > 0;)
        shift_down(h, i);
    return 0;
}

int heap_push(struct heap *h, const void *ele)
{
    int err = 0;
    if ((err = dvec_pushb(&h->dvec, ele)))
        return err;
    shift_up(h, h->dvec.len - 1);
    return 0;
}

int heap_pop(struct heap *h, void *ele)
{
    if (heap_empty(h))
        return -ENOENT;
    if (ele)
        memcpy(ele, dvec_at(&h->dvec, 0), h->dvec.elesz);
    memmove(dvec_at(&h->dvec, 0), dvec_at(&h->dvec, h->dvec.len - 1),
            h->dvec.elesz);
    dvec_popb(&h->dvec, NULL);
    shift_down(h, 0);
    return 0;
}

int heap_dup(struct heap *h, struct heap *dup)
{
    dup->cmp = h->cmp;
    return dvec_dup(&h->dvec, &dup->dvec);
}