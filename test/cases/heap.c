/* Tests for heap (binary heap over dvec). */

#include <errno.h>
#include <heap.h>
#include <utest.h>

struct rec {
    int priority;
    int id;
};

static int int_min_cmp(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;

    return (x > y) - (x < y);
}

static int int_max_cmp(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;

    return (x < y) - (x > y);
}

static int rec_min_cmp(const void *a, const void *b)
{
    const struct rec *ra = a;
    const struct rec *rb = b;

    return (ra->priority > rb->priority) - (ra->priority < rb->priority);
}

UTEST_CASE(heap_len)
{
    struct heap h;
    int x = 1;

    EXPECT_EQ_INT(heap_init_empty(&h, sizeof(int), int_min_cmp), 0);
    EXPECT_EQ_UINT(heap_len(&h), 0);
    EXPECT_EQ_INT(heap_push(&h, &x), 0);
    EXPECT_EQ_UINT(heap_len(&h), 1);
    EXPECT_EQ_INT(heap_pop(&h, &x), 0);
    EXPECT_EQ_UINT(heap_len(&h), 0);
    heap_free(&h);
}

UTEST_CASE(heap_empty)
{
    struct heap h;
    int x = 5;

    EXPECT_EQ_INT(heap_init_empty(&h, sizeof(int), int_min_cmp), 0);
    EXPECT_TRUE(heap_empty(&h));
    EXPECT_EQ_INT(heap_push(&h, &x), 0);
    EXPECT_FALSE(heap_empty(&h));
    EXPECT_EQ_INT(heap_pop(&h, &x), 0);
    EXPECT_TRUE(heap_empty(&h));
    heap_free(&h);
}

UTEST_CASE(heap_avail)
{
    struct heap h;
    int x = 1;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), 4, int_min_cmp), 0);
    EXPECT_EQ_UINT(heap_avail(&h), 4);
    EXPECT_EQ_INT(heap_push(&h, &x), 0);
    EXPECT_EQ_UINT(heap_avail(&h), 3);
    heap_free(&h);
}

UTEST_CASE(heap_init)
{
    struct heap h;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), 8, int_min_cmp), 0);
    EXPECT_EQ_UINT(heap_len(&h), 0);
    EXPECT_TRUE(heap_empty(&h));
    EXPECT_EQ_UINT(heap_avail(&h), 8);
    EXPECT_EQ_PTR(heap_cmp(&h), int_min_cmp);
    heap_free(&h);

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), 0, int_min_cmp), 0);
    EXPECT_TRUE(heap_empty(&h));
    EXPECT_EQ_UINT(heap_avail(&h), 0);
    heap_free(&h);
}

UTEST_CASE(heap_init_empty)
{
    struct heap h;

    EXPECT_EQ_INT(heap_init_empty(&h, sizeof(int), int_min_cmp), 0);
    EXPECT_EQ_UINT(heap_len(&h), 0);
    EXPECT_TRUE(heap_empty(&h));
    EXPECT_EQ_PTR(heap_cmp(&h), int_min_cmp);
    heap_free(&h);

    EXPECT_EQ_INT(heap_init_empty(&h, sizeof(struct rec), rec_min_cmp), 0);
    EXPECT_EQ_UINT(h.dvec.elesz, sizeof(struct rec));
    heap_free(&h);
}

UTEST_CASE(heap_free_null)
{
    struct heap *p = NULL;

    heap_free(p);
    EXPECT_NULL(p);
}

UTEST_CASE(heap_clear)
{
    struct heap h;
    int x = 3;
    size_t cap;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), 4, int_min_cmp), 0);
    EXPECT_EQ_INT(heap_push(&h, &x), 0);
    EXPECT_EQ_INT(heap_push(&h, &x), 0);
    cap = h.dvec.cap;
    heap_clear(&h);
    EXPECT_TRUE(heap_empty(&h));
    EXPECT_EQ_UINT(h.dvec.cap, cap);
    EXPECT_NULL(heap_top(&h));
    EXPECT_EQ_INT(heap_push(&h, &x), 0);
    EXPECT_EQ_UINT(heap_len(&h), 1);
    heap_free(&h);
}

UTEST_CASE(heap_top)
{
    struct heap h;
    int a = 5, b = 2, c = 8;

    EXPECT_EQ_INT(heap_init_empty(&h, sizeof(int), int_min_cmp), 0);
    EXPECT_NULL(heap_top(&h));
    EXPECT_EQ_INT(heap_push(&h, &a), 0);
    EXPECT_EQ_INT(*(int *)heap_top(&h), 5);
    EXPECT_EQ_INT(heap_push(&h, &b), 0);
    EXPECT_EQ_INT(*(int *)heap_top(&h), 2);
    EXPECT_EQ_INT(heap_push(&h, &c), 0);
    EXPECT_EQ_INT(*(int *)heap_top(&h), 2);
    heap_free(&h);
}

UTEST_CASE(heap_push_pop_min_order)
{
    struct heap h;
    int vals[] = {5, 3, 8, 1, 4, 9, 2, 7, 6};
    int n = (int)(sizeof(vals) / sizeof(vals[0]));
    int out, prev;
    int i;

    EXPECT_EQ_INT(heap_init_empty(&h, sizeof(int), int_min_cmp), 0);
    for (i = 0; i < n; i++)
        EXPECT_EQ_INT(heap_push(&h, &vals[i]), 0);
    EXPECT_EQ_UINT(heap_len(&h), (size_t)n);

    prev = *(int *)heap_top(&h);
    while (!heap_empty(&h)) {
        EXPECT_EQ_INT(heap_pop(&h, &out), 0);
        EXPECT_GE_INT(out, prev);
        prev = out;
    }
    heap_free(&h);
}

UTEST_CASE(heap_push_pop_max_order)
{
    struct heap h;
    int vals[] = {4, 1, 7, 3, 9, 2, 6};
    int n = (int)(sizeof(vals) / sizeof(vals[0]));
    int out, prev;
    int i;

    EXPECT_EQ_INT(heap_init_empty(&h, sizeof(int), int_max_cmp), 0);
    for (i = 0; i < n; i++)
        EXPECT_EQ_INT(heap_push(&h, &vals[i]), 0);

    EXPECT_EQ_INT(*(int *)heap_top(&h), 9);
    prev = *(int *)heap_top(&h);
    while (!heap_empty(&h)) {
        EXPECT_EQ_INT(heap_pop(&h, &out), 0);
        EXPECT_LE_INT(out, prev);
        prev = out;
    }
    heap_free(&h);
}

UTEST_CASE(heap_pop_empty)
{
    struct heap h;
    int out = 0;

    EXPECT_EQ_INT(heap_init_empty(&h, sizeof(int), int_min_cmp), 0);
    EXPECT_EQ_INT(heap_pop(&h, &out), -ENOENT);
    EXPECT_EQ_INT(out, 0);
    heap_free(&h);
}

UTEST_CASE(heap_pop_discard)
{
    struct heap h;
    int a = 3, b = 1;

    EXPECT_EQ_INT(heap_init_empty(&h, sizeof(int), int_min_cmp), 0);
    EXPECT_EQ_INT(heap_push(&h, &a), 0);
    EXPECT_EQ_INT(heap_push(&h, &b), 0);
    EXPECT_EQ_INT(heap_pop(&h, NULL), 0);
    EXPECT_EQ_UINT(heap_len(&h), 1);
    EXPECT_EQ_INT(*(int *)heap_top(&h), 3);
    heap_free(&h);
}

UTEST_CASE(heap_pop_single)
{
    struct heap h;
    int x = 42, out = 0;

    EXPECT_EQ_INT(heap_init_empty(&h, sizeof(int), int_min_cmp), 0);
    EXPECT_EQ_INT(heap_push(&h, &x), 0);
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 42);
    EXPECT_TRUE(heap_empty(&h));
    EXPECT_EQ_INT(heap_pop(&h, &out), -ENOENT);
    heap_free(&h);
}

UTEST_CASE(heap_heapify_order)
{
    struct heap h;
    int base[] = {9, 4, 7, 1, 5, 3, 8, 2, 6};
    int n = (int)(sizeof(base) / sizeof(base[0]));
    int out, prev;

    EXPECT_EQ_INT(heap_init_empty(&h, sizeof(int), int_min_cmp), 0);
    EXPECT_EQ_INT(heap_heapify(&h, base, (size_t)n), 0);
    EXPECT_EQ_UINT(heap_len(&h), (size_t)n);
    EXPECT_EQ_INT(*(int *)heap_top(&h), 1);

    prev = *(int *)heap_top(&h);
    while (!heap_empty(&h)) {
        EXPECT_EQ_INT(heap_pop(&h, &out), 0);
        EXPECT_GE_INT(out, prev);
        prev = out;
    }
    heap_free(&h);
}

UTEST_CASE(heap_heapify_single)
{
    struct heap h;
    int base[] = {42};
    int out = 0;

    EXPECT_EQ_INT(heap_init_empty(&h, sizeof(int), int_min_cmp), 0);
    EXPECT_EQ_INT(heap_heapify(&h, base, 1), 0);
    EXPECT_EQ_UINT(heap_len(&h), 1);
    EXPECT_EQ_INT(*(int *)heap_top(&h), 42);
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 42);
    heap_free(&h);
}

UTEST_CASE(heap_heapify_replaces)
{
    struct heap h;
    int old[] = {10, 20, 30};
    int new_base[] = {3, 1, 2};
    int out;

    EXPECT_EQ_INT(heap_init_empty(&h, sizeof(int), int_min_cmp), 0);
    EXPECT_EQ_INT(heap_heapify(&h, old, 3), 0);
    EXPECT_EQ_INT(heap_heapify(&h, new_base, 3), 0);
    EXPECT_EQ_UINT(heap_len(&h), 3);
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 3);
    heap_free(&h);
}

UTEST_CASE(heap_dup)
{
    struct heap h, copy;
    int vals[] = {4, 2, 7, 1};
    int n = (int)(sizeof(vals) / sizeof(vals[0]));
    int out;
    int i;

    EXPECT_EQ_INT(heap_init_empty(&h, sizeof(int), int_min_cmp), 0);
    for (i = 0; i < n; i++)
        EXPECT_EQ_INT(heap_push(&h, &vals[i]), 0);

    EXPECT_EQ_INT(heap_dup(&h, &copy), 0);
    EXPECT_EQ_UINT(heap_len(&copy), (size_t)n);
    EXPECT_EQ_PTR(heap_cmp(&copy), int_min_cmp);
    EXPECT_EQ_INT(*(int *)heap_top(&copy), 1);

    /* independent: drain copy, source intact */
    while (!heap_empty(&copy))
        EXPECT_EQ_INT(heap_pop(&copy, &out), 0);
    EXPECT_TRUE(heap_empty(&copy));
    EXPECT_EQ_UINT(heap_len(&h), (size_t)n);

    heap_free(&copy);
    heap_free(&h);
}

UTEST_CASE(heap_grow)
{
    struct heap h;
    int i, out, prev;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), 4, int_min_cmp), 0);
    for (i = 0; i < 64; i++)
        EXPECT_EQ_INT(heap_push(&h, &i), 0);
    EXPECT_EQ_UINT(heap_len(&h), 64);
    EXPECT_GT_UINT(h.dvec.cap, 4);

    prev = *(int *)heap_top(&h);
    while (!heap_empty(&h)) {
        EXPECT_EQ_INT(heap_pop(&h, &out), 0);
        EXPECT_GE_INT(out, prev);
        prev = out;
    }
    heap_free(&h);
}

UTEST_CASE(heap_rec)
{
    struct heap h;
    struct rec vals[] = {{5, 0}, {1, 1}, {3, 2}, {2, 3}, {4, 4}};
    int n = (int)(sizeof(vals) / sizeof(vals[0]));
    struct rec out;
    int i, prev_pri;

    EXPECT_EQ_INT(heap_init_empty(&h, sizeof(struct rec), rec_min_cmp), 0);
    for (i = 0; i < n; i++)
        EXPECT_EQ_INT(heap_push(&h, &vals[i]), 0);

    EXPECT_EQ_INT(((struct rec *)heap_top(&h))->priority, 1);
    prev_pri = ((struct rec *)heap_top(&h))->priority;
    while (!heap_empty(&h)) {
        EXPECT_EQ_INT(heap_pop(&h, &out), 0);
        EXPECT_GE_INT(out.priority, prev_pri);
        prev_pri = out.priority;
    }
    heap_free(&h);
}

UTEST_SUITE(heap)
{
    UTEST_RUNCASE(heap_len);
    UTEST_RUNCASE(heap_empty);
    UTEST_RUNCASE(heap_avail);
    UTEST_RUNCASE(heap_init);
    UTEST_RUNCASE(heap_init_empty);
    UTEST_RUNCASE(heap_free_null);
    UTEST_RUNCASE(heap_clear);

    UTEST_RUNCASE(heap_top);
    UTEST_RUNCASE(heap_push_pop_min_order);
    UTEST_RUNCASE(heap_push_pop_max_order);
    UTEST_RUNCASE(heap_pop_empty);
    UTEST_RUNCASE(heap_pop_discard);
    UTEST_RUNCASE(heap_pop_single);

    UTEST_RUNCASE(heap_heapify_order);
    UTEST_RUNCASE(heap_heapify_single);
    UTEST_RUNCASE(heap_heapify_replaces);

    UTEST_RUNCASE(heap_dup);
    UTEST_RUNCASE(heap_grow);
    UTEST_RUNCASE(heap_rec);
}
