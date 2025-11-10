#include <utest.h>

#include <collection/vector.h>

// @test: test the initialization of the vector
UTEST_TEST_CASE(vector_init){
    Vector vector;
    vector_init(&vector, NULL);
    EXPECT_TRUE(vector.size == 0);
    EXPECT_TRUE(vector.capacity == 0);
    EXPECT_TRUE(vector.data == NULL);
    EXPECT_TRUE(vector.destroy == NULL);
}

// @test: test the destruction of the vector
UTEST_TEST_CASE(vector_destroy){
    Vector vector;
    vector_init(&vector, NULL);
    vector_destroy(&vector);
    EXPECT_TRUE(vector.size == 0);
    EXPECT_TRUE(vector.capacity == 0);
    EXPECT_TRUE(vector.data == NULL);
    EXPECT_TRUE(vector.destroy == NULL);
}

// @test: test the size of the vector
UTEST_TEST_CASE(vector_size){
    Vector vector;
    vector_init(&vector, NULL);
    EXPECT_TRUE(vector_size(&vector) == 0);
}

// @test: test the capacity of the vector
UTEST_TEST_CASE(vector_capacity){
    Vector vector;
    vector_init(&vector, NULL);
    EXPECT_TRUE(vector_capacity(&vector) == 0);
}

// @test: test the last element of the vector
UTEST_TEST_CASE(vector_back){
    Vector vector;
    vector_init(&vector, NULL);
    EXPECT_TRUE(vector_back(&vector) == NULL);
}

// @test: test the first element of the vector
UTEST_TEST_CASE(vector_front){
    Vector vector;
    vector_init(&vector, NULL);
    EXPECT_TRUE(vector_front(&vector) == NULL);
}

// @test: test the resize of the vector
UTEST_TEST_CASE(vector_resize){
    // expand case
    {
        Vector vector;
        vector_init(&vector, NULL);

        vector_resize(&vector, 10);
        EXPECT_TRUE(vector_size(&vector) == 10);
        EXPECT_TRUE(vector_capacity(&vector) == 10);
        EXPECT_TRUE(vector.data != NULL);
        
        for (size_t i = 0; i < 10; i++) {
            EXPECT_TRUE(vector.data[i] == NULL);
        }

        vector_destroy(&vector);
    }
    // shrink case
    {
        Vector vector;
        vector_init(&vector, NULL);

        vector_resize(&vector, 10);
        const char * data = "DATA";
        for (size_t i = 0; i < 10; i++) {
            vector.data[i] = (void *)data;
        }
        EXPECT_TRUE(vector_size(&vector) == 10);
        EXPECT_TRUE(vector_capacity(&vector) == 10);
        EXPECT_TRUE(vector.data != NULL);
        
        for (size_t i = 0; i < 10; i++) {
            EXPECT_TRUE(vector.data[i] == (void *)data);
        }

        vector_resize(&vector, 5);
        EXPECT_TRUE(vector_size(&vector) == 5);
        // capacity should not change
        EXPECT_TRUE(vector_capacity(&vector) == 10);
        EXPECT_TRUE(vector.data != NULL);
        
        for (size_t i = 0; i < 5; i++) {
            EXPECT_TRUE(vector.data[i] == (void *)data);
        }

        // the elements in the range [5, 10) should be destroyed
        for (size_t i = 5; i < 10; i++) {
            EXPECT_TRUE(vector.data[i] == NULL);
        }

        vector_destroy(&vector);
    }
}


// @test: test the shrink to fit of the vector
UTEST_TEST_CASE(vector_shrink_to_fit){
    Vector vector;
    vector_init(&vector, NULL);
    vector_resize(&vector, 10);
    vector_shrink_to_fit(&vector);
    EXPECT_TRUE(vector_size(&vector) == 10);
    EXPECT_TRUE(vector_capacity(&vector) == 10);
}


UTEST_TEST_SUITE(vector){
    UTEST_RUN_TEST_CASE(vector_init);
    UTEST_RUN_TEST_CASE(vector_destroy);
    UTEST_RUN_TEST_CASE(vector_size);
    UTEST_RUN_TEST_CASE(vector_capacity);
    UTEST_RUN_TEST_CASE(vector_back);
    UTEST_RUN_TEST_CASE(vector_front);
    UTEST_RUN_TEST_CASE(vector_resize);
    UTEST_RUN_TEST_CASE(vector_shrink_to_fit);
}