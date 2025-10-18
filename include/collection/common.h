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

#ifndef COLLECTION_COMMON_H
#define COLLECTION_COMMON_H

#include <stddef.h>

#define COLLECTION_SUCCESS 0
#define COLLECTION_FAILURE -1

#define COLLECTION_TRUE 1
#define COLLECTION_FALSE 0

#define COLLECTION_MIN(a, b) ((a) < (b) ? (a) : (b))
#define COLLECTION_MAX(a, b) ((a) > (b) ? (a) : (b))


#endif