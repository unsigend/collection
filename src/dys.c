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
#include <stdlib.h>
#include <string.h>

#define MAX_PREALLOC (1024 * 1024) /* 1MB */

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

/*
 * SDS-style binary-safe string. One malloc: header + buf[cap] + NUL.
 * The handle points at buf.
 *
 *  - 0 <= len <= cap, buf[len] == '\0', [0, len) may contain NULs.
 *
 * expand() is the only realloc. It makes avail >= addlen and doubles
 * until MAX_PREALLOC. Failure returns NULL, the old string is unchanged.
 * A moved buffer invalidates the old handle.
 *
 * ncat/ncpy sources must not overlap the destination. catdys(s, s)
 * dups first. grow zero-fills up to len. NULL free is a no-op.
 */

static dys expand(dys s, size_t addlen)
{
    struct dyshdr *hdr = dysgethdr(s);
    struct dyshdr *newhdr;
    size_t newlen;

    if (addlen <= dysavail(s))
        return s;

    newlen = hdr->len + addlen;
    if (newlen < MAX_PREALLOC)
        newlen *= 2;
    else
        newlen += MAX_PREALLOC;

    newhdr = realloc(hdr, sizeof(struct dyshdr) + newlen + 1);
    if (newhdr == NULL)
        return NULL;

    newhdr->cap = newlen;
    return newhdr->buf;
}

dys dysnewlen(const void *s, size_t len)
{
    struct dyshdr *hdr;

    hdr = malloc(sizeof(struct dyshdr) + len + 1);
    if (hdr == NULL)
        return NULL;

    hdr->len = len;
    hdr->cap = len;
    if (s)
        memcpy(hdr->buf, s, len);
    else
        memset(hdr->buf, 0, len + 1);

    hdr->buf[len] = '\0';

    return hdr->buf;
}

dys dysnew(const char *s) { return dysnewlen(s, s ? strlen(s) : 0); }
dys dysnewempty(void) { return dysnewlen("", 0); }

void dysclear(dys s)
{
    struct dyshdr *hdr = dysgethdr(s);
    hdr->len = 0;
    hdr->buf[0] = '\0';
}

void dysfree(dys s)
{
    if (s == NULL)
        return;
    free(dysgethdr(s));
}

dys dysgrow(dys s, size_t len)
{
    dys new;
    size_t oldlen, addlen;

    oldlen = dyslen(s);

    if (len <= oldlen)
        return s;

    addlen = len - oldlen;
    new = expand(s, addlen);
    if (new == NULL)
        return NULL;
    if (addlen)
        memset(new + oldlen, 0, addlen);
    dysgethdr(new)->len = len;
    new[len] = '\0';
    return new;
}

dys dysdup(const dys s) { return dysndup(s, dyslen(s)); }
dys dysndup(const dys s, size_t len)
{
    struct dyshdr *hdr = dysgethdr(s);
    return dysnewlen(hdr->buf, MIN(len, dyslen(s)));
}

dys dyscat(dys s, const char *t) { return dysncat(s, t, t ? strlen(t) : 0); }
dys dysncat(dys s, const void *t, size_t len)
{
    dys new;
    size_t oldlen;

    if (len <= dysavail(s)) {
        memcpy(s + dyslen(s), t, len);
        dysgethdr(s)->len += len;
        s[dyslen(s)] = '\0';
        return s;
    }

    oldlen = dyslen(s);
    new = expand(s, len);
    if (new == NULL)
        return NULL;
    if (len)
        memcpy(new + oldlen, t, len);
    dysgethdr(new)->len += len;
    new[oldlen + len] = '\0';
    return new;
}

dys dyscatdys(dys s, const dys t)
{
    dys tmp, new;

    if (s != t)
        return dysncat(s, t, dyslen(t));

    tmp = dysdup(t);
    if (tmp == NULL)
        return NULL;
    new = dysncat(s, tmp, dyslen(tmp));
    dysfree(tmp);
    return new;
}

dys dyscpy(dys s, const char *t) { return dysncpy(s, t, t ? strlen(t) : 0); }
dys dysncpy(dys s, const void *t, size_t len)
{
    dys new;

    if (len <= dyscap(s)) {
        memcpy(s, t, len);
        dysgethdr(s)->len = len;
        s[len] = '\0';
        return s;
    }

    new = expand(s, len);
    if (new == NULL)
        return NULL;
    if (len)
        memcpy(new, t, len);
    dysgethdr(new)->len = len;
    new[len] = '\0';
    return new;
}

void dystrim(dys s, const char *t)
{
    size_t start, end, len;

    start = 0;
    end = dyslen(s);
    while (start < end && strchr(t, s[start]))
        start++;
    while (end > start && strchr(t, s[end - 1]))
        end--;
    len = end - start;
    if (start)
        memmove(s, s + start, len);
    dysgethdr(s)->len = len;
    s[len] = '\0';
}

int dyscmp(const dys s1, const dys s2)
{
    int cmp;

    cmp = memcmp(s1, s2, MIN(dyslen(s1), dyslen(s2)));
    if (cmp)
        return cmp;
    if (dyslen(s1) == dyslen(s2))
        return 0;
    return dyslen(s1) < dyslen(s2) ? -1 : 1;
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