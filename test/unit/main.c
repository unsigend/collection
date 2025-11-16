#include <stdio.h>
#include <utest.h>
#include <stdlib.h>

#define MATCH(A, B) strcmp(A, B) == 0

extern UTEST_TEST_SUITE(vector);
extern UTEST_TEST_SUITE(slist);
extern UTEST_TEST_SUITE(dlist);
extern UTEST_TEST_SUITE(clist);
extern UTEST_TEST_SUITE(stack);

//@brief: run all tests
void all_tests(void){
    UTEST_RUN_TEST_SUITE(vector);
    UTEST_RUN_TEST_SUITE(slist);
    UTEST_RUN_TEST_SUITE(dlist);
    UTEST_RUN_TEST_SUITE(clist);
    UTEST_RUN_TEST_SUITE(stack);
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
    } else if (MATCH(module, "all")) {
        all_tests();
    }
    else {
        fprintf(stderr, "Unknown module: %s\n", module);
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