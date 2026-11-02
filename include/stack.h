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

#ifndef _STACK_H
#define _STACK_H

#include <dvec.h>
#include <stddef.h>

/* Last-in, first-out (LIFO) stack. Based on dvec. */

struct stack {
    struct dvec vec;
};

#define stack_len(s) ((s)->vec.len)
#define stack_elesz(s) ((s)->vec.elesz)
#define stack_empty(s) ((s)->vec.len == 0)
#define stack_full(s) ((s)->vec.len == (s)->vec.cap)
#define stack_avail(s) ((s)->vec.cap - (s)->vec.len)

extern int stack_init(struct stack *s, size_t elesz, size_t cap);
extern int stack_init_empty(struct stack *s, size_t elesz);

extern void stack_free(struct stack *s);
extern void stack_clear(struct stack *s);

extern int stack_push(struct stack *s, const void *ele);
extern int stack_pop(struct stack *s, void *ele);
extern int stack_peek(struct stack *s, void *ele);

#endif
