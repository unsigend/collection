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

CUR_DIR   		:= .
SRC_PATH 		:= $(CUR_DIR)/src
INCLUDE_PATH 	:= $(CUR_DIR)/include
CONFIG_PATH 	:= $(CUR_DIR)/config
BUILD_PATH 		:= $(CUR_DIR)/build
OBJ_PATH 		:= $(BUILD_PATH)/obj
DEP_PATH 		:= $(BUILD_PATH)/dep
LIB_PATH 		:= $(CUR_DIR)/lib
TEST_PATH 		:= $(CUR_DIR)/test

include $(CONFIG_PATH)/config.mk

HOST_OS := $(shell uname -s)
GCC ?= gcc
AR ?= ar

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

SRCS := $(shell find $(SRC_PATH) -name "*.c")
OBJS := $(patsubst $(SRC_PATH)/%.c,$(OBJ_PATH)/%.o,$(SRCS))
DEPS := $(patsubst $(SRC_PATH)/%.c,$(DEP_PATH)/%.d,$(SRCS))
LIB_TARGET := $(LIB_PATH)/lib$(LIB_NAME)$(LIB_POSTFIX)

CC_FLAGS := -std=$(STD_C)
CC_FLAGS += -Wall -Wextra -Werror
CC_FLAGS += -I$(INCLUDE_PATH)
CC_FLAGS += -fPIC

ifneq ($(DEBUG_FLAG),)
CC_FLAGS += -fsanitize=address,undefined,bounds
CC_FLAGS += -g -O0
else
CC_FLAGS += -O2
endif

CC_DEPS_FLAGS := -MMD -MP -MF
AR_FLAGS := -rcs

LD_FLAGS :=
ifneq ($(DEBUG_FLAG),)
LD_FLAGS += -fsanitize=address,undefined,bounds
endif

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(dir $@) $(dir $(DEP_PATH)/$*.d) $(LIB_PATH)
	@$(GCC) $(CC_FLAGS) $(CC_DEPS_FLAGS) $(DEP_PATH)/$*.d -MT $@ -c $< -o $@
	@echo " + CC\t$@"

$(LIB_TARGET): $(OBJS)
	@mkdir -p $(LIB_PATH)
ifeq ($(LIB_METHOD),static)
	@$(AR) $(AR_FLAGS) $@ $(OBJS)
	@echo " + AR\t$@"
else
	@$(GCC) $(LD_FLAGS) -shared $(OBJS) -o $@
	@echo " + LD\t$@"
endif
	@echo "Build library ./$@"
	@echo ""

-include $(DEPS)

.DEFAULT_GOAL := help
.PHONY: all lib test test-% clean help docker flags clang format

all: lib
lib: $(LIB_TARGET)

test: lib
	@$(MAKE) -C $(TEST_PATH) test

test-%: lib
	@$(MAKE) -C $(TEST_PATH) test-$*

clean:
	@rm -rf $(BUILD_PATH) $(LIB_PATH)
	@$(MAKE) -C $(TEST_PATH) clean

help:
	@echo "Usage:"
	@echo "  make all       - Build the library and run all tests"
	@echo "  make lib       - Build the library"
	@echo "  make test      - Build and run all tests"
	@echo "  make test-NAME - Build and run tests for a specific module"
	@echo "  make clean     - Clean the build artifacts"
	@echo "  make flags     - Show the compile and link flags"
	@echo "  make clang     - Run clang to generate compile commands"
	@echo "  make format    - Format the code"
	@echo "  make docker    - Run Docker container with development environment"
	@echo ""

docker:
	@if [ -z "$$(docker images -q $(DOCKER_IMAGE) 2>/dev/null)" ]; then \
		echo "Building Docker image $(DOCKER_IMAGE)..."; \
		docker build -t $(DOCKER_IMAGE) -f Dockerfile .; \
	fi
	@if [ -n "$$(docker ps -aq -f name=$(DOCKER_IMAGE)-container 2>/dev/null)" ]; then \
		docker rm -f $(DOCKER_IMAGE)-container 2>/dev/null || true; \
	fi
	@docker run -it --name $(DOCKER_IMAGE)-container -v $(CUR_DIR):/workspace $(DOCKER_IMAGE) /bin/bash

flags:
	@echo "LIB_METHOD: $(LIB_METHOD)"
	@echo "LIB_TARGET: $(LIB_TARGET)"
	@echo "CC_FLAGS: $(CC_FLAGS)"
	@echo "LD_FLAGS: $(LD_FLAGS)"
	@$(MAKE) -C $(TEST_PATH) flags

clang:
	@bear -- make test

format:
	@find $(INCLUDE_PATH) $(SRC_PATH) $(TEST_PATH)/cases \
		\( -name "*.c" -o -name "*.h" \) -exec clang-format -i {} +
	@echo "Format done."

export STD_C
export DEBUG
export LIB_METHOD
export LIB_NAME
export LIB_POSTFIX
export LIBRARY_NAME