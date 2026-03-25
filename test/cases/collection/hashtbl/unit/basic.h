#include <hashtbl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <utest.h>

static uint32_t hash_int_key(void *k)
{
  return (uint32_t)(*(int *)k);
}

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

static uint32_t hash_str_key(void *k)
{
  const unsigned char *s = (const unsigned char *)k;
  uint32_t h = 5381;
  unsigned c;
  while ((c = *s++) != 0)
    h = h * 33u + c;
  return h;
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

static void free_key(void *p)
{
  free(p);
}

static void free_val(void *p)
{
  free(p);
}

UTEST_CASE(basic)
{
  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {hash_int_key, cmp_int_key, NULL, NULL};

    EXPECT_EQ_INT(hashtbl_init(NULL, &fns), -1);
    EXPECT_EQ_INT(hashtbl_init(&ht, NULL), -1);
    fns.hash = NULL;
    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), -1);
    fns.hash = hash_int_key;
    fns.cmp = NULL;
    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), -1);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {hash_int_key, cmp_int_key, NULL, NULL};
    float old_th;
    void *p;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    EXPECT_TRUE(hashtbl_empty(&ht));
    EXPECT_EQ_UINT(hashtbl_size(&ht), 0);
    EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht), (double)hashtbl_threshold(&ht));
    EXPECT_NULL(hashtbl_find(&ht, (void *)1));
    EXPECT_NULL(hashtbl_findnode(&ht, (void *)1));
    EXPECT_EQ_INT(hashtbl_remove(&ht, (void *)1, NULL), -1);
    EXPECT_EQ_INT(hashtbl_insert(&ht, NULL, NULL), -1);
    EXPECT_EQ_INT(hashtbl_update(&ht, (void *)1, NULL, NULL), -1);
    p = (void *)1;
    EXPECT_EQ_INT(hashtbl_update(&ht, p, p, NULL), -1);
    EXPECT_EQ_INT(hashtbl_setthreshold(NULL, 0.5f, NULL), -1);
    EXPECT_EQ_INT(hashtbl_setthreshold(&ht, 0.0f, NULL), -1);
    EXPECT_EQ_INT(hashtbl_setthreshold(&ht, -1.0f, NULL), -1);
    EXPECT_EQ_INT(hashtbl_setthreshold(&ht, 0.5f, &old_th), 0);
    EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht), (double)hashtbl_threshold(&ht));
    hashtbl_fini(NULL);
    hashtbl_clear(&ht);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {hash_int_key, cmp_int_key, NULL, NULL};
    int k, v;
    void *fv;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k = 11;
    v = 21;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v), 0);
    EXPECT_FALSE(hashtbl_empty(&ht));
    EXPECT_EQ_UINT(hashtbl_size(&ht), 1);
    EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht), (double)hashtbl_threshold(&ht));
    fv = hashtbl_find(&ht, &k);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_INT(*(int *)fv, v);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v), -1);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {hash_int_key, cmp_int_key, NULL, NULL};
    int k1, k2, v1, v2;
    struct hashtbl_node *n;
    void *fv;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k1 = 1;
    v1 = 10;
    k2 = 2;
    v2 = 20;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k1, &v1), 0);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k2, &v2), 0);
    EXPECT_EQ_UINT(hashtbl_size(&ht), 2);
    EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht), (double)hashtbl_threshold(&ht));
    n = hashtbl_findnode(&ht, &k2);
    EXPECT_NOTNULL(n);
    EXPECT_EQ_PTR(n->key, &k2);
    EXPECT_NOTNULL(n->val);
    EXPECT_EQ_INT(*(int *)n->val, v2);
    fv = hashtbl_find(&ht, &k1);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_INT(*(int *)fv, v1);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {hash_int_key, cmp_int_key, NULL, NULL};
    int k, v0, v1;
    void *oldv;
    void *fv;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k = 5;
    v0 = 50;
    v1 = 51;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v0), 0);
    EXPECT_EQ_INT(hashtbl_update(&ht, &k, &v1, &oldv), 0);
    EXPECT_NOTNULL(oldv);
    EXPECT_EQ_PTR(oldv, &v0);
    fv = hashtbl_find(&ht, &k);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_PTR(fv, &v1);
    EXPECT_EQ_INT(hashtbl_update(&ht, &k, &v0, NULL), 0);
    fv = hashtbl_find(&ht, &k);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_PTR(fv, &v0);
    EXPECT_EQ_INT(hashtbl_update(&ht, &k, &v1, NULL), 0);
    EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht), (double)hashtbl_threshold(&ht));
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {hash_int_key, cmp_int_key, NULL, NULL};
    int k, v;
    void *outv;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k = 9;
    v = 99;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v), 0);
    EXPECT_EQ_INT(hashtbl_remove(&ht, &k, &outv), 0);
    EXPECT_NOTNULL(outv);
    EXPECT_EQ_PTR(outv, &v);
    EXPECT_TRUE(hashtbl_empty(&ht));
    EXPECT_EQ_INT(hashtbl_remove(&ht, &k, NULL), -1);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {hash_int_key, cmp_int_key, NULL, NULL};
    int k0, k1, k2;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k0 = 0;
    k1 = 1;
    k2 = 2;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k0, NULL), 0);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k1, NULL), 0);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k2, NULL), 0);
    EXPECT_NOTNULL(hashtbl_findnode(&ht, &k0));
    EXPECT_NOTNULL(hashtbl_findnode(&ht, &k1));
    EXPECT_NOTNULL(hashtbl_findnode(&ht, &k2));
    EXPECT_NULL(hashtbl_find(&ht, &k0));
    EXPECT_NULL(hashtbl_find(&ht, &k1));
    EXPECT_NULL(hashtbl_find(&ht, &k2));
    hashtbl_clear(&ht);
    EXPECT_TRUE(hashtbl_empty(&ht));
    EXPECT_EQ_UINT(hashtbl_size(&ht), 0);
    EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht), (double)hashtbl_threshold(&ht));
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {hash_str_key, cmp_str_key, free_key, NULL};
    char *ka, *kb;
    int va, vb;
    void *fv;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    ka = strdup("alpha");
    EXPECT_NOTNULL(ka);
    kb = strdup("beta");
    if (!kb) {
      free(ka);
    }
    EXPECT_NOTNULL(kb);
    va = 1;
    vb = 2;
    EXPECT_EQ_INT(hashtbl_insert(&ht, ka, &va), 0);
    EXPECT_EQ_INT(hashtbl_insert(&ht, kb, &vb), 0);
    EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht), (double)hashtbl_threshold(&ht));
    fv = hashtbl_find(&ht, ka);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_INT(*(int *)fv, va);
    fv = hashtbl_find(&ht, kb);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_INT(*(int *)fv, vb);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {hash_int_key, cmp_int_key, free_key, free_val};
    int idx;
    int *pk;
    int *pv;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
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
      ins = hashtbl_insert(&ht, pk, pv);
      if (ins != 0) {
        free(pk);
        free(pv);
      }
      EXPECT_EQ_INT(ins, 0);
      EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht), (double)hashtbl_threshold(&ht));
    }
    EXPECT_EQ_UINT(hashtbl_size(&ht), 8);
    for (idx = 0; idx < 8; idx++) {
      int key = idx;
      pv = (int *)hashtbl_find(&ht, &key);
      EXPECT_NOTNULL(pv);
      EXPECT_EQ_INT(*pv, idx * 10);
    }
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {hash_int_key, cmp_int_key, free_key, free_val};
    int *pk;
    int *pv;
    void *outv;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
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

      ins = hashtbl_insert(&ht, pk, pv);
      if (ins != 0) {
        free(pk);
        free(pv);
      }
      EXPECT_EQ_INT(ins, 0);
    }
    EXPECT_EQ_INT(hashtbl_remove(&ht, pk, &outv), 0);
    EXPECT_NOTNULL(outv);
    EXPECT_EQ_PTR(outv, pv);
    EXPECT_EQ_INT(*(int *)outv, 420);
    free(pv);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {hash_int_key, cmp_int_key, NULL, dtor_val_inc};

    dtor_val_n = 0;
    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    EXPECT_EQ_INT(hashtbl_setthreshold(&ht, 0.5f, NULL), 0);
    EXPECT_EQ_INT(dtor_val_n, 0);
    hashtbl_fini(&ht);
    EXPECT_EQ_INT(dtor_val_n, 0);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {hash_int_key, cmp_int_key, dtor_key_inc, dtor_val_inc};
    int k, v;

    dtor_key_n = 0;
    dtor_val_n = 0;
    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k = 1;
    v = 2;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v), 0);
    hashtbl_clear(&ht);
    EXPECT_EQ_INT(dtor_key_n, 1);
    EXPECT_EQ_INT(dtor_val_n, 1);
    EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht), (double)hashtbl_threshold(&ht));
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {hash_int_key, cmp_int_key, NULL, dtor_val_inc};
    int k, v0, v1;

    dtor_val_n = 0;
    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k = 7;
    v0 = 70;
    v1 = 71;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v0), 0);
    EXPECT_EQ_INT(hashtbl_update(&ht, &k, &v1, NULL), 0);
    EXPECT_EQ_INT(dtor_val_n, 1);
    hashtbl_fini(&ht);
    EXPECT_EQ_INT(dtor_val_n, 2);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {hash_int_key, cmp_int_key, NULL, dtor_val_inc};
    int k, v;

    dtor_val_n = 0;
    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k = 3;
    v = 33;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v), 0);
    EXPECT_EQ_INT(hashtbl_remove(&ht, &k, NULL), 0);
    EXPECT_EQ_INT(dtor_val_n, 1);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {hash_int_key, cmp_int_key, NULL, NULL};
    size_t bsz0, bsz1;
    int idx;
    int kbuf[256];
    int vbuf[256];

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    EXPECT_EQ_INT(hashtbl_setthreshold(&ht, 0.5f, NULL), 0);
    kbuf[0] = 0;
    vbuf[0] = 0;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &kbuf[0], &vbuf[0]), 0);
    bsz0 = hashtbl_bucketsz(&ht);
    EXPECT_GT_UINT(bsz0, 0);
    EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht), (double)hashtbl_threshold(&ht));
    for (idx = 1; idx < 256; idx++) {
      kbuf[idx] = idx;
      vbuf[idx] = idx;
      EXPECT_EQ_INT(hashtbl_insert(&ht, &kbuf[idx], &vbuf[idx]), 0);
      EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht), (double)hashtbl_threshold(&ht));
    }
    bsz1 = hashtbl_bucketsz(&ht);
    EXPECT_GT_UINT(bsz1, bsz0);
    EXPECT_EQ_UINT(hashtbl_size(&ht), 256);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {hash_int_key, cmp_int_key, NULL, NULL};
    struct hashtbl_node **bp;
    struct hashtbl_node *n;
    int k;
    int v;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    EXPECT_EQ_PTR(hashtbl_fns(&ht), &fns);
    k = 7;
    v = 77;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v), 0);
    bp = hashtbl_buckets(&ht);
    EXPECT_NOTNULL(bp);
    EXPECT_GT_UINT(hashtbl_bucketsz(&ht), 0);
    EXPECT_EQ_PTR(hashtbl_fns(&ht), &fns);
    n = hashtbl_findnode(&ht, &k);
    EXPECT_NOTNULL(n);
    EXPECT_EQ_PTR(n->key, &k);
    EXPECT_EQ_PTR(n->val, &v);
    EXPECT_NULL(n->next);
    hashtbl_fini(&ht);
  }
}
