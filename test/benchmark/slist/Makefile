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
CXX ?= g++
CC := $(GCC)

NAME := slist
BENCH := $(BUILD_PATH)/bench-$(NAME)
OBJ := $(BUILD_PATH)/bench-$(NAME)-src.o

CC_FLAGS := -std=$(STD_C)
CC_FLAGS += -O2 -DNDEBUG
CC_FLAGS += -Wall -Wextra -Werror
CC_FLAGS += -I$(INCLUDE_PATH)

CXX_FLAGS := -std=c++11
CXX_FLAGS += -O2 -DNDEBUG
CXX_FLAGS += -Wall -Wextra -Werror
CXX_FLAGS += -I$(INCLUDE_PATH)

.DEFAULT_GOAL := all
.PHONY: all clean

all: $(BENCH)
	@$(BENCH) $(ITERS)

$(OBJ): $(SRC_PATH)/slist.c
	@mkdir -p $(BUILD_PATH)
	@$(CC) $(CC_FLAGS) -c $(SRC_PATH)/slist.c -o $(OBJ)
	@echo " + CC\ttest/build/bench-$(NAME)-src.o"

$(BENCH): slist.cc $(OBJ)
	@mkdir -p $(BUILD_PATH)
	@$(CXX) $(CXX_FLAGS) slist.cc $(OBJ) -o $(BENCH)
	@echo " + LD\ttest/build/bench-$(NAME)"

clean:
	@rm -f $(BENCH) $(OBJ)
