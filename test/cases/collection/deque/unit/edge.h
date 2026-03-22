#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <deque.h>
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
    EXPECT_NULL(deq_at(NULL, 0));
    EXPECT_NULL(deq_at(NULL, SIZE_MAX));
  }

  {
    struct deque d;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    EXPECT_NULL(deq_front(&d));
    EXPECT_NULL(deq_back(&d));
    EXPECT_GE_UINT(deq_capacity(&d), deq_size(&d));
    EXPECT_NULL(deq_at(&d, 0));
    EXPECT_NULL(deq_at(&d, SIZE_MAX));
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    x = 1;
    deq_pushback(&d, &x);
    EXPECT_NULL(deq_at(&d, 1));
    EXPECT_NULL(deq_at(&d, SIZE_MAX));
    deq_fini(&d);
  }

  {
    struct deque d;

    errno = 0;
    EXPECT_EQ_INT(deq_init(&d, SIZE_MAX, NULL), 0);
    EXPECT_EQ_INT(deq_resize(&d, 2), -1);
    EXPECT_EQ_INT(errno, ERANGE);
    EXPECT_EQ_UINT(deq_size(&d), 0);
    deq_fini(&d);
  }

  {
    struct deque d;

    errno = 0;
    EXPECT_EQ_INT(deq_init(&d, 2, NULL), 0);
    EXPECT_EQ_INT(deq_resize(&d, SIZE_MAX), -1);
    EXPECT_EQ_INT(errno, ERANGE);
    EXPECT_EQ_UINT(deq_size(&d), 0);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;

    errno = 0;
    EXPECT_EQ_INT(deq_init(&d, SIZE_MAX, NULL), 0);
    x = 0;
    EXPECT_EQ_INT(deq_pushback(&d, &x), -1);
    EXPECT_EQ_INT(errno, ERANGE);
    EXPECT_EQ_UINT(deq_size(&d), 0);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;

    errno = 0;
    EXPECT_EQ_INT(deq_init(&d, SIZE_MAX, NULL), 0);
    x = 0;
    EXPECT_EQ_INT(deq_pushfront(&d, &x), -1);
    EXPECT_EQ_INT(errno, ERANGE);
    EXPECT_EQ_UINT(deq_size(&d), 0);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;

    errno = 0;
    EXPECT_EQ_INT(deq_init(&d, SIZE_MAX, NULL), 0);
    x = 0;
    EXPECT_EQ_INT(deq_insert(&d, 0, &x), -1);
    EXPECT_EQ_INT(errno, ERANGE);
    EXPECT_EQ_UINT(deq_size(&d), 0);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    x = 1;
    deq_pushback(&d, &x);
    EXPECT_EQ_INT(deq_resize(&d, 0), 0);
    EXPECT_TRUE(deq_empty(&d));
    EXPECT_EQ_INT(deq_popback(&d, NULL), -1);
    EXPECT_EQ_INT(deq_popfront(&d, NULL), -1);
    deq_fini(&d);
  }

  {
    struct deque d;
    int a, b, c;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(deq_init(&d, sizeof(int), edg_dtor_inc), 0);
    a = 1;
    b = 2;
    c = 3;
    deq_pushback(&d, &a);
    deq_pushback(&d, &b);
    deq_pushback(&d, &c);
    EXPECT_EQ_INT(deq_resize(&d, 1), 0);
    EXPECT_EQ_INT(edg_dtor_n, 2);
    EXPECT_EQ_UINT(deq_size(&d), 1);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 1);
    deq_fini(&d);
  }

  {
    struct deque d;
    size_t idx;
    int x;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(deq_resize(&d, 8), 0);
    EXPECT_EQ_UINT(deq_size(&d), 8);
    EXPECT_GE_UINT(deq_capacity(&d), deq_size(&d));
    for (idx = 0; idx < 4; idx++) {
      void *p = deq_at(&d, idx);
      EXPECT_NOTNULL(p);
      *(int *)p = (int)idx;
    }
    EXPECT_EQ_INT(deq_resize(&d, 8), 0);
    EXPECT_EQ_UINT(deq_size(&d), 8);
    for (idx = 4; idx < 8; idx++) {
      void *p = deq_at(&d, idx);
      EXPECT_NOTNULL(p);
      x = (int)(idx + 100);
      memcpy(p, &x, sizeof(int));
    }
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;
    size_t c0, c1, c2, c3;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(deq_resize(&d, 3), 0);
    c0 = deq_capacity(&d);
    EXPECT_EQ_INT(deq_shrink(&d), 0);
    c1 = deq_capacity(&d);
    EXPECT_GE_UINT(c1, deq_size(&d));
    EXPECT_LE_UINT(c1, c0);
    x = 1;
    EXPECT_EQ_INT(deq_pushback(&d, &x), 0);
    EXPECT_EQ_UINT(deq_size(&d), 4);
    c2 = deq_capacity(&d);
    EXPECT_GE_UINT(c2, deq_size(&d));
    EXPECT_EQ_INT(deq_shrink(&d), 0);
    c3 = deq_capacity(&d);
    EXPECT_GE_UINT(c3, deq_size(&d));
    EXPECT_LE_UINT(c3, c2);
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
    deq_pushback(&d, &x);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    x = 1;
    deq_pushback(&d, &x);
    deq_clear(&d);
    deq_clear(&d);
    EXPECT_TRUE(deq_empty(&d));
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    x = 1;
    deq_pushback(&d, &x);
    EXPECT_EQ_INT(deq_remove(&d, 1, NULL), -1);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    x = 1;
    deq_pushback(&d, &x);
    EXPECT_EQ_INT(deq_insert(&d, 2, &x), 0);
    EXPECT_EQ_UINT(deq_size(&d), 2);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x, out;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    x = 42;
    deq_pushback(&d, &x);
    out = 0;
    EXPECT_EQ_INT(deq_remove(&d, 0, &out), 0);
    EXPECT_EQ_INT(out, 42);
    EXPECT_TRUE(deq_empty(&d));
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    x = 1;
    deq_pushback(&d, &x);
    EXPECT_EQ_INT(deq_popback(&d, NULL), 0);
    EXPECT_EQ_INT(deq_popback(&d, NULL), -1);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    x = 1;
    deq_pushfront(&d, &x);
    EXPECT_EQ_INT(deq_popfront(&d, NULL), 0);
    EXPECT_EQ_INT(deq_popfront(&d, NULL), -1);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(deq_pushback(&d, NULL), -1);
    x = 1;
    deq_pushback(&d, &x);
    EXPECT_EQ_INT(deq_insert(&d, 0, NULL), -1);
    EXPECT_EQ_UINT(deq_size(&d), 1);
    deq_fini(&d);
  }

  {
    struct deque d;
    size_t sz0, sz1;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    sz0 = deq_size(&d);
    EXPECT_EQ_INT(deq_pushfront(&d, NULL), -1);
    sz1 = deq_size(&d);
    EXPECT_EQ_UINT(sz0, sz1);
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
    EXPECT_EQ_INT(deq_popfront(&d, &out), 0);
    EXPECT_EQ_INT(out, 1);
    out = 0;
    EXPECT_EQ_INT(deq_popback(&d, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_TRUE(deq_empty(&d));
    deq_fini(&d);
  }

  {
    struct deque d;
    int a, b;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(deq_init(&d, sizeof(int), edg_dtor_inc), 0);
    a = 1;
    b = 2;
    deq_pushback(&d, &a);
    deq_pushback(&d, &b);
    deq_fini(&d);
    EXPECT_EQ_INT(edg_dtor_n, 2);
  }

  {
    struct deque d;
    int a, b, c;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(deq_init(&d, sizeof(int), edg_dtor_inc), 0);
    a = 1;
    b = 2;
    c = 3;
    deq_pushfront(&d, &a);
    deq_pushfront(&d, &b);
    deq_pushback(&d, &c);
    EXPECT_EQ_INT(deq_resize(&d, 1), 0);
    EXPECT_EQ_INT(edg_dtor_n, 2);
    EXPECT_EQ_UINT(deq_size(&d), 1);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 2);
    deq_fini(&d);
  }
}
