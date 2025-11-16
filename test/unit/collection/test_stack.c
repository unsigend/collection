#include <utest.h>
#include <collection/stack.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * Global helper functions for testing destroy callbacks.
 */

static void destroy_counter(void *data) {
    int *counter = (int *)data;
    if (counter != NULL) {
        (*counter)++;
    }
}

static void destroy_noop(void *data) {
    (void)data;
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
        
        EXPECT_EQUAL_UINT64(stack_size(&stack), 0);
        EXPECT_TRUE(stack_empty(&stack));
    }
    
    // Test 2: Initialize with non-NULL destroy function
    {
        Stack stack;
        stack_init(&stack, destroy_noop);
        
        EXPECT_EQUAL_UINT64(stack_size(&stack), 0);
        EXPECT_TRUE(stack_empty(&stack));
    }
    
    // Test 3: Multiple initializations
    {
        Stack stack1, stack2;
        stack_init(&stack1, NULL);
        stack_init(&stack2, destroy_noop);
        
        EXPECT_EQUAL_UINT64(stack_size(&stack1), 0);
        EXPECT_EQUAL_UINT64(stack_size(&stack2), 0);
    }
}

/**
 * Test: stack_size
 * Dependencies: stack_init
 */
UTEST_TEST_CASE(stack_size){
    // Test 1: Size of newly initialized stack
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        EXPECT_EQUAL_UINT64(stack_size(&stack), 0);
        
        stack_destroy(&stack);
    }
    
    // Test 2: Size consistency
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        EXPECT_EQUAL_UINT64(stack_size(&stack), 0);
        
        stack_destroy(&stack);
    }
}

/**
 * Test: stack_empty
 * Dependencies: stack_init
 */
UTEST_TEST_CASE(stack_empty){
    // Test 1: Empty after initialization
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        EXPECT_TRUE(stack_empty(&stack));
        
        stack_destroy(&stack);
    }
    
    // Test 2: Consistency with size
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        EXPECT_TRUE(stack_empty(&stack) == (stack_size(&stack) == 0));
        
        stack_destroy(&stack);
    }
}

/**
 * Test: stack_destroy
 * Dependencies: stack_init
 */
UTEST_TEST_CASE(stack_destroy){
    // Test 1: Destroy empty stack without destroy function
    {
        Stack stack;
        stack_init(&stack, NULL);
        stack_destroy(&stack);
        
        EXPECT_EQUAL_UINT64(stack_size(&stack), 0);
        EXPECT_TRUE(stack_empty(&stack));
    }
    
    // Test 2: Destroy empty stack with destroy function
    {
        Stack stack;
        stack_init(&stack, destroy_noop);
        stack_destroy(&stack);
        
        EXPECT_EQUAL_UINT64(stack_size(&stack), 0);
        EXPECT_TRUE(stack_empty(&stack));
    }
    
    // Test 3: Multiple destroy calls
    {
        Stack stack;
        stack_init(&stack, NULL);
        stack_destroy(&stack);
        stack_destroy(&stack);
        
        EXPECT_EQUAL_UINT64(stack_size(&stack), 0);
    }
}

/**
 * Test: stack_push
 * Dependencies: stack_init, stack_size
 */
UTEST_TEST_CASE(stack_push){
    // Test 1: Push to empty stack
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        const char *data = "first";
        int result = stack_push(&stack, (void *)data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(stack_size(&stack), 1);
        EXPECT_FALSE(stack_empty(&stack));
        
        stack_destroy(&stack);
    }
    
    // Test 2: Push multiple elements
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        for (int i = 0; i < 10; i++) {
            int result = stack_push(&stack, (void *)(long)i);
            EXPECT_EQUAL_INT(result, 0);
        }
        
        EXPECT_EQUAL_UINT64(stack_size(&stack), 10);
        
        stack_destroy(&stack);
    }
    
    // Test 3: Push NULL pointer
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        int result = stack_push(&stack, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(stack_size(&stack), 1);
        
        stack_destroy(&stack);
    }
    
    // Test 4: Order verification (LIFO)
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        EXPECT_EQUAL_INT(stack_push(&stack, (void *)"first"), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(stack_push(&stack, (void *)"second"), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(stack_push(&stack, (void *)"third"), COLLECTION_SUCCESS);
        
        void* top = stack_peek(&stack);
        EXPECT_EQUAL_STRING((char*)top, "third");
        
        stack_destroy(&stack);
    }
}

/**
 * Test: stack_pop
 * Dependencies: stack_init, stack_push, stack_size
 */
UTEST_TEST_CASE(stack_pop){
    // Test 1: Pop from empty stack
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        void* data = NULL;
        int result = stack_pop(&stack, &data);
        
        EXPECT_EQUAL_INT(result, -1);
        EXPECT_EQUAL_UINT64(stack_size(&stack), 0);
        
        stack_destroy(&stack);
    }
    
    // Test 2: Pop single element without retrieving data
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        stack_push(&stack, (void *)0x1234);
        
        int result = stack_pop(&stack, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(stack_size(&stack), 0);
        EXPECT_TRUE(stack_empty(&stack));
        
        stack_destroy(&stack);
    }
    
    // Test 3: Pop single element and retrieve data
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        const char* test_data = "test";
        stack_push(&stack, (void *)test_data);
        
        void* data = NULL;
        int result = stack_pop(&stack, &data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_TRUE(data == (void *)test_data);
        EXPECT_EQUAL_UINT64(stack_size(&stack), 0);
        
        stack_destroy(&stack);
    }
    
    // Test 4: Pop multiple elements (LIFO order)
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        for (int i = 0; i < 10; i++) {
            stack_push(&stack, (void *)(long)i);
        }
        
        for (int i = 9; i >= 0; i--) {
            void* data = NULL;
            int result = stack_pop(&stack, &data);
            EXPECT_EQUAL_INT(result, 0);
            EXPECT_TRUE(data == (void *)(long)i);
        }
        
        EXPECT_EQUAL_UINT64(stack_size(&stack), 0);
        
        stack_destroy(&stack);
    }
    
    // Test 5: Pop with destroy function (data not retrieved)
    {
        int destroy_count = 0;
        Stack stack;
        stack_init(&stack, destroy_counter);
        
        stack_push(&stack, &destroy_count);
        stack_push(&stack, &destroy_count);
        stack_push(&stack, &destroy_count);
        
        stack_pop(&stack, NULL);
        EXPECT_EQUAL_INT(destroy_count, 1);
        
        stack_pop(&stack, NULL);
        EXPECT_EQUAL_INT(destroy_count, 2);
        
        stack_pop(&stack, NULL);
        EXPECT_EQUAL_INT(destroy_count, 3);
        
        stack_destroy(&stack);
    }
    
    // Test 6: Pop with data retrieval (destroy not called)
    {
        int destroy_count = 0;
        Stack stack;
        stack_init(&stack, destroy_counter);
        
        stack_push(&stack, &destroy_count);
        stack_push(&stack, &destroy_count);
        
        void* data = NULL;
        stack_pop(&stack, &data);
        
        EXPECT_EQUAL_INT(destroy_count, 0);
        EXPECT_NOT_NULL(data);
        
        stack_destroy(&stack);
    }
    
    // Test 7: Pop maintains LIFO order
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        stack_push(&stack, (void *)"first");
        stack_push(&stack, (void *)"second");
        stack_push(&stack, (void *)"third");
        
        void* data = NULL;
        stack_pop(&stack, &data);
        EXPECT_EQUAL_STRING((char*)data, "third");
        
        stack_pop(&stack, &data);
        EXPECT_EQUAL_STRING((char*)data, "second");
        
        stack_pop(&stack, &data);
        EXPECT_EQUAL_STRING((char*)data, "first");
        
        stack_destroy(&stack);
    }
}

/**
 * Test: stack_peek
 * Dependencies: stack_init, stack_push
 */
UTEST_TEST_CASE(stack_peek){
    // Test 1: Peek at empty stack
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        EXPECT_NULL(stack_peek(&stack));
        
        stack_destroy(&stack);
    }
    
    // Test 2: Peek at single element
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        const char* test_data = "test";
        stack_push(&stack, (void *)test_data);
        
        void* top = stack_peek(&stack);
        EXPECT_TRUE(top == (void *)test_data);
        EXPECT_EQUAL_UINT64(stack_size(&stack), 1);
        
        stack_destroy(&stack);
    }
    
    // Test 3: Peek at top element without removing
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        stack_push(&stack, (void *)"first");
        stack_push(&stack, (void *)"second");
        stack_push(&stack, (void *)"third");
        
        void* top = stack_peek(&stack);
        EXPECT_EQUAL_STRING((char*)top, "third");
        EXPECT_EQUAL_UINT64(stack_size(&stack), 3);
        
        top = stack_peek(&stack);
        EXPECT_EQUAL_STRING((char*)top, "third");
        EXPECT_EQUAL_UINT64(stack_size(&stack), 3);
        
        stack_destroy(&stack);
    }
    
    // Test 4: Peek returns NULL after all elements popped
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        stack_push(&stack, (void *)"only");
        stack_pop(&stack, NULL);
        
        EXPECT_NULL(stack_peek(&stack));
        
        stack_destroy(&stack);
    }
}

/**
 * Test: stack_clear
 * Dependencies: stack_init, stack_push, stack_size
 */
UTEST_TEST_CASE(stack_clear){
    // Test 1: Clear empty stack
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        stack_clear(&stack);
        
        EXPECT_EQUAL_UINT64(stack_size(&stack), 0);
        EXPECT_TRUE(stack_empty(&stack));
        
        stack_destroy(&stack);
    }
    
    // Test 2: Clear single element stack
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        stack_push(&stack, (void *)"test");
        stack_clear(&stack);
        
        EXPECT_EQUAL_UINT64(stack_size(&stack), 0);
        EXPECT_TRUE(stack_empty(&stack));
        
        stack_destroy(&stack);
    }
    
    // Test 3: Clear multiple element stack
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        for (int i = 0; i < 10; i++) {
            stack_push(&stack, (void *)(long)i);
        }
        
        stack_clear(&stack);
        
        EXPECT_EQUAL_UINT64(stack_size(&stack), 0);
        EXPECT_TRUE(stack_empty(&stack));
        
        stack_destroy(&stack);
    }
    
    // Test 4: Clear with destroy function
    {
        int destroy_count = 0;
        Stack stack;
        stack_init(&stack, destroy_counter);
        
        stack_push(&stack, &destroy_count);
        stack_push(&stack, &destroy_count);
        stack_push(&stack, &destroy_count);
        
        stack_clear(&stack);
        
        EXPECT_EQUAL_INT(destroy_count, 3);
        EXPECT_EQUAL_UINT64(stack_size(&stack), 0);
        
        stack_destroy(&stack);
    }
    
    // Test 5: Clear and reuse stack
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        stack_push(&stack, (void *)"first");
        stack_push(&stack, (void *)"second");
        
        stack_clear(&stack);
        
        EXPECT_EQUAL_UINT64(stack_size(&stack), 0);
        
        stack_push(&stack, (void *)"new");
        
        EXPECT_EQUAL_UINT64(stack_size(&stack), 1);
        EXPECT_EQUAL_STRING((char*)stack_peek(&stack), "new");
        
        stack_destroy(&stack);
    }
    
    // Test 6: Multiple clears
    {
        Stack stack;
        stack_init(&stack, NULL);
        
        stack_push(&stack, (void *)"test");
        stack_clear(&stack);
        stack_clear(&stack);
        
        EXPECT_EQUAL_UINT64(stack_size(&stack), 0);
        
        stack_destroy(&stack);
    }
}

/**
 * Test Suite: stack
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
}

