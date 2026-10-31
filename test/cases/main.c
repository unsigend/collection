/* Test runner for collection.

   Usage: ./test -j[nthreads] [suite] */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utest.h>

#define NTHREADS 5
#define NTHREADMAX 64

int main(int argc, char *argv[])
{
    int nthreads = NTHREADS;
    const char *suite = "all";

    UTEST_INIT(UTF_STOPONASS | UTF_SHOWSUITE | UTF_STOPONCASE);

    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "-j", 2) == 0) {
            nthreads = atoi(argv[i] + 2);
            if (nthreads <= 0 || nthreads > NTHREADMAX) {
                fprintf(stderr, "invalid number of threads: %s\n", argv[i] + 2);
                return EXIT_FAILURE;
            }
        } else
            suite = argv[i];
    }

    if (strcmp(suite, "all") == 0)
        UTEST_RUNSUITES_THREAD(nthreads);
    else if (UTEST_RUNSUITE(suite) != 0) {
        fprintf(stderr, "unknown test suite: %s\n", suite);
        return EXIT_FAILURE;
    }

    UTEST_FINI();
    return EXIT_SUCCESS;
}
