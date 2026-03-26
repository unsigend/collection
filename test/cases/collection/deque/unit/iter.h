#include <deque.h>
#include <utest.h>

UTEST_CASE(iter)
{
  {
    struct deque d;
    struct deque_iter it;

    EXPECT_EQ_INT(deq_iter_init(NULL, &d), -1);
    EXPECT_EQ_INT(deq_iter_init(&it, NULL), -1);
  }

  {
    struct deque d;
    struct deque_iter it;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(deq_iter_init(&it, &d), 0);
    EXPECT_NULL(deq_iter_get(&it));
    deq_iter_inc(&it);
    EXPECT_NULL(deq_iter_get(&it));
    deq_fini(&d);
  }

  {
    struct deque d;
    struct deque_iter it;
    int x;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    x = 42;
    deq_pushback(&d, &x);
    EXPECT_EQ_INT(deq_iter_init(&it, &d), 0);
    EXPECT_NOTNULL(deq_iter_get(&it));
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 42);
    deq_iter_inc(&it);
    EXPECT_NULL(deq_iter_get(&it));
    deq_iter_inc(&it);
    EXPECT_NULL(deq_iter_get(&it));
    deq_fini(&d);
  }

  {
    struct deque d;
    struct deque_iter it;
    int a, b, c;
    size_t n;
    int sum;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    a = 2;
    b = 4;
    c = 6;
    deq_pushback(&d, &a);
    deq_pushback(&d, &b);
    deq_pushback(&d, &c);
    EXPECT_EQ_INT(deq_iter_init(&it, &d), 0);
    sum = 0;
    for (n = 0; n < 4; n++) {
      void *p = deq_iter_get(&it);

      if (p)
        sum += *(int *)p;
      deq_iter_inc(&it);
    }
    EXPECT_EQ_INT(sum, 12);
    deq_fini(&d);
  }

  {
    struct deque d;
    struct deque_iter it;
    int a, b, c;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    a = 1;
    b = 2;
    c = 3;
    deq_pushback(&d, &a);
    deq_pushback(&d, &b);
    deq_pushback(&d, &c);
    EXPECT_EQ_INT(deq_iter_init(&it, &d), 0);
    deq_iter_inc(&it);
    deq_iter_inc(&it);
    deq_iter_inc(&it);
    EXPECT_NULL(deq_iter_get(&it));
    deq_iter_dec(&it);
    EXPECT_NOTNULL(deq_iter_get(&it));
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 3);
    deq_iter_dec(&it);
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 2);
    deq_iter_dec(&it);
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 1);
    deq_iter_dec(&it);
    EXPECT_NULL(deq_iter_get(&it));
    deq_fini(&d);
  }

  {
    struct deque d;
    struct deque_iter it;
    int a, b;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    a = 7;
    b = 8;
    deq_pushback(&d, &a);
    deq_pushback(&d, &b);
    EXPECT_EQ_INT(deq_iter_init(&it, &d), 0);
    EXPECT_NOTNULL(deq_iter_get(&it));
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 7);
    deq_iter_inc(&it);
    EXPECT_NOTNULL(deq_iter_get(&it));
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 8);
    deq_iter_dec(&it);
    EXPECT_NOTNULL(deq_iter_get(&it));
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 7);
    deq_iter_inc(&it);
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 8);
    deq_iter_inc(&it);
    EXPECT_NULL(deq_iter_get(&it));
    deq_iter_dec(&it);
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 8);
    deq_iter_dec(&it);
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 7);
    deq_fini(&d);
  }

  {
    struct deque d;
    struct deque_iter it;
    int vals[5];
    size_t i;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    for (i = 0; i < 5; i++) {
      vals[i] = (int)(i * 11);
      EXPECT_EQ_INT(deq_pushback(&d, &vals[i]), 0);
    }
    EXPECT_EQ_INT(deq_iter_init(&it, &d), 0);
    for (i = 0; i < 5; i++) {
      void *p = deq_iter_get(&it);

      EXPECT_NOTNULL(p);
      EXPECT_EQ_INT(*(int *)p, (int)(i * 11));
      deq_iter_inc(&it);
    }
    EXPECT_NULL(deq_iter_get(&it));
    for (i = 5; i > 0; i--) {
      void *p;

      deq_iter_dec(&it);
      p = deq_iter_get(&it);
      EXPECT_NOTNULL(p);
      EXPECT_EQ_INT(*(int *)p, (int)((i - 1) * 11));
    }
    deq_iter_dec(&it);
    EXPECT_NULL(deq_iter_get(&it));
    deq_fini(&d);
  }

  {
    struct deque d;
    struct deque_iter it;
    int a, b, c;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    a = 1;
    b = 2;
    c = 3;
    deq_pushback(&d, &a);
    deq_pushback(&d, &b);
    deq_pushback(&d, &c);
    EXPECT_EQ_INT(deq_iter_init(&it, &d), 0);
    deq_iter_inc(&it);
    deq_iter_inc(&it);
    EXPECT_NOTNULL(deq_iter_get(&it));
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 3);
    deq_iter_dec(&it);
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 2);
    deq_iter_inc(&it);
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 3);
    deq_iter_dec(&it);
    deq_iter_dec(&it);
    EXPECT_NOTNULL(deq_iter_get(&it));
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 1);
    deq_iter_inc(&it);
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 2);
    deq_iter_inc(&it);
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 3);
    deq_iter_inc(&it);
    EXPECT_NULL(deq_iter_get(&it));
    deq_fini(&d);
  }

  {
    struct deque d;
    struct deque_iter it;
    int a, b, c;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    a = 10;
    b = 20;
    c = 30;
    deq_pushback(&d, &a);
    deq_pushback(&d, &b);
    deq_pushback(&d, &c);
    EXPECT_EQ_INT(deq_iter_init(&it, &d), 0);
    deq_iter_inc(&it);
    deq_iter_inc(&it);
    deq_iter_inc(&it);
    EXPECT_NULL(deq_iter_get(&it));
    deq_iter_inc(&it);
    EXPECT_NULL(deq_iter_get(&it));
    deq_iter_inc(&it);
    EXPECT_NULL(deq_iter_get(&it));
    deq_iter_dec(&it);
    EXPECT_NOTNULL(deq_iter_get(&it));
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 30);
    deq_fini(&d);
  }

  {
    struct deque d;
    struct deque_iter it;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(deq_iter_init(&it, &d), 0);
    deq_iter_dec(&it);
    EXPECT_NULL(deq_iter_get(&it));
    deq_fini(&d);
  }

  {
    struct deque d;
    struct deque_iter it;
    int a, b, c;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    a = 100;
    b = 200;
    c = 300;
    deq_pushback(&d, &a);
    deq_pushback(&d, &b);
    deq_pushback(&d, &c);
    EXPECT_EQ_INT(deq_iter_init(&it, &d), 0);
    deq_iter_inc(&it);
    EXPECT_NOTNULL(deq_iter_get(&it));
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 200);
    EXPECT_EQ_INT(deq_iter_init(&it, &d), 0);
    EXPECT_NOTNULL(deq_iter_get(&it));
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 100);
    deq_iter_inc(&it);
    deq_iter_inc(&it);
    deq_iter_inc(&it);
    EXPECT_NULL(deq_iter_get(&it));
    deq_iter_dec(&it);
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 300);
    EXPECT_EQ_INT(deq_iter_init(&it, &d), 0);
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 100);
    deq_fini(&d);
  }

  {
    struct deque d;
    struct deque_iter it;
    int x, y, z;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    z = 30;
    deq_pushback(&d, &z);
    y = 20;
    deq_pushfront(&d, &y);
    x = 10;
    deq_pushfront(&d, &x);
    EXPECT_EQ_INT(deq_iter_init(&it, &d), 0);
    EXPECT_NOTNULL(deq_iter_get(&it));
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 10);
    deq_iter_inc(&it);
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 20);
    deq_iter_inc(&it);
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 30);
    deq_iter_inc(&it);
    EXPECT_NULL(deq_iter_get(&it));
    deq_iter_dec(&it);
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 30);
    deq_iter_dec(&it);
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 20);
    deq_iter_dec(&it);
    EXPECT_EQ_INT(*(int *)deq_iter_get(&it), 10);
    deq_fini(&d);
  }

  {
    EXPECT_NULL(deq_iter_get(NULL));
    deq_iter_inc(NULL);
    deq_iter_dec(NULL);
  }

  {
    struct deque d;
    struct deque_iter it;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(deq_resize(&d, 4), 0);
    EXPECT_EQ_INT(deq_iter_init(&it, &d), 0);
    deq_iter_inc(&it);
    deq_iter_inc(&it);
    EXPECT_NOTNULL(deq_iter_get(&it));
    EXPECT_EQ_INT(deq_resize(&d, 1), 0);
    EXPECT_NULL(deq_iter_get(&it));
    deq_fini(&d);
  }
}