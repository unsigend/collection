#include <slist.h>
#include <utest.h>

UTEST_CASE(iter)
{
  {
    struct slist s;
    struct slist_iter it;

    EXPECT_EQ_INT(slist_iter_init(NULL, &s), -1);
    EXPECT_EQ_INT(slist_iter_init(&it, NULL), -1);
  }

  {
    struct slist s;
    struct slist_iter it;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    EXPECT_EQ_INT(slist_iter_init(&it, &s), 0);
    EXPECT_NULL(slist_iter_get(&it));
    slist_iter_inc(&it);
    EXPECT_NULL(slist_iter_get(&it));
    slist_fini(&s);
  }

  {
    struct slist s;
    struct slist_iter it;
    int x;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    x = 42;
    EXPECT_EQ_INT(slist_pushback(&s, &x), 0);
    EXPECT_EQ_INT(slist_iter_init(&it, &s), 0);
    EXPECT_NOTNULL(slist_iter_get(&it));
    EXPECT_EQ_PTR(slist_iter_get(&it), &x);
    slist_iter_inc(&it);
    EXPECT_NULL(slist_iter_get(&it));
    slist_iter_inc(&it);
    EXPECT_NULL(slist_iter_get(&it));
    slist_fini(&s);
  }

  {
    struct slist s;
    struct slist_iter it;
    int a, b, c;
    size_t n;
    int sum;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    a = 2;
    b = 4;
    c = 6;
    EXPECT_EQ_INT(slist_pushback(&s, &a), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &b), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &c), 0);
    EXPECT_EQ_INT(slist_iter_init(&it, &s), 0);
    sum = 0;
    for (n = 0; n < 4; n++) {
      void *p = slist_iter_get(&it);

      if (p)
        sum += *(int *)p;
      slist_iter_inc(&it);
    }
    EXPECT_EQ_INT(sum, 12);
    slist_fini(&s);
  }

  {
    struct slist s;
    struct slist_iter it;
    int x, y, z;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    x = 1;
    y = 2;
    z = 3;
    EXPECT_EQ_INT(slist_pushback(&s, &x), 0);
    EXPECT_EQ_INT(slist_pushfront(&s, &y), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &z), 0);
    EXPECT_EQ_INT(slist_iter_init(&it, &s), 0);
    EXPECT_EQ_PTR(slist_iter_get(&it), &y);
    slist_iter_inc(&it);
    EXPECT_EQ_PTR(slist_iter_get(&it), &x);
    slist_iter_inc(&it);
    EXPECT_EQ_PTR(slist_iter_get(&it), &z);
    slist_iter_inc(&it);
    EXPECT_NULL(slist_iter_get(&it));
    slist_fini(&s);
  }

  {
    struct slist s;
    struct slist_iter it;
    int vals[5];
    size_t i;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    for (i = 0; i < 5; i++) {
      vals[i] = (int)(i * 9);
      EXPECT_EQ_INT(slist_pushback(&s, &vals[i]), 0);
    }
    EXPECT_EQ_INT(slist_iter_init(&it, &s), 0);
    for (i = 0; i < 5; i++) {
      void *p = slist_iter_get(&it);

      EXPECT_NOTNULL(p);
      EXPECT_EQ_INT(*(int *)p, (int)(i * 9));
      slist_iter_inc(&it);
    }
    EXPECT_NULL(slist_iter_get(&it));
    slist_iter_inc(&it);
    EXPECT_NULL(slist_iter_get(&it));
    slist_fini(&s);
  }

  {
    struct slist s;
    struct slist_iter it;
    int a, b, c;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    a = 10;
    b = 20;
    c = 30;
    EXPECT_EQ_INT(slist_pushback(&s, &a), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &b), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &c), 0);
    EXPECT_EQ_INT(slist_iter_init(&it, &s), 0);
    slist_iter_inc(&it);
    EXPECT_EQ_PTR(slist_iter_get(&it), &b);
    EXPECT_EQ_INT(slist_iter_init(&it, &s), 0);
    EXPECT_EQ_PTR(slist_iter_get(&it), &a);
    slist_iter_inc(&it);
    slist_iter_inc(&it);
    slist_iter_inc(&it);
    EXPECT_NULL(slist_iter_get(&it));
    slist_fini(&s);
  }

  {
    EXPECT_NULL(slist_iter_get(NULL));
    slist_iter_inc(NULL);
  }
}
