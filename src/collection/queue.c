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

#include <collection/queue.h>

bool queue_empty(const Queue* queue){
    return slist_empty(&(queue)->slist);
}

size_t queue_size(const Queue* queue){
    return slist_size(&(queue)->slist);
}

void queue_init(Queue* queue, void (*destroy)(void *)){
    slist_init(&(queue)->slist, destroy);
}

void queue_destroy(Queue* queue){
    slist_destroy(&(queue)->slist);
}

int queue_enqueue(Queue* queue, void* data){
    return slist_push_back(&(queue)->slist, data);
}

int queue_dequeue(Queue* queue, void** data){
    return slist_pop_front(&(queue)->slist, data);
}

void* queue_peek(Queue* queue){
    return slist_data(slist_front(&(queue)->slist));
}

void queue_clear(Queue* queue){
    slist_clear(&(queue)->slist);
}