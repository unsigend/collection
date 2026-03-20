# Collection - A generic data structure and algorithms library
# Copyright (C) 2025 Yixiang Qiu
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

CUR_DIR 		:= .
ROOT_DIR 		:= ..
BUILD_PATH 		:= $(CUR_DIR)/build
OBJ_PATH 		:= $(BUILD_PATH)/obj
DEP_PATH 		:= $(BUILD_PATH)/dep
CASES_PATH 		:= $(CUR_DIR)/cases
EXTERNAL_PATH 	:= $(CUR_DIR)/external
INCLUDE_PATH 	:= $(ROOT_DIR)/include
LIB_PATH 		:= $(ROOT_DIR)/lib

include ../config/config.mk

HOST_OS := $(shell uname -s)
GCC ?= gcc
CC := $(GCC)
LD := $(CC)

LIB_METHOD ?= $(BUILD_METHOD)
ifeq ($(strip $(LIB_METHOD)),)
LIB_METHOD := static
endif

LIB_NAME ?= $(LIBRARY_NAME)
ifeq ($(strip $(LIB_NAME)),)
LIB_NAME := collection
endif

DEBUG_FLAG := $(filter true 1,$(DEBUG))

ifeq ($(LIB_METHOD),static)
LIB_POSTFIX := .a
else
ifeq ($(HOST_OS),Darwin)
LIB_POSTFIX := .dylib
else
LIB_POSTFIX := .so
endif
endif

SRCS_CASE 	:= $(shell find $(CASES_PATH) -name "*.c")
SRCS_EXT 	:= $(shell find $(EXTERNAL_PATH)/src -name "*.c")
OBJS_CASE 	:= $(patsubst $(CASES_PATH)/%.c,$(OBJ_PATH)/%.o,$(SRCS_CASE))
OBJS_EXT 	:= $(patsubst $(EXTERNAL_PATH)/src/%.c,$(OBJ_PATH)/external/%.o,$(SRCS_EXT))
OBJS 		:= $(OBJS_CASE) $(OBJS_EXT)
DEPS 		:= $(patsubst $(OBJ_PATH)/%.o,$(DEP_PATH)/%.d,$(OBJS))

CC_FLAGS := -std=$(STD_C)
CC_FLAGS += -Wall -Wextra -Werror
CC_FLAGS += -I$(INCLUDE_PATH) -I$(EXTERNAL_PATH)/include

ifeq ($(HOST_OS),Linux)
CC_FLAGS += -D_GNU_SOURCE
CC_FLAGS += -Wno-type-limits -Wno-sign-compare
endif

ifneq ($(DEBUG_FLAG),)
CC_FLAGS += -fsanitize=address,undefined,bounds
CC_FLAGS += -g -O0
else
CC_FLAGS += -O2
endif

CC_DEPS_FLAGS := -MMD -MP -MF
LD_FLAGS := -L$(LIB_PATH) -l$(LIB_NAME)
ifneq ($(DEBUG_FLAG),)
LD_FLAGS += -fsanitize=address,undefined,bounds
endif

$(OBJ_PATH)/%.o: $(CASES_PATH)/%.c
	@mkdir -p $(dir $@) $(dir $(DEP_PATH)/$*.d)
	@$(CC) $(CC_FLAGS) $(CC_DEPS_FLAGS) $(DEP_PATH)/$*.d -MT $@ -c $< -o $@
	@echo " + CC\ttest/$@"

$(OBJ_PATH)/external/%.o: $(EXTERNAL_PATH)/src/%.c
	@mkdir -p $(dir $@) $(dir $(DEP_PATH)/external/$*.d)
	@$(CC) $(CC_FLAGS) $(CC_DEPS_FLAGS) $(DEP_PATH)/external/$*.d -MT $@ -c $< -o $@
	@echo " + CC\ttest/$@"

-include $(DEPS)

.SECONDARY: $(DEPS)
.DEFAULT_GOAL := test
.PHONY: all test test-% clean flags

all: $(OBJS)
	@mkdir -p $(BUILD_PATH)
	@$(LD) $(OBJS) $(LD_FLAGS) -o $(BUILD_PATH)/test
	@echo " + LD\ttest/$(notdir $(BUILD_PATH))/test"
	@echo ""

test: all
	@$(BUILD_PATH)/test

test-%: all
	@$(BUILD_PATH)/test -m $*

clean:
	@rm -rf $(BUILD_PATH)

flags:
	@echo "TEST_LIB_METHOD: $(LIB_METHOD)"
	@echo "TEST_CC_FLAGS: $(CC_FLAGS)"
	@echo "TEST_LD_FLAGS: $(LD_FLAGS)"