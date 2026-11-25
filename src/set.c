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

#include <set.h>

int set_init(struct set *s, struct dict_fns *fns)
{
    return dict_init(&s->dict, fns);
}

void set_free(struct set *s)
{
    if (s == NULL)
        return;
    dict_free(&s->dict);
}

void set_clear(struct set *s) { dict_clear(&s->dict); }

bool set_contains(struct set *s, void *key) { return dict_has(&s->dict, key); }
int set_add(struct set *s, void *key) { return dict_sets(&s->dict, key, NULL); }
int set_rm(struct set *s, void *key) { return dict_del(&s->dict, key, NULL); }
int set_dup(struct set *s, struct set *dup)
{
    return dict_dup(&s->dict, &dup->dict);
}

void set_iter_init(struct set_iter *iter, struct set *set)
{
    dict_iter_init(&iter->iter, &set->dict);
}

void set_iter_next(struct set_iter *iter) { dict_iter_next(&iter->iter); }

bool set_iter_done(struct set_iter *iter)
{
    return dict_iter_done(&iter->iter);
}

void *set_iter_get(struct set_iter *iter)
{
    struct dict_pair *pair = dict_iter_get(&iter->iter);
    return pair ? pair->key : NULL;
}