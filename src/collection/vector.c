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

#include <stdlib.h>
#include <string.h>

#include <collection/vector.h>

void vector_init(Vector* vector, void (*destroy)(void *)){
    // allocate memory for the vector stuct
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
    vector->destroy = destroy;
}


void vector_destroy(Vector* vector){
    // destroy the elements if the destroy function is provided
    if (vector->destroy) {
        for (size_t i = 0; i < vector->size; i++) {
            vector->destroy(vector->data[i]);
        }
    }

    free(vector->data);
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
    vector->destroy = NULL;
}