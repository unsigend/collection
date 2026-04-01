#include <avltree.h>
#include <stdlib.h>
#include <utest.h>

static int edg_cmp_int(void *a, void *b)
{
  int ka = *(int *)a;
  int kb = *(int *)b;
  if (ka < kb)
    return -1;
  if (ka > kb)
    return 1;
  return 0;
}

static int edg_key_dtor_n;
static int edg_val_dtor_n;

static void edg_key_dtor_inc(void *p)
{
  (void)p;
  edg_key_dtor_n++;
}

static void edg_val_dtor_inc(void *p)
{
  (void)p;
  edg_val_dtor_n++;
}

static void edg_free_key(void *p)
{
  free(p);
}

UTEST_CASE(edge)
{
  {
    int k, v;

    k = 0;
    v = 0;
    EXPECT_NULL(avltree_find(NULL, &k));
    EXPECT_NULL(avltree_findnode(NULL, &k));
    EXPECT_EQ_INT(avltree_remove(NULL, &k, NULL), -1);
    EXPECT_EQ_INT(avltree_insert(NULL, &k, &v), -1);
    EXPECT_EQ_INT(avltree_update(NULL, &k, &v, NULL), -1);
    avltree_clear(NULL);
    avltree_fini(NULL);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {edg_cmp_int, NULL, NULL};
    int k, v;
    void *p;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    k = 1;
    v = 1;
    EXPECT_NULL(avltree_find(&t, &k));
    EXPECT_NULL(avltree_findnode(&t, &k));
    EXPECT_EQ_INT(avltree_remove(&t, &k, NULL), -1);
    EXPECT_NULL(avltree_find(&t, NULL));
    EXPECT_NULL(avltree_findnode(&t, NULL));
    EXPECT_EQ_INT(avltree_remove(&t, NULL, NULL), -1);
    EXPECT_EQ_INT(avltree_update(&t, NULL, &v, NULL), -1);
    EXPECT_EQ_INT(avltree_update(&t, &k, NULL, NULL), -1);
    p = &v;
    EXPECT_EQ_INT(avltree_update(&t, &k, p, &p), -1);
    EXPECT_EQ_INT(avltree_insert(&t, &k, &v), 0);
    EXPECT_EQ_INT(avltree_update(&t, &k, NULL, NULL), -1);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {edg_cmp_int, NULL, NULL};

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    avltree_clear(&t);
    avltree_clear(&t);
    EXPECT_TRUE(avltree_empty(&t));
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {edg_cmp_int, NULL, NULL};
    int k, v;
    void *oldv;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    k = 9;
    v = 90;
    EXPECT_EQ_INT(avltree_insert(&t, &k, &v), 0);
    EXPECT_EQ_INT(avltree_update(&t, &k, &v, &oldv), 0);
    EXPECT_NOTNULL(oldv);
    EXPECT_EQ_PTR(oldv, &v);
    EXPECT_EQ_INT(avltree_update(&t, &k, &v, NULL), 0);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {edg_cmp_int, NULL, edg_val_dtor_inc};
    int k, v0, v1;
    void *oldv;

    edg_val_dtor_n = 0;
    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    k = 2;
    v0 = 20;
    v1 = 21;
    EXPECT_EQ_INT(avltree_insert(&t, &k, &v0), 0);
    EXPECT_EQ_INT(avltree_update(&t, &k, &v1, &oldv), 0);
    EXPECT_NOTNULL(oldv);
    EXPECT_EQ_PTR(oldv, &v0);
    EXPECT_EQ_INT(edg_val_dtor_n, 0);
    avltree_fini(&t);
    EXPECT_EQ_INT(edg_val_dtor_n, 1);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {edg_cmp_int, NULL, NULL};
    int k;
    void *oldv;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    k = 3;
    EXPECT_EQ_INT(avltree_insert(&t, &k, NULL), 0);
    EXPECT_NOTNULL(avltree_findnode(&t, &k));
    EXPECT_NULL(avltree_find(&t, &k));
    oldv = (void *)1;
    EXPECT_EQ_INT(avltree_update(&t, &k, &k, &oldv), 0);
    EXPECT_NULL(oldv);
    EXPECT_EQ_PTR(avltree_find(&t, &k), &k);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {edg_cmp_int, NULL, NULL};
    int k;
    void *outv;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    k = 4;
    EXPECT_EQ_INT(avltree_insert(&t, &k, NULL), 0);
    outv = (void *)1;
    EXPECT_EQ_INT(avltree_remove(&t, &k, &outv), 0);
    EXPECT_NULL(outv);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {edg_cmp_int, NULL, NULL};
    int keys[12];
    int vals[12];
    int idx;
    int q;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    for (idx = 0; idx < 12; idx++) {
      keys[idx] = idx;
      vals[idx] = idx * 100;
      EXPECT_EQ_INT(avltree_insert(&t, &keys[idx], &vals[idx]), 0);
    }
    EXPECT_EQ_UINT(avltree_size(&t), 12);
    for (idx = 0; idx < 12; idx++) {
      q = idx;
      {
        void *fv = avltree_find(&t, &q);

        EXPECT_NOTNULL(fv);
        EXPECT_EQ_INT(*(int *)fv, idx * 100);
      }
    }
    EXPECT_EQ_INT(avltree_remove(&t, &keys[6], NULL), 0);
    EXPECT_EQ_UINT(avltree_size(&t), 11);
    q = 6;
    EXPECT_NULL(avltree_findnode(&t, &q));
    EXPECT_EQ_INT(avltree_remove(&t, &keys[0], NULL), 0);
    EXPECT_EQ_INT(avltree_remove(&t, &keys[11], NULL), 0);
    EXPECT_EQ_UINT(avltree_size(&t), 9);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {edg_cmp_int, NULL, NULL};
    int a, b, c, v;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    a = 1;
    b = 2;
    c = 3;
    v = 7;
    EXPECT_EQ_INT(avltree_insert(&t, &a, &v), 0);
    EXPECT_EQ_INT(avltree_insert(&t, &b, &v), 0);
    EXPECT_EQ_INT(avltree_insert(&t, &c, &v), 0);
    EXPECT_EQ_INT(avltree_update(&t, &b, &v, NULL), 0);
    EXPECT_EQ_PTR(avltree_find(&t, &b), &v);
    EXPECT_EQ_INT(avltree_remove(&t, &a, NULL), 0);
    EXPECT_NOTNULL(avltree_findnode(&t, &c));
    EXPECT_NOTNULL(avltree_findnode(&t, &b));
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {edg_cmp_int, NULL, NULL};
    int k1, k2, v;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    k1 = 42;
    k2 = 42;
    v = 1;
    EXPECT_EQ_INT(avltree_insert(&t, &k1, &v), 0);
    EXPECT_EQ_INT(avltree_insert(&t, &k2, &v), -1);
    EXPECT_EQ_UINT(avltree_size(&t), 1);
    EXPECT_EQ_PTR(avltree_find(&t, &k1), &v);
    EXPECT_EQ_PTR(avltree_find(&t, &k2), &v);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {edg_cmp_int, edg_free_key, NULL};
    int *pk;
    int v;
    int klookup;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    pk = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(pk);
    *pk = 100;
    v = 1;
    EXPECT_EQ_INT(avltree_insert(&t, pk, &v), 0);
    klookup = 100;
    EXPECT_EQ_PTR(avltree_find(&t, &klookup), &v);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {edg_cmp_int, edg_key_dtor_inc, edg_val_dtor_inc};
    int k, v;

    edg_key_dtor_n = 0;
    edg_val_dtor_n = 0;
    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    k = 8;
    v = 80;
    EXPECT_EQ_INT(avltree_insert(&t, &k, &v), 0);
    EXPECT_EQ_INT(avltree_remove(&t, &k, NULL), 0);
    EXPECT_EQ_INT(edg_key_dtor_n, 1);
    EXPECT_EQ_INT(edg_val_dtor_n, 1);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {edg_cmp_int, edg_key_dtor_inc, NULL};
    int k, v;
    void *outv;

    edg_key_dtor_n = 0;
    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    k = 5;
    v = 50;
    EXPECT_EQ_INT(avltree_insert(&t, &k, &v), 0);
    EXPECT_EQ_INT(avltree_remove(&t, &k, &outv), 0);
    EXPECT_NOTNULL(outv);
    EXPECT_EQ_PTR(outv, &v);
    EXPECT_EQ_INT(edg_key_dtor_n, 1);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {edg_cmp_int, NULL, edg_val_dtor_inc};
    int k, v;
    void *outv;

    edg_val_dtor_n = 0;
    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    k = 6;
    v = 60;
    EXPECT_EQ_INT(avltree_insert(&t, &k, &v), 0);
    EXPECT_EQ_INT(avltree_remove(&t, &k, &outv), 0);
    EXPECT_NOTNULL(outv);
    EXPECT_EQ_PTR(outv, &v);
    EXPECT_EQ_INT(edg_val_dtor_n, 0);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {edg_cmp_int, NULL, NULL};
    int kbuf[64];
    int vbuf[64];
    int step;
    int q;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    for (step = 0; step < 64; step++) {
      kbuf[step] = step + 1000;
      vbuf[step] = step;
      EXPECT_EQ_INT(avltree_insert(&t, &kbuf[step], &vbuf[step]), 0);
    }
    EXPECT_EQ_UINT(avltree_size(&t), 64);
    for (step = 0; step < 64; step++) {
      q = step + 1000;
      {
        void *fv = avltree_find(&t, &q);

        EXPECT_NOTNULL(fv);
        EXPECT_EQ_INT(*(int *)fv, step);
      }
    }
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {edg_cmp_int, NULL, NULL};
    int k1, k2, km;
    int v;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    k1 = 11;
    k2 = 22;
    v = 0;
    EXPECT_EQ_INT(avltree_insert(&t, &k1, &v), 0);
    EXPECT_EQ_INT(avltree_insert(&t, &k2, &v), 0);
    km = 9999;
    EXPECT_EQ_INT(avltree_update(&t, &km, &v, NULL), -1);
    EXPECT_EQ_INT(avltree_remove(&t, &km, NULL), -1);
    EXPECT_NULL(avltree_find(&t, &km));
    EXPECT_NULL(avltree_findnode(&t, &km));
    EXPECT_EQ_UINT(avltree_size(&t), 2);
    avltree_fini(&t);
  }
}
