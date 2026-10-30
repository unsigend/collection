/* dvec vs std::vector benchmark.
   Build/run: make bench-dvec
   Always compiled -O2, no sanitizers. */

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <dvec.h>

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

static double now_ms(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1e6;
}

static void bench(const char *name, const char *impl, double ms, int n,
                  double other_ms)
{
    double ns = ms * 1e6 / (double)n;

    std::printf("  %-16s  %-7s  %8.2f  %8.1f", name, impl, ms, ns);
    if (other_ms > 0.0)
        std::printf("  %5.1fx", other_ms / ms);
    else
        std::printf("       -");
    std::printf("\n");
}

static void fill_dvec(struct dvec *v, int n)
{
    int i;

    dvec_init_empty(v, sizeof(int));
    for (i = 0; i < n; i++)
        dvec_pushb(v, &i);
}

static void push_back(int n)
{
    struct dvec v;
    std::vector<int> sv;
    double t0, dms, sms;
    int i;

    dvec_init_empty(&v, sizeof(int));
    t0 = now_ms();
    for (i = 0; i < n; i++)
        dvec_pushb(&v, &i);
    dms = now_ms() - t0;
    if (dvec_len(&v) != (size_t)n)
        std::abort();
    dvec_free(&v);

    t0 = now_ms();
    for (i = 0; i < n; i++)
        sv.push_back(i);
    sms = now_ms() - t0;
    if (sv.size() != (size_t)n)
        std::abort();

    bench("push-back", "dvec", dms, n, sms);
    bench("push-back", "std", sms, n, 0.0);
}

static void reserve_push(int n)
{
    struct dvec v;
    std::vector<int> sv;
    double t0, dms, sms;
    int i;

    dvec_init_empty(&v, sizeof(int));
    t0 = now_ms();
    dvec_reserve(&v, (size_t)n);
    for (i = 0; i < n; i++)
        dvec_pushb(&v, &i);
    dms = now_ms() - t0;
    dvec_free(&v);

    t0 = now_ms();
    sv.reserve((size_t)n);
    for (i = 0; i < n; i++)
        sv.push_back(i);
    sms = now_ms() - t0;

    bench("reserve-push", "dvec", dms, n, sms);
    bench("reserve-push", "std", sms, n, 0.0);
}

static void pop_back(int n)
{
    struct dvec v;
    std::vector<int> sv;
    double t0, dms, sms;
    int i, out;
    volatile int sink = 0;

    fill_dvec(&v, n);
    t0 = now_ms();
    for (i = 0; i < n; i++) {
        dvec_popb(&v, &out);
        sink += out;
    }
    dms = now_ms() - t0;
    dvec_free(&v);

    sv.reserve((size_t)n);
    for (i = 0; i < n; i++)
        sv.push_back(i);
    t0 = now_ms();
    for (i = 0; i < n; i++) {
        sink += sv.back();
        sv.pop_back();
    }
    sms = now_ms() - t0;
    (void)sink;

    bench("pop-back", "dvec", dms, n, sms);
    bench("pop-back", "std", sms, n, 0.0);
}

static void at_index(int n)
{
    struct dvec v;
    std::vector<int> sv;
    double t0, dms, sms;
    int i, reps = n * 10;
    volatile int sink = 0;

    fill_dvec(&v, n);
    t0 = now_ms();
    for (i = 0; i < reps; i++)
        sink += *(int *)dvec_at(&v, (size_t)((i * 3) % n));
    dms = now_ms() - t0;
    dvec_free(&v);

    sv.reserve((size_t)n);
    for (i = 0; i < n; i++)
        sv.push_back(i);
    t0 = now_ms();
    for (i = 0; i < reps; i++)
        sink += sv[(size_t)((i * 3) % n)];
    sms = now_ms() - t0;
    (void)sink;

    bench("at", "dvec", dms, reps, sms);
    bench("at", "std", sms, reps, 0.0);
}

static void dup_copy(int n)
{
    struct dvec v, d;
    std::vector<int> sv;
    double t0, dms, sms;
    int i;

    fill_dvec(&v, n);
    t0 = now_ms();
    dvec_dup(&v, &d);
    dms = now_ms() - t0;
    if (dvec_len(&d) != (size_t)n)
        std::abort();
    dvec_free(&d);
    dvec_free(&v);

    sv.reserve((size_t)n);
    for (i = 0; i < n; i++)
        sv.push_back(i);
    t0 = now_ms();
    std::vector<int> sc(sv);
    sms = now_ms() - t0;
    if (sc.size() != (size_t)n)
        std::abort();

    bench("dup", "dvec", dms, n, sms);
    bench("dup", "std", sms, n, 0.0);
}

static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

static void sort_n(int n)
{
    struct dvec v;
    std::vector<int> sv;
    double t0, dms, sms;
    int i, x;

    dvec_init_empty(&v, sizeof(int));
    for (i = 0; i < n; i++) {
        x = n - 1 - i;
        dvec_pushb(&v, &x);
    }
    t0 = now_ms();
    dvec_sort(&v, cmp_int);
    dms = now_ms() - t0;
    dvec_free(&v);

    sv.reserve((size_t)n);
    for (i = 0; i < n; i++)
        sv.push_back(n - 1 - i);
    t0 = now_ms();
    std::sort(sv.begin(), sv.end());
    sms = now_ms() - t0;

    bench("sort", "dvec", dms, n, sms);
    bench("sort", "std", sms, n, 0.0);
}

int main(int argc, char **argv)
{
    int n = 2000000;

    if (argc > 1) {
        n = std::atoi(argv[1]);
        if (n <= 0)
            n = 2000000;
    }

    std::printf("dvec benchmark  (iters=%d)\n", n);
    std::printf("std = C++ std::vector<int>\n\n");
    std::printf("  %-16s  %-7s  %8s  %8s  %s\n", "name", "impl", "ms", "ns/op",
                "vs std");
    std::printf("  ----------------  -------  --------  --------  -------\n");

    push_back(n);
    reserve_push(n);
    pop_back(n);
    at_index(n);
    dup_copy(n);
    sort_n(n);

    std::printf("\nvs std: higher means dvec is faster than std::vector.\n");
    return 0;
}
