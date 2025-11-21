#include <stdio.h>
#include <utest.h>
#include <stdlib.h>

#define MATCH(A, B) strcmp(A, B) == 0

extern UTEST_TEST_SUITE(vector);
extern UTEST_TEST_SUITE(slist);
extern UTEST_TEST_SUITE(dlist);
extern UTEST_TEST_SUITE(clist);
extern UTEST_TEST_SUITE(stack);
extern UTEST_TEST_SUITE(queue);
extern UTEST_TEST_SUITE(hash);
extern UTEST_TEST_SUITE(chtbl);
extern UTEST_TEST_SUITE(set);
extern UTEST_TEST_SUITE(sort);

const char * AVAILABLE_MODULES[] = {
    "vector",
    "slist",
    "dlist",
    "clist",
    "stack",
    "queue",
    "hash",
    "chtbl",
    "set",
    "sort",
    "all",
};

//@brief: run all tests
void all_tests(void){
    UTEST_RUN_TEST_SUITE(vector);
    UTEST_RUN_TEST_SUITE(slist);
    UTEST_RUN_TEST_SUITE(dlist);
    UTEST_RUN_TEST_SUITE(clist);
    UTEST_RUN_TEST_SUITE(stack);
    UTEST_RUN_TEST_SUITE(queue);
    UTEST_RUN_TEST_SUITE(hash);
    UTEST_RUN_TEST_SUITE(chtbl);
    UTEST_RUN_TEST_SUITE(set);
    UTEST_RUN_TEST_SUITE(sort);
}

//@brief: dispatch the tests
void dispatch_tests(char * module){
    if (MATCH(module, "vector")) {
        UTEST_RUN_TEST_SUITE(vector);
    } else if (MATCH(module, "slist")) {
        UTEST_RUN_TEST_SUITE(slist);
    } else if (MATCH(module, "dlist")) {
        UTEST_RUN_TEST_SUITE(dlist);
    } else if (MATCH(module, "clist")) {
        UTEST_RUN_TEST_SUITE(clist);
    } else if (MATCH(module, "stack")) {
        UTEST_RUN_TEST_SUITE(stack);
    } else if (MATCH(module, "queue")) {
        UTEST_RUN_TEST_SUITE(queue);
    } else if (MATCH(module, "hash")) {
        UTEST_RUN_TEST_SUITE(hash);
    } else if (MATCH(module, "chtbl")) {
        UTEST_RUN_TEST_SUITE(chtbl);
    } else if (MATCH(module, "set")) {
        UTEST_RUN_TEST_SUITE(set);
    } else if (MATCH(module, "sort")) {
        UTEST_RUN_TEST_SUITE(sort);
    } else if (MATCH(module, "all")) {
        all_tests();
    }
    else {
        fprintf(stderr, "Unknown module: %s\n", module);
        fprintf(stderr, "Available modules: ");
        for (size_t i = 0; i < sizeof(AVAILABLE_MODULES) / sizeof(AVAILABLE_MODULES[0]); i++) {
            fprintf(stderr, "%s ", AVAILABLE_MODULES[i]);
        }
        fprintf(stderr, "\n");
        exit(1);
    }
}

/**
 * Main function for the unit tests.
 * usage -m [module]
 */
int main(int argc, char * argv[]){
    // clear the style full flag
    UTEST_BEGIN();
    UTEST_CLEAR_FLAG(UTEST_FLAG_STYLE_FULL);

    // dispatch the tests
    if (argc > 2 && MATCH(argv[1], "-m")) {
        dispatch_tests(argv[2]);
    } else {
        // hide the test cases
        UTEST_CLEAR_FLAG(UTEST_FLAG_SHOW_CASE);

        // run all tests
        dispatch_tests("all");
    }

    UTEST_END();
}