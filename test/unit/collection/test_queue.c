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

#include <collection/queue.h>
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
 * Test: queue_init
 * Dependencies: None
 * Description: Tests basic initialization of the queue structure.
 */
UTEST_TEST_CASE(queue_init){
    // Test 1: Initialize with NULL destroy function
    {
        Queue queue;
        queue_init(&queue, NULL);
        EXPECT_EQUAL_UINT(queue_size(&queue), 0);
        EXPECT_TRUE(queue_empty(&queue));
        queue_destroy(&queue);
    }

    // Test 2: Initialize with non-NULL destroy function
    {
        Queue queue;
        queue_init(&queue, free);
        EXPECT_EQUAL_UINT(queue_size(&queue), 0);
        EXPECT_TRUE(queue_empty(&queue));
        queue_destroy(&queue);
    }
}

/**
 * Test: queue_size
 * Dependencies: queue_init
 * Description: Tests the queue_size function to get the number of elements.
 */
UTEST_TEST_CASE(queue_size){
    // Test 1: Size of empty queue
    {
        Queue queue;
        queue_init(&queue, NULL);
        EXPECT_EQUAL_UINT(queue_size(&queue), 0);
        queue_destroy(&queue);
    }

    // Test 2: Size after enqueue operations
    {
        Queue queue;
        queue_init(&queue, NULL);
        int a = 1, b = 2, c = 3;
        queue_enqueue(&queue, &a);
        EXPECT_EQUAL_UINT(queue_size(&queue), 1);
        queue_enqueue(&queue, &b);
        EXPECT_EQUAL_UINT(queue_size(&queue), 2);
        queue_enqueue(&queue, &c);
        EXPECT_EQUAL_UINT(queue_size(&queue), 3);
        queue_destroy(&queue);
    }

    // Test 3: Size after dequeue
    {
        Queue queue;
        queue_init(&queue, NULL);
        int a = 1, b = 2;
        queue_enqueue(&queue, &a);
        queue_enqueue(&queue, &b);
        queue_dequeue(&queue, NULL);
        EXPECT_EQUAL_UINT(queue_size(&queue), 1);
        queue_destroy(&queue);
    }

    // Test 4: Size with NULL queue
    {
        EXPECT_EQUAL_UINT(queue_size(NULL), 0);
    }
}

/**
 * Test: queue_empty
 * Dependencies: queue_init, queue_size
 * Description: Tests the queue_empty function to check if queue is empty.
 */
UTEST_TEST_CASE(queue_empty){
    // Test 1: Empty queue
    {
        Queue queue;
        queue_init(&queue, NULL);
        EXPECT_TRUE(queue_empty(&queue));
        queue_destroy(&queue);
    }

    // Test 2: Non-empty queue
    {
        Queue queue;
        queue_init(&queue, NULL);
        int a = 1;
        queue_enqueue(&queue, &a);
        EXPECT_FALSE(queue_empty(&queue));
        queue_destroy(&queue);
    }

    // Test 3: Empty after clear
    {
        Queue queue;
        queue_init(&queue, NULL);
        int a = 1, b = 2;
        queue_enqueue(&queue, &a);
        queue_enqueue(&queue, &b);
        EXPECT_FALSE(queue_empty(&queue));
        queue_clear(&queue);
        EXPECT_TRUE(queue_empty(&queue));
        queue_destroy(&queue);
    }

    // Test 4: Empty with NULL queue
    {
        EXPECT_FALSE(queue_empty(NULL));
    }
}

/**
 * Test: queue_destroy
 * Dependencies: queue_init
 * Description: Tests destruction of the queue and cleanup.
 */
UTEST_TEST_CASE(queue_destroy){
    // Test 1: Destroy empty queue
    {
        Queue queue;
        queue_init(&queue, NULL);
        queue_destroy(&queue);
        EXPECT_EQUAL_UINT(queue_size(&queue), 0);
        EXPECT_TRUE(queue_empty(&queue));
    }

    // Test 2: Destroy queue with elements and destroy function
    {
        Queue queue;
        queue_init(&queue, destroy_counter);
        int a = 1, b = 2, c = 3;
        queue_enqueue(&queue, &a);
        queue_enqueue(&queue, &b);
        queue_enqueue(&queue, &c);
        destroy_count = 0;
        queue_destroy(&queue);
        EXPECT_EQUAL_INT(destroy_count, 3);
    }

    // Test 3: Destroy queue without destroy function
    {
        Queue queue;
        queue_init(&queue, NULL);
        int a = 1, b = 2;
        queue_enqueue(&queue, &a);
        queue_enqueue(&queue, &b);
        queue_destroy(&queue);
        EXPECT_EQUAL_UINT(queue_size(&queue), 0);
    }
}

/**
 * Test: queue_enqueue
 * Dependencies: queue_init, queue_size
 * Description: Tests adding elements to the back of the queue.
 */
UTEST_TEST_CASE(queue_enqueue){
    // Test 1: Enqueue to empty queue
    {
        Queue queue;
        queue_init(&queue, NULL);
        int a = 1;
        EXPECT_EQUAL_INT(queue_enqueue(&queue, &a), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(queue_size(&queue), 1);
        EXPECT_EQUAL_UINT(*(int*)queue_peek(&queue), 1);
        queue_destroy(&queue);
    }

    // Test 2: Enqueue multiple elements
    {
        Queue queue;
        queue_init(&queue, NULL);
        int a = 1, b = 2, c = 3;
        EXPECT_EQUAL_INT(queue_enqueue(&queue, &a), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(queue_enqueue(&queue, &b), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(queue_enqueue(&queue, &c), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(queue_size(&queue), 3);
        EXPECT_EQUAL_UINT(*(int*)queue_peek(&queue), 1);
        queue_destroy(&queue);
    }

    // Test 3: Enqueue NULL element
    {
        Queue queue;
        queue_init(&queue, NULL);
        EXPECT_EQUAL_INT(queue_enqueue(&queue, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(queue_size(&queue), 1);
        EXPECT_NULL(queue_peek(&queue));
        queue_destroy(&queue);
    }

    // Test 4: Enqueue maintains FIFO order
    {
        Queue queue;
        queue_init(&queue, NULL);
        int a = 1, b = 2, c = 3;
        queue_enqueue(&queue, &a);
        queue_enqueue(&queue, &b);
        queue_enqueue(&queue, &c);
        void* dequeued = NULL;
        queue_dequeue(&queue, &dequeued);
        EXPECT_EQUAL_UINT(*(int*)dequeued, 1);
        queue_dequeue(&queue, &dequeued);
        EXPECT_EQUAL_UINT(*(int*)dequeued, 2);
        queue_dequeue(&queue, &dequeued);
        EXPECT_EQUAL_UINT(*(int*)dequeued, 3);
        queue_destroy(&queue);
    }

    // Test 5: Enqueue with NULL queue
    {
        EXPECT_EQUAL_INT(queue_enqueue(NULL, NULL), COLLECTION_FAILURE);
    }
}

/**
 * Test: queue_dequeue
 * Dependencies: queue_init, queue_enqueue, queue_size
 * Description: Tests removing elements from the front of the queue.
 */
UTEST_TEST_CASE(queue_dequeue){
    // Test 1: Dequeue from empty queue
    {
        Queue queue;
        queue_init(&queue, NULL);
        EXPECT_EQUAL_INT(queue_dequeue(&queue, NULL), COLLECTION_FAILURE);
        EXPECT_EQUAL_UINT(queue_size(&queue), 0);
        queue_destroy(&queue);
    }

    // Test 2: Dequeue without retrieving data
    {
        Queue queue;
        queue_init(&queue, NULL);
        int a = 1, b = 2;
        queue_enqueue(&queue, &a);
        queue_enqueue(&queue, &b);
        EXPECT_EQUAL_INT(queue_dequeue(&queue, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(queue_size(&queue), 1);
        EXPECT_EQUAL_UINT(*(int*)queue_peek(&queue), 2);
        queue_destroy(&queue);
    }

    // Test 3: Dequeue with retrieving data
    {
        Queue queue;
        queue_init(&queue, NULL);
        int a = 1, b = 2;
        queue_enqueue(&queue, &a);
        queue_enqueue(&queue, &b);
        void* dequeued = NULL;
        EXPECT_EQUAL_INT(queue_dequeue(&queue, &dequeued), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)dequeued, 1);
        EXPECT_EQUAL_UINT(queue_size(&queue), 1);
        queue_destroy(&queue);
    }

    // Test 4: Dequeue with destroy function
    {
        Queue queue;
        queue_init(&queue, destroy_counter);
        int a = 1, b = 2;
        queue_enqueue(&queue, &a);
        queue_enqueue(&queue, &b);
        destroy_count = 0;
        EXPECT_EQUAL_INT(queue_dequeue(&queue, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(destroy_count, 1);
        EXPECT_EQUAL_UINT(queue_size(&queue), 1);
        queue_destroy(&queue);
    }

    // Test 5: Dequeue all elements
    {
        Queue queue;
        queue_init(&queue, NULL);
        int a = 1, b = 2, c = 3;
        queue_enqueue(&queue, &a);
        queue_enqueue(&queue, &b);
        queue_enqueue(&queue, &c);
        EXPECT_EQUAL_INT(queue_dequeue(&queue, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(queue_dequeue(&queue, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_INT(queue_dequeue(&queue, NULL), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(queue_size(&queue), 0);
        EXPECT_TRUE(queue_empty(&queue));
        queue_destroy(&queue);
    }

    // Test 6: Dequeue maintains FIFO order
    {
        Queue queue;
        queue_init(&queue, NULL);
        int values[5] = {1, 2, 3, 4, 5};
        for (int i = 0; i < 5; i++) {
            queue_enqueue(&queue, &values[i]);
        }
        for (int i = 0; i < 5; i++) {
            void* dequeued = NULL;
            queue_dequeue(&queue, &dequeued);
            EXPECT_EQUAL_UINT(*(int*)dequeued, i + 1);
        }
        EXPECT_TRUE(queue_empty(&queue));
        queue_destroy(&queue);
    }
}

/**
 * Test: queue_peek
 * Dependencies: queue_init, queue_enqueue
 * Description: Tests peeking at the front element without removing it.
 */
UTEST_TEST_CASE(queue_peek){
    // Test 1: Peek at empty queue
    {
        Queue queue;
        queue_init(&queue, NULL);
        EXPECT_NULL(queue_peek(&queue));
        queue_destroy(&queue);
    }

    // Test 2: Peek at non-empty queue
    {
        Queue queue;
        queue_init(&queue, NULL);
        int a = 1, b = 2;
        queue_enqueue(&queue, &a);
        EXPECT_EQUAL_UINT(*(int*)queue_peek(&queue), 1);
        queue_enqueue(&queue, &b);
        EXPECT_EQUAL_UINT(*(int*)queue_peek(&queue), 1);
        queue_destroy(&queue);
    }

    // Test 3: Peek doesn't remove element
    {
        Queue queue;
        queue_init(&queue, NULL);
        int a = 1, b = 2;
        queue_enqueue(&queue, &a);
        queue_enqueue(&queue, &b);
        EXPECT_EQUAL_UINT(*(int*)queue_peek(&queue), 1);
        EXPECT_EQUAL_UINT(queue_size(&queue), 2);
        EXPECT_EQUAL_UINT(*(int*)queue_peek(&queue), 1);
        EXPECT_EQUAL_UINT(queue_size(&queue), 2);
        queue_destroy(&queue);
    }

    // Test 4: Peek returns front element
    {
        Queue queue;
        queue_init(&queue, NULL);
        int a = 1, b = 2, c = 3;
        queue_enqueue(&queue, &a);
        queue_enqueue(&queue, &b);
        queue_enqueue(&queue, &c);
        EXPECT_EQUAL_UINT(*(int*)queue_peek(&queue), 1);
        queue_dequeue(&queue, NULL);
        EXPECT_EQUAL_UINT(*(int*)queue_peek(&queue), 2);
        queue_dequeue(&queue, NULL);
        EXPECT_EQUAL_UINT(*(int*)queue_peek(&queue), 3);
        queue_destroy(&queue);
    }

    // Test 5: Peek with NULL element
    {
        Queue queue;
        queue_init(&queue, NULL);
        queue_enqueue(&queue, NULL);
        EXPECT_NULL(queue_peek(&queue));
        queue_destroy(&queue);
    }
}

/**
 * Test: queue_clear
 * Dependencies: queue_init, queue_enqueue, queue_size
 * Description: Tests removing all elements from the queue.
 */
UTEST_TEST_CASE(queue_clear){
    // Test 1: Clear empty queue
    {
        Queue queue;
        queue_init(&queue, NULL);
        queue_clear(&queue);
        EXPECT_EQUAL_UINT(queue_size(&queue), 0);
        EXPECT_TRUE(queue_empty(&queue));
        queue_destroy(&queue);
    }

    // Test 2: Clear queue with elements
    {
        Queue queue;
        queue_init(&queue, NULL);
        int a = 1, b = 2, c = 3;
        queue_enqueue(&queue, &a);
        queue_enqueue(&queue, &b);
        queue_enqueue(&queue, &c);
        queue_clear(&queue);
        EXPECT_EQUAL_UINT(queue_size(&queue), 0);
        EXPECT_TRUE(queue_empty(&queue));
        EXPECT_NULL(queue_peek(&queue));
        queue_destroy(&queue);
    }

    // Test 3: Clear with destroy function
    {
        Queue queue;
        queue_init(&queue, destroy_counter);
        int a = 1, b = 2, c = 3;
        queue_enqueue(&queue, &a);
        queue_enqueue(&queue, &b);
        queue_enqueue(&queue, &c);
        destroy_count = 0;
        queue_clear(&queue);
        EXPECT_EQUAL_INT(destroy_count, 3);
        EXPECT_EQUAL_UINT(queue_size(&queue), 0);
        queue_destroy(&queue);
    }

    // Test 4: Use after clear
    {
        Queue queue;
        queue_init(&queue, NULL);
        int a = 1, b = 2;
        queue_enqueue(&queue, &a);
        queue_clear(&queue);
        queue_enqueue(&queue, &b);
        EXPECT_EQUAL_UINT(queue_size(&queue), 1);
        EXPECT_EQUAL_UINT(*(int*)queue_peek(&queue), 2);
        queue_destroy(&queue);
    }
}

/**
 * Test: queue_memory_leak
 * Dependencies: queue_init, queue_destroy, queue_enqueue, queue_dequeue, queue_clear
 * Description: Tests for memory leaks by tracking destroy function calls.
 */
UTEST_TEST_CASE(queue_memory_leak){
    // Test 1: All elements destroyed on queue_destroy
    {
        Queue queue;
        queue_init(&queue, destroy_counter);
        int values[10];
        for (int i = 0; i < 10; i++) {
            values[i] = i;
            queue_enqueue(&queue, &values[i]);
        }
        destroy_count = 0;
        queue_destroy(&queue);
        EXPECT_EQUAL_INT(destroy_count, 10);
    }

    // Test 2: Elements destroyed on dequeue
    {
        Queue queue;
        queue_init(&queue, destroy_counter);
        int values[5];
        for (int i = 0; i < 5; i++) {
            values[i] = i;
            queue_enqueue(&queue, &values[i]);
        }
        destroy_count = 0;
        for (int i = 0; i < 5; i++) {
            queue_dequeue(&queue, NULL);
        }
        EXPECT_EQUAL_INT(destroy_count, 5);
        queue_destroy(&queue);
        EXPECT_EQUAL_INT(destroy_count, 5);
    }

    // Test 3: Elements destroyed on clear
    {
        Queue queue;
        queue_init(&queue, destroy_counter);
        int values[10];
        for (int i = 0; i < 10; i++) {
            values[i] = i;
            queue_enqueue(&queue, &values[i]);
        }
        destroy_count = 0;
        queue_clear(&queue);
        EXPECT_EQUAL_INT(destroy_count, 10);
        queue_destroy(&queue);
        EXPECT_EQUAL_INT(destroy_count, 10);
    }
    // Test 4: Elements freed by free
    {
        Queue queue;
        queue_init(&queue, free);
        for (int i = 0; i < 10; i++) {
            int* value = malloc(sizeof(int));
            *value = i;
            queue_enqueue(&queue, value);
        }
        EXPECT_EQUAL_UINT(queue_size(&queue), 10);
        queue_destroy(&queue);
    }
}

/**
 * Test: queue_integration
 * Dependencies: All queue functions
 * Description: Integration test combining multiple operations.
 */
UTEST_TEST_CASE(queue_integration){
    // Test 1: Complex sequence of operations
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        int values[10];
        for (int i = 0; i < 10; i++) {
            values[i] = i;
            EXPECT_EQUAL_INT(queue_enqueue(&queue, &values[i]), COLLECTION_SUCCESS);
        }
        EXPECT_EQUAL_UINT(queue_size(&queue), 10);
        
        void* peeked = queue_peek(&queue);
        EXPECT_EQUAL_UINT(*(int*)peeked, 0);
        EXPECT_EQUAL_UINT(queue_size(&queue), 10);
        
        void* dequeued = NULL;
        EXPECT_EQUAL_INT(queue_dequeue(&queue, &dequeued), COLLECTION_SUCCESS);
        EXPECT_EQUAL_UINT(*(int*)dequeued, 0);
        EXPECT_EQUAL_UINT(queue_size(&queue), 9);
        
        queue_clear(&queue);
        EXPECT_EQUAL_UINT(queue_size(&queue), 0);
        EXPECT_TRUE(queue_empty(&queue));
        
        queue_destroy(&queue);
    }

    // Test 2: FIFO order verification
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        int values[5] = {1, 2, 3, 4, 5};
        for (int i = 0; i < 5; i++) {
            queue_enqueue(&queue, &values[i]);
        }
        
        for (int i = 0; i < 5; i++) {
            void* dequeued = NULL;
            queue_dequeue(&queue, &dequeued);
            EXPECT_EQUAL_UINT(*(int*)dequeued, values[i]);
        }
        
        EXPECT_TRUE(queue_empty(&queue));
        queue_destroy(&queue);
    }

    // Test 3: Enqueue-dequeue pattern
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        int a = 1, b = 2, c = 3;
        queue_enqueue(&queue, &a);
        queue_enqueue(&queue, &b);
        
        void* dequeued = NULL;
        queue_dequeue(&queue, &dequeued);
        EXPECT_EQUAL_UINT(*(int*)dequeued, 1);
        
        queue_enqueue(&queue, &c);
        EXPECT_EQUAL_UINT(queue_size(&queue), 2);
        EXPECT_EQUAL_UINT(*(int*)queue_peek(&queue), 2);
        
        queue_destroy(&queue);
    }

    // Test 4: Edge cases combination
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        EXPECT_TRUE(queue_empty(&queue));
        EXPECT_NULL(queue_peek(&queue));
        
        int a = 1;
        queue_enqueue(&queue, &a);
        EXPECT_FALSE(queue_empty(&queue));
        EXPECT_EQUAL_UINT(*(int*)queue_peek(&queue), 1);
        
        queue_dequeue(&queue, NULL);
        EXPECT_TRUE(queue_empty(&queue));
        EXPECT_NULL(queue_peek(&queue));
        
        queue_destroy(&queue);
    }

    // Test 5: Multiple enqueue-dequeue cycles
    {
        Queue queue;
        queue_init(&queue, NULL);
        
        for (int cycle = 0; cycle < 3; cycle++) {
            int values[3] = {cycle * 3 + 1, cycle * 3 + 2, cycle * 3 + 3};
            for (int i = 0; i < 3; i++) {
                queue_enqueue(&queue, &values[i]);
            }
            for (int i = 0; i < 3; i++) {
                void* dequeued = NULL;
                queue_dequeue(&queue, &dequeued);
                EXPECT_EQUAL_UINT(*(int*)dequeued, values[i]);
            }
            EXPECT_TRUE(queue_empty(&queue));
        }
        
        queue_destroy(&queue);
    }
}

/**
 * Test suite: queue
 * Description: Test suite for queue data structure
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
    UTEST_RUN_TEST_CASE(queue_memory_leak);
    UTEST_RUN_TEST_CASE(queue_integration);
}

