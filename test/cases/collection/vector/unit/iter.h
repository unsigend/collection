#include <utest.h>
#include <vector.h>

UTEST_CASE(iter)
{
  {
    struct vector v;
    struct vector_iter it;

    EXPECT_EQ_INT(vec_iter_init(NULL, &v), -1);
    EXPECT_EQ_INT(vec_iter_init(&it, NULL), -1);
  }

  {
    struct vector v;
    struct vector_iter it;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(vec_iter_init(&it, &v), 0);
    EXPECT_NULL(vec_iter_get(&it));
    vec_iter_inc(&it);
    EXPECT_NULL(vec_iter_get(&it));
    vec_fini(&v);
  }

  {
    struct vector v;
    struct vector_iter it;
    int x;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    x = 42;
    vec_pushback(&v, &x);
    EXPECT_EQ_INT(vec_iter_init(&it, &v), 0);
    EXPECT_NOTNULL(vec_iter_get(&it));
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 42);
    vec_iter_inc(&it);
    EXPECT_NULL(vec_iter_get(&it));
    vec_iter_inc(&it);
    EXPECT_NULL(vec_iter_get(&it));
    vec_fini(&v);
  }

  {
    struct vector v;
    struct vector_iter it;
    int a, b, c;
    size_t n;
    int sum;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    a = 2;
    b = 4;
    c = 6;
    vec_pushback(&v, &a);
    vec_pushback(&v, &b);
    vec_pushback(&v, &c);
    EXPECT_EQ_INT(vec_iter_init(&it, &v), 0);
    sum = 0;
    for (n = 0; n < 4; n++) {
      void *p = vec_iter_get(&it);

      if (p)
        sum += *(int *)p;
      vec_iter_inc(&it);
    }
    EXPECT_EQ_INT(sum, 12);
    vec_fini(&v);
  }

  {
    struct vector v;
    struct vector_iter it;
    int a, b, c;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    a = 1;
    b = 2;
    c = 3;
    vec_pushback(&v, &a);
    vec_pushback(&v, &b);
    vec_pushback(&v, &c);
    EXPECT_EQ_INT(vec_iter_init(&it, &v), 0);
    vec_iter_inc(&it);
    vec_iter_inc(&it);
    vec_iter_inc(&it);
    EXPECT_NULL(vec_iter_get(&it));
    vec_iter_dec(&it);
    EXPECT_NOTNULL(vec_iter_get(&it));
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 3);
    vec_iter_dec(&it);
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 2);
    vec_iter_dec(&it);
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 1);
    vec_iter_dec(&it);
    EXPECT_NULL(vec_iter_get(&it));
    vec_fini(&v);
  }

  {
    struct vector v;
    struct vector_iter it;
    int a, b;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    a = 7;
    b = 8;
    vec_pushback(&v, &a);
    vec_pushback(&v, &b);
    EXPECT_EQ_INT(vec_iter_init(&it, &v), 0);
    EXPECT_NOTNULL(vec_iter_get(&it));
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 7);
    vec_iter_inc(&it);
    EXPECT_NOTNULL(vec_iter_get(&it));
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 8);
    vec_iter_dec(&it);
    EXPECT_NOTNULL(vec_iter_get(&it));
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 7);
    vec_iter_inc(&it);
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 8);
    vec_iter_inc(&it);
    EXPECT_NULL(vec_iter_get(&it));
    vec_iter_dec(&it);
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 8);
    vec_iter_dec(&it);
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 7);
    vec_fini(&v);
  }

  {
    struct vector v;
    struct vector_iter it;
    int vals[5];
    size_t i;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    for (i = 0; i < 5; i++) {
      vals[i] = (int)(i * 11);
      EXPECT_EQ_INT(vec_pushback(&v, &vals[i]), 0);
    }
    EXPECT_EQ_INT(vec_iter_init(&it, &v), 0);
    for (i = 0; i < 5; i++) {
      void *p = vec_iter_get(&it);

      EXPECT_NOTNULL(p);
      EXPECT_EQ_INT(*(int *)p, (int)(i * 11));
      vec_iter_inc(&it);
    }
    EXPECT_NULL(vec_iter_get(&it));
    for (i = 5; i > 0; i--) {
      void *p;

      vec_iter_dec(&it);
      p = vec_iter_get(&it);
      EXPECT_NOTNULL(p);
      EXPECT_EQ_INT(*(int *)p, (int)((i - 1) * 11));
    }
    vec_iter_dec(&it);
    EXPECT_NULL(vec_iter_get(&it));
    vec_fini(&v);
  }

  {
    struct vector v;
    struct vector_iter it;
    int a, b, c;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    a = 1;
    b = 2;
    c = 3;
    vec_pushback(&v, &a);
    vec_pushback(&v, &b);
    vec_pushback(&v, &c);
    EXPECT_EQ_INT(vec_iter_init(&it, &v), 0);
    vec_iter_inc(&it);
    vec_iter_inc(&it);
    EXPECT_NOTNULL(vec_iter_get(&it));
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 3);
    vec_iter_dec(&it);
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 2);
    vec_iter_inc(&it);
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 3);
    vec_iter_dec(&it);
    vec_iter_dec(&it);
    EXPECT_NOTNULL(vec_iter_get(&it));
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 1);
    vec_iter_inc(&it);
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 2);
    vec_iter_inc(&it);
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 3);
    vec_iter_inc(&it);
    EXPECT_NULL(vec_iter_get(&it));
    vec_fini(&v);
  }

  {
    struct vector v;
    struct vector_iter it;
    int a, b, c;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    a = 10;
    b = 20;
    c = 30;
    vec_pushback(&v, &a);
    vec_pushback(&v, &b);
    vec_pushback(&v, &c);
    EXPECT_EQ_INT(vec_iter_init(&it, &v), 0);
    vec_iter_inc(&it);
    vec_iter_inc(&it);
    vec_iter_inc(&it);
    EXPECT_NULL(vec_iter_get(&it));
    vec_iter_inc(&it);
    EXPECT_NULL(vec_iter_get(&it));
    vec_iter_inc(&it);
    EXPECT_NULL(vec_iter_get(&it));
    vec_iter_dec(&it);
    EXPECT_NOTNULL(vec_iter_get(&it));
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 30);
    vec_fini(&v);
  }

  {
    struct vector v;
    struct vector_iter it;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(vec_iter_init(&it, &v), 0);
    vec_iter_dec(&it);
    EXPECT_NULL(vec_iter_get(&it));
    vec_fini(&v);
  }

  {
    struct vector v;
    struct vector_iter it;
    int a, b, c;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    a = 100;
    b = 200;
    c = 300;
    vec_pushback(&v, &a);
    vec_pushback(&v, &b);
    vec_pushback(&v, &c);
    EXPECT_EQ_INT(vec_iter_init(&it, &v), 0);
    vec_iter_inc(&it);
    EXPECT_NOTNULL(vec_iter_get(&it));
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 200);
    EXPECT_EQ_INT(vec_iter_init(&it, &v), 0);
    EXPECT_NOTNULL(vec_iter_get(&it));
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 100);
    vec_iter_inc(&it);
    vec_iter_inc(&it);
    vec_iter_inc(&it);
    EXPECT_NULL(vec_iter_get(&it));
    vec_iter_dec(&it);
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 300);
    EXPECT_EQ_INT(vec_iter_init(&it, &v), 0);
    EXPECT_EQ_INT(*(int *)vec_iter_get(&it), 100);
    vec_fini(&v);
  }

  {
    EXPECT_NULL(vec_iter_get(NULL));
    vec_iter_inc(NULL);
    vec_iter_dec(NULL);
  }

  {
    struct vector v;
    struct vector_iter it;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(vec_resize(&v, 4), 0);
    EXPECT_EQ_INT(vec_iter_init(&it, &v), 0);
    vec_iter_inc(&it);
    vec_iter_inc(&it);
    EXPECT_NOTNULL(vec_iter_get(&it));
    EXPECT_EQ_INT(vec_resize(&v, 1), 0);
    EXPECT_NULL(vec_iter_get(&it));
    vec_fini(&v);
  }
}
