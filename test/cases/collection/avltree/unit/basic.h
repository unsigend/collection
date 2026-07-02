#include <avltree.h>
#include <stdlib.h>
#include <string.h>
#include <utest.h>

static int cmp_int_key(void *a, void *b)
{
  int ka = *(int *)a;
  int kb = *(int *)b;
  if (ka < kb)
    return -1;
  if (ka > kb)
    return 1;
  return 0;
}

static int cmp_str_key(void *a, void *b)
{
  return strcmp((const char *)a, (const char *)b);
}

static int dtor_key_n;
static int dtor_val_n;

static void dtor_key_inc(void *p)
{
  (void)p;
  dtor_key_n++;
}

static void dtor_val_inc(void *p)
{
  (void)p;
  dtor_val_n++;
}

static void free_key(void *p) { free(p); }

static void free_val(void *p) { free(p); }

UTEST_CASE(basic)
{
  {
    struct avltree *t;

    EXPECT_EQ_INT(avltree_init(NULL, cmp_int_key, NULL, NULL), -1);
    EXPECT_EQ_INT(avltree_init(&t, NULL, NULL, NULL), -1);
  }

  {
    struct avltree *t;
    int k, v;

    EXPECT_EQ_INT(avltree_init(&t, cmp_int_key, NULL, NULL), 0);
    EXPECT_TRUE(avltree_empty(NULL));
    EXPECT_EQ_UINT(avltree_size(NULL), 0);
    EXPECT_NULL(avltree_root(NULL));
    EXPECT_TRUE(avltree_empty(t));
    EXPECT_EQ_UINT(avltree_size(t), 0);
    EXPECT_NULL(avltree_root(t));
    k = 1;
    v = 0;
    EXPECT_NULL(avltree_find(t, &k));
    EXPECT_NULL(avltree_findnode(t, &k));
    EXPECT_EQ_INT(avltree_remove(t, &k, NULL), -1);
    EXPECT_EQ_INT(avltree_insert(t, NULL, NULL), -1);
    EXPECT_EQ_INT(avltree_update(t, &k, &v, NULL), -1);
    avltree_free(NULL);
    avltree_clear(t);
    EXPECT_TRUE(avltree_empty(t));
    EXPECT_EQ_UINT(avltree_size(t), 0);
    avltree_free(t);
  }

  {
    struct avltree *t;
    int k, v;
    void *fv;

    EXPECT_EQ_INT(avltree_init(&t, cmp_int_key, NULL, NULL), 0);
    k = 11;
    v = 21;
    EXPECT_EQ_INT(avltree_insert(t, &k, &v), 0);
    EXPECT_FALSE(avltree_empty(t));
    EXPECT_EQ_UINT(avltree_size(t), 1);
    EXPECT_NOTNULL(avltree_root(t));
    fv = avltree_find(t, &k);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_INT(*(int *)fv, v);
    EXPECT_EQ_INT(avltree_insert(t, &k, &v), -1);
    avltree_free(t);
  }

  {
    struct avltree *t;
    int k1, k2, v1, v2;
    struct avltree_node *n;
    void *fv;

    EXPECT_EQ_INT(avltree_init(&t, cmp_int_key, NULL, NULL), 0);
    k1 = 1;
    v1 = 10;
    k2 = 2;
    v2 = 20;
    EXPECT_EQ_INT(avltree_insert(t, &k1, &v1), 0);
    EXPECT_EQ_INT(avltree_insert(t, &k2, &v2), 0);
    EXPECT_EQ_UINT(avltree_size(t), 2);
    n = avltree_findnode(t, &k2);
    EXPECT_NOTNULL(n);
    fv = avltree_find(t, &k2);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_INT(*(int *)fv, v2);
    fv = avltree_find(t, &k1);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_INT(*(int *)fv, v1);
    avltree_free(t);
  }

  {
    struct avltree *t;
    int k, v0, v1;
    void *oldv;
    void *fv;

    EXPECT_EQ_INT(avltree_init(&t, cmp_int_key, NULL, NULL), 0);
    k = 5;
    v0 = 50;
    v1 = 51;
    EXPECT_EQ_INT(avltree_insert(t, &k, &v0), 0);
    EXPECT_EQ_INT(avltree_update(t, &k, &v1, &oldv), 0);
    EXPECT_NOTNULL(oldv);
    EXPECT_EQ_PTR(oldv, &v0);
    fv = avltree_find(t, &k);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_PTR(fv, &v1);
    EXPECT_EQ_INT(avltree_update(t, &k, &v0, NULL), 0);
    fv = avltree_find(t, &k);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_PTR(fv, &v0);
    EXPECT_EQ_INT(avltree_update(t, &k, &v1, NULL), 0);
    avltree_free(t);
  }

  {
    struct avltree *t;
    int k, v;
    void *outv;

    EXPECT_EQ_INT(avltree_init(&t, cmp_int_key, NULL, NULL), 0);
    k = 9;
    v = 99;
    EXPECT_EQ_INT(avltree_insert(t, &k, &v), 0);
    EXPECT_EQ_INT(avltree_remove(t, &k, &outv), 0);
    EXPECT_NOTNULL(outv);
    EXPECT_EQ_PTR(outv, &v);
    EXPECT_TRUE(avltree_empty(t));
    EXPECT_EQ_INT(avltree_remove(t, &k, NULL), -1);
    avltree_free(t);
  }

  {
    struct avltree *t;
    int k0, k1, k2;

    EXPECT_EQ_INT(avltree_init(&t, cmp_int_key, NULL, NULL), 0);
    k0 = 0;
    k1 = 1;
    k2 = 2;
    EXPECT_EQ_INT(avltree_insert(t, &k0, NULL), 0);
    EXPECT_EQ_INT(avltree_insert(t, &k1, NULL), 0);
    EXPECT_EQ_INT(avltree_insert(t, &k2, NULL), 0);
    EXPECT_NOTNULL(avltree_findnode(t, &k0));
    EXPECT_NOTNULL(avltree_findnode(t, &k1));
    EXPECT_NOTNULL(avltree_findnode(t, &k2));
    EXPECT_NULL(avltree_find(t, &k0));
    EXPECT_NULL(avltree_find(t, &k1));
    EXPECT_NULL(avltree_find(t, &k2));
    avltree_clear(t);
    EXPECT_TRUE(avltree_empty(t));
    EXPECT_EQ_UINT(avltree_size(t), 0);
    avltree_free(t);
  }

  {
    struct avltree *t;
    char *ka, *kb;
    int va, vb;
    void *fv;

    EXPECT_EQ_INT(avltree_init(&t, cmp_str_key, free_key, NULL), 0);
    ka = strdup("alpha");
    EXPECT_NOTNULL(ka);
    kb = strdup("beta");
    if (!kb) {
      free(ka);
    }
    EXPECT_NOTNULL(kb);
    va = 1;
    vb = 2;
    EXPECT_EQ_INT(avltree_insert(t, ka, &va), 0);
    EXPECT_EQ_INT(avltree_insert(t, kb, &vb), 0);
    fv = avltree_find(t, ka);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_INT(*(int *)fv, va);
    fv = avltree_find(t, kb);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_INT(*(int *)fv, vb);
    avltree_free(t);
  }

  {
    struct avltree *t;
    int idx;
    int *pk;
    int *pv;

    EXPECT_EQ_INT(avltree_init(&t, cmp_int_key, free_key, free_val), 0);
    for (idx = 0; idx < 8; idx++) {
      int ins;

      pk = (int *)malloc(sizeof(int));
      EXPECT_NOTNULL(pk);
      pv = (int *)malloc(sizeof(int));
      if (!pv) {
        free(pk);
      }
      EXPECT_NOTNULL(pv);
      *pk = idx;
      *pv = idx * 10;
      ins = avltree_insert(t, pk, pv);
      if (ins != 0) {
        free(pk);
        free(pv);
      }
      EXPECT_EQ_INT(ins, 0);
    }
    EXPECT_EQ_UINT(avltree_size(t), 8);
    for (idx = 0; idx < 8; idx++) {
      int key = idx;
      pv = (int *)avltree_find(t, &key);
      EXPECT_NOTNULL(pv);
      EXPECT_EQ_INT(*pv, idx * 10);
    }
    avltree_free(t);
  }

  {
    struct avltree *t;
    int *pk;
    int *pv;
    void *outv;

    EXPECT_EQ_INT(avltree_init(&t, cmp_int_key, free_key, free_val), 0);
    pk = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(pk);
    pv = (int *)malloc(sizeof(int));
    if (!pv) {
      free(pk);
    }
    EXPECT_NOTNULL(pv);
    *pk = 42;
    *pv = 420;
    {
      int ins;

      ins = avltree_insert(t, pk, pv);
      if (ins != 0) {
        free(pk);
        free(pv);
      }
      EXPECT_EQ_INT(ins, 0);
    }
    EXPECT_EQ_INT(avltree_remove(t, pk, &outv), 0);
    EXPECT_NOTNULL(outv);
    EXPECT_EQ_PTR(outv, pv);
    EXPECT_EQ_INT(*(int *)outv, 420);
    free(pv);
    avltree_free(t);
  }

  {
    struct avltree *t;
    int k, v;

    dtor_key_n = 0;
    dtor_val_n = 0;
    EXPECT_EQ_INT(avltree_init(&t, cmp_int_key, dtor_key_inc, dtor_val_inc), 0);
    k = 1;
    v = 2;
    EXPECT_EQ_INT(avltree_insert(t, &k, &v), 0);
    avltree_clear(t);
    EXPECT_EQ_INT(dtor_key_n, 1);
    EXPECT_EQ_INT(dtor_val_n, 1);
    EXPECT_TRUE(avltree_empty(t));
    avltree_free(t);
  }

  {
    struct avltree *t;
    int k, v0, v1;

    dtor_val_n = 0;
    EXPECT_EQ_INT(avltree_init(&t, cmp_int_key, NULL, dtor_val_inc), 0);
    k = 7;
    v0 = 70;
    v1 = 71;
    EXPECT_EQ_INT(avltree_insert(t, &k, &v0), 0);
    EXPECT_EQ_INT(avltree_update(t, &k, &v1, NULL), 0);
    EXPECT_EQ_INT(dtor_val_n, 1);
    avltree_free(t);
    EXPECT_EQ_INT(dtor_val_n, 2);
  }

  {
    struct avltree *t;
    int k, v;

    dtor_val_n = 0;
    EXPECT_EQ_INT(avltree_init(&t, cmp_int_key, NULL, dtor_val_inc), 0);
    k = 3;
    v = 33;
    EXPECT_EQ_INT(avltree_insert(t, &k, &v), 0);
    EXPECT_EQ_INT(avltree_remove(t, &k, NULL), 0);
    EXPECT_EQ_INT(dtor_val_n, 1);
    avltree_free(t);
  }
}
