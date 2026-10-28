# collection - A high-performance C data structure library
# Copyright (C) 2026-2027 Yixiang Qiu
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

# Configuration for Collection

# variables for tools
GCC             :=          gcc
CXX             :=          g++
LD              :=          gcc
AR              :=          ar

# C standard
STD_C           :=          c11

# build method
BUILD_METHOD    :=          static

# library name
LIBRARY_NAME    :=          collection

# debugger mode
DEBUG           :=          true

# docker config
DOCKER_IMAGE    :=          collection