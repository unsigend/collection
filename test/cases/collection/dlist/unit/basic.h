#include <collection.h>
#include <utest.h>

static int dtor_n;
static void dtor_inc(void *p)
{
    (void)p;
    dtor_n++;
}

UTEST_CASE(basic)
{
    {
        struct dlist *d;

        EXPECT_EQ_INT(dlist_init(NULL, NULL), -1);
        EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
        dlist_free(d);
    }

    {
        struct dlist *d;
        void *out;

        EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
        EXPECT_TRUE(dlist_empty(d));
        EXPECT_EQ_UINT(dlist_size(d), 0);
        EXPECT_NULL(dlist_front(d));
        EXPECT_NULL(dlist_back(d));
        out = (void *)1;
        EXPECT_EQ_INT(dlist_popfront(d, &out), -1);
        EXPECT_EQ_INT(dlist_popback(d, &out), -1);
        EXPECT_EQ_INT(dlist_popfront(d, NULL), -1);
        EXPECT_EQ_INT(dlist_popback(d, NULL), -1);
        dlist_free(d);
    }

    {
        struct dlist *d;
        int x;

        EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
        x = 10;
        EXPECT_EQ_INT(dlist_pushfront(d, &x), 0);
        EXPECT_FALSE(dlist_empty(d));
        EXPECT_EQ_UINT(dlist_size(d), 1);
        EXPECT_EQ_PTR(dlist_front(d), &x);
        EXPECT_EQ_PTR(dlist_back(d), &x);
        EXPECT_NOTNULL(dlist_head(d));
        EXPECT_NOTNULL(dlist_tail(d));
        EXPECT_EQ_PTR(dlist_head(d), dlist_tail(d));
        EXPECT_EQ_PTR(dlist_data(dlist_head(d)), &x);
        EXPECT_NULL(dlist_prev(dlist_head(d)));
        EXPECT_NULL(dlist_next(dlist_head(d)));
        dlist_free(d);
    }

    {
        struct dlist *d;
        int a, b, c;
        void *out;

        EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
        a = 1;
        b = 2;
        c = 3;
        EXPECT_EQ_INT(dlist_pushback(d, &a), 0);
        EXPECT_EQ_INT(dlist_pushback(d, &b), 0);
        EXPECT_EQ_INT(dlist_pushback(d, &c), 0);
        EXPECT_EQ_UINT(dlist_size(d), 3);
        EXPECT_EQ_PTR(dlist_front(d), &a);
        EXPECT_EQ_PTR(dlist_back(d), &c);
        out = NULL;
        EXPECT_EQ_INT(dlist_popfront(d, &out), 0);
        EXPECT_EQ_PTR(out, &a);
        out = NULL;
        EXPECT_EQ_INT(dlist_popback(d, &out), 0);
        EXPECT_EQ_PTR(out, &c);
        out = NULL;
        EXPECT_EQ_INT(dlist_popfront(d, &out), 0);
        EXPECT_EQ_PTR(out, &b);
        EXPECT_TRUE(dlist_empty(d));
        EXPECT_EQ_INT(dlist_popfront(d, NULL), -1);
        EXPECT_EQ_INT(dlist_popback(d, NULL), -1);
        dlist_free(d);
    }

    {
        struct dlist *d;
        int a, b, c, d0;
        struct dlist_node *n0, *n1, *n2, *n3;
        void *out;

        EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
        a = 1;
        b = 2;
        c = 3;
        d0 = 4;
        EXPECT_EQ_INT(dlist_pushback(d, &a), 0);
        EXPECT_EQ_INT(dlist_pushback(d, &c), 0);
        n0 = dlist_head(d);
        EXPECT_NOTNULL(n0);
        EXPECT_EQ_INT(dlist_insert(d, n0, &b), 0);
        EXPECT_EQ_INT(dlist_insert(d, NULL, &d0), 0);
        EXPECT_EQ_UINT(dlist_size(d), 4);
        n0 = dlist_head(d);
        EXPECT_NOTNULL(n0);
        n1 = dlist_next(n0);
        EXPECT_NOTNULL(n1);
        n2 = dlist_next(n1);
        EXPECT_NOTNULL(n2);
        n3 = dlist_next(n2);
        EXPECT_NOTNULL(n3);
        EXPECT_EQ_PTR(dlist_data(n0), &d0);
        EXPECT_EQ_PTR(dlist_data(n1), &b);
        EXPECT_EQ_PTR(dlist_data(n2), &a);
        EXPECT_EQ_PTR(dlist_data(n3), &c);
        EXPECT_NULL(dlist_prev(n0));
        EXPECT_EQ_PTR(dlist_prev(n1), n0);
        EXPECT_EQ_PTR(dlist_prev(n2), n1);
        EXPECT_EQ_PTR(dlist_prev(n3), n2);
        EXPECT_NULL(dlist_next(n3));
        out = NULL;
        EXPECT_EQ_INT(dlist_remove(d, dlist_head(d), &out), 0);
        EXPECT_EQ_PTR(out, &d0);
        EXPECT_EQ_PTR(dlist_front(d), &b);
        EXPECT_EQ_INT(dlist_remove(d, dlist_tail(d), &out), 0);
        EXPECT_EQ_PTR(out, &c);
        EXPECT_EQ_PTR(dlist_back(d), &a);
        EXPECT_EQ_UINT(dlist_size(d), 2);
        dlist_free(d);
    }

    {
        struct dlist *d;
        int a, b, c;

        EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
        a = 5;
        b = 6;
        c = 7;
        dlist_pushback(d, &a);
        dlist_pushback(d, &b);
        dlist_pushback(d, &c);
        dlist_clear(d);
        EXPECT_TRUE(dlist_empty(d));
        EXPECT_EQ_UINT(dlist_size(d), 0);
        EXPECT_NULL(dlist_front(d));
        EXPECT_NULL(dlist_back(d));
        EXPECT_EQ_INT(dlist_pushfront(d, &a), 0);
        EXPECT_EQ_UINT(dlist_size(d), 1);
        dlist_free(d);
    }

    {
        struct dlist *d;
        int x;

        dtor_n = 0;
        EXPECT_EQ_INT(dlist_init(&d, dtor_inc), 0);
        x = 7;
        EXPECT_EQ_INT(dlist_pushfront(d, &x), 0);
        EXPECT_EQ_INT(dlist_popfront(d, NULL), 0);
        EXPECT_EQ_INT(dtor_n, 1);
        dlist_free(d);
    }

    {
        struct dlist *d;
        int x;

        dtor_n = 0;
        EXPECT_EQ_INT(dlist_init(&d, dtor_inc), 0);
        x = 8;
        EXPECT_EQ_INT(dlist_pushback(d, &x), 0);
        EXPECT_EQ_INT(dlist_popback(d, NULL), 0);
        EXPECT_EQ_INT(dtor_n, 1);
        dlist_free(d);
    }

    {
        struct dlist *d;
        int x;
        void *out;

        dtor_n = 0;
        EXPECT_EQ_INT(dlist_init(&d, dtor_inc), 0);
        x = 9;
        dlist_pushfront(d, &x);
        out = NULL;
        EXPECT_EQ_INT(dlist_popfront(d, &out), 0);
        EXPECT_EQ_PTR(out, &x);
        EXPECT_EQ_INT(dtor_n, 0);
        dlist_free(d);
    }

    {
        struct dlist *d;
        int a, b, c;
        void *out;

        dtor_n = 0;
        EXPECT_EQ_INT(dlist_init(&d, dtor_inc), 0);
        a = 1;
        b = 2;
        c = 3;
        dlist_pushback(d, &a);
        dlist_pushback(d, &b);
        dlist_pushback(d, &c);
        out = NULL;
        EXPECT_EQ_INT(dlist_remove(d, dlist_head(d), &out), 0);
        EXPECT_EQ_PTR(out, &a);
        EXPECT_EQ_INT(dtor_n, 0);
        EXPECT_EQ_INT(dlist_remove(d, dlist_tail(d), NULL), 0);
        EXPECT_EQ_INT(dtor_n, 1);
        dlist_free(d);
        EXPECT_EQ_INT(dtor_n, 2);
    }

    {
        int x;
        struct dlist_node *np;
        void *out;
        struct dlist *d;

        EXPECT_EQ_INT(dlist_pushfront(NULL, &x), -1);
        EXPECT_EQ_INT(dlist_pushback(NULL, &x), -1);
        out = NULL;
        EXPECT_EQ_INT(dlist_popfront(NULL, &out), -1);
        EXPECT_EQ_INT(dlist_popback(NULL, &out), -1);
        EXPECT_EQ_INT(dlist_insert(NULL, NULL, &x), -1);
        EXPECT_EQ_INT(dlist_remove(NULL, NULL, &out), -1);
        EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
        EXPECT_EQ_INT(dlist_pushfront(d, NULL), -1);
        EXPECT_EQ_INT(dlist_pushback(d, NULL), -1);
        EXPECT_EQ_INT(dlist_insert(d, NULL, NULL), -1);
        EXPECT_EQ_INT(dlist_remove(d, NULL, &out), -1);
        dlist_free(d);
        np = NULL;
        EXPECT_NULL(dlist_next(np));
        EXPECT_NULL(dlist_prev(np));
        EXPECT_NULL(dlist_data(np));
    }

    {
        EXPECT_TRUE(dlist_empty(NULL));
        EXPECT_EQ_UINT(dlist_size(NULL), 0);
        EXPECT_NULL(dlist_front(NULL));
        EXPECT_NULL(dlist_back(NULL));
        EXPECT_NULL(dlist_head(NULL));
        EXPECT_NULL(dlist_tail(NULL));
        dlist_free(NULL);
        dlist_clear(NULL);
    }
}