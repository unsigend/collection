#include <stdlib.h>
#include <string.h>
#include <queue.h>
#include <utest.h>

struct intg_pair {
  int x;
  int y;
};

struct intg_nested {
  short layer;
  int id;
};

struct intg_heap_box {
  int *p;
};

static int intg_dtor_n;
static void intg_dtor_inc(void *p)
{
  (void)p;
  intg_dtor_n++;
}

static int intg_heap_dtor_n;
static void intg_heap_dtor(void *ele)
{
  struct intg_heap_box *b = (struct intg_heap_box *)ele;
  free(b->p);
  b->p = NULL;
  intg_heap_dtor_n++;
}

UTEST_CASE(integration)
{
  {
    struct queue q;
    int x;
    size_t idx;
    void *p;
    int out;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    for (idx = 0; idx < 5; idx++) {
      x = (int)idx;
      EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    }
    EXPECT_EQ_UINT(queue_size(&q), 5);
    EXPECT_FALSE(queue_empty(&q));
    for (idx = 0; idx < 5; idx++) {
      p = queue_peek(&q);
      EXPECT_NOTNULL(p);
      EXPECT_EQ_INT(*(int *)p, (int)idx);
      x = 0;
      EXPECT_EQ_INT(queue_deq(&q, &x), 0);
      EXPECT_EQ_INT(x, (int)idx);
    }
    EXPECT_TRUE(queue_empty(&q));
    EXPECT_NULL(queue_peek(&q));
    out = 0;
    EXPECT_EQ_INT(queue_deq(&q, &out), -1);
    queue_fini(&q);
  }

  {
    struct queue q;
    int x;
    int out;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    x = 1;
    queue_enq(&q, &x);
    x = 2;
    queue_enq(&q, &x);
    x = 3;
    queue_enq(&q, &x);
    EXPECT_EQ_INT(*(int *)queue_peek(&q), 1);
    out = 0;
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(*(int *)queue_peek(&q), 2);
    x = 4;
    EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    EXPECT_EQ_INT(*(int *)queue_peek(&q), 2);
    out = 0;
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out, 2);
    out = 0;
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out, 3);
    out = 0;
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out, 4);
    EXPECT_TRUE(queue_empty(&q));
    queue_fini(&q);
  }

  {
    struct queue q;
    int x;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    queue_clear(&q);
    EXPECT_TRUE(queue_empty(&q));
    x = 10;
    queue_enq(&q, &x);
    x = 20;
    EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    EXPECT_EQ_INT(*(int *)queue_peek(&q), 10);
    EXPECT_EQ_INT(queue_deq(&q, NULL), 0);
    EXPECT_EQ_UINT(queue_size(&q), 1);
    x = 30;
    EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    EXPECT_EQ_UINT(queue_size(&q), 2);
    EXPECT_EQ_INT(*(int *)queue_peek(&q), 20);
    EXPECT_NOTNULL(queue_peek(&q));
    queue_fini(&q);
  }

  {
    struct queue q;
    unsigned char a, b, c, out;

    EXPECT_EQ_INT(queue_init(&q, sizeof(unsigned char), NULL), 0);
    a = 7;
    b = 8;
    c = 9;
    EXPECT_EQ_INT(queue_enq(&q, &a), 0);
    EXPECT_EQ_INT(queue_enq(&q, &b), 0);
    EXPECT_EQ_INT(queue_enq(&q, &c), 0);
    EXPECT_EQ_UINT(queue_size(&q), 3);
    out = 0;
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT((int)out, 7);
    EXPECT_EQ_INT((int)*(unsigned char *)queue_peek(&q), 8);
    out = 0;
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT((int)out, 8);
    EXPECT_EQ_INT((int)*(unsigned char *)queue_peek(&q), 9);
    queue_fini(&q);
  }

  {
    struct queue q;
    int x;
    size_t idx;

    intg_dtor_n = 0;
    EXPECT_EQ_INT(queue_init(&q, sizeof(int), intg_dtor_inc), 0);
    for (idx = 0; idx < 4; idx++) {
      x = (int)(idx + 10);
      queue_enq(&q, &x);
    }
    EXPECT_EQ_INT(queue_deq(&q, NULL), 0);
    EXPECT_EQ_INT(queue_deq(&q, NULL), 0);
    EXPECT_EQ_INT(intg_dtor_n, 2);
    EXPECT_EQ_INT(*(int *)queue_peek(&q), 12);
    queue_clear(&q);
    EXPECT_EQ_INT(intg_dtor_n, 4);
    EXPECT_TRUE(queue_empty(&q));
    x = 1;
    queue_enq(&q, &x);
    EXPECT_EQ_INT(queue_deq(&q, NULL), 0);
    EXPECT_EQ_INT(intg_dtor_n, 5);
    queue_fini(&q);
    EXPECT_EQ_INT(intg_dtor_n, 5);
  }

  {
    struct queue q;
    int x;
    int out;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    x = 1;
    queue_enq(&q, &x);
    x = 2;
    queue_enq(&q, &x);
    x = 3;
    queue_enq(&q, &x);
    queue_clear(&q);
    EXPECT_TRUE(queue_empty(&q));
    x = 7;
    queue_enq(&q, &x);
    EXPECT_EQ_UINT(queue_size(&q), 1);
    EXPECT_EQ_INT(*(int *)queue_peek(&q), 7);
    x = 8;
    EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    EXPECT_EQ_INT(*(int *)queue_peek(&q), 7);
    out = 0;
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out, 7);
    out = 0;
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out, 8);
    EXPECT_TRUE(queue_empty(&q));
    queue_fini(&q);
  }

  {
    struct queue q;
    int x;
    int out;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    x = 1;
    queue_enq(&q, &x);
    x = 2;
    queue_enq(&q, &x);
    x = 3;
    queue_enq(&q, &x);
    out = 0;
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_TRUE(queue_empty(&q));
    x = 100;
    queue_enq(&q, &x);
    x = 200;
    queue_enq(&q, &x);
    EXPECT_EQ_INT(*(int *)queue_peek(&q), 100);
    out = 0;
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out, 100);
    EXPECT_EQ_INT(*(int *)queue_peek(&q), 200);
    queue_fini(&q);
  }

  {
    struct queue q;
    int x;
    size_t idx;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    for (idx = 0; idx < 3; idx++) {
      x = (int)(3 - idx);
      EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    }
    EXPECT_EQ_UINT(queue_size(&q), 3);
    for (idx = 0; idx < 3; idx++) {
      EXPECT_NOTNULL(queue_peek(&q));
      EXPECT_EQ_INT(*(int *)queue_peek(&q), (int)(3 - idx));
      EXPECT_EQ_INT(queue_deq(&q, NULL), 0);
    }
    EXPECT_TRUE(queue_empty(&q));
    queue_fini(&q);
  }

  {
    struct queue q;
    int x;
    size_t idx;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    for (idx = 0; idx < 5; idx++) {
      x = (int)(idx * 2);
      queue_enq(&q, &x);
    }
    x = 1000;
    queue_enq(&q, &x);
    x = 2000;
    queue_enq(&q, &x);
    EXPECT_EQ_UINT(queue_size(&q), 7);
    EXPECT_EQ_INT(*(int *)queue_peek(&q), 0);
    EXPECT_EQ_INT(queue_deq(&q, NULL), 0);
    EXPECT_EQ_INT(*(int *)queue_peek(&q), 2);
    EXPECT_EQ_INT(queue_deq(&q, NULL), 0);
    EXPECT_EQ_INT(*(int *)queue_peek(&q), 4);
    EXPECT_EQ_UINT(queue_size(&q), 5);
    queue_clear(&q);
    EXPECT_TRUE(queue_empty(&q));
    x = 9;
    queue_enq(&q, &x);
    EXPECT_EQ_UINT(queue_size(&q), 1);
    queue_fini(&q);
  }

  {
    struct queue q;
    int x;
    size_t idx;
    void *p;
    int out;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    for (idx = 0; idx < 40; idx++) {
      x = (int)idx;
      EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    }
    EXPECT_EQ_UINT(queue_size(&q), 40);
    for (idx = 0; idx < 40; idx++) {
      p = queue_peek(&q);
      EXPECT_NOTNULL(p);
      EXPECT_EQ_INT(*(int *)p, (int)idx);
      out = -1;
      EXPECT_EQ_INT(queue_deq(&q, &out), 0);
      EXPECT_EQ_INT(out, (int)idx);
    }
    EXPECT_TRUE(queue_empty(&q));
    queue_fini(&q);
  }

  {
    struct queue q;
    int x;
    size_t idx;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), NULL), 0);
    for (idx = 0; idx < 25; idx++) {
      x = (int)idx;
      EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    }
    for (idx = 0; idx < 10; idx++) {
      x = 0;
      EXPECT_EQ_INT(queue_deq(&q, &x), 0);
      EXPECT_EQ_INT(x, (int)idx);
    }
    EXPECT_EQ_UINT(queue_size(&q), 15);
    EXPECT_EQ_INT(*(int *)queue_peek(&q), 10);
    for (idx = 0; idx < 10; idx++) {
      x = -(int)(idx + 1);
      EXPECT_EQ_INT(queue_enq(&q, &x), 0);
    }
    EXPECT_EQ_UINT(queue_size(&q), 25);
    EXPECT_EQ_INT(*(int *)queue_peek(&q), 10);
    x = 0;
    EXPECT_EQ_INT(queue_deq(&q, &x), 0);
    EXPECT_EQ_INT(x, 10);
    for (idx = 0; idx < 8; idx++)
      EXPECT_EQ_INT(queue_deq(&q, NULL), 0);
    EXPECT_EQ_UINT(queue_size(&q), 16);
    EXPECT_EQ_INT(*(int *)queue_peek(&q), 19);
    x = 0;
    EXPECT_EQ_INT(queue_deq(&q, &x), 0);
    EXPECT_EQ_INT(x, 19);
    queue_clear(&q);
    EXPECT_TRUE(queue_empty(&q));
    queue_fini(&q);
  }

  {
    struct queue q;
    struct intg_pair a, b, c, out;

    EXPECT_EQ_INT(queue_init(&q, sizeof(struct intg_pair), NULL), 0);
    a.x = 1;
    a.y = 10;
    b.x = 2;
    b.y = 20;
    c.x = 3;
    c.y = 30;
    EXPECT_EQ_INT(queue_enq(&q, &a), 0);
    EXPECT_EQ_INT(queue_enq(&q, &b), 0);
    EXPECT_EQ_INT(queue_enq(&q, &c), 0);
    EXPECT_NOTNULL(queue_peek(&q));
    EXPECT_EQ_INT(((struct intg_pair *)queue_peek(&q))->x, 1);
    EXPECT_EQ_INT(((struct intg_pair *)queue_peek(&q))->y, 10);
    memset(&out, 0, sizeof(out));
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out.x, 1);
    EXPECT_EQ_INT(out.y, 10);
    EXPECT_EQ_INT(((struct intg_pair *)queue_peek(&q))->x, 2);
    a.x = 0;
    a.y = 0;
    EXPECT_EQ_INT(queue_enq(&q, &a), 0);
    EXPECT_EQ_INT(((struct intg_pair *)queue_peek(&q))->y, 20);
    memset(&out, 0, sizeof(out));
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out.x, 2);
    EXPECT_EQ_INT(out.y, 20);
    memset(&out, 0, sizeof(out));
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out.x, 3);
    EXPECT_EQ_INT(out.y, 30);
    memset(&out, 0, sizeof(out));
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(out.x, 0);
    EXPECT_EQ_INT(out.y, 0);
    EXPECT_TRUE(queue_empty(&q));
    queue_fini(&q);
  }

  {
    struct queue q;
    struct intg_nested n0, n1;

    EXPECT_EQ_INT(queue_init(&q, sizeof(struct intg_nested), NULL), 0);
    n0.layer = 7;
    n0.id = 700;
    n1.layer = 8;
    n1.id = 800;
    EXPECT_EQ_INT(queue_enq(&q, &n0), 0);
    EXPECT_EQ_INT(queue_enq(&q, &n1), 0);
    EXPECT_NOTNULL(queue_peek(&q));
    EXPECT_EQ_INT((int)((struct intg_nested *)queue_peek(&q))->layer, 7);
    EXPECT_EQ_INT(((struct intg_nested *)queue_peek(&q))->id, 700);
    EXPECT_EQ_INT(queue_deq(&q, NULL), 0);
    EXPECT_NOTNULL(queue_peek(&q));
    EXPECT_EQ_INT((int)((struct intg_nested *)queue_peek(&q))->layer, 8);
    EXPECT_EQ_INT(((struct intg_nested *)queue_peek(&q))->id, 800);
    queue_fini(&q);
  }

  {
    struct queue q;
    double a, b, c, out;

    EXPECT_EQ_INT(queue_init(&q, sizeof(double), NULL), 0);
    a = 0.25;
    b = 0.5;
    c = 0.75;
    EXPECT_EQ_INT(queue_enq(&q, &a), 0);
    EXPECT_EQ_INT(queue_enq(&q, &b), 0);
    EXPECT_EQ_INT(queue_enq(&q, &c), 0);
    EXPECT_EQ_DOUBLE(*(double *)queue_peek(&q), 0.25);
    out = 0.0;
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_DOUBLE(out, 0.25);
    queue_clear(&q);
    EXPECT_TRUE(queue_empty(&q));
    a = 1.0;
    EXPECT_EQ_INT(queue_enq(&q, &a), 0);
    EXPECT_EQ_DOUBLE(*(double *)queue_peek(&q), 1.0);
    queue_fini(&q);
  }

  {
    struct queue q;
    struct intg_heap_box h;
    size_t idx;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(queue_init(&q, sizeof(struct intg_heap_box), intg_heap_dtor), 0);
    for (idx = 0; idx < 3; idx++) {
      h.p = (int *)malloc(sizeof(int));
      EXPECT_NOTNULL(h.p);
      *h.p = (int)(idx + 1);
      EXPECT_EQ_INT(queue_enq(&q, &h), 0);
    }
    queue_clear(&q);
    EXPECT_EQ_INT(intg_heap_dtor_n, 3);
    EXPECT_TRUE(queue_empty(&q));
    queue_fini(&q);
    EXPECT_EQ_INT(intg_heap_dtor_n, 3);
  }

  {
    struct queue q;
    struct intg_heap_box h, out;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(queue_init(&q, sizeof(struct intg_heap_box), intg_heap_dtor), 0);
    h.p = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(h.p);
    *h.p = 77;
    EXPECT_EQ_INT(queue_enq(&q, &h), 0);
    memset(&out, 0, sizeof(out));
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 0);
    EXPECT_NOTNULL(out.p);
    EXPECT_EQ_INT(*out.p, 77);
    free(out.p);
    out.p = NULL;
    EXPECT_TRUE(queue_empty(&q));
    queue_fini(&q);
    EXPECT_EQ_INT(intg_heap_dtor_n, 0);
  }

  {
    struct queue q;
    struct intg_heap_box h0, h1, out;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(queue_init(&q, sizeof(struct intg_heap_box), intg_heap_dtor), 0);
    h0.p = (int *)malloc(sizeof(int));
    h1.p = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(h0.p);
    EXPECT_NOTNULL(h1.p);
    *h0.p = 1;
    *h1.p = 2;
    queue_enq(&q, &h0);
    queue_enq(&q, &h1);
    memset(&out, 0, sizeof(out));
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 0);
    EXPECT_NOTNULL(out.p);
    EXPECT_EQ_INT(*out.p, 1);
    free(out.p);
    queue_fini(&q);
    EXPECT_EQ_INT(intg_heap_dtor_n, 1);
  }

  {
    struct queue q;
    struct intg_heap_box h;
    size_t idx;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(queue_init(&q, sizeof(struct intg_heap_box), intg_heap_dtor), 0);
    for (idx = 0; idx < 4; idx++) {
      h.p = (int *)malloc(sizeof(int));
      EXPECT_NOTNULL(h.p);
      *h.p = (int)(idx * 10);
      EXPECT_EQ_INT(queue_enq(&q, &h), 0);
    }
    EXPECT_EQ_INT(queue_deq(&q, NULL), 0);
    EXPECT_EQ_INT(queue_deq(&q, NULL), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 2);
    EXPECT_NOTNULL(queue_peek(&q));
    EXPECT_NOTNULL(((struct intg_heap_box *)queue_peek(&q))->p);
    EXPECT_EQ_INT(*((struct intg_heap_box *)queue_peek(&q))->p, 20);
    queue_fini(&q);
    EXPECT_EQ_INT(intg_heap_dtor_n, 4);
  }

  {
    struct queue q;
    struct intg_heap_box h;
    struct intg_heap_box *slot;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(queue_init(&q, sizeof(struct intg_heap_box), NULL), 0);
    h.p = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(h.p);
    *h.p = 99;
    EXPECT_EQ_INT(queue_enq(&q, &h), 0);
    slot = (struct intg_heap_box *)queue_peek(&q);
    EXPECT_NOTNULL(slot);
    EXPECT_NOTNULL(slot->p);
    free(slot->p);
    slot->p = NULL;
    queue_clear(&q);
    queue_fini(&q);
    EXPECT_EQ_INT(intg_heap_dtor_n, 0);
  }

  {
    struct queue q;
    struct intg_heap_box h0, h1, h2, out;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(queue_init(&q, sizeof(struct intg_heap_box), intg_heap_dtor), 0);
    h0.p = (int *)malloc(sizeof(int));
    h1.p = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(h0.p);
    EXPECT_NOTNULL(h1.p);
    *h0.p = 1;
    *h1.p = 2;
    queue_enq(&q, &h0);
    queue_enq(&q, &h1);
    memset(&out, 0, sizeof(out));
    EXPECT_EQ_INT(queue_deq(&q, &out), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 0);
    EXPECT_NOTNULL(out.p);
    EXPECT_EQ_INT(*out.p, 1);
    free(out.p);
    out.p = NULL;
    h2.p = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(h2.p);
    *h2.p = 50;
    EXPECT_EQ_INT(queue_enq(&q, &h2), 0);
    out.p = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(out.p);
    *out.p = 99;
    EXPECT_EQ_INT(queue_enq(&q, &out), 0);
    EXPECT_EQ_INT(*((struct intg_heap_box *)queue_peek(&q))->p, 2);
    EXPECT_EQ_INT(queue_deq(&q, NULL), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 1);
    EXPECT_EQ_INT(*((struct intg_heap_box *)queue_peek(&q))->p, 50);
    EXPECT_EQ_INT(queue_deq(&q, NULL), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 2);
    EXPECT_EQ_INT(*((struct intg_heap_box *)queue_peek(&q))->p, 99);
    EXPECT_EQ_INT(queue_deq(&q, NULL), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 3);
    EXPECT_TRUE(queue_empty(&q));
    queue_fini(&q);
    EXPECT_EQ_INT(intg_heap_dtor_n, 3);
  }

  {
    struct queue q;
    struct intg_heap_box h;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(queue_init(&q, sizeof(struct intg_heap_box), intg_heap_dtor), 0);
    h.p = (int *)malloc(sizeof(int));
    *h.p = 5;
    queue_enq(&q, &h);
    EXPECT_EQ_INT(queue_deq(&q, NULL), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 1);
    queue_fini(&q);
    EXPECT_EQ_INT(intg_heap_dtor_n, 1);
  }
}
