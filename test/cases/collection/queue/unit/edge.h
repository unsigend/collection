#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <queue.h>
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
    struct queue q;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    EXPECT_TRUE(queue_empty(&q));
    EXPECT_NULL(queue_peek(&q));
    EXPECT_EQ_INT(queue_deq(&q, NULL), -1);
    queue_fini(&q);
  }

  {
    struct queue q;
    int x;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    x = 1;
    EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    EXPECT_EQ_INT(queue_deq(&q, NULL), 0);
    EXPECT_EQ_INT(queue_deq(&q, NULL), -1);
    queue_fini(&q);
  }

  {
    struct queue q;
    int x;

    errno = 0;
    EXPECT_EQ_INT(queue_init(&q, SIZE_MAX, NULL), 0);
    x = 0;
    EXPECT_EQ_INT(queue_enq(&q, &x), -1);
    EXPECT_EQ_INT(errno, ERANGE);
    EXPECT_EQ_UINT(queue_size(&q), 0);
    queue_fini(&q);
  }

  {
    struct queue q;
    int x;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    x = 1;
    EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    EXPECT_EQ_INT(queue_deq(&q, NULL), 0);
    EXPECT_TRUE(queue_empty(&q));
    EXPECT_NULL(queue_peek(&q));
    EXPECT_EQ_INT(queue_deq(&q, NULL), -1);
    queue_fini(&q);
  }

  {
    struct queue q;
    int x;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    x = 1;
    EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    queue_clear(&q);
    EXPECT_TRUE(queue_empty(&q));
    EXPECT_EQ_INT(queue_deq(&q, NULL), -1);
    queue_fini(&q);
  }

  {
    struct queue q;
    int x;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    x = 1;
    EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    queue_clear(&q);
    queue_clear(&q);
    EXPECT_TRUE(queue_empty(&q));
    queue_fini(&q);
  }

  {
    struct queue q;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    queue_clear(&q);
    queue_clear(&q);
    EXPECT_TRUE(queue_empty(&q));
    queue_fini(&q);
  }

  {
    struct queue q;
    int x, out;
    void *p0, *p1;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    x = 1;
    EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    x = 2;
    EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    p0 = queue_peek(&q);
    p1 = queue_peek(&q);
    EXPECT_NOTNULL(p0);
    EXPECT_EQ_PTR(p0, p1);
    EXPECT_EQ_INT(*(int *)p0, 1);
    out = 0;
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_NOTNULL(queue_peek(&q));
    EXPECT_EQ_INT(*(int *)queue_peek(&q), 2);
    queue_fini(&q);
  }

  {
    struct queue q;
    uint8_t a, b, out;

    EXPECT_EQ_INT(queue_init(&q, sizeof(uint8_t), NULL), 0);
    a = 200;
    b = 201;
    EXPECT_EQ_INT(queue_enq(&q, &a), 0);
    EXPECT_EQ_INT(queue_enq(&q, &b), 0);
    EXPECT_EQ_UINT(*(uint8_t *)queue_peek(&q), 200);
    out = 0;
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_UINT(out, 200);
    EXPECT_EQ_UINT(*(uint8_t *)queue_peek(&q), 201);
    queue_fini(&q);
  }

  {
    struct queue q;
    int16_t a, b, out;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int16_t), NULL), 0);
    a = -30000;
    b = 30000;
    EXPECT_EQ_INT(queue_enq(&q, &a), 0);
    EXPECT_EQ_INT(queue_enq(&q, &b), 0);
    out = 0;
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out, -30000);
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out, 30000);
    queue_fini(&q);
  }

  {
    struct pair {
      char u;
      char v;
    } s0, s1, out;
    struct queue q;

    EXPECT_EQ_INT(queue_init(&q, sizeof(struct pair), NULL), 0);
    s0.u = 1;
    s0.v = 2;
    s1.u = 3;
    s1.v = 4;
    EXPECT_EQ_INT(queue_enq(&q, &s0), 0);
    EXPECT_EQ_INT(queue_enq(&q, &s1), 0);
    memset(&out, 0, sizeof(out));
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_CHAR(out.u, 1);
    EXPECT_EQ_CHAR(out.v, 2);
    memset(&out, 0, sizeof(out));
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_CHAR(out.u, 3);
    EXPECT_EQ_CHAR(out.v, 4);
    queue_fini(&q);
  }

  {
    struct queue q;
    int x;
    size_t sz0, sz1;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    x = 1;
    sz0 = queue_size(&q);
    EXPECT_EQ_INT(queue_enq(&q, NULL), -1);
    sz1 = queue_size(&q);
    EXPECT_EQ_UINT(sz0, sz1);
    EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    queue_fini(&q);
  }

  {
    struct queue q;
    int x, y;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    x = 100;
    EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    x = 200;
    EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    y = 0;
    EXPECT_EQ_INT(queue_deq(&q, &y), 0);
    EXPECT_EQ_INT(y, 100);
    x = 300;
    EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    EXPECT_EQ_INT(*(int *)queue_peek(&q), 200);
    y = 0;
    EXPECT_EQ_INT(queue_deq(&q, &y), 0);
    EXPECT_EQ_INT(y, 200);
    y = 0;
    EXPECT_EQ_INT(queue_deq(&q, &y), 0);
    EXPECT_EQ_INT(y, 300);
    queue_fini(&q);
  }

  {
    struct queue q;
    int x, out;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    x = 42;
    EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    out = 0;
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out, 42);
    EXPECT_TRUE(queue_empty(&q));
    EXPECT_NULL(queue_peek(&q));
    queue_fini(&q);
  }

  {
    struct queue q;
    int a, b;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(queue_init(&q, sizeof(int), edg_dtor_inc), 0);
    a = 1;
    b = 2;
    queue_enq(&q, &a);
    queue_enq(&q, &b);
    queue_fini(&q);
    EXPECT_EQ_INT(edg_dtor_n, 2);
  }

  {
    struct queue q;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(queue_init(&q, sizeof(int), edg_dtor_inc), 0);
    queue_clear(&q);
    EXPECT_EQ_INT(edg_dtor_n, 0);
    queue_fini(&q);
  }

  {
    struct queue q;
    int x;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(queue_init(&q, sizeof(int), edg_dtor_inc), 0);
    x = 1;
    queue_enq(&q, &x);
    queue_clear(&q);
    EXPECT_EQ_INT(edg_dtor_n, 1);
    queue_fini(&q);
  }

  {
    struct queue q;
    int x, out;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(queue_init(&q, sizeof(int), edg_dtor_inc), 0);
    x = 9;
    queue_enq(&q, &x);
    out = 0;
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out, 9);
    EXPECT_EQ_INT(edg_dtor_n, 0);
    queue_fini(&q);
  }

  {
    struct queue q;
    int x;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(queue_init(&q, sizeof(int), edg_dtor_inc), 0);
    x = 1;
    queue_enq(&q, &x);
    x = 2;
    queue_enq(&q, &x);
    EXPECT_EQ_INT(queue_deq(&q, NULL), 0);
    EXPECT_EQ_INT(edg_dtor_n, 1);
    EXPECT_EQ_INT(queue_deq(&q, NULL), 0);
    EXPECT_EQ_INT(edg_dtor_n, 2);
    queue_fini(&q);
  }
}
