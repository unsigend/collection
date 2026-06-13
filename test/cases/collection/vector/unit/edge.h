#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <utest.h>
#include <vector.h>

static int edg_dtor_n;
static void edg_dtor_inc(void *p)
{
  (void)p;
  edg_dtor_n++;
}

UTEST_CASE(edge)
{
  {
    EXPECT_NULL(vec_at(NULL, 0));
    EXPECT_NULL(vec_at(NULL, SIZE_MAX));
  }

  {
    struct vector v;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    EXPECT_NULL(vec_front(&v));
    EXPECT_NULL(vec_back(&v));
    EXPECT_GE_UINT(vec_capacity(&v), vec_size(&v));
    EXPECT_NULL(vec_at(&v, 0));
    EXPECT_NULL(vec_at(&v, SIZE_MAX));
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    x = 1;
    vec_pushback(&v, &x);
    EXPECT_NULL(vec_at(&v, 1));
    EXPECT_NULL(vec_at(&v, SIZE_MAX));
    vec_fini(&v);
  }

  {
    struct vector v;

    errno = 0;
    EXPECT_EQ_INT(vec_init(&v, SIZE_MAX, NULL), 0);
    EXPECT_EQ_INT(vec_resize(&v, 2), -1);
    EXPECT_EQ_INT(errno, ERANGE);
    EXPECT_EQ_UINT(vec_size(&v), 0);
    vec_fini(&v);
  }

  {
    struct vector v;

    errno = 0;
    EXPECT_EQ_INT(vec_init(&v, 2, NULL), 0);
    EXPECT_EQ_INT(vec_resize(&v, SIZE_MAX), -1);
    EXPECT_EQ_INT(errno, ERANGE);
    EXPECT_EQ_UINT(vec_size(&v), 0);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;

    errno = 0;
    EXPECT_EQ_INT(vec_init(&v, SIZE_MAX, NULL), 0);
    x = 0;
    EXPECT_EQ_INT(vec_pushback(&v, &x), -1);
    EXPECT_EQ_INT(errno, ERANGE);
    EXPECT_EQ_UINT(vec_size(&v), 0);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;

    errno = 0;
    EXPECT_EQ_INT(vec_init(&v, SIZE_MAX, NULL), 0);
    x = 0;
    EXPECT_EQ_INT(vec_insert(&v, 0, &x), -1);
    EXPECT_EQ_INT(errno, ERANGE);
    EXPECT_EQ_UINT(vec_size(&v), 0);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    x = 1;
    vec_pushback(&v, &x);
    EXPECT_EQ_INT(vec_resize(&v, 0), 0);
    EXPECT_TRUE(vec_empty(&v));
    EXPECT_EQ_INT(vec_popback(&v, NULL), -1);
    vec_fini(&v);
  }

  {
    struct vector v;
    int a, b, c;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(vec_init(&v, sizeof(int), edg_dtor_inc), 0);
    a = 1;
    b = 2;
    c = 3;
    vec_pushback(&v, &a);
    vec_pushback(&v, &b);
    vec_pushback(&v, &c);
    EXPECT_EQ_INT(vec_resize(&v, 1), 0);
    EXPECT_EQ_INT(edg_dtor_n, 2);
    EXPECT_EQ_UINT(vec_size(&v), 1);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 0), 1);
    vec_fini(&v);
  }

  {
    struct vector v;
    size_t idx;
    int x;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(vec_resize(&v, 8), 0);
    EXPECT_EQ_UINT(vec_size(&v), 8);
    EXPECT_GE_UINT(vec_capacity(&v), vec_size(&v));
    for (idx = 0; idx < 4; idx++) {
      void *p = vec_at(&v, idx);
      EXPECT_NOTNULL(p);
      *(int *)p = (int)idx;
    }
    EXPECT_EQ_INT(vec_resize(&v, 8), 0);
    EXPECT_EQ_UINT(vec_size(&v), 8);
    for (idx = 4; idx < 8; idx++) {
      void *p = vec_at(&v, idx);
      EXPECT_NOTNULL(p);
      x = (int)(idx + 100);
      memcpy(p, &x, sizeof(int));
    }
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;
    size_t c0, c1, c2, c3;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(vec_resize(&v, 3), 0);
    c0 = vec_capacity(&v);
    EXPECT_EQ_INT(vec_shrink(&v), 0);
    c1 = vec_capacity(&v);
    EXPECT_GE_UINT(c1, vec_size(&v));
    EXPECT_LE_UINT(c1, c0);
    x = 1;
    EXPECT_EQ_INT(vec_pushback(&v, &x), 0);
    EXPECT_EQ_UINT(vec_size(&v), 4);
    c2 = vec_capacity(&v);
    EXPECT_GE_UINT(c2, vec_size(&v));
    EXPECT_EQ_INT(vec_shrink(&v), 0);
    c3 = vec_capacity(&v);
    EXPECT_GE_UINT(c3, vec_size(&v));
    EXPECT_LE_UINT(c3, c2);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(vec_resize(&v, 2), 0);
    vec_clear(&v);
    EXPECT_EQ_INT(vec_shrink(&v), 0);
    EXPECT_TRUE(vec_empty(&v));
    x = 1;
    vec_pushback(&v, &x);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    x = 1;
    vec_pushback(&v, &x);
    vec_clear(&v);
    vec_clear(&v);
    EXPECT_TRUE(vec_empty(&v));
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    x = 1;
    vec_pushback(&v, &x);
    EXPECT_EQ_INT(vec_remove(&v, 1, NULL), -1);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    x = 1;
    vec_pushback(&v, &x);
    EXPECT_EQ_INT(vec_insert(&v, 2, &x), 0);
    EXPECT_EQ_UINT(vec_size(&v), 2);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x, out;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    x = 42;
    vec_pushback(&v, &x);
    out = 0;
    EXPECT_EQ_INT(vec_remove(&v, 0, &out), 0);
    EXPECT_EQ_INT(out, 42);
    EXPECT_TRUE(vec_empty(&v));
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    x = 1;
    vec_pushback(&v, &x);
    EXPECT_EQ_INT(vec_popback(&v, NULL), 0);
    EXPECT_EQ_INT(vec_popback(&v, NULL), -1);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(vec_pushback(&v, NULL), -1);
    x = 1;
    vec_pushback(&v, &x);
    EXPECT_EQ_INT(vec_insert(&v, 0, NULL), -1);
    EXPECT_EQ_UINT(vec_size(&v), 1);
    vec_fini(&v);
  }

  {
    struct vector v;
    int a, b;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(vec_init(&v, sizeof(int), edg_dtor_inc), 0);
    a = 1;
    b = 2;
    vec_pushback(&v, &a);
    vec_pushback(&v, &b);
    vec_fini(&v);
    EXPECT_EQ_INT(edg_dtor_n, 2);
  }
}
