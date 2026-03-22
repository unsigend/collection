#include <deque.h>
#include <string.h>
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
    struct deque d;

    EXPECT_EQ_INT(deq_init(NULL, sizeof(int), NULL), -1);
    EXPECT_EQ_INT(deq_init(&d, 0, NULL), -1);
  }

  {
    struct deque d;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    EXPECT_TRUE(deq_empty(&d));
    EXPECT_EQ_UINT(deq_size(&d), 0);
    EXPECT_GE_UINT(deq_capacity(&d), deq_size(&d));
    EXPECT_NULL(deq_at(&d, 0));
    EXPECT_NULL(deq_at(&d, 1));
    EXPECT_EQ_INT(deq_popback(&d, NULL), -1);
    EXPECT_EQ_INT(deq_popfront(&d, NULL), -1);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    x = 10;
    EXPECT_EQ_INT(deq_pushback(&d, &x), 0);
    EXPECT_FALSE(deq_empty(&d));
    EXPECT_EQ_UINT(deq_size(&d), 1);
    EXPECT_NOTNULL(deq_at(&d, 0));
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 10);
    EXPECT_NOTNULL(deq_front(&d));
    EXPECT_EQ_PTR(deq_front(&d), deq_at(&d, 0));
    EXPECT_NOTNULL(deq_back(&d));
    EXPECT_EQ_PTR(deq_back(&d), deq_at(&d, 0));
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    x = 7;
    EXPECT_EQ_INT(deq_pushfront(&d, &x), 0);
    EXPECT_EQ_UINT(deq_size(&d), 1);
    EXPECT_EQ_INT(*(int *)deq_front(&d), 7);
    EXPECT_EQ_INT(*(int *)deq_back(&d), 7);
    deq_fini(&d);
  }

  {
    struct deque d;
    int a, b, c, out;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    a = 1;
    b = 2;
    c = 3;
    EXPECT_EQ_INT(deq_pushback(&d, &a), 0);
    EXPECT_EQ_INT(deq_pushback(&d, &b), 0);
    EXPECT_EQ_INT(deq_pushback(&d, &c), 0);
    EXPECT_EQ_UINT(deq_size(&d), 3);
    out = 0;
    EXPECT_EQ_INT(deq_popback(&d, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_EQ_INT(deq_popback(&d, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(deq_popback(&d, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_TRUE(deq_empty(&d));
    EXPECT_EQ_INT(deq_popback(&d, NULL), -1);
    deq_fini(&d);
  }

  {
    struct deque d;
    int a, b, c, out;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    a = 1;
    b = 2;
    c = 3;
    EXPECT_EQ_INT(deq_pushfront(&d, &c), 0);
    EXPECT_EQ_INT(deq_pushfront(&d, &b), 0);
    EXPECT_EQ_INT(deq_pushfront(&d, &a), 0);
    EXPECT_EQ_UINT(deq_size(&d), 3);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 1);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 1), 2);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 2), 3);
    out = 0;
    EXPECT_EQ_INT(deq_popfront(&d, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(deq_popfront(&d, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(deq_popfront(&d, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_TRUE(deq_empty(&d));
    EXPECT_EQ_INT(deq_popfront(&d, NULL), -1);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x, y, z, out;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    x = 1;
    y = 2;
    EXPECT_EQ_INT(deq_pushback(&d, &x), 0);
    EXPECT_EQ_INT(deq_pushback(&d, &y), 0);
    z = 0;
    EXPECT_EQ_INT(deq_pushfront(&d, &z), 0);
    EXPECT_EQ_UINT(deq_size(&d), 3);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 0);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 1), 1);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 2), 2);
    out = 0;
    EXPECT_EQ_INT(deq_popback(&d, &out), 0);
    EXPECT_EQ_INT(out, 2);
    out = 0;
    EXPECT_EQ_INT(deq_popfront(&d, &out), 0);
    EXPECT_EQ_INT(out, 0);
    EXPECT_EQ_UINT(deq_size(&d), 1);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 1);
    deq_fini(&d);
  }

  {
    struct deque d;
    int a, b, c;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    a = 5;
    b = 6;
    c = 7;
    deq_pushback(&d, &a);
    deq_pushback(&d, &b);
    deq_pushback(&d, &c);
    deq_clear(&d);
    EXPECT_TRUE(deq_empty(&d));
    EXPECT_EQ_UINT(deq_size(&d), 0);
    EXPECT_EQ_INT(deq_pushback(&d, &a), 0);
    EXPECT_EQ_UINT(deq_size(&d), 1);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;
    void *p;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(deq_resize(&d, 1), 0);
    p = deq_at(&d, 0);
    EXPECT_NOTNULL(p);
    *(int *)p = 42;
    EXPECT_EQ_INT(deq_resize(&d, 3), 0);
    EXPECT_EQ_UINT(deq_size(&d), 3);
    p = deq_at(&d, 1);
    EXPECT_NOTNULL(p);
    x = 1;
    memcpy(p, &x, sizeof(int));
    p = deq_at(&d, 2);
    EXPECT_NOTNULL(p);
    x = 2;
    memcpy(p, &x, sizeof(int));
    EXPECT_EQ_INT(deq_resize(&d, 1), 0);
    EXPECT_NOTNULL(deq_at(&d, 0));
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 42);
    deq_fini(&d);
  }

  {
    struct deque d;
    size_t idx;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(deq_resize(&d, 4), 0);
    EXPECT_EQ_UINT(deq_size(&d), 4);
    EXPECT_GE_UINT(deq_capacity(&d), deq_size(&d));
    for (idx = 0; idx < 4; idx++) {
      void *p = deq_at(&d, idx);
      EXPECT_NOTNULL(p);
      *(int *)p = (int)(idx + 1);
    }
    EXPECT_EQ_INT(*(int *)deq_at(&d, 3), 4);
    EXPECT_EQ_INT(deq_resize(&d, 2), 0);
    EXPECT_EQ_UINT(deq_size(&d), 2);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 1);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 1), 2);
    EXPECT_EQ_INT(deq_resize(&d, 2), 0);
    EXPECT_EQ_UINT(deq_size(&d), 2);
    deq_fini(&d);
  }

  {
    struct deque d;
    size_t cap_a, cap_b;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(deq_resize(&d, 3), 0);
    cap_a = deq_capacity(&d);
    EXPECT_EQ_INT(deq_shrink(&d), 0);
    cap_b = deq_capacity(&d);
    EXPECT_EQ_UINT(deq_size(&d), 3);
    EXPECT_GE_UINT(cap_b, deq_size(&d));
    EXPECT_LE_UINT(cap_b, cap_a);
    EXPECT_EQ_INT(deq_shrink(&d), 0);
    EXPECT_EQ_UINT(deq_capacity(&d), cap_b);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(deq_resize(&d, 2), 0);
    deq_clear(&d);
    EXPECT_EQ_INT(deq_shrink(&d), 0);
    EXPECT_TRUE(deq_empty(&d));
    x = 1;
    EXPECT_EQ_INT(deq_pushback(&d, &x), 0);
    deq_fini(&d);
  }

  {
    struct deque d;
    int i;
    size_t idx;
    size_t cap0;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    i = 0;
    EXPECT_EQ_INT(deq_pushback(&d, &i), 0);
    cap0 = deq_capacity(&d);
    for (idx = 1; idx < 17; idx++) {
      i = (int)idx;
      EXPECT_EQ_INT(deq_pushback(&d, &i), 0);
    }
    EXPECT_EQ_UINT(deq_size(&d), 17);
    EXPECT_GE_UINT(deq_capacity(&d), deq_size(&d));
    EXPECT_GE_UINT(deq_capacity(&d), cap0);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 0);
    EXPECT_EQ_INT(*(int *)deq_back(&d), 16);
    deq_fini(&d);
  }

  {
    struct deque d;
    double a, b, out;

    EXPECT_EQ_INT(deq_init(&d, sizeof(double), NULL), 0);
    a = 3.14;
    b = 2.71;
    EXPECT_EQ_INT(deq_pushback(&d, &a), 0);
    EXPECT_EQ_INT(deq_pushback(&d, &b), 0);
    EXPECT_EQ_UINT(deq_size(&d), 2);
    EXPECT_NOTNULL(deq_at(&d, 0));
    EXPECT_EQ_DOUBLE(*(double *)deq_at(&d, 0), 3.14);
    EXPECT_EQ_DOUBLE(*(double *)deq_back(&d), 2.71);
    out = 0.0;
    EXPECT_EQ_INT(deq_popback(&d, &out), 0);
    EXPECT_EQ_DOUBLE(out, 2.71);
    deq_fini(&d);
  }

  {
    struct deque d;
    int a, b, c, out;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    a = 1;
    b = 2;
    deq_pushback(&d, &a);
    deq_pushback(&d, &b);
    c = 9;
    EXPECT_EQ_INT(deq_insert(&d, 0, &c), 0);
    EXPECT_EQ_UINT(deq_size(&d), 3);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 9);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 2), 2);
    c = 5;
    EXPECT_EQ_INT(deq_insert(&d, 2, &c), 0);
    EXPECT_EQ_UINT(deq_size(&d), 4);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 2), 5);
    c = 8;
    EXPECT_EQ_INT(deq_insert(&d, 4, &c), 0);
    EXPECT_EQ_UINT(deq_size(&d), 5);
    EXPECT_EQ_INT(*(int *)deq_back(&d), 8);
    out = 0;
    EXPECT_EQ_INT(deq_remove(&d, 2, &out), 0);
    EXPECT_EQ_INT(out, 5);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 2), 2);
    EXPECT_EQ_INT(deq_remove(&d, 3, NULL), 0);
    EXPECT_EQ_UINT(deq_size(&d), 3);
    EXPECT_EQ_INT(deq_remove(&d, 0, NULL), 0);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 1);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    x = 42;
    EXPECT_EQ_INT(deq_insert(&d, 0, &x), 0);
    EXPECT_EQ_UINT(deq_size(&d), 1);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 42);
    deq_fini(&d);
  }

  {
    struct deque d;
    int a, b, out;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    a = 1;
    b = 2;
    deq_pushback(&d, &a);
    deq_pushback(&d, &b);
    out = 0;
    EXPECT_EQ_INT(deq_remove(&d, 1, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_UINT(deq_size(&d), 1);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 1);
    EXPECT_EQ_INT(deq_remove(&d, 0, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_TRUE(deq_empty(&d));
    deq_fini(&d);
  }

  {
    struct deque d;

    dtor_n = 0;
    EXPECT_EQ_INT(deq_init(&d, sizeof(int), dtor_inc), 0);
    deq_fini(&d);
    EXPECT_EQ_INT(dtor_n, 0);
  }

  {
    struct deque d;
    int a, b, c;

    dtor_n = 0;
    EXPECT_EQ_INT(deq_init(&d, sizeof(int), dtor_inc), 0);
    a = 1;
    b = 2;
    c = 3;
    deq_pushback(&d, &a);
    deq_pushback(&d, &b);
    deq_pushback(&d, &c);
    deq_clear(&d);
    EXPECT_EQ_INT(dtor_n, 3);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;

    dtor_n = 0;
    EXPECT_EQ_INT(deq_init(&d, sizeof(int), dtor_inc), 0);
    x = 7;
    deq_pushback(&d, &x);
    EXPECT_EQ_INT(deq_popback(&d, NULL), 0);
    EXPECT_EQ_INT(dtor_n, 1);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;

    dtor_n = 0;
    EXPECT_EQ_INT(deq_init(&d, sizeof(int), dtor_inc), 0);
    x = 7;
    deq_pushfront(&d, &x);
    EXPECT_EQ_INT(deq_popfront(&d, NULL), 0);
    EXPECT_EQ_INT(dtor_n, 1);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x, out;

    dtor_n = 0;
    EXPECT_EQ_INT(deq_init(&d, sizeof(int), dtor_inc), 0);
    x = 7;
    deq_pushback(&d, &x);
    out = 0;
    EXPECT_EQ_INT(deq_popback(&d, &out), 0);
    EXPECT_EQ_INT(out, 7);
    EXPECT_EQ_INT(dtor_n, 0);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x, out;

    dtor_n = 0;
    EXPECT_EQ_INT(deq_init(&d, sizeof(int), dtor_inc), 0);
    x = 7;
    deq_pushfront(&d, &x);
    out = 0;
    EXPECT_EQ_INT(deq_popfront(&d, &out), 0);
    EXPECT_EQ_INT(out, 7);
    EXPECT_EQ_INT(dtor_n, 0);
    deq_fini(&d);
  }

  {
    struct deque d;
    int a, b;

    dtor_n = 0;
    EXPECT_EQ_INT(deq_init(&d, sizeof(int), dtor_inc), 0);
    a = 1;
    b = 2;
    deq_pushback(&d, &a);
    deq_pushback(&d, &b);
    EXPECT_EQ_INT(deq_remove(&d, 0, NULL), 0);
    EXPECT_EQ_INT(dtor_n, 1);
    deq_fini(&d);
  }

  {
    struct deque d;
    int a, b, out;

    dtor_n = 0;
    EXPECT_EQ_INT(deq_init(&d, sizeof(int), dtor_inc), 0);
    a = 1;
    b = 2;
    deq_pushback(&d, &a);
    deq_pushback(&d, &b);
    out = 0;
    EXPECT_EQ_INT(deq_remove(&d, 0, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(dtor_n, 0);
    deq_fini(&d);
  }

  {
    int x;

    EXPECT_EQ_INT(deq_resize(NULL, 1), -1);
    EXPECT_EQ_INT(deq_shrink(NULL), -1);
    EXPECT_EQ_INT(deq_pushback(NULL, &x), -1);
    EXPECT_EQ_INT(deq_pushfront(NULL, &x), -1);
    EXPECT_EQ_INT(deq_insert(NULL, 0, &x), -1);
    EXPECT_EQ_INT(deq_remove(NULL, 0, NULL), -1);
    EXPECT_EQ_INT(deq_popback(NULL, &x), -1);
    EXPECT_EQ_INT(deq_popfront(NULL, &x), -1);
  }

  {
    struct deque d;
    int a;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    a = 1;
    EXPECT_EQ_INT(deq_pushback(&d, NULL), -1);
    EXPECT_EQ_INT(deq_pushfront(&d, NULL), -1);
    EXPECT_EQ_INT(deq_insert(&d, 0, NULL), -1);
    deq_pushback(&d, &a);
    EXPECT_EQ_INT(deq_remove(&d, 99, NULL), -1);
    deq_fini(&d);
  }

  {
    deq_fini(NULL);
    deq_clear(NULL);
  }
}
