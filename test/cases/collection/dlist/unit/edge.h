#include <dlist.h>
#include <utest.h>

static int edg_dtor_n;
static void edg_dtor_inc(void *p)
{
  (void)p;
  edg_dtor_n++;
}

UTEST_CASE(edge)
{
  {
    struct dlist d;
    void *out;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    EXPECT_TRUE(dlist_empty(&d));
    EXPECT_NULL(dlist_front(&d));
    EXPECT_NULL(dlist_back(&d));
    out = (void *)1;
    EXPECT_EQ_INT(dlist_popfront(&d, &out), -1);
    EXPECT_EQ_INT(dlist_popback(&d, &out), -1);
    EXPECT_EQ_INT(dlist_remove(&d, NULL, &out), -1);
    dlist_fini(&d);
  }

  {
    struct dlist d;
    int x;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    x = 1;
    EXPECT_EQ_INT(dlist_pushfront(&d, &x), 0);
    EXPECT_EQ_INT(dlist_popback(&d, NULL), 0);
    EXPECT_TRUE(dlist_empty(&d));
    EXPECT_EQ_INT(dlist_popfront(&d, NULL), -1);
    dlist_fini(&d);
  }

  {
    struct dlist d;
    int x;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    x = 1;
    EXPECT_EQ_INT(dlist_pushback(&d, &x), 0);
    EXPECT_EQ_INT(dlist_popfront(&d, NULL), 0);
    EXPECT_TRUE(dlist_empty(&d));
    EXPECT_EQ_INT(dlist_popback(&d, NULL), -1);
    dlist_fini(&d);
  }

  {
    struct dlist d;
    int x;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    x = 7;
    EXPECT_EQ_INT(dlist_pushback(&d, &x), 0);
    dlist_clear(&d);
    EXPECT_TRUE(dlist_empty(&d));
    EXPECT_EQ_INT(dlist_popfront(&d, NULL), -1);
    dlist_fini(&d);
  }

  {
    struct dlist d;
    int x;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    x = 7;
    EXPECT_EQ_INT(dlist_pushfront(&d, &x), 0);
    dlist_clear(&d);
    dlist_clear(&d);
    EXPECT_TRUE(dlist_empty(&d));
    EXPECT_EQ_UINT(dlist_size(&d), 0);
    dlist_fini(&d);
  }

  {
    struct dlist d;
    int a, b, c;
    void *out;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    a = 10;
    b = 20;
    c = 30;
    EXPECT_EQ_INT(dlist_pushback(&d, &a), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, &c), 0);
    EXPECT_EQ_INT(dlist_insert(&d, d.tail, &b), 0);
    EXPECT_EQ_UINT(dlist_size(&d), 3);
    EXPECT_EQ_PTR(dlist_front(&d), &a);
    EXPECT_EQ_PTR(dlist_back(&d), &c);
    EXPECT_EQ_PTR(dlist_data(d.head), &a);
    EXPECT_EQ_PTR(dlist_data(dlist_next(d.head)), &b);
    EXPECT_EQ_PTR(dlist_data(d.tail), &c);
    out = NULL;
    EXPECT_EQ_INT(dlist_remove(&d, dlist_next(d.head), &out), 0);
    EXPECT_EQ_PTR(out, &b);
    EXPECT_EQ_UINT(dlist_size(&d), 2);
    EXPECT_EQ_PTR(dlist_front(&d), &a);
    EXPECT_EQ_PTR(dlist_back(&d), &c);
    dlist_fini(&d);
  }

  {
    struct dlist d;
    int a, b;
    void *out;
    struct dlist_node *n0, *n1;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    a = 100;
    b = 200;
    EXPECT_EQ_INT(dlist_pushback(&d, &a), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, &b), 0);
    n0 = d.head;
    n1 = d.tail;
    EXPECT_NOTNULL(n0);
    EXPECT_NOTNULL(n1);
    EXPECT_NULL(dlist_prev(n0));
    EXPECT_EQ_PTR(dlist_next(n0), n1);
    EXPECT_EQ_PTR(dlist_prev(n1), n0);
    EXPECT_NULL(dlist_next(n1));
    out = NULL;
    EXPECT_EQ_INT(dlist_popfront(&d, &out), 0);
    EXPECT_EQ_PTR(out, &a);
    out = NULL;
    EXPECT_EQ_INT(dlist_popfront(&d, &out), 0);
    EXPECT_EQ_PTR(out, &b);
    EXPECT_TRUE(dlist_empty(&d));
    dlist_fini(&d);
  }

  {
    struct dlist d;
    int a, b, c;
    void *out;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    a = 1;
    b = 2;
    c = 3;
    EXPECT_EQ_INT(dlist_pushback(&d, &a), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, &b), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, &c), 0);
    out = NULL;
    EXPECT_EQ_INT(dlist_remove(&d, d.head, &out), 0);
    EXPECT_EQ_PTR(out, &a);
    EXPECT_NOTNULL(d.head);
    EXPECT_NULL(dlist_prev(d.head));
    EXPECT_EQ_INT(dlist_remove(&d, d.tail, &out), 0);
    EXPECT_EQ_PTR(out, &c);
    EXPECT_NOTNULL(d.tail);
    EXPECT_NULL(dlist_next(d.tail));
    EXPECT_EQ_PTR(dlist_front(&d), &b);
    EXPECT_EQ_PTR(dlist_back(&d), &b);
    dlist_fini(&d);
  }

  {
    struct dlist d;
    int a, b;
    void *out;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(dlist_init(&d, edg_dtor_inc), 0);
    a = 1;
    b = 2;
    EXPECT_EQ_INT(dlist_pushback(&d, &a), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, &b), 0);
    out = NULL;
    EXPECT_EQ_INT(dlist_popfront(&d, &out), 0);
    EXPECT_EQ_PTR(out, &a);
    EXPECT_EQ_INT(edg_dtor_n, 0);
    EXPECT_EQ_INT(dlist_popback(&d, NULL), 0);
    EXPECT_EQ_INT(edg_dtor_n, 1);
    dlist_fini(&d);
  }

  {
    struct dlist d;
    int x;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(dlist_init(&d, edg_dtor_inc), 0);
    x = 9;
    EXPECT_EQ_INT(dlist_pushfront(&d, &x), 0);
    dlist_clear(&d);
    EXPECT_EQ_INT(edg_dtor_n, 1);
    EXPECT_TRUE(dlist_empty(&d));
    dlist_fini(&d);
  }

  {
    int x;
    void *out;

    EXPECT_EQ_INT(dlist_init(NULL, edg_dtor_inc), -1);
    EXPECT_EQ_INT(dlist_pushfront(NULL, &x), -1);
    EXPECT_EQ_INT(dlist_pushback(NULL, &x), -1);
    out = NULL;
    EXPECT_EQ_INT(dlist_popfront(NULL, &out), -1);
    EXPECT_EQ_INT(dlist_popback(NULL, &out), -1);
    EXPECT_EQ_INT(dlist_insert(NULL, NULL, &x), -1);
    EXPECT_EQ_INT(dlist_remove(NULL, NULL, &out), -1);
  }

  {
    struct dlist d;
    int x;
    struct dlist_node *np;
    void *out;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    x = 1;
    out = NULL;
    EXPECT_EQ_INT(dlist_pushfront(&d, NULL), -1);
    EXPECT_EQ_INT(dlist_pushback(&d, NULL), -1);
    EXPECT_EQ_INT(dlist_insert(&d, NULL, NULL), -1);
    EXPECT_EQ_INT(dlist_remove(&d, NULL, &out), -1);
    EXPECT_EQ_INT(dlist_insert(&d, NULL, &x), 0);
    EXPECT_EQ_INT(dlist_remove(&d, NULL, &out), 0);
    EXPECT_EQ_PTR(out, &x);
    np = NULL;
    EXPECT_NULL(dlist_next(np));
    EXPECT_NULL(dlist_prev(np));
    EXPECT_NULL(dlist_data(np));
    dlist_fini(&d);
  }

  {
    dlist_fini(NULL);
    dlist_clear(NULL);
  }
}
