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

CUR_DIR		:= .
ROOT_DIR	:= ../../..
BUILD_PATH	:= $(ROOT_DIR)/test/build
SRC_PATH	:= $(ROOT_DIR)/src
INCLUDE_PATH	:= $(ROOT_DIR)/include

include $(ROOT_DIR)/config/config.mk

GCC ?= gcc
CC := $(GCC)

NAME := dys
BENCH := $(BUILD_PATH)/bench-$(NAME)

CC_FLAGS := -std=$(STD_C)
CC_FLAGS += -O2 -DNDEBUG
CC_FLAGS += -Wall -Wextra -Werror
CC_FLAGS += -I$(INCLUDE_PATH)

.DEFAULT_GOAL := all
.PHONY: all clean

all: $(BENCH)
	@$(BENCH) $(ITERS)

$(BENCH): dys.c $(SRC_PATH)/dys.c
	@mkdir -p $(BUILD_PATH)
	@$(CC) $(CC_FLAGS) dys.c $(SRC_PATH)/dys.c -o $(BENCH)
	@echo " + LD\ttest/build/bench-$(NAME)"

clean:
	@rm -f $(BENCH)
