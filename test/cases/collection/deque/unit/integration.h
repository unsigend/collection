#include <deque.h>
#include <stdlib.h>
#include <string.h>
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
    struct deque d;
    int x;
    size_t idx;
    int out;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    for (idx = 0; idx < 5; idx++) {
      x = (int)idx;
      EXPECT_EQ_INT(deq_pushback(&d, &x), 0);
    }
    EXPECT_EQ_UINT(deq_size(&d), 5);
    for (idx = 0; idx < 5; idx++) {
      EXPECT_NOTNULL(deq_at(&d, idx));
      EXPECT_EQ_INT(*(int *)deq_at(&d, idx), (int)idx);
    }
    x = -1;
    EXPECT_EQ_INT(deq_insert(&d, 0, &x), 0);
    EXPECT_EQ_UINT(deq_size(&d), 6);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), -1);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 5), 4);
    out = 0;
    EXPECT_EQ_INT(deq_remove(&d, 5, &out), 0);
    EXPECT_EQ_INT(out, 4);
    EXPECT_EQ_UINT(deq_size(&d), 5);
    EXPECT_EQ_INT(*(int *)deq_back(&d), 3);
    EXPECT_EQ_INT(deq_remove(&d, 0, NULL), 0);
    EXPECT_EQ_UINT(deq_size(&d), 4);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 0);
    x = 99;
    EXPECT_EQ_INT(deq_insert(&d, 2, &x), 0);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 2), 99);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 3), 2);
    EXPECT_EQ_INT(deq_popback(&d, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_EQ_UINT(deq_size(&d), 4);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;
    size_t idx;
    void *p;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(deq_resize(&d, 8), 0);
    for (idx = 0; idx < 8; idx++) {
      p = deq_at(&d, idx);
      EXPECT_NOTNULL(p);
      x = (int)(idx * 10);
      memcpy(p, &x, sizeof(int));
    }
    EXPECT_EQ_INT(deq_resize(&d, 4), 0);
    EXPECT_EQ_UINT(deq_size(&d), 4);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 3), 30);
    EXPECT_EQ_INT(deq_resize(&d, 6), 0);
    EXPECT_EQ_UINT(deq_size(&d), 6);
    EXPECT_GE_UINT(deq_capacity(&d), deq_size(&d));
    p = deq_at(&d, 4);
    EXPECT_NOTNULL(p);
    x = 404;
    memcpy(p, &x, sizeof(int));
    p = deq_at(&d, 5);
    EXPECT_NOTNULL(p);
    x = 505;
    memcpy(p, &x, sizeof(int));
    for (idx = 0; idx < 4; idx++)
      EXPECT_EQ_INT(*(int *)deq_at(&d, idx), (int)(idx * 10));
    EXPECT_EQ_INT(*(int *)deq_at(&d, 4), 404);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 5), 505);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;
    size_t idx;
    size_t cap_a, cap_b;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    for (idx = 0; idx < 5; idx++) {
      x = (int)(idx + 1);
      EXPECT_EQ_INT(deq_pushback(&d, &x), 0);
    }
    cap_a = deq_capacity(&d);
    EXPECT_EQ_INT(deq_shrink(&d), 0);
    cap_b = deq_capacity(&d);
    EXPECT_GE_UINT(cap_b, deq_size(&d));
    EXPECT_LE_UINT(cap_b, cap_a);
    for (idx = 0; idx < 5; idx++)
      EXPECT_EQ_INT(*(int *)deq_at(&d, idx), (int)(idx + 1));
    for (idx = 0; idx < 3; idx++) {
      x = (int)(100 + idx);
      EXPECT_EQ_INT(deq_pushback(&d, &x), 0);
    }
    EXPECT_EQ_UINT(deq_size(&d), 8);
    EXPECT_GE_UINT(deq_capacity(&d), deq_size(&d));
    EXPECT_EQ_INT(*(int *)deq_at(&d, 4), 5);
    EXPECT_EQ_INT(*(int *)deq_back(&d), 102);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    x = 1;
    deq_pushback(&d, &x);
    x = 2;
    deq_pushback(&d, &x);
    x = 3;
    deq_pushback(&d, &x);
    EXPECT_EQ_INT(deq_remove(&d, 0, NULL), 0);
    EXPECT_EQ_UINT(deq_size(&d), 2);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 2);
    x = 4;
    deq_pushback(&d, &x);
    EXPECT_EQ_INT(deq_remove(&d, 0, NULL), 0);
    EXPECT_EQ_UINT(deq_size(&d), 2);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 3);
    EXPECT_EQ_INT(*(int *)deq_back(&d), 4);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;
    size_t idx;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    deq_clear(&d);
    EXPECT_TRUE(deq_empty(&d));
    EXPECT_EQ_INT(deq_shrink(&d), 0);
    x = 10;
    deq_pushback(&d, &x);
    x = 20;
    EXPECT_EQ_INT(deq_insert(&d, 0, &x), 0);
    EXPECT_EQ_INT(*(int *)deq_front(&d), 20);
    EXPECT_EQ_INT(*(int *)deq_back(&d), 10);
    EXPECT_EQ_INT(deq_popback(&d, NULL), 0);
    EXPECT_EQ_UINT(deq_size(&d), 1);
    x = 30;
    EXPECT_EQ_INT(deq_insert(&d, 1, &x), 0);
    EXPECT_EQ_UINT(deq_size(&d), 2);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 1), 30);
    for (idx = 0; idx < 2; idx++)
      EXPECT_NOTNULL(deq_at(&d, idx));
    deq_fini(&d);
  }

  {
    struct deque d;
    unsigned char a, b, c, out;

    EXPECT_EQ_INT(deq_init(&d, sizeof(unsigned char), NULL), 0);
    a = 7;
    b = 8;
    c = 9;
    EXPECT_EQ_INT(deq_pushback(&d, &a), 0);
    EXPECT_EQ_INT(deq_pushback(&d, &b), 0);
    c = 5;
    EXPECT_EQ_INT(deq_insert(&d, 1, &c), 0);
    EXPECT_EQ_UINT(deq_size(&d), 3);
    EXPECT_EQ_INT((int)*(unsigned char *)deq_at(&d, 0), 7);
    EXPECT_EQ_INT((int)*(unsigned char *)deq_at(&d, 1), 5);
    EXPECT_EQ_INT((int)*(unsigned char *)deq_at(&d, 2), 8);
    out = 0;
    EXPECT_EQ_INT(deq_remove(&d, 1, &out), 0);
    EXPECT_EQ_INT((int)out, 5);
    EXPECT_EQ_UINT(deq_size(&d), 2);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;
    size_t idx;

    intg_dtor_n = 0;
    EXPECT_EQ_INT(deq_init(&d, sizeof(int), intg_dtor_inc), 0);
    for (idx = 0; idx < 4; idx++) {
      x = (int)(idx + 10);
      deq_pushback(&d, &x);
    }
    EXPECT_EQ_INT(deq_resize(&d, 2), 0);
    EXPECT_EQ_INT(intg_dtor_n, 2);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 10);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 1), 11);
    deq_clear(&d);
    EXPECT_EQ_INT(intg_dtor_n, 4);
    EXPECT_TRUE(deq_empty(&d));
    x = 1;
    deq_pushback(&d, &x);
    EXPECT_EQ_INT(deq_popback(&d, NULL), 0);
    EXPECT_EQ_INT(intg_dtor_n, 5);
    deq_fini(&d);
    EXPECT_EQ_INT(intg_dtor_n, 5);
  }

  {
    struct deque d;
    int x;
    int out;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    x = 1;
    deq_pushback(&d, &x);
    x = 2;
    deq_pushback(&d, &x);
    x = 3;
    deq_pushback(&d, &x);
    EXPECT_EQ_INT(deq_resize(&d, 0), 0);
    EXPECT_TRUE(deq_empty(&d));
    x = 7;
    deq_pushback(&d, &x);
    EXPECT_EQ_UINT(deq_size(&d), 1);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 7);
    x = 8;
    EXPECT_EQ_INT(deq_insert(&d, 0, &x), 0);
    out = 0;
    EXPECT_EQ_INT(deq_remove(&d, 1, &out), 0);
    EXPECT_EQ_INT(out, 7);
    EXPECT_EQ_UINT(deq_size(&d), 1);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 8);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;
    size_t idx;
    int out;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    for (idx = 0; idx < 3; idx++) {
      x = (int)(idx + 1);
      deq_pushback(&d, &x);
    }
    out = 0;
    EXPECT_EQ_INT(deq_popback(&d, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_EQ_INT(deq_popback(&d, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(deq_popback(&d, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_TRUE(deq_empty(&d));
    x = 100;
    deq_pushback(&d, &x);
    x = 200;
    EXPECT_EQ_INT(deq_insert(&d, 0, &x), 0);
    EXPECT_EQ_INT(*(int *)deq_front(&d), 200);
    EXPECT_EQ_INT(*(int *)deq_back(&d), 100);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;
    size_t idx;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    x = 3;
    EXPECT_EQ_INT(deq_insert(&d, 0, &x), 0);
    x = 2;
    EXPECT_EQ_INT(deq_insert(&d, 0, &x), 0);
    x = 1;
    EXPECT_EQ_INT(deq_insert(&d, 0, &x), 0);
    EXPECT_EQ_UINT(deq_size(&d), 3);
    for (idx = 0; idx < 3; idx++) {
      EXPECT_NOTNULL(deq_at(&d, idx));
      EXPECT_EQ_INT(*(int *)deq_at(&d, idx), (int)(idx + 1));
    }
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;
    size_t idx;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    for (idx = 0; idx < 5; idx++) {
      x = (int)(idx * 2);
      deq_pushback(&d, &x);
    }
    x = 1000;
    EXPECT_EQ_INT(deq_insert(&d, 2, &x), 0);
    x = 2000;
    EXPECT_EQ_INT(deq_insert(&d, 6, &x), 0);
    EXPECT_EQ_UINT(deq_size(&d), 7);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 2), 1000);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 3), 4);
    EXPECT_EQ_INT(*(int *)deq_back(&d), 2000);
    EXPECT_EQ_INT(deq_remove(&d, 0, NULL), 0);
    EXPECT_EQ_INT(deq_remove(&d, 5, NULL), 0);
    EXPECT_EQ_UINT(deq_size(&d), 5);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 2);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 1), 1000);
    EXPECT_EQ_INT(*(int *)deq_back(&d), 8);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;
    size_t idx;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    for (idx = 0; idx < 6; idx++) {
      x = (int)idx;
      deq_pushback(&d, &x);
    }
    EXPECT_EQ_INT(deq_resize(&d, 3), 0);
    EXPECT_EQ_UINT(deq_size(&d), 3);
    x = 30;
    deq_pushback(&d, &x);
    x = 40;
    EXPECT_EQ_INT(deq_insert(&d, 1, &x), 0);
    EXPECT_EQ_UINT(deq_size(&d), 5);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 0);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 1), 40);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 2), 1);
    EXPECT_EQ_INT(*(int *)deq_back(&d), 30);
    EXPECT_EQ_INT(deq_shrink(&d), 0);
    EXPECT_GE_UINT(deq_capacity(&d), deq_size(&d));
    deq_clear(&d);
    EXPECT_TRUE(deq_empty(&d));
    x = 9;
    deq_pushback(&d, &x);
    EXPECT_EQ_UINT(deq_size(&d), 1);
    deq_fini(&d);
  }

  {
    struct deque d;
    struct intg_pair a, b, c, out;

    EXPECT_EQ_INT(deq_init(&d, sizeof(struct intg_pair), NULL), 0);
    a.x = 1;
    a.y = 10;
    b.x = 2;
    b.y = 20;
    c.x = 3;
    c.y = 30;
    EXPECT_EQ_INT(deq_pushback(&d, &a), 0);
    EXPECT_EQ_INT(deq_pushback(&d, &b), 0);
    EXPECT_EQ_INT(deq_pushback(&d, &c), 0);
    EXPECT_NOTNULL(deq_at(&d, 1));
    EXPECT_EQ_INT(((struct intg_pair *)deq_at(&d, 1))->x, 2);
    EXPECT_EQ_INT(((struct intg_pair *)deq_at(&d, 1))->y, 20);
    a.x = 0;
    a.y = 0;
    EXPECT_EQ_INT(deq_insert(&d, 1, &a), 0);
    EXPECT_EQ_INT(((struct intg_pair *)deq_at(&d, 1))->x, 0);
    EXPECT_EQ_INT(((struct intg_pair *)deq_at(&d, 2))->x, 2);
    EXPECT_EQ_INT(deq_remove(&d, 3, &out), 0);
    EXPECT_EQ_INT(out.x, 3);
    EXPECT_EQ_INT(out.y, 30);
    EXPECT_EQ_UINT(deq_size(&d), 3);
    deq_fini(&d);
  }

  {
    struct deque d;
    struct intg_nested n0, n1;

    EXPECT_EQ_INT(deq_init(&d, sizeof(struct intg_nested), NULL), 0);
    n0.layer = 7;
    n0.id = 700;
    n1.layer = 8;
    n1.id = 800;
    EXPECT_EQ_INT(deq_pushback(&d, &n0), 0);
    EXPECT_EQ_INT(deq_pushback(&d, &n1), 0);
    EXPECT_NOTNULL(deq_at(&d, 0));
    EXPECT_EQ_INT((int)((struct intg_nested *)deq_at(&d, 0))->layer, 7);
    EXPECT_EQ_INT(((struct intg_nested *)deq_at(&d, 0))->id, 700);
    EXPECT_EQ_INT((int)((struct intg_nested *)deq_at(&d, 1))->layer, 8);
    EXPECT_EQ_INT(((struct intg_nested *)deq_back(&d))->id, 800);
    deq_fini(&d);
  }

  {
    struct deque d;
    struct intg_heap_box h;
    size_t idx;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(deq_init(&d, sizeof(struct intg_heap_box), intg_heap_dtor),
                  0);
    for (idx = 0; idx < 3; idx++) {
      h.p = (int *)malloc(sizeof(int));
      EXPECT_NOTNULL(h.p);
      *h.p = (int)(idx + 1);
      EXPECT_EQ_INT(deq_pushback(&d, &h), 0);
    }
    deq_clear(&d);
    EXPECT_EQ_INT(intg_heap_dtor_n, 3);
    EXPECT_TRUE(deq_empty(&d));
    deq_fini(&d);
    EXPECT_EQ_INT(intg_heap_dtor_n, 3);
  }

  {
    struct deque d;
    struct intg_heap_box h, out;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(deq_init(&d, sizeof(struct intg_heap_box), intg_heap_dtor),
                  0);
    h.p = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(h.p);
    *h.p = 77;
    EXPECT_EQ_INT(deq_pushback(&d, &h), 0);
    memset(&out, 0, sizeof(out));
    EXPECT_EQ_INT(deq_popback(&d, &out), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 0);
    EXPECT_NOTNULL(out.p);
    EXPECT_EQ_INT(*out.p, 77);
    free(out.p);
    out.p = NULL;
    EXPECT_TRUE(deq_empty(&d));
    deq_fini(&d);
    EXPECT_EQ_INT(intg_heap_dtor_n, 0);
  }

  {
    struct deque d;
    struct intg_heap_box h0, h1, out;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(deq_init(&d, sizeof(struct intg_heap_box), intg_heap_dtor),
                  0);
    h0.p = (int *)malloc(sizeof(int));
    h1.p = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(h0.p);
    EXPECT_NOTNULL(h1.p);
    *h0.p = 1;
    *h1.p = 2;
    deq_pushback(&d, &h0);
    deq_pushback(&d, &h1);
    memset(&out, 0, sizeof(out));
    EXPECT_EQ_INT(deq_remove(&d, 0, &out), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 0);
    EXPECT_NOTNULL(out.p);
    EXPECT_EQ_INT(*out.p, 1);
    free(out.p);
    deq_fini(&d);
    EXPECT_EQ_INT(intg_heap_dtor_n, 1);
  }

  {
    struct deque d;
    struct intg_heap_box h;
    size_t idx;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(deq_init(&d, sizeof(struct intg_heap_box), intg_heap_dtor),
                  0);
    for (idx = 0; idx < 4; idx++) {
      h.p = (int *)malloc(sizeof(int));
      EXPECT_NOTNULL(h.p);
      *h.p = (int)(idx * 10);
      EXPECT_EQ_INT(deq_pushback(&d, &h), 0);
    }
    EXPECT_EQ_INT(deq_resize(&d, 2), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 2);
    EXPECT_NOTNULL(deq_at(&d, 0));
    EXPECT_NOTNULL(((struct intg_heap_box *)deq_at(&d, 0))->p);
    EXPECT_EQ_INT(*((struct intg_heap_box *)deq_at(&d, 0))->p, 0);
    EXPECT_EQ_INT(*((struct intg_heap_box *)deq_at(&d, 1))->p, 10);
    deq_fini(&d);
    EXPECT_EQ_INT(intg_heap_dtor_n, 4);
  }

  {
    struct deque d;
    struct intg_heap_box h;
    struct intg_heap_box *slot;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(deq_init(&d, sizeof(struct intg_heap_box), NULL), 0);
    h.p = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(h.p);
    *h.p = 99;
    EXPECT_EQ_INT(deq_pushback(&d, &h), 0);
    EXPECT_NOTNULL(deq_at(&d, 0));
    slot = (struct intg_heap_box *)deq_at(&d, 0);
    EXPECT_NOTNULL(slot->p);
    free(slot->p);
    slot->p = NULL;
    deq_clear(&d);
    deq_fini(&d);
    EXPECT_EQ_INT(intg_heap_dtor_n, 0);
  }

  {
    struct deque d;
    struct intg_heap_box h0, h1, h2;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(deq_init(&d, sizeof(struct intg_heap_box), intg_heap_dtor),
                  0);
    h0.p = (int *)malloc(sizeof(int));
    h1.p = (int *)malloc(sizeof(int));
    *h0.p = 1;
    *h1.p = 2;
    deq_pushback(&d, &h0);
    deq_pushback(&d, &h1);
    h2.p = (int *)malloc(sizeof(int));
    *h2.p = 50;
    EXPECT_EQ_INT(deq_insert(&d, 1, &h2), 0);
    EXPECT_EQ_INT(*((struct intg_heap_box *)deq_at(&d, 0))->p, 1);
    EXPECT_EQ_INT(*((struct intg_heap_box *)deq_at(&d, 1))->p, 50);
    EXPECT_EQ_INT(*((struct intg_heap_box *)deq_at(&d, 2))->p, 2);
    EXPECT_EQ_INT(deq_remove(&d, 1, NULL), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 1);
    deq_fini(&d);
    EXPECT_EQ_INT(intg_heap_dtor_n, 3);
  }

  {
    struct deque d;
    struct intg_heap_box h;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(deq_init(&d, sizeof(struct intg_heap_box), intg_heap_dtor),
                  0);
    h.p = (int *)malloc(sizeof(int));
    *h.p = 5;
    deq_pushback(&d, &h);
    EXPECT_EQ_INT(deq_popback(&d, NULL), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 1);
    deq_fini(&d);
    EXPECT_EQ_INT(intg_heap_dtor_n, 1);
  }

  {
    struct deque d;
    int a, b, c, out;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    a = 1;
    b = 2;
    c = 3;
    deq_pushback(&d, &a);
    deq_pushback(&d, &b);
    deq_pushback(&d, &c);
    out = 0;
    EXPECT_EQ_INT(deq_popfront(&d, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(*(int *)deq_front(&d), 2);
    EXPECT_EQ_INT(*(int *)deq_back(&d), 3);
    deq_pushfront(&d, &out);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), 1);
    EXPECT_EQ_INT(*(int *)deq_back(&d), 3);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x;
    size_t idx;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    for (idx = 0; idx < 4; idx++) {
      x = (int)(idx + 1);
      EXPECT_EQ_INT(deq_pushfront(&d, &x), 0);
    }
    EXPECT_EQ_UINT(deq_size(&d), 4);
    for (idx = 0; idx < 4; idx++)
      EXPECT_EQ_INT(*(int *)deq_at(&d, idx), (int)(4 - idx));
    x = 0;
    EXPECT_EQ_INT(deq_pushback(&d, &x), 0);
    EXPECT_EQ_INT(*(int *)deq_back(&d), 0);
    EXPECT_EQ_INT(*(int *)deq_front(&d), 4);
    deq_fini(&d);
  }

  {
    struct deque d;
    int x, out;
    size_t idx;

    EXPECT_EQ_INT(deq_init(&d, sizeof(int), NULL), 0);
    for (idx = 0; idx < 25; idx++) {
      x = (int)idx;
      EXPECT_EQ_INT(deq_pushback(&d, &x), 0);
    }
    for (idx = 0; idx < 25; idx++) {
      x = -(int)(idx + 1);
      EXPECT_EQ_INT(deq_pushfront(&d, &x), 0);
    }
    EXPECT_EQ_UINT(deq_size(&d), 50);
    EXPECT_GE_UINT(deq_capacity(&d), deq_size(&d));
    EXPECT_EQ_INT(*(int *)deq_front(&d), -25);
    EXPECT_EQ_INT(*(int *)deq_back(&d), 24);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 0), -25);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 24), -1);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 25), 0);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 49), 24);
    for (idx = 0; idx < 10; idx++)
      EXPECT_EQ_INT(deq_popfront(&d, NULL), 0);
    for (idx = 0; idx < 10; idx++)
      EXPECT_EQ_INT(deq_popback(&d, NULL), 0);
    EXPECT_EQ_UINT(deq_size(&d), 30);
    EXPECT_EQ_INT(*(int *)deq_front(&d), -15);
    EXPECT_EQ_INT(*(int *)deq_back(&d), 14);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 15), 0);
    x = 7777;
    EXPECT_EQ_INT(deq_insert(&d, 15, &x), 0);
    EXPECT_EQ_UINT(deq_size(&d), 31);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 15), 7777);
    EXPECT_EQ_INT(*(int *)deq_at(&d, 16), 0);
    out = 0;
    EXPECT_EQ_INT(deq_remove(&d, 0, &out), 0);
    EXPECT_EQ_INT(out, -15);
    EXPECT_EQ_UINT(deq_size(&d), 30);
    EXPECT_EQ_INT(*(int *)deq_front(&d), -14);
    EXPECT_EQ_INT(*(int *)deq_back(&d), 14);
    deq_clear(&d);
    EXPECT_TRUE(deq_empty(&d));
    deq_fini(&d);
  }

  {
    struct deque d;
    int x, out;
    size_t idx;
    void *p;

    intg_dtor_n = 0;
    EXPECT_EQ_INT(deq_init(&d, sizeof(int), intg_dtor_inc), 0);
    EXPECT_EQ_INT(deq_resize(&d, 16), 0);
    for (idx = 0; idx < 16; idx++) {
      p = deq_at(&d, idx);
      EXPECT_NOTNULL(p);
      x = (int)idx;
      memcpy(p, &x, sizeof(int));
    }
    EXPECT_EQ_INT(deq_shrink(&d), 0);
    EXPECT_EQ_UINT(deq_size(&d), 16);
    EXPECT_EQ_UINT(deq_capacity(&d), deq_size(&d));
    for (idx = 0; idx < 8; idx++) {
      out = -1;
      EXPECT_EQ_INT(deq_popfront(&d, &out), 0);
      EXPECT_EQ_INT(out, (int)idx);
    }
    for (idx = 0; idx < 8; idx++) {
      x = (int)(100 + idx);
      EXPECT_EQ_INT(deq_pushback(&d, &x), 0);
    }
    EXPECT_EQ_UINT(deq_size(&d), 16);
    EXPECT_EQ_UINT(deq_capacity(&d), deq_size(&d));
    for (idx = 0; idx < 8; idx++)
      EXPECT_EQ_INT(*(int *)deq_at(&d, idx), (int)(idx + 8));
    for (idx = 0; idx < 8; idx++)
      EXPECT_EQ_INT(*(int *)deq_at(&d, 8 + idx), (int)(100 + idx));
    EXPECT_EQ_INT(intg_dtor_n, 0);
    deq_fini(&d);
    EXPECT_EQ_INT(intg_dtor_n, 16);
  }
}
