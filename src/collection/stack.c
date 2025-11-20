/* collection - A generic data structure and algorithms library for modern C
 * Copyright (C) 2025 Yixiang Qiu
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

#include <collection/stack.h>

bool stack_empty(const Stack* stack){
    return stack ? slist_empty(&(stack)->slist) : false;
}

size_t stack_size(const Stack* stack){
    return stack ? slist_size(&(stack)->slist) : 0;
}

void stack_init(Stack* stack, void (*destroy)(void *)){
    if (!stack) return;
    slist_init(&(stack)->slist, destroy);
}

void stack_destroy(Stack* stack){
    if (!stack) return;
    slist_destroy(&(stack)->slist);
}

int stack_push(Stack* stack, void* data){
    return stack ? slist_push_front(&(stack)->slist, data) : COLLECTION_FAILURE;
}

int stack_pop(Stack* stack, void** data){
    return stack ? slist_pop_front(&(stack)->slist, data) : COLLECTION_FAILURE;
}

void* stack_peek(Stack* stack){
    return stack ? slist_data(slist_front(&(stack)->slist)) : NULL;
}

void stack_clear(Stack* stack){
    if (!stack) return;
    slist_clear(&(stack)->slist);
}