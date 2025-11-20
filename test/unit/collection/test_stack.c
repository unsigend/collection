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

#include <collection/stack.h>
#include <collection/common.h>
#include <utest.h>
#include <stdlib.h>
#include <string.h>

/* Global destroy counter for testing */
static int destroy_count = 0;

static void destroy_counter(void* data) {
    if (data != NULL) {
        destroy_count++;
    }
}

/**
 * Test: stack_init
 * Dependencies: None
 * Description: Tests basic initialization of the stack structure.
 */
UTEST_TEST_CASE(stack_init){
    // Test 1: Initialize with NULL destroy function
    {
        Stack stack;
        stack_init(&stack, NULL);
        EXPECT_EQUAL_UINT(stack_size(&stack), 0);
        EXPECT_TRUE(stack_empty(&stack));
        stack_destroy(&stack);
    }

    // Test 2: Initialize with non-NULL destroy function
    {
        Stack stack;
        stack_init(&stack, free);
        EXPECT_EQUAL_UINT(stack_size(&stack), 0);
        EXPECT_TRUE(stack_empty(&stack));
        stack_destroy(&stack);
    }
}

/**
 * Test: stack_size
 * Dependencies: stack_init
 * Description: Tests the stack_size function to get the number of elements.
 */
UTEST_TEST_CASE(stack_size){
    // Test 1: Size of empty stack
    {
        Stack stack;
        stack_init(&stack, NULL);
        EXPECT_EQUAL_UINT(stack_size(&stack), 0);
        stack_destroy(&stack);
    }

    // Test 2: Size after push operations
    {
        Stack stack;
        stack_init(&stack, NULL);
        int a = 1, b = 2, c = 3;
        stack_push(&stack, &a);
        EXPECT_EQUAL_UINT(stack_size(&stack), 1);
        stack_push(&stack, &b);
        EXPECT_EQUAL_UINT(stack_size(&stack), 2);
        stack_push(&stack, &c);
        EXPECT_EQUAL_UINT(stack_size(&stack), 3);
        stack_destroy(&stack);
    }

    // Test 3: Size after pop
    {
        Stack stack;
        stack_init(&stack, NULL);
        int a = 1, b = 2;
        stack_push(&stack, &a);
        stack_push(&stack, &b);
        stack_pop(&stack, NULL);
        EXPECT_EQUAL_UINT(stack_size(&stack), 1);
        stack_destroy(&stack);
    }

    // Test 4: Size with NULL stack
    {
        EXPECT_EQUAL_UINT(stack_size(NULL), 0);
    }
}

/**
 * Test: stack_empty
 * Dependencies: stack_init, stack_size
 * Description: Tests the stack_empty function to check if stack is empty.
 */
UTEST_TEST_CASE(stack_empty){
    // Test 1: Empty stack
    {
        Stack stack;
        stack_init(&stack, NULL);
        EXPECT_TRUE(stack_empty(&stack));
        stack_destroy(&stack);
    }

    // Test 2: Non-empty stack
    {
        Stack stack;
        stack_init(&stack, NULL);
        int a = 1;
        stack_push(&stack, &a);
        EXPECT_FALSE(stack_empty(&stack));
        stack_destroy(&stack);
    }

    // Test 3: Empty after clear
    {
        Stack stack;
        stack_init(&stack, NULL);
        int a = 1, b = 2;
        stack_push(&stack, &a);
        stack_push(&stack, &b);
        EXPECT_FALSE(stack_empty(&stack));
        stack_clear(&stack);
        EXPECT_TRUE(stack_empty(&stack));
        stack_destroy(&stack);
    }

    // Test 4: Empty with NULL stack
    {
        EXPECT_FALSE(stack_empty(NULL));
    }
}

/**
 * Test: stack_destroy
 * Dependencies: stack_init
 * Description: Tests destruction of the stack and cleanup.
 */
UTEST_TEST_CASE(stack_destroy){
    // Test 1: Destroy empty stack
    {
        Stack stack;
        stack_init(&stack, NULL);
        stack_destroy(&stack);
        EXPECT_EQUAL_UINT(stack_size(&stack), 0);
        EXPECT_TRUE(stack_empty(&stack));
    }

    // Test 2: Destroy stack with elements and destroy function
    {
        Stack stack;
        stack_init(&stack, destroy_counter);
        int a = 1, b = 2, c = 3;
        stack_push(&stack, &a);
        stack_push(&stack, &b);
        stack_push(&stack, &c);
        destroy_count = 0;
        stack_destroy(&stack);
        EXPECT_EQUAL_INT(destroy_count, 3);
    }

    // Test 3: Destroy stack without destroy function
    {
        Stack stack;
        stack_init(&stack, NULL);
        int a = 1, b = 2;
        stack_push(&stack, &a);
        stack_push(&stack, &b);
        stack_destroy(&stack);
        EXPECT_EQUAL_UINT(stack_size(&stack), 0);
    }
}

/**
 * Test: stack_push
 * Dependencies: stack_init, stack_size
 * Description: Tests pushing elements onto the top of the stack.
 */
UTEST_TEST_CASE(stack_push){
    // Test 1: Push to empty stack
    {
        Stack stack;
        stack_init(&stack, NULL);
        int a = 1;
        EXPECT_EQUAL_INT(stack_push(&stack, &a), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(stack_size(&stack), 1);
        EXPECT_EQUAL_UINT(*(int*)stack_peek(&stack), 1);
        stack_destroy(&stack);
    }

    // Test 2: Push multiple elements
    {
        Stack stack;
        stack_init(&stack, NULL);
        int a = 1, b = 2, c = 3;
        EXPECT_EQUAL_INT(stack_push(&stack, &a), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(stack_push(&stack, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(stack_push(&stack, &c), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(stack_size(&stack), 3);
        EXPECT_EQUAL_UINT(*(int*)stack_peek(&stack), 3);
        stack_destroy(&stack);
    }

    // Test 3: Push NULL element
    {
        Stack stack;
        stack_init(&stack, NULL);
        EXPECT_EQUAL_INT(stack_push(&stack, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(stack_size(&stack), 1);
        EXPECT_NULL(stack_peek(&stack));
        stack_destroy(&stack);
    }

    // Test 4: Push maintains LIFO order
    {
        Stack stack;
        stack_init(&stack, NULL);
        int a = 1, b = 2, c = 3;
        stack_push(&stack, &a);
        stack_push(&stack, &b);
        stack_push(&stack, &c);
        void* popped = NULL;
        stack_pop(&stack, &popped);
        EXPECT_EQUAL_UINT(*(int*)popped, 3);
        stack_pop(&stack, &popped);
        EXPECT_EQUAL_UINT(*(int*)popped, 2);
        stack_pop(&stack, &popped);
        EXPECT_EQUAL_UINT(*(int*)popped, 1);
        stack_destroy(&stack);
    }

    // Test 5: Push with NULL stack
    {
        EXPECT_EQUAL_INT(stack_push(NULL, NULL), COLLECTION_FAILURE);
    }
}

/**
 * Test: stack_pop
 * Dependencies: stack_init, stack_push, stack_size
 * Description: Tests removing elements from the top of the stack.
 */
UTEST_TEST_CASE(stack_pop){
    // Test 1: Pop from empty stack
    {
        Stack stack;
        stack_init(&stack, NULL);
        EXPECT_EQUAL_INT(stack_pop(&stack, NULL), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(stack_size(&stack), 0);
        stack_destroy(&stack);
    }

    // Test 2: Pop without retrieving data
    {
        Stack stack;
        stack_init(&stack, NULL);
        int a = 1, b = 2;
        stack_push(&stack, &a);
        stack_push(&stack, &b);
        EXPECT_EQUAL_INT(stack_pop(&stack, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(stack_size(&stack), 1);
        EXPECT_EQUAL_UINT(*(int*)stack_peek(&stack), 1);
        stack_destroy(&stack);
    }

    // Test 3: Pop with retrieving data
    {
        Stack stack;
        stack_init(&stack, NULL);
        int a = 1, b = 2;
        stack_push(&stack, &a);
        stack_push(&stack, &b);
        void* popped = NULL;
        EXPECT_EQUAL_INT(stack_pop(&stack, &popped), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)popped, 2);
        EXPECT_EQUAL_UINT(stack_size(&stack), 1);
        stack_destroy(&stack);
    }

    // Test 4: Pop with destroy function
    {
        Stack stack;
        stack_init(&stack, destroy_counter);
        int a = 1, b = 2;
        stack_push(&stack, &a);
        stack_push(&stack, &b);
        destroy_count = 0;
        EXPECT_EQUAL_INT(stack_pop(&stack, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(destroy_count, 1);
        EXPECT_EQUAL_UINT(stack_size(&stack), 1);
        stack_destroy(&stack);
    }

    // Test 5: Pop all elements
    {
        Stack stack;
        stack_init(&stack, NULL);
        int a = 1, b = 2, c = 3;
        stack_push(&stack, &a);
        stack_push(&stack, &b);
        stack_push(&stack, &c);
        EXPECT_EQUAL_INT(stack_pop(&stack, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(stack_pop(&stack, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(stack_pop(&stack, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(stack_size(&stack), 0);
        EXPECT_TRUE(stack_empty(&stack));
        stack_destroy(&stack);
    }

    // Test 6: Pop maintains LIFO order
    {
        Stack stack;
        stack_init(&stack, NULL);
        int values[5] = {1, 2, 3, 4, 5};
        for (int i = 0; i < 5; i++) {
            stack_push(&stack, &values[i]);
        }
        for (int i = 4; i >= 0; i--) {
            void* popped = NULL;
            stack_pop(&stack, &popped);
            EXPECT_EQUAL_UINT(*(int*)popped, i + 1);
        }
        EXPECT_TRUE(stack_empty(&stack));
        stack_destroy(&stack);
    }
}

/**
 * Test: stack_peek
 * Dependencies: stack_init, stack_push
 * Description: Tests peeking at the top element without removing it.
 */
UTEST_TEST_CASE(stack_peek){
    // Test 1: Peek at empty stack
    {
        Stack stack;
        stack_init(&stack, NULL);
        EXPECT_NULL(stack_peek(&stack));
        stack_destroy(&stack);
    }

    // Test 2: Peek at non-empty stack
    {
        Stack stack;
        stack_init(&stack, NULL);
        int a = 1, b = 2;
        stack_push(&stack, &a);
        EXPECT_EQUAL_UINT(*(int*)stack_peek(&stack), 1);
        stack_push(&stack, &b);
        EXPECT_EQUAL_UINT(*(int*)stack_peek(&stack), 2);
        stack_destroy(&stack);
    }

    // Test 3: Peek doesn't remove element
    {
        Stack stack;
        stack_init(&stack, NULL);
        int a = 1, b = 2;
        stack_push(&stack, &a);
        stack_push(&stack, &b);
        EXPECT_EQUAL_UINT(*(int*)stack_peek(&stack), 2);
        EXPECT_EQUAL_UINT(stack_size(&stack), 2);
        EXPECT_EQUAL_UINT(*(int*)stack_peek(&stack), 2);
        EXPECT_EQUAL_UINT(stack_size(&stack), 2);
        stack_destroy(&stack);
    }

    // Test 4: Peek returns top element
    {
        Stack stack;
        stack_init(&stack, NULL);
        int a = 1, b = 2, c = 3;
        stack_push(&stack, &a);
        stack_push(&stack, &b);
        stack_push(&stack, &c);
        EXPECT_EQUAL_UINT(*(int*)stack_peek(&stack), 3);
        stack_pop(&stack, NULL);
        EXPECT_EQUAL_UINT(*(int*)stack_peek(&stack), 2);
        stack_pop(&stack, NULL);
        EXPECT_EQUAL_UINT(*(int*)stack_peek(&stack), 1);
        stack_destroy(&stack);
    }

    // Test 5: Peek with NULL element
    {
        Stack stack;
        stack_init(&stack, NULL);
        stack_push(&stack, NULL);
        EXPECT_NULL(stack_peek(&stack));
        stack_destroy(&stack);
    }
}

/**
 * Test: stack_clear
 * Dependencies: stack_init, stack_push, stack_size
 * Description: Tests removing all elements from the stack.
 */
UTEST_TEST_CASE(stack_clear){
    // Test 1: Clear empty stack
    {
        Stack stack;
        stack_init(&stack, NULL);
        stack_clear(&stack);
        EXPECT_EQUAL_UINT(stack_size(&stack), 0);
        EXPECT_TRUE(stack_empty(&stack));
        stack_destroy(&stack);
    }

    // Test 2: Clear stack with elements
    {
        Stack stack;
        stack_init(&stack, NULL);
        int a = 1, b = 2, c = 3;
        stack_push(&stack, &a);
        stack_push(&stack, &b);
        stack_push(&stack, &c);
        stack_clear(&stack);
        EXPECT_EQUAL_UINT(stack_size(&stack), 0);
        EXPECT_TRUE(stack_empty(&stack));
        EXPECT_NULL(stack_peek(&stack));
        stack_destroy(&stack);
    }

    // Test 3: Clear with destroy function
    {
        Stack stack;
        stack_init(&stack, destroy_counter);
        int a = 1, b = 2, c = 3;
        stack_push(&stack, &a);
        stack_push(&stack, &b);
        stack_push(&stack, &c);
        destroy_count = 0;
        stack_clear(&stack);
        EXPECT_EQUAL_INT(destroy_count, 3);
        EXPECT_EQUAL_UINT(stack_size(&stack), 0);
        stack_destroy(&stack);
    }

    // Test 4: Use after clear
    {
        Stack stack;
        stack_init(&stack, NULL);
        int a = 1, b = 2;
        stack_push(&stack, &a);
        stack_clear(&stack);
        stack_push(&stack, &b);
        EXPECT_EQUAL_UINT(stack_size(&stack), 1);
        EXPECT_EQUAL_UINT(*(int*)stack_peek(&stack), 2);
        stack_destroy(&stack);
    }
}

/**
 * Test: stack_memory_leak
 * Dependencies: stack_init, stack_destroy, stack_push, stack_pop, stack_clear
 * Description: Tests for memory leaks by tracking destroy function calls.
 */
UTEST_TEST_CASE(stack_memory_leak){
    // Test 1: All elements destroyed on stack_destroy
    {
        Stack stack;
        stack_init(&stack, destroy_counter);
        int values[10];
        for (int i = 0; i < 10; i++) {
            values[i] = i;
            stack_push(&stack, &values[i]);
        }
        destroy_count = 0;
        stack_destroy(&stack);
        EXPECT_EQUAL_INT(destroy_count, 10);
    }

    // Test 2: Elements destroyed on pop
    {
        Stack stack;
        stack_init(&stack, destroy_counter);
        int values[5];
        for (int i = 0; i < 5; i++) {
            values[i] = i;
            stack_push(&stack, &values[i]);
        }
        destroy_count = 0;
        for (int i = 0; i < 5; i++) {
            stack_pop(&stack, NULL);
        }
        EXPECT_EQUAL_INT(destroy_count, 5);
        stack_destroy(&stack);
        EXPECT_EQUAL_INT(destroy_count, 5);
    }

    // Test 3: Elements destroyed on clear
    {
        Stack stack;
        stack_init(&stack, destroy_counter);
        int values[10];
        for (int i = 0; i < 10; i++) {
            values[i] = i;
            stack_push(&stack, &values[i]);
        }
        destroy_count = 0;
        stack_clear(&stack);
        EXPECT_EQUAL_INT(destroy_count, 10);
        stack_destroy(&stack);
        EXPECT_EQUAL_INT(destroy_count, 10);
    }
    // Test 4: Elements freed by free
    {
        Stack stack;
        stack_init(&stack, free);
        for (int i = 0; i < 10; i++) {
            int* value = malloc(sizeof(int));
            *value = i;
            stack_push(&stack, value);
        }
        EXPECT_EQUAL_UINT(stack_size(&stack), 10);
        stack_destroy(&stack);
    }
}

/**
 * Test: stack_integration
 * Dependencies: All stack functions
 * Description: Integration test combining multiple operations.
 */
UTEST_TEST_CASE(stack_integration){
    // Test 1: Complex sequence of operations
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        int values[10];
        for (int i = 0; i < 10; i++) {
            values[i] = i;
            EXPECT_EQUAL_INT(stack_push(&stack, &values[i]), COLLECTION_SUCCESS);
        }
        EXPECT_EQUAL_UINT(stack_size(&stack), 10);
        
        void* peeked = stack_peek(&stack);
        EXPECT_EQUAL_UINT(*(int*)peeked, 9);
        EXPECT_EQUAL_UINT(stack_size(&stack), 10);
        
        void* popped = NULL;
        EXPECT_EQUAL_INT(stack_pop(&stack, &popped), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)popped, 9);
        EXPECT_EQUAL_UINT(stack_size(&stack), 9);
        
        stack_clear(&stack);
        EXPECT_EQUAL_UINT(stack_size(&stack), 0);
        EXPECT_TRUE(stack_empty(&stack));
        
        stack_destroy(&stack);
    }

    // Test 2: LIFO order verification
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        int values[5] = {1, 2, 3, 4, 5};
        for (int i = 0; i < 5; i++) {
            stack_push(&stack, &values[i]);
        }
        
        for (int i = 4; i >= 0; i--) {
            void* popped = NULL;
            stack_pop(&stack, &popped);
            EXPECT_EQUAL_UINT(*(int*)popped, values[i]);
        }
        
        EXPECT_TRUE(stack_empty(&stack));
        stack_destroy(&stack);
    }

    // Test 3: Push-pop pattern
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        int a = 1, b = 2, c = 3;
        stack_push(&stack, &a);
        stack_push(&stack, &b);
        
        void* popped = NULL;
        stack_pop(&stack, &popped);
        EXPECT_EQUAL_UINT(*(int*)popped, 2);
        
        stack_push(&stack, &c);
        EXPECT_EQUAL_UINT(stack_size(&stack), 2);
        EXPECT_EQUAL_UINT(*(int*)stack_peek(&stack), 3);
        
        stack_destroy(&stack);
    }

    // Test 4: Edge cases combination
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        EXPECT_TRUE(stack_empty(&stack));
        EXPECT_NULL(stack_peek(&stack));
        
        int a = 1;
        stack_push(&stack, &a);
        EXPECT_FALSE(stack_empty(&stack));
        EXPECT_EQUAL_UINT(*(int*)stack_peek(&stack), 1);
        
        stack_pop(&stack, NULL);
        EXPECT_TRUE(stack_empty(&stack));
        EXPECT_NULL(stack_peek(&stack));
        
        stack_destroy(&stack);
    }

    // Test 5: Multiple push-pop cycles
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        for (int cycle = 0; cycle < 3; cycle++) {
            int values[3] = {cycle * 3 + 1, cycle * 3 + 2, cycle * 3 + 3};
            for (int i = 0; i < 3; i++) {
                stack_push(&stack, &values[i]);
            }
            for (int i = 2; i >= 0; i--) {
                void* popped = NULL;
                stack_pop(&stack, &popped);
                EXPECT_EQUAL_UINT(*(int*)popped, values[i]);
            }
            EXPECT_TRUE(stack_empty(&stack));
        }
        
        stack_destroy(&stack);
    }

    // Test 6: Expression evaluation pattern
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        int a = 10, b = 20, c = 30;
        stack_push(&stack, &a);
        stack_push(&stack, &b);
        stack_push(&stack, &c);
        
        void* result = NULL;
        stack_pop(&stack, &result);
        int val_c = *(int*)result;
        
        stack_pop(&stack, &result);
        int val_b = *(int*)result;
        
        stack_pop(&stack, &result);
        int val_a = *(int*)result;
        
        int answer = val_c - val_b + val_a;
        EXPECT_EQUAL_INT(answer, 20);
        
        stack_destroy(&stack);
    }
}

/**
 * Test suite: stack
 * Description: Test suite for stack data structure
 */
UTEST_TEST_SUITE(stack){
    UTEST_RUN_TEST_CASE(stack_init);
    UTEST_RUN_TEST_CASE(stack_size);
    UTEST_RUN_TEST_CASE(stack_empty);
    UTEST_RUN_TEST_CASE(stack_destroy);
    UTEST_RUN_TEST_CASE(stack_push);
    UTEST_RUN_TEST_CASE(stack_pop);
    UTEST_RUN_TEST_CASE(stack_peek);
    UTEST_RUN_TEST_CASE(stack_clear);
    UTEST_RUN_TEST_CASE(stack_memory_leak);
    UTEST_RUN_TEST_CASE(stack_integration);
}

