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

#ifndef _DYS_H_
#define _DYS_H_

/* Dynamic string library implementation, compatible with ANSI/ISO string
 * library, binary safe and null-terminated. Design idea from Redis sds with
 * simplifications. */

#include <stddef.h>

typedef char *dys;

#ifdef __cplusplus
extern "C" {
#endif

dys dysinit(const char *s);
dys dysinitlen(const void *s, size_t len);
dys dysinit_empty(void);
void dysfree(dys s);

size_t dyslen(dys s);
size_t dyscap(dys s);
size_t dysavail(dys s);

dys dysdup(dys s);
dys dysclear(dys s);
dys dysgrow(dys s, size_t len);
dys dyscat(dys s, const char *t);
dys dysncat(dys s, const char *t, size_t n);
dys dyscatdys(dys s, dys t);
dys dyscpy(dys s, const char *t);
dys dysncpy(dys s, const char *t, size_t n);
dys dystrim(dys s, const char *t);
int dyscmp(dys s1, dys s2);
void dystolower(dys s);
void dystoupper(dys s);

#ifdef __cplusplus
}
#endif

#endif /* _DYS_H_ */