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
#include <stdlib.h>
#include <string.h>

#define at(v, index, elesz) ((void *)((char *)(v)->buf + (index) * (elesz)))

int dvec_init(struct dvec *v, size_t elesz, size_t cap)
{
    if (elesz == 0)
        return -EINVAL;

    if (cap) {
        v->buf = calloc(cap, elesz);
        if (v->buf == NULL)
            return -ENOMEM;
    } else
        v->buf = NULL;

    v->elesz = elesz;
    v->len = 0;
    v->cap = cap;

    return 0;
}

int dvec_init_empty(struct dvec *v, size_t elesz)
{
    return dvec_init(v, elesz, 0);
}

void dvec_free(struct dvec *v)
{
    if (v == NULL)
        return;
    if (v->buf)
        free(v->buf);
    memset(v, 0, sizeof(*v));
}

void dvec_clear(struct dvec *v) { v->len = 0; }

void *dvec_at(struct dvec *v, size_t index)
{
    if (index >= v->len)
        return NULL;
    return at(v, index, v->elesz);
}

void *dvec_back(struct dvec *v)
{
    return v->len ? at(v, v->len - 1, v->elesz) : NULL;
}
void *dvec_front(struct dvec *v) { return v->len ? at(v, 0, v->elesz) : NULL; }

int dvec_reserve(struct dvec *v, size_t cap)
{
    void *newbuf;

    if (cap <= v->cap)
        return 0;

    newbuf = realloc(v->buf, cap * v->elesz);
    if (newbuf == NULL)
        return -ENOMEM;

    v->buf = newbuf;
    v->cap = cap;

    return 0;
}

void dvec_shrink(struct dvec *v)
{
    void *newbuf;

    if (v->cap == v->len)
        return;

    if (v->len == 0) {
        free(v->buf);
        v->buf = NULL;
        v->cap = 0;
        return;
    }

    newbuf = realloc(v->buf, v->len * v->elesz);
    if (newbuf == NULL)
        return;

    v->buf = newbuf;
    v->cap = v->len;
}

int dvec_pushb(struct dvec *v, const void *ele)
{
    int err;

    if (dvec_avail(v) == 0) {
        if ((err = dvec_reserve(v, v->cap ? v->cap * 2 : 1)))
            return err;
    }

    memcpy(at(v, v->len, v->elesz), ele, v->elesz);
    v->len++;

    return 0;
}

int dvec_pushf(struct dvec *v, const void *ele)
{
    int err;

    if (dvec_avail(v) == 0) {
        if ((err = dvec_reserve(v, v->cap ? v->cap * 2 : 1)))
            return err;
    }

    if (v->len)
        memmove(at(v, 1, v->elesz), at(v, 0, v->elesz), v->len * v->elesz);
    memcpy(at(v, 0, v->elesz), ele, v->elesz);
    v->len++;

    return 0;
}

int dvec_ins(struct dvec *v, size_t index, const void *ele)
{
    int err;

    if (index > v->len)
        return -ERANGE;

    if (dvec_avail(v) == 0) {
        if ((err = dvec_reserve(v, v->cap ? v->cap * 2 : 1)))
            return err;
    }

    if (index < v->len)
        memmove(at(v, index + 1, v->elesz), at(v, index, v->elesz),
                (v->len - index) * v->elesz);
    memcpy(at(v, index, v->elesz), ele, v->elesz);
    v->len++;

    return 0;
}

int dvec_popb(struct dvec *v, void *ele)
{
    if (v->len == 0)
        return -ENOENT;

    if (ele)
        memcpy(ele, at(v, v->len - 1, v->elesz), v->elesz);
    v->len--;

    return 0;
}

int dvec_popf(struct dvec *v, void *ele)
{
    if (v->len == 0)
        return -ENOENT;

    if (ele)
        memcpy(ele, at(v, 0, v->elesz), v->elesz);
    if (v->len > 1)
        memmove(at(v, 0, v->elesz), at(v, 1, v->elesz),
                (v->len - 1) * v->elesz);
    v->len--;

    return 0;
}

int dvec_del(struct dvec *v, size_t index, void *ele)
{
    if (index >= v->len)
        return -ERANGE;

    if (ele)
        memcpy(ele, at(v, index, v->elesz), v->elesz);
    if (index < v->len - 1)
        memmove(at(v, index, v->elesz), at(v, index + 1, v->elesz),
                (v->len - index - 1) * v->elesz);
    v->len--;

    return 0;
}

int dvec_dup(struct dvec *v, struct dvec *dup)
{
    int err;

    if (v == dup)
        return -EINVAL;

    if ((err = dvec_init(dup, v->elesz, v->cap)))
        return err;

    if (v->len)
        memcpy(dup->buf, v->buf, v->len * v->elesz);
    dup->len = v->len;

    return 0;
}

void dvec_sort(struct dvec *v, int (*cmp)(const void *, const void *))
{
    if (v->len < 2)
        return;

    qsort(v->buf, v->len, v->elesz, cmp);
}