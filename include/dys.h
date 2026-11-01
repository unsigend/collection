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

#ifndef _DYS_H
#define _DYS_H

#include <stdbool.h>
#include <stddef.h>

/* Simple dynamic string implementation, originally from redis sds. Compatible
 * with ANSI C string functions and binary safe. */

typedef char *dys;

struct dyshdr {
    size_t len;
    size_t cap;
    char buf[];
};

#define dysgethdr(s) ((struct dyshdr *)((s) - sizeof(struct dyshdr)))
#define dyslen(s) (dysgethdr(s)->len)
#define dyscap(s) (dysgethdr(s)->cap)
#define dysavail(s) (dyscap(s) - dyslen(s))
#define dysempty(s) (dyslen(s) == 0)

extern dys dysnewlen(const void *s, size_t len);
extern dys dysnew(const char *s);
extern dys dysnewempty(void);

extern void dysclear(dys s);
extern void dysfree(dys s);

extern dys dysgrow(dys s, size_t len);
extern dys dysdup(const dys s);
extern dys dysndup(const dys s, size_t len);
extern dys dyscat(dys s, const char *t);
extern dys dysncat(dys s, const void *t, size_t len);
extern dys dyscatdys(dys s, const dys t);
extern dys dyscpy(dys s, const char *t);
extern dys dysncpy(dys s, const void *t, size_t len);
extern void dystrim(dys s, const char *t);

extern int dyscmp(const dys s1, const dys s2);
extern void dystolower(dys s);
extern void dystoupper(dys s);

#endif /* _DYS_H */