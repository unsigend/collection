# Collection - A generic data structure and algorithms library for modern C
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

# Main Makefile for Collection

# variables for paths
CUR_DIR         :=          .
SRC_PATH 		:= 			$(CUR_DIR)/src
CONFIG_PATH 	:= 			$(CUR_DIR)/config
INCLUDE_PATH 	:= 			$(CUR_DIR)/include
TEST_PATH 		:= 			$(CUR_DIR)/test
BUILD_PATH 		:= 			$(CUR_DIR)/build
OBJ_PATH 		:= 			$(BUILD_PATH)/obj
DEP_PATH 		:= 			$(BUILD_PATH)/dep
LIB_PATH 		:= 			$(CUR_DIR)/lib

# include config
include $(CONFIG_PATH)/config.mk

# variables for flags
CC_FLAGS        :=          -std=$(STD_C)
CC_FLAGS        +=          -Wall -Wextra -Werror
CC_FLAGS        +=          -Wno-unused-parameter
CC_FLAGS        +=          -fPIC
CC_FLAGS        +=          -O2
CC_FLAGS        +=          -I$(INCLUDE_PATH)
CC_DEPS_FLAGS   :=          -MMD -MP -MF
AR_FLAGS        :=          -rcs

SRCS            :=          $(shell find $(SRC_PATH) -name "*.c")
OBJS            :=          $(patsubst $(SRC_PATH)/%.c, $(OBJ_PATH)/%.o, $(SRCS))
DEPS            :=          $(patsubst $(SRC_PATH)/%.c, $(DEP_PATH)/%.d, $(SRCS))

# variables for host OS
HOST_OS         :=          $(shell uname -s)

# variable for dynamic library postfix
# for now it supports only Linux and macOS
DYLIB_POSTFIX   	:=          $(if $(findstring Darwin, $(HOST_OS)),.dylib,.so)
STATIC_LIB_POSTFIX  :=          .a

ifeq ($(BUILD_METHOD), static)
LIB_POSTFIX         :=          $(STATIC_LIB_POSTFIX)
else
LIB_POSTFIX         :=          $(DYLIB_POSTFIX)
endif

# general rules for all objects
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(dir $@)
	@$(GCC) $(CC_FLAGS) $(CC_DEPS_FLAGS) $(DEP_PATH)/$(notdir $(@F:.o=.d)) -c $< -o $@
	@echo " + CC\t$@"

$(LIB_PATH)/lib$(LIBRARY_NAME)$(LIB_POSTFIX): $(OBJS)
ifeq ($(BUILD_METHOD), static)
	@$(AR) $(AR_FLAGS) $@ $(OBJS)
	@echo " + AR\t$@"
else
	@$(GCC) $(LD_FLAGS) -shared $(OBJS) -o $@
	@echo " + LD\t$@"
endif
	@echo "Build $(LIBRARY_NAME) library ./$@"
	@echo ""

# export the variables to the sub-make
export STD_C
export LIBRARY_NAME
export LIB_POSTFIX

# include dependency files
-include $(DEP_PATH)/*.d

.DEFAULT_GOAL := help
.PHONY: all clean test help create_build_dir lib test-bench

# lib target
lib: create_build_dir $(LIB_PATH)/lib$(LIBRARY_NAME)$(LIB_POSTFIX)

# default goal
all: lib

# clean target
clean:
	@rm -rf $(BUILD_PATH)
	@rm -rf $(LIB_PATH)
	@$(MAKE) -C $(TEST_PATH) clean

# test target
test: lib
	@$(MAKE) -C $(TEST_PATH) test

# test-bench target
test-bench: lib
	@$(MAKE) -C $(TEST_PATH) benchmark

# help target
help:
	@echo "Collection - A generic data structure and algorithms library for modern C"
	@echo "Copyright (C) 2025 Yixiang Qiu"
	@echo "Usage:"
	@echo "\tmake all         - Build the library and tests"
	@echo "\tmake clean       - Clean the build directory"
	@echo "\tmake test        - Run the tests"
	@echo "\tmake test-bench  - Run the benchmark tests"
	@echo "\tmake help        - Show this help message"
	@echo ""

# generate dir
create_build_dir:
	@mkdir -p $(BUILD_PATH)
	@mkdir -p $(OBJ_PATH)
	@mkdir -p $(DEP_PATH)
	@mkdir -p $(LIB_PATH)