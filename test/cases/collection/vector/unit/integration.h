#include <utest.h>
#include <stdlib.h>
#include <string.h>
#include <vector.h>

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
    struct vector v;
    int x;
    size_t idx;
    int out;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    for (idx = 0; idx < 5; idx++) {
      x = (int)idx;
      EXPECT_EQ_INT(vec_pushback(&v, &x), 0);
    }
    EXPECT_EQ_UINT(vec_size(&v), 5u);
    for (idx = 0; idx < 5; idx++) {
      EXPECT_NOTNULL(vec_at(&v, idx));
      EXPECT_EQ_INT(*(int *)vec_at(&v, idx), (int)idx);
    }
    x = -1;
    EXPECT_EQ_INT(vec_insert(&v, 0u, &x), 0);
    EXPECT_EQ_UINT(vec_size(&v), 6u);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 0u), -1);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 5u), 4);
    out = 0;
    EXPECT_EQ_INT(vec_remove(&v, 5u, &out), 0);
    EXPECT_EQ_INT(out, 4);
    EXPECT_EQ_UINT(vec_size(&v), 5u);
    EXPECT_EQ_INT(*(int *)vec_back(&v), 3);
    EXPECT_EQ_INT(vec_remove(&v, 0u, NULL), 0);
    EXPECT_EQ_UINT(vec_size(&v), 4u);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 0u), 0);
    x = 99;
    EXPECT_EQ_INT(vec_insert(&v, 2u, &x), 0);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 2u), 99);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 3u), 2);
    EXPECT_EQ_INT(vec_popback(&v, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_EQ_UINT(vec_size(&v), 4u);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;
    size_t idx;
    void *p;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    EXPECT_EQ_INT(vec_resize(&v, 8u), 0);
    for (idx = 0; idx < 8; idx++) {
      p = vec_at(&v, idx);
      EXPECT_NOTNULL(p);
      x = (int)(idx * 10);
      memcpy(p, &x, sizeof(int));
    }
    EXPECT_EQ_INT(vec_resize(&v, 4u), 0);
    EXPECT_EQ_UINT(vec_size(&v), 4u);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 3u), 30);
    EXPECT_EQ_INT(vec_resize(&v, 6u), 0);
    EXPECT_EQ_UINT(vec_size(&v), 6u);
    EXPECT_GE_UINT(vec_capacity(&v), vec_size(&v));
    p = vec_at(&v, 4u);
    EXPECT_NOTNULL(p);
    x = 404;
    memcpy(p, &x, sizeof(int));
    p = vec_at(&v, 5u);
    EXPECT_NOTNULL(p);
    x = 505;
    memcpy(p, &x, sizeof(int));
    for (idx = 0; idx < 4; idx++)
      EXPECT_EQ_INT(*(int *)vec_at(&v, idx), (int)(idx * 10));
    EXPECT_EQ_INT(*(int *)vec_at(&v, 4u), 404);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 5u), 505);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;
    size_t idx;
    size_t cap_a, cap_b;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    for (idx = 0; idx < 5; idx++) {
      x = (int)(idx + 1);
      EXPECT_EQ_INT(vec_pushback(&v, &x), 0);
    }
    cap_a = vec_capacity(&v);
    EXPECT_EQ_INT(vec_shrink(&v), 0);
    cap_b = vec_capacity(&v);
    EXPECT_GE_UINT(cap_b, vec_size(&v));
    EXPECT_LE_UINT(cap_b, cap_a);
    for (idx = 0; idx < 5; idx++)
      EXPECT_EQ_INT(*(int *)vec_at(&v, idx), (int)(idx + 1));
    for (idx = 0; idx < 3; idx++) {
      x = (int)(100 + idx);
      EXPECT_EQ_INT(vec_pushback(&v, &x), 0);
    }
    EXPECT_EQ_UINT(vec_size(&v), 8u);
    EXPECT_GE_UINT(vec_capacity(&v), vec_size(&v));
    EXPECT_EQ_INT(*(int *)vec_at(&v, 4u), 5);
    EXPECT_EQ_INT(*(int *)vec_back(&v), 102);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    x = 1;
    vec_pushback(&v, &x);
    x = 2;
    vec_pushback(&v, &x);
    x = 3;
    vec_pushback(&v, &x);
    EXPECT_EQ_INT(vec_remove(&v, 0u, NULL), 0);
    EXPECT_EQ_UINT(vec_size(&v), 2u);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 0u), 2);
    x = 4;
    vec_pushback(&v, &x);
    EXPECT_EQ_INT(vec_remove(&v, 0u, NULL), 0);
    EXPECT_EQ_UINT(vec_size(&v), 2u);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 0u), 3);
    EXPECT_EQ_INT(*(int *)vec_back(&v), 4);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;
    size_t idx;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    vec_clear(&v);
    EXPECT_TRUE(vec_empty(&v));
    EXPECT_EQ_INT(vec_shrink(&v), 0);
    x = 10;
    vec_pushback(&v, &x);
    x = 20;
    EXPECT_EQ_INT(vec_insert(&v, 0u, &x), 0);
    EXPECT_EQ_INT(*(int *)vec_front(&v), 20);
    EXPECT_EQ_INT(*(int *)vec_back(&v), 10);
    EXPECT_EQ_INT(vec_popback(&v, NULL), 0);
    EXPECT_EQ_UINT(vec_size(&v), 1u);
    x = 30;
    EXPECT_EQ_INT(vec_insert(&v, 1u, &x), 0);
    EXPECT_EQ_UINT(vec_size(&v), 2u);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 1u), 30);
    for (idx = 0; idx < 2; idx++)
      EXPECT_NOTNULL(vec_at(&v, idx));
    vec_fini(&v);
  }

  {
    struct vector v;
    unsigned char a, b, c, out;

    EXPECT_EQ_INT(vec_init(&v, sizeof(unsigned char), NULL), 0);
    a = 7u;
    b = 8u;
    c = 9u;
    EXPECT_EQ_INT(vec_pushback(&v, &a), 0);
    EXPECT_EQ_INT(vec_pushback(&v, &b), 0);
    c = 5u;
    EXPECT_EQ_INT(vec_insert(&v, 1u, &c), 0);
    EXPECT_EQ_UINT(vec_size(&v), 3u);
    EXPECT_EQ_INT((int)*(unsigned char *)vec_at(&v, 0u), 7);
    EXPECT_EQ_INT((int)*(unsigned char *)vec_at(&v, 1u), 5);
    EXPECT_EQ_INT((int)*(unsigned char *)vec_at(&v, 2u), 8);
    out = 0u;
    EXPECT_EQ_INT(vec_remove(&v, 1u, &out), 0);
    EXPECT_EQ_INT((int)out, 5);
    EXPECT_EQ_UINT(vec_size(&v), 2u);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;
    size_t idx;

    intg_dtor_n = 0;
    EXPECT_EQ_INT(vec_init(&v, sizeof(int), intg_dtor_inc), 0);
    for (idx = 0; idx < 4; idx++) {
      x = (int)(idx + 10);
      vec_pushback(&v, &x);
    }
    EXPECT_EQ_INT(vec_resize(&v, 2u), 0);
    EXPECT_EQ_INT(intg_dtor_n, 2);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 0u), 10);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 1u), 11);
    vec_clear(&v);
    EXPECT_EQ_INT(intg_dtor_n, 4);
    EXPECT_TRUE(vec_empty(&v));
    x = 1;
    vec_pushback(&v, &x);
    EXPECT_EQ_INT(vec_popback(&v, NULL), 0);
    EXPECT_EQ_INT(intg_dtor_n, 5);
    vec_fini(&v);
    EXPECT_EQ_INT(intg_dtor_n, 5);
  }

  {
    struct vector v;
    int x;
    int out;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    x = 1;
    vec_pushback(&v, &x);
    x = 2;
    vec_pushback(&v, &x);
    x = 3;
    vec_pushback(&v, &x);
    EXPECT_EQ_INT(vec_resize(&v, 0u), 0);
    EXPECT_TRUE(vec_empty(&v));
    x = 7;
    vec_pushback(&v, &x);
    EXPECT_EQ_UINT(vec_size(&v), 1u);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 0u), 7);
    x = 8;
    EXPECT_EQ_INT(vec_insert(&v, 0u, &x), 0);
    out = 0;
    EXPECT_EQ_INT(vec_remove(&v, 1u, &out), 0);
    EXPECT_EQ_INT(out, 7);
    EXPECT_EQ_UINT(vec_size(&v), 1u);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 0u), 8);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;
    size_t idx;
    int out;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    for (idx = 0; idx < 3; idx++) {
      x = (int)(idx + 1);
      vec_pushback(&v, &x);
    }
    out = 0;
    EXPECT_EQ_INT(vec_popback(&v, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_EQ_INT(vec_popback(&v, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(vec_popback(&v, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_TRUE(vec_empty(&v));
    x = 100;
    vec_pushback(&v, &x);
    x = 200;
    EXPECT_EQ_INT(vec_insert(&v, 0u, &x), 0);
    EXPECT_EQ_INT(*(int *)vec_front(&v), 200);
    EXPECT_EQ_INT(*(int *)vec_back(&v), 100);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;
    size_t idx;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    x = 3;
    EXPECT_EQ_INT(vec_insert(&v, 0u, &x), 0);
    x = 2;
    EXPECT_EQ_INT(vec_insert(&v, 0u, &x), 0);
    x = 1;
    EXPECT_EQ_INT(vec_insert(&v, 0u, &x), 0);
    EXPECT_EQ_UINT(vec_size(&v), 3u);
    for (idx = 0; idx < 3; idx++) {
      EXPECT_NOTNULL(vec_at(&v, idx));
      EXPECT_EQ_INT(*(int *)vec_at(&v, idx), (int)(idx + 1));
    }
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;
    size_t idx;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    for (idx = 0; idx < 5; idx++) {
      x = (int)(idx * 2);
      vec_pushback(&v, &x);
    }
    x = 1000;
    EXPECT_EQ_INT(vec_insert(&v, 2u, &x), 0);
    x = 2000;
    EXPECT_EQ_INT(vec_insert(&v, 6u, &x), 0);
    EXPECT_EQ_UINT(vec_size(&v), 7u);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 2u), 1000);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 3u), 4);
    EXPECT_EQ_INT(*(int *)vec_back(&v), 2000);
    EXPECT_EQ_INT(vec_remove(&v, 0u, NULL), 0);
    EXPECT_EQ_INT(vec_remove(&v, 5u, NULL), 0);
    EXPECT_EQ_UINT(vec_size(&v), 5u);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 0u), 2);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 1u), 1000);
    EXPECT_EQ_INT(*(int *)vec_back(&v), 8);
    vec_fini(&v);
  }

  {
    struct vector v;
    int x;
    size_t idx;

    EXPECT_EQ_INT(vec_init(&v, sizeof(int), NULL), 0);
    for (idx = 0; idx < 6; idx++) {
      x = (int)idx;
      vec_pushback(&v, &x);
    }
    EXPECT_EQ_INT(vec_resize(&v, 3u), 0);
    EXPECT_EQ_UINT(vec_size(&v), 3u);
    x = 30;
    vec_pushback(&v, &x);
    x = 40;
    EXPECT_EQ_INT(vec_insert(&v, 1u, &x), 0);
    EXPECT_EQ_UINT(vec_size(&v), 5u);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 0u), 0);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 1u), 40);
    EXPECT_EQ_INT(*(int *)vec_at(&v, 2u), 1);
    EXPECT_EQ_INT(*(int *)vec_back(&v), 30);
    EXPECT_EQ_INT(vec_shrink(&v), 0);
    EXPECT_GE_UINT(vec_capacity(&v), vec_size(&v));
    vec_clear(&v);
    EXPECT_TRUE(vec_empty(&v));
    x = 9;
    vec_pushback(&v, &x);
    EXPECT_EQ_UINT(vec_size(&v), 1u);
    vec_fini(&v);
  }

  {
    struct vector v;
    struct intg_pair a, b, c, out;

    EXPECT_EQ_INT(vec_init(&v, sizeof(struct intg_pair), NULL), 0);
    a.x = 1;
    a.y = 10;
    b.x = 2;
    b.y = 20;
    c.x = 3;
    c.y = 30;
    EXPECT_EQ_INT(vec_pushback(&v, &a), 0);
    EXPECT_EQ_INT(vec_pushback(&v, &b), 0);
    EXPECT_EQ_INT(vec_pushback(&v, &c), 0);
    EXPECT_NOTNULL(vec_at(&v, 1u));
    EXPECT_EQ_INT(((struct intg_pair *)vec_at(&v, 1u))->x, 2);
    EXPECT_EQ_INT(((struct intg_pair *)vec_at(&v, 1u))->y, 20);
    a.x = 0;
    a.y = 0;
    EXPECT_EQ_INT(vec_insert(&v, 1u, &a), 0);
    EXPECT_EQ_INT(((struct intg_pair *)vec_at(&v, 1u))->x, 0);
    EXPECT_EQ_INT(((struct intg_pair *)vec_at(&v, 2u))->x, 2);
    EXPECT_EQ_INT(vec_remove(&v, 3u, &out), 0);
    EXPECT_EQ_INT(out.x, 3);
    EXPECT_EQ_INT(out.y, 30);
    EXPECT_EQ_UINT(vec_size(&v), 3u);
    vec_fini(&v);
  }

  {
    struct vector v;
    struct intg_nested n0, n1;

    EXPECT_EQ_INT(vec_init(&v, sizeof(struct intg_nested), NULL), 0);
    n0.layer = 7;
    n0.id = 700;
    n1.layer = 8;
    n1.id = 800;
    EXPECT_EQ_INT(vec_pushback(&v, &n0), 0);
    EXPECT_EQ_INT(vec_pushback(&v, &n1), 0);
    EXPECT_NOTNULL(vec_at(&v, 0u));
    EXPECT_EQ_INT((int)((struct intg_nested *)vec_at(&v, 0u))->layer, 7);
    EXPECT_EQ_INT(((struct intg_nested *)vec_at(&v, 0u))->id, 700);
    EXPECT_EQ_INT((int)((struct intg_nested *)vec_at(&v, 1u))->layer, 8);
    EXPECT_EQ_INT(((struct intg_nested *)vec_back(&v))->id, 800);
    vec_fini(&v);
  }

  {
    struct vector v;
    struct intg_heap_box h;
    size_t idx;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(vec_init(&v, sizeof(struct intg_heap_box), intg_heap_dtor), 0);
    for (idx = 0; idx < 3; idx++) {
      h.p = (int *)malloc(sizeof(int));
      EXPECT_NOTNULL(h.p);
      *h.p = (int)(idx + 1);
      EXPECT_EQ_INT(vec_pushback(&v, &h), 0);
    }
    vec_clear(&v);
    EXPECT_EQ_INT(intg_heap_dtor_n, 3);
    EXPECT_TRUE(vec_empty(&v));
    vec_fini(&v);
    EXPECT_EQ_INT(intg_heap_dtor_n, 3);
  }

  {
    struct vector v;
    struct intg_heap_box h, out;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(vec_init(&v, sizeof(struct intg_heap_box), intg_heap_dtor), 0);
    h.p = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(h.p);
    *h.p = 77;
    EXPECT_EQ_INT(vec_pushback(&v, &h), 0);
    memset(&out, 0, sizeof(out));
    EXPECT_EQ_INT(vec_popback(&v, &out), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 0);
    EXPECT_NOTNULL(out.p);
    EXPECT_EQ_INT(*out.p, 77);
    free(out.p);
    out.p = NULL;
    EXPECT_TRUE(vec_empty(&v));
    vec_fini(&v);
    EXPECT_EQ_INT(intg_heap_dtor_n, 0);
  }

  {
    struct vector v;
    struct intg_heap_box h0, h1, out;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(vec_init(&v, sizeof(struct intg_heap_box), intg_heap_dtor), 0);
    h0.p = (int *)malloc(sizeof(int));
    h1.p = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(h0.p);
    EXPECT_NOTNULL(h1.p);
    *h0.p = 1;
    *h1.p = 2;
    vec_pushback(&v, &h0);
    vec_pushback(&v, &h1);
    memset(&out, 0, sizeof(out));
    EXPECT_EQ_INT(vec_remove(&v, 0u, &out), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 0);
    EXPECT_NOTNULL(out.p);
    EXPECT_EQ_INT(*out.p, 1);
    free(out.p);
    vec_fini(&v);
    EXPECT_EQ_INT(intg_heap_dtor_n, 1);
  }

  {
    struct vector v;
    struct intg_heap_box h;
    size_t idx;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(vec_init(&v, sizeof(struct intg_heap_box), intg_heap_dtor), 0);
    for (idx = 0; idx < 4; idx++) {
      h.p = (int *)malloc(sizeof(int));
      EXPECT_NOTNULL(h.p);
      *h.p = (int)(idx * 10);
      EXPECT_EQ_INT(vec_pushback(&v, &h), 0);
    }
    EXPECT_EQ_INT(vec_resize(&v, 2u), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 2);
    EXPECT_NOTNULL(vec_at(&v, 0u));
    EXPECT_NOTNULL(((struct intg_heap_box *)vec_at(&v, 0u))->p);
    EXPECT_EQ_INT(*((struct intg_heap_box *)vec_at(&v, 0u))->p, 0);
    EXPECT_EQ_INT(*((struct intg_heap_box *)vec_at(&v, 1u))->p, 10);
    vec_fini(&v);
    EXPECT_EQ_INT(intg_heap_dtor_n, 4);
  }

  {
    struct vector v;
    struct intg_heap_box h;
    struct intg_heap_box *slot;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(vec_init(&v, sizeof(struct intg_heap_box), NULL), 0);
    h.p = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(h.p);
    *h.p = 99;
    EXPECT_EQ_INT(vec_pushback(&v, &h), 0);
    EXPECT_NOTNULL(vec_at(&v, 0u));
    slot = (struct intg_heap_box *)vec_at(&v, 0u);
    EXPECT_NOTNULL(slot->p);
    free(slot->p);
    slot->p = NULL;
    vec_clear(&v);
    vec_fini(&v);
    EXPECT_EQ_INT(intg_heap_dtor_n, 0);
  }

  {
    struct vector v;
    struct intg_heap_box h0, h1, h2;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(vec_init(&v, sizeof(struct intg_heap_box), intg_heap_dtor), 0);
    h0.p = (int *)malloc(sizeof(int));
    h1.p = (int *)malloc(sizeof(int));
    *h0.p = 1;
    *h1.p = 2;
    vec_pushback(&v, &h0);
    vec_pushback(&v, &h1);
    h2.p = (int *)malloc(sizeof(int));
    *h2.p = 50;
    EXPECT_EQ_INT(vec_insert(&v, 1u, &h2), 0);
    EXPECT_EQ_INT(*((struct intg_heap_box *)vec_at(&v, 0u))->p, 1);
    EXPECT_EQ_INT(*((struct intg_heap_box *)vec_at(&v, 1u))->p, 50);
    EXPECT_EQ_INT(*((struct intg_heap_box *)vec_at(&v, 2u))->p, 2);
    EXPECT_EQ_INT(vec_remove(&v, 1u, NULL), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 1);
    vec_fini(&v);
    EXPECT_EQ_INT(intg_heap_dtor_n, 3);
  }

  {
    struct vector v;
    struct intg_heap_box h;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(vec_init(&v, sizeof(struct intg_heap_box), intg_heap_dtor), 0);
    h.p = (int *)malloc(sizeof(int));
    *h.p = 5;
    vec_pushback(&v, &h);
    EXPECT_EQ_INT(vec_popback(&v, NULL), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 1);
    vec_fini(&v);
    EXPECT_EQ_INT(intg_heap_dtor_n, 1);
  }
}
