#include <utest.h>
#include <collection/queue.h>
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
 * Test: queue_init
 * Dependencies: None
 * Description: Tests basic initialization of the queue structure.
 */
UTEST_TEST_CASE(queue_init){
    // Test 1: Initialize with NULL destroy function
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        EXPECT_EQUAL_UINT64(queue_size(&queue), 0);
        EXPECT_TRUE(queue_empty(&queue));
    }
    
    // Test 2: Initialize with non-NULL destroy function
    {
        Queue queue;
        queue_init(&queue, destroy_noop);
        
        EXPECT_EQUAL_UINT64(queue_size(&queue), 0);
        EXPECT_TRUE(queue_empty(&queue));
    }
    
    // Test 3: Multiple initializations
    {
        Queue queue1, queue2;
        queue_init(&queue1, NULL);
        queue_init(&queue2, destroy_noop);
        
        EXPECT_EQUAL_UINT64(queue_size(&queue1), 0);
        EXPECT_EQUAL_UINT64(queue_size(&queue2), 0);
    }
}

/**
 * Test: queue_size
 * Dependencies: queue_init
 */
UTEST_TEST_CASE(queue_size){
    // Test 1: Size of newly initialized queue
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        EXPECT_EQUAL_UINT64(queue_size(&queue), 0);
        
        queue_destroy(&queue);
    }
    
    // Test 2: Size consistency
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        EXPECT_EQUAL_UINT64(queue_size(&queue), 0);
        
        queue_destroy(&queue);
    }
}

/**
 * Test: queue_empty
 * Dependencies: queue_init
 */
UTEST_TEST_CASE(queue_empty){
    // Test 1: Empty after initialization
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        EXPECT_TRUE(queue_empty(&queue));
        
        queue_destroy(&queue);
    }
    
    // Test 2: Consistency with size
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        EXPECT_TRUE(queue_empty(&queue) == (queue_size(&queue) == 0));
        
        queue_destroy(&queue);
    }
}

/**
 * Test: queue_destroy
 * Dependencies: queue_init
 */
UTEST_TEST_CASE(queue_destroy){
    // Test 1: Destroy empty queue without destroy function
    {
        Queue queue;
        queue_init(&queue, NULL);
        queue_destroy(&queue);
        
        EXPECT_EQUAL_UINT64(queue_size(&queue), 0);
        EXPECT_TRUE(queue_empty(&queue));
    }
    
    // Test 2: Destroy empty queue with destroy function
    {
        Queue queue;
        queue_init(&queue, destroy_noop);
        queue_destroy(&queue);
        
        EXPECT_EQUAL_UINT64(queue_size(&queue), 0);
        EXPECT_TRUE(queue_empty(&queue));
    }
    
    // Test 3: Multiple destroy calls
    {
        Queue queue;
        queue_init(&queue, NULL);
        queue_destroy(&queue);
        queue_destroy(&queue);
        
        EXPECT_EQUAL_UINT64(queue_size(&queue), 0);
    }
}

/**
 * Test: queue_enqueue
 * Dependencies: queue_init, queue_size
 */
UTEST_TEST_CASE(queue_enqueue){
    // Test 1: Enqueue to empty queue
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        const char *data = "first";
        int result = queue_enqueue(&queue, (void *)data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(queue_size(&queue), 1);
        EXPECT_FALSE(queue_empty(&queue));
        
        queue_destroy(&queue);
    }
    
    // Test 2: Enqueue multiple elements
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        for (int i = 0; i < 10; i++) {
            int result = queue_enqueue(&queue, (void *)(long)i);
            EXPECT_EQUAL_INT(result, 0);
        }
        
        EXPECT_EQUAL_UINT64(queue_size(&queue), 10);
        
        queue_destroy(&queue);
    }
    
    // Test 3: Enqueue NULL pointer
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        int result = queue_enqueue(&queue, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(queue_size(&queue), 1);
        
        queue_destroy(&queue);
    }
    
    // Test 4: Order verification (FIFO)
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        queue_enqueue(&queue, (void *)"first");
        queue_enqueue(&queue, (void *)"second");
        queue_enqueue(&queue, (void *)"third");
        
        void* front = queue_peek(&queue);
        EXPECT_EQUAL_STRING((char*)front, "first");
        
        queue_destroy(&queue);
    }
}

/**
 * Test: queue_dequeue
 * Dependencies: queue_init, queue_enqueue, queue_size
 */
UTEST_TEST_CASE(queue_dequeue){
    // Test 1: Dequeue from empty queue
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        void* data = NULL;
        int result = queue_dequeue(&queue, &data);
        
        EXPECT_EQUAL_INT(result, -1);
        EXPECT_EQUAL_UINT64(queue_size(&queue), 0);
        
        queue_destroy(&queue);
    }
    
    // Test 2: Dequeue single element without retrieving data
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        queue_enqueue(&queue, (void *)0x1234);
        
        int result = queue_dequeue(&queue, NULL);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_EQUAL_UINT64(queue_size(&queue), 0);
        EXPECT_TRUE(queue_empty(&queue));
        
        queue_destroy(&queue);
    }
    
    // Test 3: Dequeue single element and retrieve data
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        const char* test_data = "test";
        queue_enqueue(&queue, (void *)test_data);
        
        void* data = NULL;
        int result = queue_dequeue(&queue, &data);
        
        EXPECT_EQUAL_INT(result, 0);
        EXPECT_TRUE(data == (void *)test_data);
        EXPECT_EQUAL_UINT64(queue_size(&queue), 0);
        
        queue_destroy(&queue);
    }
    
    // Test 4: Dequeue multiple elements (FIFO order)
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        for (int i = 0; i < 10; i++) {
            queue_enqueue(&queue, (void *)(long)i);
        }
        
        for (int i = 0; i < 10; i++) {
            void* data = NULL;
            int result = queue_dequeue(&queue, &data);
            EXPECT_EQUAL_INT(result, 0);
            EXPECT_TRUE(data == (void *)(long)i);
        }
        
        EXPECT_EQUAL_UINT64(queue_size(&queue), 0);
        
        queue_destroy(&queue);
    }
    
    // Test 5: Dequeue with destroy function (data not retrieved)
    {
        int destroy_count = 0;
        Queue queue;
        queue_init(&queue, destroy_counter);
        
        queue_enqueue(&queue, &destroy_count);
        queue_enqueue(&queue, &destroy_count);
        queue_enqueue(&queue, &destroy_count);
        
        queue_dequeue(&queue, NULL);
        EXPECT_EQUAL_INT(destroy_count, 1);
        
        queue_dequeue(&queue, NULL);
        EXPECT_EQUAL_INT(destroy_count, 2);
        
        queue_dequeue(&queue, NULL);
        EXPECT_EQUAL_INT(destroy_count, 3);
        
        queue_destroy(&queue);
    }
    
    // Test 6: Dequeue with data retrieval (destroy not called)
    {
        int destroy_count = 0;
        Queue queue;
        queue_init(&queue, destroy_counter);
        
        queue_enqueue(&queue, &destroy_count);
        queue_enqueue(&queue, &destroy_count);
        
        void* data = NULL;
        queue_dequeue(&queue, &data);
        
        EXPECT_EQUAL_INT(destroy_count, 0);
        EXPECT_NOT_NULL(data);
        
        queue_destroy(&queue);
    }
    
    // Test 7: Dequeue maintains FIFO order
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        queue_enqueue(&queue, (void *)"first");
        queue_enqueue(&queue, (void *)"second");
        queue_enqueue(&queue, (void *)"third");
        
        void* data = NULL;
        queue_dequeue(&queue, &data);
        EXPECT_EQUAL_STRING((char*)data, "first");
        
        queue_dequeue(&queue, &data);
        EXPECT_EQUAL_STRING((char*)data, "second");
        
        queue_dequeue(&queue, &data);
        EXPECT_EQUAL_STRING((char*)data, "third");
        
        queue_destroy(&queue);
    }
}

/**
 * Test: queue_peek
 * Dependencies: queue_init, queue_enqueue
 */
UTEST_TEST_CASE(queue_peek){
    // Test 1: Peek at empty queue
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        EXPECT_NULL(queue_peek(&queue));
        
        queue_destroy(&queue);
    }
    
    // Test 2: Peek at single element
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        const char* test_data = "test";
        queue_enqueue(&queue, (void *)test_data);
        
        void* front = queue_peek(&queue);
        EXPECT_TRUE(front == (void *)test_data);
        EXPECT_EQUAL_UINT64(queue_size(&queue), 1);
        
        queue_destroy(&queue);
    }
    
    // Test 3: Peek at front element without removing
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        queue_enqueue(&queue, (void *)"first");
        queue_enqueue(&queue, (void *)"second");
        queue_enqueue(&queue, (void *)"third");
        
        void* front = queue_peek(&queue);
        EXPECT_EQUAL_STRING((char*)front, "first");
        EXPECT_EQUAL_UINT64(queue_size(&queue), 3);
        
        front = queue_peek(&queue);
        EXPECT_EQUAL_STRING((char*)front, "first");
        EXPECT_EQUAL_UINT64(queue_size(&queue), 3);
        
        queue_destroy(&queue);
    }
    
    // Test 4: Peek returns NULL after all elements dequeued
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        queue_enqueue(&queue, (void *)"only");
        queue_dequeue(&queue, NULL);
        
        EXPECT_NULL(queue_peek(&queue));
        
        queue_destroy(&queue);
    }
    
    // Test 5: Peek returns front element (FIFO)
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        queue_enqueue(&queue, (void *)"first");
        queue_enqueue(&queue, (void *)"second");
        
        void* front = queue_peek(&queue);
        EXPECT_EQUAL_STRING((char*)front, "first");
        
        queue_dequeue(&queue, NULL);
        front = queue_peek(&queue);
        EXPECT_EQUAL_STRING((char*)front, "second");
        
        queue_destroy(&queue);
    }
}

/**
 * Test: queue_clear
 * Dependencies: queue_init, queue_enqueue, queue_size
 */
UTEST_TEST_CASE(queue_clear){
    // Test 1: Clear empty queue
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        queue_clear(&queue);
        
        EXPECT_EQUAL_UINT64(queue_size(&queue), 0);
        EXPECT_TRUE(queue_empty(&queue));
        
        queue_destroy(&queue);
    }
    
    // Test 2: Clear single element queue
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        queue_enqueue(&queue, (void *)"test");
        queue_clear(&queue);
        
        EXPECT_EQUAL_UINT64(queue_size(&queue), 0);
        EXPECT_TRUE(queue_empty(&queue));
        
        queue_destroy(&queue);
    }
    
    // Test 3: Clear multiple element queue
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        for (int i = 0; i < 10; i++) {
            queue_enqueue(&queue, (void *)(long)i);
        }
        
        queue_clear(&queue);
        
        EXPECT_EQUAL_UINT64(queue_size(&queue), 0);
        EXPECT_TRUE(queue_empty(&queue));
        
        queue_destroy(&queue);
    }
    
    // Test 4: Clear with destroy function
    {
        int destroy_count = 0;
        Queue queue;
        queue_init(&queue, destroy_counter);
        
        queue_enqueue(&queue, &destroy_count);
        queue_enqueue(&queue, &destroy_count);
        queue_enqueue(&queue, &destroy_count);
        
        queue_clear(&queue);
        
        EXPECT_EQUAL_INT(destroy_count, 3);
        EXPECT_EQUAL_UINT64(queue_size(&queue), 0);
        
        queue_destroy(&queue);
    }
    
    // Test 5: Clear and reuse queue
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        queue_enqueue(&queue, (void *)"first");
        queue_enqueue(&queue, (void *)"second");
        
        queue_clear(&queue);
        
        EXPECT_EQUAL_UINT64(queue_size(&queue), 0);
        
        queue_enqueue(&queue, (void *)"new");
        
        EXPECT_EQUAL_UINT64(queue_size(&queue), 1);
        EXPECT_EQUAL_STRING((char*)queue_peek(&queue), "new");
        
        queue_destroy(&queue);
    }
    
    // Test 6: Multiple clears
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        queue_enqueue(&queue, (void *)"test");
        queue_clear(&queue);
        queue_clear(&queue);
        
        EXPECT_EQUAL_UINT64(queue_size(&queue), 0);
        
        queue_destroy(&queue);
    }
}

/**
 * Test Suite: queue
 */
UTEST_TEST_SUITE(queue){
    UTEST_RUN_TEST_CASE(queue_init);
    UTEST_RUN_TEST_CASE(queue_size);
    UTEST_RUN_TEST_CASE(queue_empty);
    UTEST_RUN_TEST_CASE(queue_destroy);
    UTEST_RUN_TEST_CASE(queue_enqueue);
    UTEST_RUN_TEST_CASE(queue_dequeue);
    UTEST_RUN_TEST_CASE(queue_peek);
    UTEST_RUN_TEST_CASE(queue_clear);
}

