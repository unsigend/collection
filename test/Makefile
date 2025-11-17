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

# Main Makefile for Collection tests

# variables for tools
CC            :=          gcc
CXX           :=          g++
LD            :=          gcc
AR            :=          ar

# variables for paths
CUR_DIR       		:=          .
ROOT_DIR      		:=          ..
CUR_DIR_NAME        :=          $(shell basename $(abspath $(CUR_DIR)))
INCLUDE_PATH  		:=          $(ROOT_DIR)/include
LIB_PATH      		:=          $(ROOT_DIR)/lib
TESTS_PATH          :=          $(CUR_DIR)/unit
BENCHMARKS_PATH     :=          $(CUR_DIR)/benchmark
BUILD_PATH    		:=          $(CUR_DIR)/build
DEP_PATH      		:=          $(BUILD_PATH)/dep
OBJ_PATH      		:=          $(BUILD_PATH)/obj
EXTERNAL_PATH       :=          $(CUR_DIR)/external

# variables for flags
CC_FLAGS      :=          -std=$(STD_C)
CC_FLAGS      +=  		  -Wall -Wextra -Werror
CC_FLAGS      +=          -Wno-unused-parameter
CC_FLAGS      +=          -Wno-unused-variable
CC_FLAGS      +=          -Wno-unused-function
CC_FLAGS      +=          -O2
CC_FLAGS      +=          -I$(INCLUDE_PATH)
CC_FLAGS      +=          -I$(EXTERNAL_PATH)/include
ifeq ($(DEBUG), true)
CC_FLAGS      +=          -fsanitize=address
CC_FLAGS      +=          -fno-omit-frame-pointer
CC_FLAGS      +=          -g
endif
CC_DEPS_FLAGS :=          -MMD -MP -MF
LD_FLAGS      :=          -L$(LIB_PATH) -l$(LIBRARY_NAME)
ifeq ($(DEBUG), true)
LD_FLAGS      +=          -fsanitize=address
endif

# source files
TEST_SRCS           :=          $(shell find $(TESTS_PATH) -name "*.c")
EXTERNAL_SRCS       :=          $(shell find $(EXTERNAL_PATH)/src -name "*.c")
BENCHMARK_SRCS      :=          $(shell find $(BENCHMARKS_PATH) -name "*.c")

# objects
TEST_OBJS           :=          $(patsubst $(TESTS_PATH)/%.c, $(OBJ_PATH)/%.o, $(TEST_SRCS))
BENCHMARK_OBJS      :=          $(patsubst $(BENCHMARKS_PATH)/%.c, $(OBJ_PATH)/%.o, $(BENCHMARK_SRCS))
EXTERNAL_OBJS       :=          $(patsubst $(EXTERNAL_PATH)/src/%.c, $(OBJ_PATH)/%.o, $(EXTERNAL_SRCS))

# dependencies
TEST_DEPS           :=          $(patsubst $(TESTS_PATH)/%.c, $(DEP_PATH)/%.d, $(TEST_SRCS))
BENCHMARK_DEPS      :=          $(patsubst $(BENCHMARKS_PATH)/%.c, $(DEP_PATH)/%.d, $(BENCHMARK_SRCS))

# final dependencies OBJS
OBJS_TEST           :=          $(TEST_OBJS) $(EXTERNAL_OBJS)
OBJS_BENCHMARK     :=          $(BENCHMARK_OBJS) $(EXTERNAL_OBJS)

# general rules for all tests
$(OBJ_PATH)/%.o: $(TESTS_PATH)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CC_FLAGS) $(CC_DEPS_FLAGS) $(DEP_PATH)/$(notdir $(@F:.o=.d)) -c $< -o $@
	@echo " + CC\t$(CUR_DIR_NAME)/$@"

# general rules for all benchmarks
$(OBJ_PATH)/%.o: $(BENCHMARKS_PATH)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CC_FLAGS) $(CC_DEPS_FLAGS) $(DEP_PATH)/$(notdir $(@F:.o=.d)) -c $< -o $@
	@echo " + CC\t$(CUR_DIR_NAME)/$@"

# general rules for all external objects
$(OBJ_PATH)/%.o: $(EXTERNAL_PATH)/src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CC_FLAGS) -c $< -o $@
	@echo " + CC\t$(CUR_DIR_NAME)/$@"

# include the dependencies
-include $(TEST_DEPS)
-include $(BENCHMARK_DEPS)

.DEFAULT_GOAL := test
.PHONY: test benchmark create_build_dir

# create build directory target
create_build_dir:
	@mkdir -p $(BUILD_PATH)
	@mkdir -p $(OBJ_PATH)
	@mkdir -p $(DEP_PATH)

# clean target
clean:
	@rm -rf $(BUILD_PATH)

# unit test
test: create_build_dir $(OBJS_TEST)
	@$(CC) $(LD_FLAGS) -o $(BUILD_PATH)/test $(OBJS_TEST) \
	$(LIB_PATH)/lib$(LIBRARY_NAME)$(LIB_POSTFIX)
	@echo " + LD\t$(CUR_DIR_NAME)/$(notdir $(BUILD_PATH))/test"
	@echo "Build unit test ./test/build/$@\n"

# unit test module target
test-%: 
	@$(MAKE) test
	@$(BUILD_PATH)/test -m $*

# benchmark test
benchmark: create_build_dir $(OBJS_BENCHMARK)
	@$(CC) $(LD_FLAGS) -o $(BUILD_PATH)/benchmark $(OBJS_BENCHMARK) \
	$(LIB_PATH)/lib$(LIBRARY_NAME)$(LIB_POSTFIX)
	@echo " + LD\t$(CUR_DIR_NAME)/$(notdir $(BUILD_PATH))/benchmark"
	@echo "Build benchmark test ./test/build/$@\n"
