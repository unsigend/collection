#include <queue.h>
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
    struct queue q;

    EXPECT_EQ_INT(queue_init(NULL, sizeof(int), NULL), -1);
    EXPECT_EQ_INT(queue_init(&q, 0, NULL), -1);
  }

  {
    struct queue q;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    EXPECT_TRUE(queue_empty(&q));
    EXPECT_EQ_UINT(queue_size(&q), 0);
    EXPECT_NULL(queue_peek(&q));
    EXPECT_EQ_INT(queue_deq(&q, NULL), -1);
    queue_fini(&q);
  }

  {
    struct queue q;
    int x;
    void *p;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    x = 10;
    EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    EXPECT_FALSE(queue_empty(&q));
    EXPECT_EQ_UINT(queue_size(&q), 1);
    p = queue_peek(&q);
    EXPECT_NOTNULL(p);
    EXPECT_EQ_INT(*(int *)p, 10);
    EXPECT_EQ_PTR(queue_peek(&q), p);
    queue_fini(&q);
  }

  {
    struct queue q;
    int a, b, c, out;
    void *p;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    a = 1;
    b = 2;
    c = 3;
    EXPECT_EQ_INT(queue_enq(&q, &a), 0);
    EXPECT_EQ_INT(queue_enq(&q, &b), 0);
    EXPECT_EQ_INT(queue_enq(&q, &c), 0);
    EXPECT_EQ_UINT(queue_size(&q), 3);
    p = queue_peek(&q);
    EXPECT_NOTNULL(p);
    EXPECT_EQ_INT(*(int *)p, 1);
    out = 0;
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_TRUE(queue_empty(&q));
    EXPECT_EQ_INT(queue_deq(&q, NULL), -1);
    queue_fini(&q);
  }

  {
    struct queue q;
    int a, b, c;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    a = 5;
    b = 6;
    c = 7;
    queue_enq(&q, &a);
    queue_enq(&q, &b);
    queue_enq(&q, &c);
    queue_clear(&q);
    EXPECT_TRUE(queue_empty(&q));
    EXPECT_EQ_UINT(queue_size(&q), 0);
    EXPECT_EQ_INT(queue_enq(&q, &a), 0);
    EXPECT_EQ_UINT(queue_size(&q), 1);
    queue_fini(&q);
  }

  {
    struct queue q;
    double a, b, out;

    EXPECT_EQ_INT(queue_init(&q, sizeof(double), NULL), 0);
    a = 3.14;
    b = 2.71;
    EXPECT_EQ_INT(queue_enq(&q, &a), 0);
    EXPECT_EQ_INT(queue_enq(&q, &b), 0);
    EXPECT_EQ_UINT(queue_size(&q), 2);
    EXPECT_NOTNULL(queue_peek(&q));
    EXPECT_EQ_DOUBLE(*(double *)queue_peek(&q), 3.14);
    out = 0.0;
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_DOUBLE(out, 3.14);
    EXPECT_EQ_DOUBLE(*(double *)queue_peek(&q), 2.71);
    queue_fini(&q);
  }

  {
    struct queue q;
    int x;
    void *p;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    x = 1;
    EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    p = queue_peek(&q);
    EXPECT_NOTNULL(p);
    EXPECT_EQ_INT(*(int *)p, 1);
    x = 2;
    EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    p = queue_peek(&q);
    EXPECT_NOTNULL(p);
    EXPECT_EQ_INT(*(int *)p, 1);
    x = 3;
    EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    p = queue_peek(&q);
    EXPECT_NOTNULL(p);
    EXPECT_EQ_INT(*(int *)p, 1);
    queue_fini(&q);
  }

  {
    struct queue q;

    dtor_n = 0;
    EXPECT_EQ_INT(queue_init(&q, sizeof(int), dtor_inc), 0);
    queue_fini(&q);
    EXPECT_EQ_INT(dtor_n, 0);
  }

  {
    struct queue q;
    int a, b, c;

    dtor_n = 0;
    EXPECT_EQ_INT(queue_init(&q, sizeof(int), dtor_inc), 0);
    a = 1;
    b = 2;
    c = 3;
    queue_enq(&q, &a);
    queue_enq(&q, &b);
    queue_enq(&q, &c);
    queue_clear(&q);
    EXPECT_EQ_INT(dtor_n, 3);
    queue_fini(&q);
  }

  {
    struct queue q;
    int x;

    dtor_n = 0;
    EXPECT_EQ_INT(queue_init(&q, sizeof(int), dtor_inc), 0);
    x = 7;
    queue_enq(&q, &x);
    EXPECT_EQ_INT(queue_deq(&q, NULL), 0);
    EXPECT_EQ_INT(dtor_n, 1);
    queue_fini(&q);
  }

  {
    struct queue q;
    int x, out;

    dtor_n = 0;
    EXPECT_EQ_INT(queue_init(&q, sizeof(int), dtor_inc), 0);
    x = 7;
    queue_enq(&q, &x);
    out = 0;
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out, 7);
    EXPECT_EQ_INT(dtor_n, 0);
    queue_fini(&q);
  }

  {
    int x;

    EXPECT_EQ_INT(queue_enq(NULL, &x), -1);
    EXPECT_EQ_INT(queue_deq(NULL, &x), -1);
    EXPECT_NULL(queue_peek(NULL));
  }

  {
    struct queue q;
    int a;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    a = 1;
    EXPECT_EQ_INT(queue_enq(&q, NULL), -1);
    queue_enq(&q, &a);
    queue_fini(&q);
  }

  {
    queue_fini(NULL);
    queue_clear(NULL);
  }
}
