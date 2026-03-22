#include <utest.h>
#include <string.h>
#include <vector.h>

static int dtor_n;
static void dtor_inc(void *p)
{
  (void)p;
  dtor_n++;
}

UTEST_CASE(basic)
{
  {
    struct vector v;

    EXPECT_EQ_INT(vec_init(NULL, sizeof(int), NULL), -1);
    EXPECT_EQ_INT(vec_init(&v, 0, NULL), -1);
  }

  {
    struct vector v;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    EXPECT_TRUE(vec_empty(&v));
    EXPECT_EQ_UINT(vec_size(&v), 0u);
    EXPECT_GE_UINT(vec_capacity(&v), vec_size(&v));
    EXPECT_NULL(vec_at(&v, 0u));
    EXPECT_NULL(vec_at(&v, 1u));
    EXPECT_EQ_INT(vec_popback(&v, NULL), -1);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    x = 10;
    EXPECT_EQ_INT(vec_pushback(&v, &x), 0);
    EXPECT_FALSE(vec_empty(&v));
    EXPECT_EQ_UINT(vec_size(&v), 1u);
    EXPECT_NOTNULL(vec_at(&v, 0u));
    EXPECT_EQ_INT(*(int *)vec_at(&v, 0u), 10);
    EXPECT_NOTNULL(vec_front(&v));
    EXPECT_EQ_PTR(vec_front(&v), vec_at(&v, 0u));
    EXPECT_NOTNULL(vec_back(&v));
    EXPECT_EQ_PTR(vec_back(&v), vec_at(&v, 0u));
    vec_fini(&v);
  }

  {
    struct vector v;
    int a, b, c, out;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    a = 1;
    b = 2;
    c = 3;
    EXPECT_EQ_INT(vec_pushback(&v, &a), 0);
    EXPECT_EQ_INT(vec_pushback(&v, &b), 0);
    EXPECT_EQ_INT(vec_pushback(&v, &c), 0);
    EXPECT_EQ_UINT(vec_size(&v), 3u);
    out = 0;
    EXPECT_EQ_INT(vec_popback(&v, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_EQ_INT(vec_popback(&v, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(vec_popback(&v, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_TRUE(vec_empty(&v));
    EXPECT_EQ_INT(vec_popback(&v, NULL), -1);
    vec_fini(&v);
  }

  {
    struct vector v;
    int a, b, c;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    a = 5;
    b = 6;
    c = 7;
    vec_pushback(&v, &a);
    vec_pushback(&v, &b);
    vec_pushback(&v, &c);
    vec_clear(&v);
    EXPECT_TRUE(vec_empty(&v));
    EXPECT_EQ_UINT(vec_size(&v), 0u);
    EXPECT_EQ_INT(vec_pushback(&v, &a), 0);
    EXPECT_EQ_UINT(vec_size(&v), 1u);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;
    void *p;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(vec_resize(&v, 1u), 0);
    p = vec_at(&v, 0u);
    EXPECT_NOTNULL(p);
    *(int *)p = 42;
    EXPECT_EQ_INT(vec_resize(&v, 3u), 0);
    EXPECT_EQ_UINT(vec_size(&v), 3u);
    p = vec_at(&v, 1u);
    EXPECT_NOTNULL(p);
    x = 1;
    memcpy(p, &x, sizeof(int));
    p = vec_at(&v, 2u);
    EXPECT_NOTNULL(p);
    x = 2;
    memcpy(p, &x, sizeof(int));
    EXPECT_EQ_INT(vec_resize(&v, 1u), 0);
    EXPECT_NOTNULL(vec_at(&v, 0u));
    EXPECT_EQ_INT(*(int *)vec_at(&v, 0u), 42);
    vec_fini(&v);
  }

  {
    struct vector v;
    size_t idx;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(vec_resize(&v, 4u), 0);
    EXPECT_EQ_UINT(vec_size(&v), 4u);
    EXPECT_GE_UINT(vec_capacity(&v), vec_size(&v));
    for (idx = 0; idx < 4; idx++) {
      void *p = vec_at(&v, idx);
      EXPECT_NOTNULL(p);
      *(int *)p = (int)(idx + 1);
    }
    EXPECT_EQ_INT(*(int *)vec_at(&v, 3u), 4);
    EXPECT_EQ_INT(vec_resize(&v, 2u), 0);
    EXPECT_EQ_UINT(vec_size(&v), 2u);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 0u), 1);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 1u), 2);
    EXPECT_EQ_INT(vec_resize(&v, 2u), 0);
    EXPECT_EQ_UINT(vec_size(&v), 2u);
    vec_fini(&v);
  }

  {
    struct vector v;
    size_t cap_a, cap_b;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(vec_resize(&v, 3u), 0);
    cap_a = vec_capacity(&v);
    EXPECT_EQ_INT(vec_shrink(&v), 0);
    cap_b = vec_capacity(&v);
    EXPECT_EQ_UINT(vec_size(&v), 3u);
    EXPECT_GE_UINT(cap_b, vec_size(&v));
    EXPECT_LE_UINT(cap_b, cap_a);
    EXPECT_EQ_INT(vec_shrink(&v), 0);
    EXPECT_EQ_UINT(vec_capacity(&v), cap_b);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(vec_resize(&v, 2u), 0);
    vec_clear(&v);
    EXPECT_EQ_INT(vec_shrink(&v), 0);
    EXPECT_TRUE(vec_empty(&v));
    x = 1;
    EXPECT_EQ_INT(vec_pushback(&v, &x), 0);
    vec_fini(&v);
  }

  {
    struct vector v;
    int i;
    size_t idx;
    size_t cap0;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    i = 0;
    EXPECT_EQ_INT(vec_pushback(&v, &i), 0);
    cap0 = vec_capacity(&v);
    for (idx = 1; idx < 17; idx++) {
      i = (int)idx;
      EXPECT_EQ_INT(vec_pushback(&v, &i), 0);
    }
    EXPECT_EQ_UINT(vec_size(&v), 17u);
    EXPECT_GE_UINT(vec_capacity(&v), vec_size(&v));
    EXPECT_GE_UINT(vec_capacity(&v), cap0);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 0u), 0);
    EXPECT_EQ_INT(*(int *)vec_back(&v), 16);
    vec_fini(&v);
  }

  {
    struct vector v;
    double a, b, out;

    EXPECT_EQ_INT(vec_init(&v, sizeof(double), NULL), 0);
    a = 3.14;
    b = 2.71;
    EXPECT_EQ_INT(vec_pushback(&v, &a), 0);
    EXPECT_EQ_INT(vec_pushback(&v, &b), 0);
    EXPECT_EQ_UINT(vec_size(&v), 2u);
    EXPECT_NOTNULL(vec_at(&v, 0u));
    EXPECT_EQ_DOUBLE(*(double *)vec_at(&v, 0u), 3.14);
    EXPECT_EQ_DOUBLE(*(double *)vec_back(&v), 2.71);
    out = 0.0;
    EXPECT_EQ_INT(vec_popback(&v, &out), 0);
    EXPECT_EQ_DOUBLE(out, 2.71);
    vec_fini(&v);
  }

  {
    struct vector v;
    int a, b, c, out;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    a = 1;
    b = 2;
    vec_pushback(&v, &a);
    vec_pushback(&v, &b);
    c = 9;
    EXPECT_EQ_INT(vec_insert(&v, 0u, &c), 0);
    EXPECT_EQ_UINT(vec_size(&v), 3u);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 0u), 9);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 2u), 2);
    c = 5;
    EXPECT_EQ_INT(vec_insert(&v, 2u, &c), 0);
    EXPECT_EQ_UINT(vec_size(&v), 4u);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 2u), 5);
    c = 8;
    EXPECT_EQ_INT(vec_insert(&v, 4u, &c), 0);
    EXPECT_EQ_UINT(vec_size(&v), 5u);
    EXPECT_EQ_INT(*(int *)vec_back(&v), 8);
    out = 0;
    EXPECT_EQ_INT(vec_remove(&v, 2u, &out), 0);
    EXPECT_EQ_INT(out, 5);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 2u), 2);
    EXPECT_EQ_INT(vec_remove(&v, 3u, NULL), 0);
    EXPECT_EQ_UINT(vec_size(&v), 3u);
    EXPECT_EQ_INT(vec_remove(&v, 0u, NULL), 0);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 0u), 1);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    x = 42;
    EXPECT_EQ_INT(vec_insert(&v, 0u, &x), 0);
    EXPECT_EQ_UINT(vec_size(&v), 1u);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 0u), 42);
    vec_fini(&v);
  }

  {
    struct vector v;
    int a, b, out;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    a = 1;
    b = 2;
    vec_pushback(&v, &a);
    vec_pushback(&v, &b);
    out = 0;
    EXPECT_EQ_INT(vec_remove(&v, 1u, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_UINT(vec_size(&v), 1u);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 0u), 1);
    EXPECT_EQ_INT(vec_remove(&v, 0u, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_TRUE(vec_empty(&v));
    vec_fini(&v);
  }

  {
    struct vector v;

    dtor_n = 0;
    EXPECT_EQ_INT(vec_init(&v, sizeof(int), dtor_inc), 0);
    vec_fini(&v);
    EXPECT_EQ_INT(dtor_n, 0);
  }

  {
    struct vector v;
    int a, b, c;

    dtor_n = 0;
    EXPECT_EQ_INT(vec_init(&v, sizeof(int), dtor_inc), 0);
    a = 1;
    b = 2;
    c = 3;
    vec_pushback(&v, &a);
    vec_pushback(&v, &b);
    vec_pushback(&v, &c);
    vec_clear(&v);
    EXPECT_EQ_INT(dtor_n, 3);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;

    dtor_n = 0;
    EXPECT_EQ_INT(vec_init(&v, sizeof(int), dtor_inc), 0);
    x = 7;
    vec_pushback(&v, &x);
    EXPECT_EQ_INT(vec_popback(&v, NULL), 0);
    EXPECT_EQ_INT(dtor_n, 1);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x, out;

    dtor_n = 0;
    EXPECT_EQ_INT(vec_init(&v, sizeof(int), dtor_inc), 0);
    x = 7;
    vec_pushback(&v, &x);
    out = 0;
    EXPECT_EQ_INT(vec_popback(&v, &out), 0);
    EXPECT_EQ_INT(out, 7);
    EXPECT_EQ_INT(dtor_n, 0);
    vec_fini(&v);
  }

  {
    struct vector v;
    int a, b;

    dtor_n = 0;
    EXPECT_EQ_INT(vec_init(&v, sizeof(int), dtor_inc), 0);
    a = 1;
    b = 2;
    vec_pushback(&v, &a);
    vec_pushback(&v, &b);
    EXPECT_EQ_INT(vec_remove(&v, 0u, NULL), 0);
    EXPECT_EQ_INT(dtor_n, 1);
    vec_fini(&v);
  }

  {
    struct vector v;
    int a, b, out;

    dtor_n = 0;
    EXPECT_EQ_INT(vec_init(&v, sizeof(int), dtor_inc), 0);
    a = 1;
    b = 2;
    vec_pushback(&v, &a);
    vec_pushback(&v, &b);
    out = 0;
    EXPECT_EQ_INT(vec_remove(&v, 0u, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(dtor_n, 0);
    vec_fini(&v);
  }

  {
    int x;

    EXPECT_EQ_INT(vec_resize(NULL, 1u), -1);
    EXPECT_EQ_INT(vec_shrink(NULL), -1);
    EXPECT_EQ_INT(vec_pushback(NULL, &x), -1);
    EXPECT_EQ_INT(vec_insert(NULL, 0u, &x), -1);
    EXPECT_EQ_INT(vec_remove(NULL, 0u, NULL), -1);
  }

  {
    struct vector v;
    int a;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    a = 1;
    EXPECT_EQ_INT(vec_pushback(&v, NULL), -1);
    EXPECT_EQ_INT(vec_insert(&v, 0u, NULL), -1);
    vec_pushback(&v, &a);
    EXPECT_EQ_INT(vec_remove(&v, 99u, NULL), -1);
    vec_fini(&v);
  }

  {
    vec_fini(NULL);
    vec_clear(NULL);
  }
}
