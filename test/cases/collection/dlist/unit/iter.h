#include <dlist.h>
#include <utest.h>

UTEST_CASE(iter)
{
  {
    struct dlist d;
    struct dlist_iter it;

    EXPECT_EQ_INT(dlist_iter_init(NULL, &d), -1);
    EXPECT_EQ_INT(dlist_iter_init(&it, NULL), -1);
    EXPECT_EQ_INT(dlist_iter_initrev(NULL, &d), -1);
    EXPECT_EQ_INT(dlist_iter_initrev(&it, NULL), -1);
  }

  {
    struct dlist d;
    struct dlist_iter it;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    EXPECT_EQ_INT(dlist_iter_init(&it, &d), 0);
    EXPECT_NULL(dlist_iter_get(&it));
    dlist_iter_inc(&it);
    EXPECT_NULL(dlist_iter_get(&it));
    dlist_iter_dec(&it);
    EXPECT_NULL(dlist_iter_get(&it));
    EXPECT_EQ_INT(dlist_iter_initrev(&it, &d), 0);
    EXPECT_NULL(dlist_iter_get(&it));
    dlist_iter_dec(&it);
    EXPECT_NULL(dlist_iter_get(&it));
    dlist_fini(&d);
  }

  {
    struct dlist d;
    struct dlist_iter it;
    int x;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    x = 42;
    EXPECT_EQ_INT(dlist_pushback(&d, &x), 0);
    EXPECT_EQ_INT(dlist_iter_init(&it, &d), 0);
    EXPECT_NOTNULL(dlist_iter_get(&it));
    EXPECT_EQ_PTR(dlist_iter_get(&it), &x);
    dlist_iter_inc(&it);
    EXPECT_NULL(dlist_iter_get(&it));
    dlist_iter_inc(&it);
    EXPECT_NULL(dlist_iter_get(&it));
    EXPECT_EQ_INT(dlist_iter_initrev(&it, &d), 0);
    EXPECT_EQ_PTR(dlist_iter_get(&it), &x);
    dlist_iter_dec(&it);
    EXPECT_NULL(dlist_iter_get(&it));
    dlist_fini(&d);
  }

  {
    struct dlist d;
    struct dlist_iter it;
    int a, b, c;
    size_t n;
    int fwd;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    a = 2;
    b = 4;
    c = 6;
    EXPECT_EQ_INT(dlist_pushback(&d, &a), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, &b), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, &c), 0);
    EXPECT_EQ_INT(dlist_iter_init(&it, &d), 0);
    fwd = 0;
    for (n = 0; n < 4; n++) {
      void *p = dlist_iter_get(&it);

      if (p)
        fwd += *(int *)p;
      dlist_iter_inc(&it);
    }
    EXPECT_EQ_INT(fwd, 12);
    EXPECT_EQ_INT(dlist_iter_initrev(&it, &d), 0);
    fwd = 0;
    for (n = 0; n < 4; n++) {
      void *p = dlist_iter_get(&it);

      if (p)
        fwd += *(int *)p;
      dlist_iter_dec(&it);
    }
    EXPECT_EQ_INT(fwd, 12);
    dlist_fini(&d);
  }

  {
    struct dlist d;
    struct dlist_iter it;
    int x, y, z;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    x = 1;
    y = 2;
    z = 3;
    EXPECT_EQ_INT(dlist_pushback(&d, &x), 0);
    EXPECT_EQ_INT(dlist_pushfront(&d, &y), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, &z), 0);
    EXPECT_EQ_INT(dlist_iter_init(&it, &d), 0);
    EXPECT_EQ_PTR(dlist_iter_get(&it), &y);
    dlist_iter_inc(&it);
    EXPECT_EQ_PTR(dlist_iter_get(&it), &x);
    dlist_iter_inc(&it);
    EXPECT_EQ_PTR(dlist_iter_get(&it), &z);
    dlist_iter_inc(&it);
    EXPECT_NULL(dlist_iter_get(&it));
    EXPECT_EQ_INT(dlist_iter_initrev(&it, &d), 0);
    EXPECT_EQ_PTR(dlist_iter_get(&it), &z);
    dlist_iter_dec(&it);
    EXPECT_EQ_PTR(dlist_iter_get(&it), &x);
    dlist_iter_dec(&it);
    EXPECT_EQ_PTR(dlist_iter_get(&it), &y);
    dlist_iter_dec(&it);
    EXPECT_NULL(dlist_iter_get(&it));
    dlist_fini(&d);
  }

  {
    struct dlist d;
    struct dlist_iter it;
    int a, b, c;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    a = 1;
    b = 2;
    c = 3;
    EXPECT_EQ_INT(dlist_pushback(&d, &a), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, &b), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, &c), 0);
    EXPECT_EQ_INT(dlist_iter_initrev(&it, &d), 0);
    dlist_iter_inc(&it);
    EXPECT_NULL(dlist_iter_get(&it));
    dlist_iter_inc(&it);
    EXPECT_NULL(dlist_iter_get(&it));
    EXPECT_EQ_INT(dlist_iter_initrev(&it, &d), 0);
    EXPECT_EQ_PTR(dlist_iter_get(&it), &c);
    dlist_fini(&d);
  }

  {
    struct dlist d;
    struct dlist_iter it;
    int a, b, c;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    a = 10;
    b = 20;
    c = 30;
    EXPECT_EQ_INT(dlist_pushback(&d, &a), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, &b), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, &c), 0);
    EXPECT_EQ_INT(dlist_iter_init(&it, &d), 0);
    dlist_iter_inc(&it);
    EXPECT_EQ_PTR(dlist_iter_get(&it), &b);
    EXPECT_EQ_INT(dlist_iter_init(&it, &d), 0);
    EXPECT_EQ_PTR(dlist_iter_get(&it), &a);
    dlist_iter_inc(&it);
    dlist_iter_inc(&it);
    dlist_iter_inc(&it);
    EXPECT_NULL(dlist_iter_get(&it));
    EXPECT_EQ_INT(dlist_iter_initrev(&it, &d), 0);
    EXPECT_EQ_PTR(dlist_iter_get(&it), &c);
    dlist_fini(&d);
  }

  {
    struct dlist d;
    struct dlist_iter it;
    int a, b;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    a = 7;
    b = 8;
    EXPECT_EQ_INT(dlist_pushback(&d, &a), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, &b), 0);
    EXPECT_EQ_INT(dlist_iter_init(&it, &d), 0);
    dlist_iter_inc(&it);
    EXPECT_EQ_PTR(dlist_iter_get(&it), &b);
    dlist_iter_dec(&it);
    EXPECT_EQ_PTR(dlist_iter_get(&it), &a);
    dlist_iter_inc(&it);
    EXPECT_EQ_PTR(dlist_iter_get(&it), &b);
    EXPECT_EQ_INT(dlist_iter_initrev(&it, &d), 0);
    EXPECT_EQ_PTR(dlist_iter_get(&it), &b);
    dlist_iter_dec(&it);
    EXPECT_EQ_PTR(dlist_iter_get(&it), &a);
    dlist_fini(&d);
  }

  {
    EXPECT_NULL(dlist_iter_get(NULL));
    dlist_iter_inc(NULL);
    dlist_iter_dec(NULL);
  }
}
