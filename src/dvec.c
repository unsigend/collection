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
#include <stdlib.h>
#include <string.h>

#define GROW_FACTOR 2

int dvec_init(struct dvec *v, size_t cap, size_t elesz)
{
    if (elesz == 0)
        return -1;

    if (cap) {
        v->buf = calloc(cap, elesz);
        if (!v->buf)
            return -1;
        v->cap = cap;
    } else {
        v->buf = NULL;
        v->cap = 0;
    }
    v->len = 0;
    v->elesz = elesz;
    return 0;
}

int dvec_init_empty(struct dvec *v, size_t elesz)
{
    return dvec_init(v, 0, elesz);
}

void dvec_free(struct dvec *v)
{
    if (v == NULL)
        return;
    free(v->buf);
    memset(v, 0, sizeof(struct dvec));
}

void dvec_clear(struct dvec *v)
{
    if (v == NULL)
        return;
    v->len = 0;
}

void *dvec_at(struct dvec *v, size_t index)
{
    if (index >= v->len)
        return NULL;
    return (void *)((char *)v->buf + index * v->elesz);
}

void *dvec_front(struct dvec *v) { return dvec_at(v, 0); }
void *dvec_back(struct dvec *v)
{
    return v->len ? dvec_at(v, v->len - 1) : NULL;
}

int dvec_pushb(struct dvec *v, const void *data)
{
    int err;

    if (dvec_avail(v) == 0) {
        if ((err = dvec_reserve(v, v->cap ? v->cap * GROW_FACTOR : 1)))
            return err;
    }
    memcpy((char *)v->buf + v->len * v->elesz, data, v->elesz);
    v->len++;
    return 0;
}

int dvec_pushf(struct dvec *v, const void *data)
{
    int err;

    if (dvec_avail(v) == 0) {
        if ((err = dvec_reserve(v, v->cap ? v->cap * GROW_FACTOR : 1)))
            return err;
    }
    if (v->len)
        memmove((char *)v->buf + v->elesz, v->buf, v->len * v->elesz);
    memcpy(v->buf, data, v->elesz);
    v->len++;
    return 0;
}

int dvec_popb(struct dvec *v, void *data)
{
    if (v->len == 0)
        return -1;
    if (data)
        memcpy(data, dvec_back(v), v->elesz);
    v->len--;
    return 0;
}

int dvec_popf(struct dvec *v, void *data)
{
    if (v->len == 0)
        return -1;
    if (data)
        memcpy(data, dvec_front(v), v->elesz);
    if (v->len > 1)
        memmove(v->buf, (char *)v->buf + v->elesz, (v->len - 1) * v->elesz);
    v->len--;
    return 0;
}

int dvec_ins(struct dvec *v, size_t index, const void *data)
{
    int err;

    if (index > v->len)
        return -1;

    if (dvec_avail(v) == 0) {
        if ((err = dvec_reserve(v, v->cap ? v->cap * GROW_FACTOR : 1)))
            return err;
    }
    if (index != v->len)
        memmove((char *)v->buf + (index + 1) * v->elesz,
                (char *)v->buf + index * v->elesz, (v->len - index) * v->elesz);
    memcpy((char *)v->buf + index * v->elesz, data, v->elesz);
    v->len++;
    return 0;
}

int dvec_del(struct dvec *v, size_t index)
{
    if (index >= v->len)
        return -1;

    if (index != v->len - 1)
        memmove((char *)v->buf + index * v->elesz,
                (char *)v->buf + (index + 1) * v->elesz,
                (v->len - index - 1) * v->elesz);
    v->len--;
    return 0;
}

int dvec_dup(const struct dvec *v, struct dvec *dst)
{
    int err;

    err = dvec_init(dst, v->cap, v->elesz);
    if (err)
        return err;
    if (v->len)
        memcpy(dst->buf, v->buf, v->len * v->elesz);
    dst->len = v->len;
    return 0;
}

void dvec_shrink(struct dvec *v)
{
    void *newbuf;

    if (v->len == v->cap)
        return;

    if (v->len == 0) {
        free(v->buf);
        v->buf = NULL;
        v->cap = 0;
        return;
    }

    newbuf = realloc(v->buf, v->len * v->elesz);
    if (!newbuf)
        return;
    v->buf = newbuf;
    v->cap = v->len;
}

int dvec_reserve(struct dvec *v, size_t newcap)
{
    void *newbuf;

    if (newcap <= dvec_cap(v))
        return 0;

    newbuf = realloc(v->buf, newcap * v->elesz);
    if (!newbuf)
        return -1;
    v->buf = newbuf;
    v->cap = newcap;
    return 0;
}

void dvec_sort(struct dvec *v, int (*cmp)(const void *a, const void *b))
{
    if (v->len < 2)
        return;
    qsort(v->buf, v->len, v->elesz, cmp);
}