/* Simple dys vs naive realloc+memcpy benchmark.
   Build/run: make bench-dys
   Always compiled -O2, no sanitizers. */

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <dys.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CHUNK "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
#define CHUNKLEN 64

static double now_ms(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1e6;
}

static char *naive_cat(char *s, const char *t, size_t n)
{
    size_t slen = s ? strlen(s) : 0;
    char *p = realloc(s, slen + n + 1);

    if (!p)
        abort();
    memcpy(p + slen, t, n);
    p[slen + n] = '\0';
    return p;
}

static void bench(const char *name, const char *impl, double ms, int n,
                  double naive_ms)
{
    double ns = ms * 1e6 / (double)n;

    printf("  %-16s  %-7s  %8.2f  %8.1f", name, impl, ms, ns);
    if (naive_ms > 0.0)
        printf("  %5.1fx", naive_ms / ms);
    else
        printf("       -");
    printf("\n");
}

static void append_byte(int n)
{
    dys s;
    char *p;
    double t0, dys_ms, naive_ms;
    int i;

    s = dysnew_empty();
    t0 = now_ms();
    for (i = 0; i < n; i++)
        s = dysncat(s, "x", 1);
    dys_ms = now_ms() - t0;
    if (dyslen(s) != (size_t)n)
        abort();
    dysfree(s);

    p = calloc(1, 1);
    t0 = now_ms();
    for (i = 0; i < n; i++)
        p = naive_cat(p, "x", 1);
    naive_ms = now_ms() - t0;
    if (strlen(p) != (size_t)n)
        abort();
    free(p);

    bench("append-1B", "dys", dys_ms, n, naive_ms);
    bench("append-1B", "naive", naive_ms, n, 0.0);
}

static void append_chunk(int n)
{
    dys s;
    char *p;
    double t0, dys_ms, naive_ms;
    int i;

    s = dysnew_empty();
    t0 = now_ms();
    for (i = 0; i < n; i++)
        s = dysncat(s, CHUNK, CHUNKLEN);
    dys_ms = now_ms() - t0;
    if (dyslen(s) != (size_t)n * CHUNKLEN)
        abort();
    dysfree(s);

    p = calloc(1, 1);
    t0 = now_ms();
    for (i = 0; i < n; i++)
        p = naive_cat(p, CHUNK, CHUNKLEN);
    naive_ms = now_ms() - t0;
    if (strlen(p) != (size_t)n * CHUNKLEN)
        abort();
    free(p);

    bench("append-64B", "dys", dys_ms, n, naive_ms);
    bench("append-64B", "naive", naive_ms, n, 0.0);
}

static void new_free(int n)
{
    double t0, ms;
    int i;
    volatile size_t sink = 0;

    t0 = now_ms();
    for (i = 0; i < n; i++) {
        dys s = dysnew("hello world");
        sink += dyslen(s);
        dysfree(s);
    }
    ms = now_ms() - t0;
    (void)sink;
    bench("new+free", "dys", ms, n, 0.0);
}

static void cpy_grow(int n)
{
    dys s;
    double t0, ms;
    int i;

    s = dysnew("x");
    t0 = now_ms();
    for (i = 0; i < n; i++)
        s = (i & 1) ? dyscpy(s, "short") : dyscpy(s, CHUNK);
    ms = now_ms() - t0;
    dysfree(s);
    bench("cpy-shrink/grow", "dys", ms, n, 0.0);
}

static void dup_n(int n)
{
    dys s, d;
    double t0, ms;
    int i;

    s = dysnew(CHUNK);
    t0 = now_ms();
    for (i = 0; i < n; i++) {
        d = dysdup(s);
        dysfree(d);
    }
    ms = now_ms() - t0;
    dysfree(s);
    bench("dup", "dys", ms, n, 0.0);
}

static void cmp_n(int n)
{
    dys a, b;
    double t0, ms;
    int i;
    volatile int sink = 0;

    a = dysnew("the quick brown fox");
    b = dysnew("the quick brown fox");
    t0 = now_ms();
    for (i = 0; i < n; i++)
        sink += dyscmp(a, b);
    ms = now_ms() - t0;
    (void)sink;
    dysfree(a);
    dysfree(b);
    bench("cmp-equal", "dys", ms, n, 0.0);
}

int main(int argc, char **argv)
{
    int n = 200000;

    if (argc > 1) {
        n = atoi(argv[1]);
        if (n <= 0)
            n = 200000;
    }

    printf("dys benchmark  (iters=%d)\n", n);
    printf("naive = realloc + memcpy on every append (no prealloc)\n\n");
    printf("  %-16s  %-7s  %8s  %8s  %s\n", "name", "impl", "ms", "ns/op",
           "vs naive");
    printf("  ----------------  -------  --------  --------  -------\n");

    append_byte(n);
    append_chunk(n / 8);
    new_free(n);
    cpy_grow(n);
    dup_n(n);
    cmp_n(n * 10);

    printf("\nappend rows: higher vs-naive is better (prealloc vs exact "
           "realloc).\n");
    return 0;
}
