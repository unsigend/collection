/* dlist vs std::list benchmark.
   Build/run: make bench-dlist
   Always compiled -O2, no sanitizers. */

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <dlist.h>

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iterator>
#include <list>

#define V(n) ((void *)(std::ptrdiff_t)(n))

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

static void add_tail(int n)
{
    struct dlist *l;
    std::list<void *> sl;
    double t0, dms, sms;
    int i;

    l = dlist_init();
    t0 = now_ms();
    for (i = 0; i < n; i++)
        dlist_add_tail(l, V(i));
    dms = now_ms() - t0;
    if (dlist_len(l) != (size_t)n)
        std::abort();
    dlist_free(l);

    t0 = now_ms();
    for (i = 0; i < n; i++)
        sl.push_back(V(i));
    sms = now_ms() - t0;
    if (sl.size() != (size_t)n)
        std::abort();

    bench("add-tail", "dlist", dms, n, sms);
    bench("add-tail", "std", sms, n, 0.0);
}

static void add_head(int n)
{
    struct dlist *l;
    std::list<void *> sl;
    double t0, dms, sms;
    int i;

    l = dlist_init();
    t0 = now_ms();
    for (i = 0; i < n; i++)
        dlist_add_head(l, V(i));
    dms = now_ms() - t0;
    dlist_free(l);

    t0 = now_ms();
    for (i = 0; i < n; i++)
        sl.push_front(V(i));
    sms = now_ms() - t0;

    bench("add-head", "dlist", dms, n, sms);
    bench("add-head", "std", sms, n, 0.0);
}

static void del_head(int n)
{
    struct dlist *l;
    std::list<void *> sl;
    double t0, dms, sms;
    int i;

    l = dlist_init();
    for (i = 0; i < n; i++)
        dlist_add_tail(l, V(i));
    t0 = now_ms();
    while (dlist_len(l))
        dlist_del(l, dlist_first(l));
    dms = now_ms() - t0;
    dlist_free(l);

    for (i = 0; i < n; i++)
        sl.push_back(V(i));
    t0 = now_ms();
    while (!sl.empty())
        sl.pop_front();
    sms = now_ms() - t0;

    bench("del-head", "dlist", dms, n, sms);
    bench("del-head", "std", sms, n, 0.0);
}

static void index_mid(int n)
{
    struct dlist *l;
    std::list<void *> sl;
    double t0, dms, sms;
    int i, mid = n / 2;
    volatile std::uintptr_t sink = 0;

    l = dlist_init();
    for (i = 0; i < n; i++)
        dlist_add_tail(l, V(i));
    t0 = now_ms();
    for (i = 0; i < n; i++) {
        size_t idx = (size_t)((mid + (i & 1)) % n);
        sink += (std::uintptr_t)dlist_nodevalue(dlist_index(l, idx));
    }
    dms = now_ms() - t0;
    dlist_free(l);

    for (i = 0; i < n; i++)
        sl.push_back(V(i));
    t0 = now_ms();
    for (i = 0; i < n; i++) {
        size_t idx = (size_t)((mid + (i & 1)) % n);
        auto it = sl.begin();
        std::advance(it, (std::ptrdiff_t)idx);
        sink += (std::uintptr_t)*it;
    }
    sms = now_ms() - t0;
    (void)sink;

    bench("index-mid", "dlist", dms, n, sms);
    bench("index-mid", "std", sms, n, 0.0);
}

static void search_last(int n)
{
    struct dlist *l;
    std::list<void *> sl;
    double t0, dms, sms;
    int i;
    void *key = V(n - 1);
    volatile int sink = 0;

    l = dlist_init();
    for (i = 0; i < n; i++)
        dlist_add_tail(l, V(i));
    t0 = now_ms();
    for (i = 0; i < n; i++)
        sink += dlist_search(l, key) != NULL;
    dms = now_ms() - t0;
    dlist_free(l);

    for (i = 0; i < n; i++)
        sl.push_back(V(i));
    t0 = now_ms();
    for (i = 0; i < n; i++) {
        int found = 0;
        for (auto p : sl) {
            if (p == key) {
                found = 1;
                break;
            }
        }
        sink += found;
    }
    sms = now_ms() - t0;
    (void)sink;

    bench("search-last", "dlist", dms, n, sms);
    bench("search-last", "std", sms, n, 0.0);
}

static void dup_copy(int n)
{
    struct dlist *l, *c;
    std::list<void *> sl;
    double t0, dms, sms;
    int i;

    l = dlist_init();
    for (i = 0; i < n; i++)
        dlist_add_tail(l, V(i));
    t0 = now_ms();
    c = dlist_dup(l);
    dms = now_ms() - t0;
    if (dlist_len(c) != (size_t)n)
        std::abort();
    dlist_free(c);
    dlist_free(l);

    for (i = 0; i < n; i++)
        sl.push_back(V(i));
    t0 = now_ms();
    std::list<void *> sc(sl);
    sms = now_ms() - t0;
    if (sc.size() != (size_t)n)
        std::abort();

    bench("dup", "dlist", dms, n, sms);
    bench("dup", "std", sms, n, 0.0);
}

int main(int argc, char **argv)
{
    int n = 50000;

    if (argc > 1) {
        n = std::atoi(argv[1]);
        if (n <= 0)
            n = 50000;
    }

    std::printf("dlist benchmark  (iters=%d)\n", n);
    std::printf("std = C++ std::list<void *>\n\n");
    std::printf("  %-16s  %-7s  %8s  %8s  %s\n", "name", "impl", "ms", "ns/op",
                "vs std");
    std::printf("  ----------------  -------  --------  --------  -------\n");

    add_tail(n);
    add_head(n);
    del_head(n);
    index_mid(n / 10 > 0 ? n / 10 : 1);
    search_last(n / 10 > 0 ? n / 10 : 1);
    dup_copy(n);

    std::printf("\nvs std: higher means dlist is faster than std::list.\n");
    return 0;
}
