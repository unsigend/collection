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

#ifndef _SET_H
#define _SET_H

#include <dict.h>
#include <stdbool.h>
#include <stddef.h>

struct set {
    struct dict dict;
};

struct set_iter {
    struct dict_iter iter;
};

#define set_len(s) (dict_len(&(s)->dict))
#define set_empty(s) (dict_empty(&(s)->dict))

extern int set_init(struct set *s, struct dict_fns *fns);
extern void set_free(struct set *s);
extern void set_clear(struct set *s);

extern bool set_contains(struct set *s, void *key);
extern int set_add(struct set *s, void *key);
extern int set_rm(struct set *s, void *key);
extern int set_dup(struct set *s, struct set *dup);

extern void set_iter_init(struct set_iter *iter, struct set *set);
extern void set_iter_next(struct set_iter *iter);
extern bool set_iter_done(struct set_iter *iter);
extern void *set_iter_get(struct set_iter *iter);

#endif