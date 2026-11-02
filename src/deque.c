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

#include <deque.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define deque_tail(d)                                                          \
    (deque_cap(d) ? (deque_head(d) + deque_len(d)) % deque_cap(d) : 0)
#define atp(d, index) ((void *)((char *)(d)->buf + (index) * (d)->elesz))
#define atl(d, index) atp(d, (d->head + index) % deque_cap(d))

static inline void flatten(const struct deque *d, struct deque *flat)
{
    size_t n;

    if (d->len) {
        n = d->cap - d->head;
        if (d->len <= n)
            memcpy(flat->buf, atp(d, d->head), d->len * d->elesz);
        else {
            memcpy(flat->buf, atp(d, d->head), n * d->elesz);
            memcpy((char *)flat->buf + n * d->elesz, d->buf,
                   (d->len - n) * d->elesz);
        }
    }

    flat->elesz = d->elesz;
    flat->len = d->len;
    flat->head = 0;
}

static void shiftright(struct deque *d, size_t li, size_t n)
{
    size_t src, i;

    if (n == 0)
        return;

    src = (d->head + li) % d->cap;
    if (src + n < d->cap) {
        memmove(atp(d, src + 1), atp(d, src), n * d->elesz);
        return;
    }

    for (i = n; i > 0; i--)
        memcpy(atl(d, li + i), atl(d, li + i - 1), d->elesz);
}

static void shiftleft(struct deque *d, size_t li, size_t n)
{
    size_t src, i;

    if (n == 0)
        return;

    src = (d->head + li) % d->cap;
    if (src > 0 && src + n <= d->cap) {
        memmove(atp(d, src - 1), atp(d, src), n * d->elesz);
        return;
    }

    for (i = 0; i < n; i++)
        memcpy(atl(d, li + i - 1), atl(d, li + i), d->elesz);
}

int deque_init(struct deque *d, size_t elesz, size_t cap)
{
    if (elesz == 0)
        return -EINVAL;

    if (cap) {
        d->buf = calloc(cap, elesz);
        if (d->buf == NULL)
            return -ENOMEM;
    } else
        d->buf = NULL;

    d->elesz = elesz;
    d->cap = cap;
    d->len = 0;
    d->head = 0;

    return 0;
}

int deque_init_empty(struct deque *d, size_t elesz)
{
    return deque_init(d, elesz, 0);
}

void deque_free(struct deque *d)
{
    if (d == NULL)
        return;
    if (d->buf)
        free(d->buf);
    memset(d, 0, sizeof(*d));
}

void deque_clear(struct deque *d) { d->len = 0; }

void *deque_at(struct deque *d, size_t index)
{
    if (index >= deque_len(d))
        return NULL;

    return atl(d, index);
}

void *deque_front(struct deque *d) { return deque_at(d, 0); };
void *deque_back(struct deque *d) { return deque_at(d, deque_len(d) - 1); };

int deque_reserve(struct deque *d, size_t cap)
{
    struct deque flat;
    int err;

    if (cap <= deque_cap(d))
        return 0;

    if ((err = deque_init(&flat, d->elesz, cap)))
        return err;

    flatten(d, &flat);
    deque_free(d);
    *d = flat;

    return 0;
}

void deque_shrink(struct deque *d)
{
    struct deque flat;
    int err;

    if (deque_len(d) == deque_cap(d))
        return;

    if ((err = deque_init(&flat, d->elesz, deque_len(d))))
        return;

    flatten(d, &flat);
    deque_free(d);
    *d = flat;
}

int deque_pushf(struct deque *d, const void *ele)
{
    int err;

    if (deque_full(d)) {
        if ((err = deque_reserve(d, d->cap ? d->cap * 2 : 1)))
            return err;
    }

    d->head = (d->head + d->cap - 1) % d->cap;
    memcpy(atp(d, d->head), ele, d->elesz);
    d->len++;

    return 0;
}

int deque_pushb(struct deque *d, const void *ele)
{
    int err;

    if (deque_full(d)) {
        if ((err = deque_reserve(d, d->cap ? d->cap * 2 : 1)))
            return err;
    }

    memcpy(atp(d, deque_tail(d)), ele, d->elesz);
    d->len++;

    return 0;
}

int deque_popf(struct deque *d, void *ele)
{
    if (deque_empty(d))
        return -ENOENT;

    if (ele)
        memcpy(ele, atp(d, d->head), d->elesz);

    d->head = (d->head + 1) % d->cap;
    d->len--;

    return 0;
}

int deque_popb(struct deque *d, void *ele)
{
    if (deque_empty(d))
        return -ENOENT;

    if (ele)
        memcpy(ele, atl(d, d->len - 1), d->elesz);
    d->len--;

    return 0;
}

int deque_ins(struct deque *d, size_t index, const void *ele)
{
    int err;

    if (index > d->len)
        return -ERANGE;

    if (index == 0)
        return deque_pushf(d, ele);
    if (index == d->len)
        return deque_pushb(d, ele);

    if (deque_full(d)) {
        if ((err = deque_reserve(d, d->cap ? d->cap * 2 : 1)))
            return err;
    }

    if (index <= d->len - index) {
        d->head = (d->head + d->cap - 1) % d->cap;
        shiftleft(d, 1, index);
    } else
        shiftright(d, index, d->len - index);

    memcpy(atl(d, index), ele, d->elesz);
    d->len++;

    return 0;
}

int deque_del(struct deque *d, size_t index, void *ele)
{
    if (index >= d->len)
        return -ERANGE;

    if (index == 0)
        return deque_popf(d, ele);
    if (index == d->len - 1)
        return deque_popb(d, ele);

    if (ele)
        memcpy(ele, atl(d, index), d->elesz);

    if (index <= d->len - index - 1) {
        shiftright(d, 0, index);
        d->head = (d->head + 1) % d->cap;
    } else
        shiftleft(d, index + 1, d->len - index - 1);

    d->len--;

    return 0;
}

int deque_dup(struct deque *d, struct deque *dup)
{
    int err;

    if (d == dup)
        return -EINVAL;

    if ((err = deque_init(dup, d->elesz, deque_len(d))))
        return err;

    if (deque_len(d))
        flatten(d, dup);
    return 0;
}

void deque_sort(struct deque *d, int (*cmp)(const void *, const void *))
{
    struct deque flat;
    void *tmp;

    if (d->len < 2)
        return;

    if (d->len <= d->cap - d->head) {
        qsort(atp(d, d->head), d->len, d->elesz, cmp);
        return;
    }

    tmp = malloc(d->len * d->elesz);
    if (tmp == NULL)
        return;

    flat.buf = tmp;
    flatten(d, &flat);
    qsort(tmp, d->len, d->elesz, cmp);
    memcpy(d->buf, tmp, d->len * d->elesz);
    d->head = 0;
    free(tmp);
}