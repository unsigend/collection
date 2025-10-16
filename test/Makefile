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

# variables for flags
CC_FLAGS      :=          -std=$(STD_C)
CC_FLAGS      +=  		  -Wall -Wextra -Werror
CC_FLAGS      +=          -Wno-unused-parameter
CC_FLAGS      +=          -O2
CC_FLAGS      +=          -I$(INCLUDE_PATH)
CC_DEPS_FLAGS :=          -MMD -MP -MF
LD_FLAGS      :=          -L$(LIB_PATH) -l$(LIBRARY_NAME)

TEST_SRCS           :=          $(shell find $(TESTS_PATH) -name "*.c")
BENCHMARK_SRCS      :=          $(shell find $(BENCHMARKS_PATH) -name "*.c")
TEST_OBJS           :=          $(patsubst $(TESTS_PATH)/%.c, $(OBJ_PATH)/%.o, $(TEST_SRCS))
BENCHMARK_OBJS      :=          $(patsubst $(BENCHMARKS_PATH)/%.c, $(OBJ_PATH)/%.o, $(BENCHMARK_SRCS))
TEST_DEPS           :=          $(patsubst $(TESTS_PATH)/%.c, $(DEP_PATH)/%.d, $(TEST_SRCS))
BENCHMARK_DEPS      :=          $(patsubst $(BENCHMARKS_PATH)/%.c, $(DEP_PATH)/%.d, $(BENCHMARK_SRCS))

TEST_MAIN_OBJ       :=          $(OBJ_PATH)/main.o
BENCHMARK_MAIN_OBJ  :=          $(OBJ_PATH)/benchmark.main.o

TEST_OBJS           +=          $(TEST_MAIN_OBJ)
BENCHMARK_OBJS      +=          $(BENCHMARK_MAIN_OBJ)

TEST_DEPS           +=          $(TEST_MAIN_OBJ:.o=.d)
BENCHMARK_DEPS      +=          $(BENCHMARK_MAIN_OBJ:.o=.d)

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

# rules for main objects
$(OBJ_PATH)/%.o: $(CUR_DIR)/%.c
	@$(CC) $(CC_FLAGS) $(CC_DEPS_FLAGS) $(DEP_PATH)/$(notdir $(@F:.o=.d)) -c $< -o $@
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
test: create_build_dir $(TEST_OBJS)
	@$(CC) $(LD_FLAGS) -o $(BUILD_PATH)/test $(TEST_OBJS) \
	$(LIB_PATH)/lib$(LIBRARY_NAME)$(LIB_POSTFIX)
	@echo " + LD\t$(CUR_DIR_NAME)/$(notdir $(BUILD_PATH))/test"
	@echo "Build unit test ./test/build/$@\n"

# benchmark test
benchmark: create_build_dir $(BENCHMARK_OBJS)
	@$(CC) $(LD_FLAGS) -o $(BUILD_PATH)/benchmark $(BENCHMARK_OBJS) \
	$(LIB_PATH)/lib$(LIBRARY_NAME)$(LIB_POSTFIX)
	@echo " + LD\t$(CUR_DIR_NAME)/$(notdir $(BUILD_PATH))/benchmark"
	@echo "Build benchmark test ./test/build/$@\n"