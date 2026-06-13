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
    struct avltree t;
    struct avltree_fns fns = {cmp_int_key, NULL, NULL};

    EXPECT_EQ_INT(avltree_init(NULL, &fns), -1);
    EXPECT_EQ_INT(avltree_init(&t, NULL), -1);
    fns.cmp = NULL;
    EXPECT_EQ_INT(avltree_init(&t, &fns), -1);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {cmp_int_key, NULL, NULL};
    int k, v;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    EXPECT_TRUE(avltree_empty(&t));
    EXPECT_EQ_UINT(avltree_size(&t), 0);
    EXPECT_NULL(avltree_root(&t));
    EXPECT_EQ_PTR(avltree_fns(&t), &fns);
    k = 1;
    v = 0;
    EXPECT_NULL(avltree_find(&t, &k));
    EXPECT_NULL(avltree_findnode(&t, &k));
    EXPECT_EQ_INT(avltree_remove(&t, &k, NULL), -1);
    EXPECT_EQ_INT(avltree_insert(&t, NULL, NULL), -1);
    EXPECT_EQ_INT(avltree_update(&t, &k, &v, NULL), -1);
    avltree_fini(NULL);
    avltree_clear(&t);
    EXPECT_TRUE(avltree_empty(&t));
    EXPECT_EQ_UINT(avltree_size(&t), 0);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {cmp_int_key, NULL, NULL};
    int k, v;
    void *fv;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    k = 11;
    v = 21;
    EXPECT_EQ_INT(avltree_insert(&t, &k, &v), 0);
    EXPECT_FALSE(avltree_empty(&t));
    EXPECT_EQ_UINT(avltree_size(&t), 1);
    EXPECT_NOTNULL(avltree_root(&t));
    fv = avltree_find(&t, &k);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_INT(*(int *)fv, v);
    EXPECT_EQ_INT(avltree_insert(&t, &k, &v), -1);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {cmp_int_key, NULL, NULL};
    int k1, k2, v1, v2;
    struct avltree_node *n;
    void *fv;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    k1 = 1;
    v1 = 10;
    k2 = 2;
    v2 = 20;
    EXPECT_EQ_INT(avltree_insert(&t, &k1, &v1), 0);
    EXPECT_EQ_INT(avltree_insert(&t, &k2, &v2), 0);
    EXPECT_EQ_UINT(avltree_size(&t), 2);
    n = avltree_findnode(&t, &k2);
    EXPECT_NOTNULL(n);
    EXPECT_EQ_PTR(n->key, &k2);
    EXPECT_NOTNULL(n->val);
    EXPECT_EQ_INT(*(int *)n->val, v2);
    fv = avltree_find(&t, &k1);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_INT(*(int *)fv, v1);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {cmp_int_key, NULL, NULL};
    int k, v0, v1;
    void *oldv;
    void *fv;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    k = 5;
    v0 = 50;
    v1 = 51;
    EXPECT_EQ_INT(avltree_insert(&t, &k, &v0), 0);
    EXPECT_EQ_INT(avltree_update(&t, &k, &v1, &oldv), 0);
    EXPECT_NOTNULL(oldv);
    EXPECT_EQ_PTR(oldv, &v0);
    fv = avltree_find(&t, &k);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_PTR(fv, &v1);
    EXPECT_EQ_INT(avltree_update(&t, &k, &v0, NULL), 0);
    fv = avltree_find(&t, &k);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_PTR(fv, &v0);
    EXPECT_EQ_INT(avltree_update(&t, &k, &v1, NULL), 0);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {cmp_int_key, NULL, NULL};
    int k, v;
    void *outv;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    k = 9;
    v = 99;
    EXPECT_EQ_INT(avltree_insert(&t, &k, &v), 0);
    EXPECT_EQ_INT(avltree_remove(&t, &k, &outv), 0);
    EXPECT_NOTNULL(outv);
    EXPECT_EQ_PTR(outv, &v);
    EXPECT_TRUE(avltree_empty(&t));
    EXPECT_EQ_INT(avltree_remove(&t, &k, NULL), -1);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {cmp_int_key, NULL, NULL};
    int k0, k1, k2;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    k0 = 0;
    k1 = 1;
    k2 = 2;
    EXPECT_EQ_INT(avltree_insert(&t, &k0, NULL), 0);
    EXPECT_EQ_INT(avltree_insert(&t, &k1, NULL), 0);
    EXPECT_EQ_INT(avltree_insert(&t, &k2, NULL), 0);
    EXPECT_NOTNULL(avltree_findnode(&t, &k0));
    EXPECT_NOTNULL(avltree_findnode(&t, &k1));
    EXPECT_NOTNULL(avltree_findnode(&t, &k2));
    EXPECT_NULL(avltree_find(&t, &k0));
    EXPECT_NULL(avltree_find(&t, &k1));
    EXPECT_NULL(avltree_find(&t, &k2));
    avltree_clear(&t);
    EXPECT_TRUE(avltree_empty(&t));
    EXPECT_EQ_UINT(avltree_size(&t), 0);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {cmp_str_key, free_key, NULL};
    char *ka, *kb;
    int va, vb;
    void *fv;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    ka = strdup("alpha");
    EXPECT_NOTNULL(ka);
    kb = strdup("beta");
    if (!kb) {
      free(ka);
    }
    EXPECT_NOTNULL(kb);
    va = 1;
    vb = 2;
    EXPECT_EQ_INT(avltree_insert(&t, ka, &va), 0);
    EXPECT_EQ_INT(avltree_insert(&t, kb, &vb), 0);
    fv = avltree_find(&t, ka);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_INT(*(int *)fv, va);
    fv = avltree_find(&t, kb);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_INT(*(int *)fv, vb);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {cmp_int_key, free_key, free_val};
    int idx;
    int *pk;
    int *pv;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
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
      ins = avltree_insert(&t, pk, pv);
      if (ins != 0) {
        free(pk);
        free(pv);
      }
      EXPECT_EQ_INT(ins, 0);
    }
    EXPECT_EQ_UINT(avltree_size(&t), 8);
    for (idx = 0; idx < 8; idx++) {
      int key = idx;
      pv = (int *)avltree_find(&t, &key);
      EXPECT_NOTNULL(pv);
      EXPECT_EQ_INT(*pv, idx * 10);
    }
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {cmp_int_key, free_key, free_val};
    int *pk;
    int *pv;
    void *outv;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
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

      ins = avltree_insert(&t, pk, pv);
      if (ins != 0) {
        free(pk);
        free(pv);
      }
      EXPECT_EQ_INT(ins, 0);
    }
    EXPECT_EQ_INT(avltree_remove(&t, pk, &outv), 0);
    EXPECT_NOTNULL(outv);
    EXPECT_EQ_PTR(outv, pv);
    EXPECT_EQ_INT(*(int *)outv, 420);
    free(pv);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {cmp_int_key, dtor_key_inc, dtor_val_inc};
    int k, v;

    dtor_key_n = 0;
    dtor_val_n = 0;
    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    k = 1;
    v = 2;
    EXPECT_EQ_INT(avltree_insert(&t, &k, &v), 0);
    avltree_clear(&t);
    EXPECT_EQ_INT(dtor_key_n, 1);
    EXPECT_EQ_INT(dtor_val_n, 1);
    EXPECT_TRUE(avltree_empty(&t));
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {cmp_int_key, NULL, dtor_val_inc};
    int k, v0, v1;

    dtor_val_n = 0;
    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    k = 7;
    v0 = 70;
    v1 = 71;
    EXPECT_EQ_INT(avltree_insert(&t, &k, &v0), 0);
    EXPECT_EQ_INT(avltree_update(&t, &k, &v1, NULL), 0);
    EXPECT_EQ_INT(dtor_val_n, 1);
    avltree_fini(&t);
    EXPECT_EQ_INT(dtor_val_n, 2);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {cmp_int_key, NULL, dtor_val_inc};
    int k, v;

    dtor_val_n = 0;
    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    k = 3;
    v = 33;
    EXPECT_EQ_INT(avltree_insert(&t, &k, &v), 0);
    EXPECT_EQ_INT(avltree_remove(&t, &k, NULL), 0);
    EXPECT_EQ_INT(dtor_val_n, 1);
    avltree_fini(&t);
  }
}
