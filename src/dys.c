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

#include <ctype.h>
#include <dys.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PREALLOC (1024 * 1024) /* 1MB */

struct dyshdr {
    size_t len;
    size_t cap;
    char buf[]; /* flexible buffer */
};

static struct dyshdr *header(dys s)
{
    return (struct dyshdr *)(s - sizeof(struct dyshdr));
}

size_t dyslen(dys s) { return header(s)->len; }
size_t dyscap(dys s) { return header(s)->cap; }
size_t dysavail(dys s) { return dyscap(s) - dyslen(s) - 1; }

dys dysinitlen(const void *s, size_t len)
{
    struct dyshdr *ds;

    if (s)
        ds = malloc(sizeof(struct dyshdr) + len + 1);
    else
        ds = calloc(1, sizeof(struct dyshdr) + len + 1);

    if (ds == NULL)
        return NULL;

    ds->len = len;
    ds->cap = len + 1;
    if (s && len)
        memcpy(ds->buf, s, len);
    ds->buf[len] = '\0';
    return ds->buf;
}

dys dysinit(const char *s) { return dysinitlen(s, s ? strlen(s) : 0); }
dys dysinit_empty(void) { return dysinitlen(NULL, 0); };
void dysfree(dys s)
{
    if (!s)
        return;
    free(header(s));
};

dys dysdup(dys s) { return dysinitlen(s, dyslen(s)); }
dys dysclear(dys s)
{
    struct dyshdr *ds = header(s);
    ds->len = 0;
    ds->buf[0] = '\0';
    return s;
}

static inline dys grow(dys s, size_t addlen)
{
    struct dyshdr *ds, *newds;
    size_t free = dysavail(s);
    size_t newlen;

    if (free >= addlen)
        return s;

    ds = header(s);
    newlen = ds->len + addlen;
    if (newlen < MAX_PREALLOC)
        newlen *= 2;
    else
        newlen += MAX_PREALLOC;

    newds = realloc(ds, sizeof(struct dyshdr) + newlen + 1);
    if (newds == NULL)
        return NULL;

    newds->cap = newlen + 1;
    return newds->buf;
}

dys dysgrow(dys s, size_t len)
{
    struct dyshdr *ds = header(s);

    if (len <= ds->len)
        return s;
    s = grow(s, len - ds->len);
    if (s == NULL)
        return NULL;
    ds = header(s);
    memset(s + ds->len, 0, len - ds->len + 1);
    ds->len = len;
    return s;
}

dys dyscat(dys s, const char *t) { return dysncat(s, t, t ? strlen(t) : 0); }

dys dyscatdys(dys s, dys t)
{
    struct dyshdr *ds = header(s);
    size_t oldlen = ds->len;

    if (s != t)
        return dysncat(s, t, dyslen(t));

    s = grow(s, oldlen);
    if (s == NULL)
        return NULL;
    ds = header(s);
    memcpy(s + oldlen, s, oldlen);
    ds->len += oldlen;
    s[ds->len] = '\0';
    return s;
};

dys dysncat(dys s, const char *t, size_t n)
{
    struct dyshdr *ds = header(s);

    if (!t || !n)
        return s;

    s = grow(s, n);
    if (s == NULL)
        return NULL;
    ds = header(s);
    memcpy(s + ds->len, t, n);
    ds->len += n;
    s[ds->len] = '\0';
    return s;
}

dys dyscpy(dys s, const char *t) { return dysncpy(s, t, t ? strlen(t) : 0); }

dys dysncpy(dys s, const char *t, size_t n)
{
    struct dyshdr *ds = header(s);

    if (n > ds->len) {
        s = grow(s, n - ds->len);
        if (s == NULL)
            return NULL;
        ds = header(s);
    }

    if (t)
        memcpy(s, t, n);
    ds->len = n;
    s[ds->len] = '\0';
    return s;
}

dys dystrim(dys s, const char *t)
{
    struct dyshdr *ds = header(s);
    char *sp, *ep;
    size_t len;

    sp = s;
    ep = s + ds->len - 1;
    while (sp <= ep && strchr(t, *sp))
        sp++;
    while (ep > sp && strchr(t, *ep))
        ep--;
    len = (sp > ep) ? 0 : (size_t)(ep - sp + 1);
    if (sp != s)
        memmove(s, sp, len);
    s[len] = '\0';
    ds->len = len;
    return s;
}

int dyscmp(dys s1, dys s2)
{
    size_t l1, l2, minlen;
    int cmp;

    l1 = dyslen(s1);
    l2 = dyslen(s2);
    minlen = l1 < l2 ? l1 : l2;

    cmp = memcmp(s1, s2, minlen);
    if (cmp != 0)
        return cmp;

    return l1 - l2;
}

void dystolower(dys s)
{
    for (size_t i = 0; i < dyslen(s); i++)
        s[i] = tolower((unsigned char)s[i]);
}

void dystoupper(dys s)
{
    for (size_t i = 0; i < dyslen(s); i++)
        s[i] = toupper((unsigned char)s[i]);
}