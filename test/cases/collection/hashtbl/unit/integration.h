#include <hashtbl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <utest.h>

struct intg_point {
  int x;
  int y;
};

struct intg_rec {
  long id;
  double w;
};

struct intg_val_box {
  int *payload;
};

static int intg_dk_n;
static int intg_dv_n;

static void intg_dk_inc(void *p)
{
  (void)p;
  intg_dk_n++;
}

static void intg_dv_inc(void *p)
{
  (void)p;
  intg_dv_n++;
}

static uint32_t intg_hash_int(void *k) { return (uint32_t)(*(int *)k); }

static int intg_cmp_int(void *a, void *b)
{
  int ka = *(int *)a;
  int kb = *(int *)b;
  if (ka < kb)
    return -1;
  if (ka > kb)
    return 1;
  return 0;
}

static uint32_t intg_hash_point(void *k)
{
  struct intg_point *p = (struct intg_point *)k;
  return (uint32_t)(p->x * 1315423911u ^ (uint32_t)p->y);
}

static int intg_cmp_point(void *a, void *b)
{
  struct intg_point *pa = (struct intg_point *)a;
  struct intg_point *pb = (struct intg_point *)b;
  if (pa->x != pb->x)
    return pa->x < pb->x ? -1 : 1;
  if (pa->y != pb->y)
    return pa->y < pb->y ? -1 : 1;
  return 0;
}

static uint32_t intg_hash_str(void *k)
{
  const unsigned char *s = (const unsigned char *)k;
  uint32_t h = 5381;
  unsigned c;
  while ((c = *s++) != 0)
    h = h * 33u + c;
  return h;
}

static int intg_cmp_str(void *a, void *b)
{
  return strcmp((const char *)a, (const char *)b);
}

static uint32_t intg_hash_zero(void *k)
{
  (void)k;
  return 0u;
}

static void intg_free_key(void *p) { free(p); }

static void intg_free_val(void *p) { free(p); }

static void intg_destroy_val_box(void *v)
{
  struct intg_val_box *b = (struct intg_val_box *)v;

  free(b->payload);
  free(b);
}

UTEST_CASE(integration)
{
  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {intg_hash_int, intg_cmp_int, NULL, NULL};
    int keys[40];
    int vals[40];
    int idx;
    int q;
    void *fv;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    EXPECT_EQ_INT(hashtbl_setthreshold(&ht, 0.5f, NULL), 0);
    for (idx = 0; idx < 40; idx++) {
      keys[idx] = idx * 3 + 1;
      vals[idx] = idx * 100;
      EXPECT_EQ_INT(hashtbl_insert(&ht, &keys[idx], &vals[idx]), 0);
      EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht),
                       (double)hashtbl_threshold(&ht));
    }
    EXPECT_EQ_UINT(hashtbl_size(&ht), 40);
    for (idx = 0; idx < 40; idx++) {
      q = idx * 3 + 1;
      fv = hashtbl_find(&ht, &q);
      EXPECT_NOTNULL(fv);
      EXPECT_EQ_INT(*(int *)fv, idx * 100);
    }
    q = 4;
    EXPECT_NOTNULL(hashtbl_find(&ht, &q));
    EXPECT_EQ_INT(hashtbl_update(&ht, &keys[1], &vals[5], NULL), 0);
    q = 4;
    fv = hashtbl_find(&ht, &q);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_INT(*(int *)fv, 500);
    EXPECT_EQ_INT(hashtbl_remove(&ht, &keys[0], NULL), 0);
    EXPECT_EQ_INT(hashtbl_remove(&ht, &keys[39], NULL), 0);
    EXPECT_EQ_UINT(hashtbl_size(&ht), 38);
    q = 1;
    EXPECT_NULL(hashtbl_findnode(&ht, &q));
    q = 118;
    EXPECT_NULL(hashtbl_findnode(&ht, &q));
    hashtbl_clear(&ht);
    EXPECT_TRUE(hashtbl_empty(&ht));
    keys[0] = 999;
    vals[0] = -1;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &keys[0], &vals[0]), 0);
    EXPECT_EQ_UINT(hashtbl_size(&ht), 1);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {intg_hash_int, intg_cmp_int, NULL, NULL};
    int ka, kb, kc, va, vb, vc, t;
    void *oldv;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    ka = 1;
    va = 10;
    kb = 2;
    vb = 20;
    kc = 3;
    vc = 30;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &ka, &va), 0);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &kb, &vb), 0);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &kc, &vc), 0);
    t = 200;
    EXPECT_EQ_INT(hashtbl_update(&ht, &kb, &t, &oldv), 0);
    EXPECT_EQ_PTR(oldv, &vb);
    EXPECT_EQ_INT(hashtbl_remove(&ht, &ka, NULL), 0);
    EXPECT_NULL(hashtbl_findnode(&ht, &ka));
    EXPECT_NOTNULL(hashtbl_findnode(&ht, &kc));
    EXPECT_EQ_INT(*(int *)hashtbl_find(&ht, &kb), 200);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &ka, &va), 0);
    EXPECT_EQ_UINT(hashtbl_size(&ht), 3);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {intg_hash_int, intg_cmp_int, intg_free_key,
                              intg_free_val};
    int idx;
    int *pk;
    int *pv;
    int q;
    int ins;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    for (idx = 0; idx < 24; idx++) {
      pk = (int *)malloc(sizeof(int));
      EXPECT_NOTNULL(pk);
      pv = (int *)malloc(sizeof(int));
      if (!pv) {
        free(pk);
      }
      EXPECT_NOTNULL(pv);
      *pk = idx + 500;
      *pv = (idx + 1) * 7;
      ins = hashtbl_insert(&ht, pk, pv);
      if (ins != 0) {
        free(pk);
        free(pv);
      }
      EXPECT_EQ_INT(ins, 0);
      EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht),
                       (double)hashtbl_threshold(&ht));
    }
    for (idx = 0; idx < 24; idx++) {
      q = idx + 500;
      pv = (int *)hashtbl_find(&ht, &q);
      EXPECT_NOTNULL(pv);
      EXPECT_EQ_INT(*pv, (idx + 1) * 7);
    }
    q = 502;
    EXPECT_EQ_INT(hashtbl_remove(&ht, &q, NULL), 0);
    q = 515;
    EXPECT_EQ_INT(hashtbl_remove(&ht, &q, NULL), 0);
    EXPECT_EQ_UINT(hashtbl_size(&ht), 22);
    q = 502;
    EXPECT_NULL(hashtbl_findnode(&ht, &q));
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {intg_hash_int, intg_cmp_int, NULL, intg_free_val};
    int keys[10];
    int idx;
    int *pv;
    int q;
    void *outv;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    for (idx = 0; idx < 10; idx++) {
      keys[idx] = idx;
      pv = (int *)malloc(sizeof(int));
      EXPECT_NOTNULL(pv);
      *pv = idx * 11;
      EXPECT_EQ_INT(hashtbl_insert(&ht, &keys[idx], pv), 0);
      EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht),
                       (double)hashtbl_threshold(&ht));
    }
    q = 5;
    EXPECT_EQ_INT(hashtbl_remove(&ht, &q, &outv), 0);
    EXPECT_NOTNULL(outv);
    EXPECT_EQ_INT(*(int *)outv, 55);
    free(outv);
    q = 3;
    pv = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(pv);
    *pv = 333;
    EXPECT_EQ_INT(hashtbl_update(&ht, &keys[3], pv, NULL), 0);
    q = 3;
    EXPECT_EQ_INT(*(int *)hashtbl_find(&ht, &q), 333);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {intg_hash_point, intg_cmp_point, NULL, NULL};
    struct intg_point pk[6];
    struct intg_rec rv[6];
    struct intg_point lq;
    struct intg_rec *pr;
    int idx;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    for (idx = 0; idx < 6; idx++) {
      pk[idx].x = idx;
      pk[idx].y = idx * 10;
      rv[idx].id = (long)(100 + idx);
      rv[idx].w = (double)idx * 1.5;
      EXPECT_EQ_INT(hashtbl_insert(&ht, &pk[idx], &rv[idx]), 0);
      EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht),
                       (double)hashtbl_threshold(&ht));
    }
    lq.x = 3;
    lq.y = 30;
    pr = (struct intg_rec *)hashtbl_find(&ht, &lq);
    EXPECT_NOTNULL(pr);
    EXPECT_EQ_INT((int)pr->id, 103);
    EXPECT_EQ_DOUBLE(pr->w, 4.5);
    lq.x = 1;
    lq.y = 10;
    EXPECT_EQ_INT(hashtbl_remove(&ht, &lq, NULL), 0);
    lq.x = 1;
    lq.y = 10;
    EXPECT_NULL(hashtbl_findnode(&ht, &lq));
    lq.x = 5;
    lq.y = 50;
    EXPECT_NOTNULL(hashtbl_findnode(&ht, &lq));
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {intg_hash_point, intg_cmp_point, intg_free_key,
                              intg_free_val};
    struct intg_point *pk;
    int *pv;
    struct intg_point lq;
    int idx;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    for (idx = 0; idx < 8; idx++) {
      pk = (struct intg_point *)malloc(sizeof(struct intg_point));
      EXPECT_NOTNULL(pk);
      pv = (int *)malloc(sizeof(int));
      if (!pv) {
        free(pk);
      }
      EXPECT_NOTNULL(pv);
      pk->x = idx;
      pk->y = -idx;
      *pv = idx * 1000;
      EXPECT_EQ_INT(hashtbl_insert(&ht, pk, pv), 0);
    }
    for (idx = 0; idx < 8; idx++) {
      lq.x = idx;
      lq.y = -idx;
      pv = (int *)hashtbl_find(&ht, &lq);
      EXPECT_NOTNULL(pv);
      EXPECT_EQ_INT(*pv, idx * 1000);
    }
    lq.x = 4;
    lq.y = -4;
    EXPECT_EQ_INT(hashtbl_remove(&ht, &lq, NULL), 0);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {intg_hash_str, intg_cmp_str, intg_free_key, NULL};
    char *ks[5];
    int vals[5];
    int idx;
    int qv;
    char *kdup;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    for (idx = 0; idx < 5; idx++) {
      kdup = (char *)malloc(16);
      EXPECT_NOTNULL(kdup);
      memset(kdup, 0, 16);
      kdup[0] = (char)('a' + idx);
      ks[idx] = kdup;
      vals[idx] = idx * 9;
      EXPECT_EQ_INT(hashtbl_insert(&ht, ks[idx], &vals[idx]), 0);
      EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht),
                       (double)hashtbl_threshold(&ht));
    }
    kdup = strdup("c");
    EXPECT_NOTNULL(kdup);
    qv = *(int *)hashtbl_find(&ht, kdup);
    free(kdup);
    EXPECT_EQ_INT(qv, 18);
    kdup = strdup("e");
    EXPECT_NOTNULL(kdup);
    EXPECT_EQ_INT(hashtbl_remove(&ht, kdup, NULL), 0);
    free(kdup);
    EXPECT_EQ_UINT(hashtbl_size(&ht), 4);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {intg_hash_zero, intg_cmp_int, NULL, NULL};
    int keys[20];
    int vals[20];
    int idx;
    int q;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    for (idx = 0; idx < 20; idx++) {
      keys[idx] = idx;
      vals[idx] = idx + 1;
      EXPECT_EQ_INT(hashtbl_insert(&ht, &keys[idx], &vals[idx]), 0);
      EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht),
                       (double)hashtbl_threshold(&ht));
    }
    for (idx = 0; idx < 20; idx += 2)
      EXPECT_EQ_INT(hashtbl_remove(&ht, &keys[idx], NULL), 0);
    EXPECT_EQ_UINT(hashtbl_size(&ht), 10);
    for (idx = 1; idx < 20; idx += 2) {
      q = idx;
      EXPECT_NOTNULL(hashtbl_findnode(&ht, &q));
    }
    for (idx = 0; idx < 20; idx += 2) {
      q = idx;
      EXPECT_NULL(hashtbl_findnode(&ht, &q));
    }
    EXPECT_EQ_INT(hashtbl_insert(&ht, &keys[0], &vals[0]), 0);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &keys[2], &vals[2]), 0);
    EXPECT_EQ_UINT(hashtbl_size(&ht), 12);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {intg_hash_int, intg_cmp_int, NULL,
                              intg_destroy_val_box};
    int k0, k1;
    struct intg_val_box *b0;
    struct intg_val_box *b1;
    void *oldv;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k0 = 1;
    k1 = 2;
    b0 = (struct intg_val_box *)malloc(sizeof(struct intg_val_box));
    EXPECT_NOTNULL(b0);
    b0->payload = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(b0->payload);
    *b0->payload = 111;
    b1 = (struct intg_val_box *)malloc(sizeof(struct intg_val_box));
    EXPECT_NOTNULL(b1);
    b1->payload = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(b1->payload);
    *b1->payload = 222;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k0, b0), 0);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k1, b1), 0);
    b1 = (struct intg_val_box *)malloc(sizeof(struct intg_val_box));
    EXPECT_NOTNULL(b1);
    b1->payload = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(b1->payload);
    *b1->payload = 999;
    EXPECT_EQ_INT(hashtbl_update(&ht, &k1, b1, &oldv), 0);
    EXPECT_NOTNULL(oldv);
    EXPECT_EQ_INT(*(int *)((struct intg_val_box *)oldv)->payload, 222);
    intg_destroy_val_box(oldv);
    EXPECT_EQ_INT(
        *(int *)((struct intg_val_box *)hashtbl_find(&ht, &k1))->payload, 999);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {intg_hash_int, intg_cmp_int, intg_dk_inc,
                              intg_dv_inc};
    int k, v;

    intg_dk_n = 0;
    intg_dv_n = 0;
    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k = 7;
    v = 70;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v), 0);
    EXPECT_EQ_INT(hashtbl_setthreshold(&ht, 0.55f, NULL), 0);
    k = 8;
    v = 80;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v), 0);
    EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht),
                     (double)hashtbl_threshold(&ht));
    hashtbl_clear(&ht);
    EXPECT_EQ_INT(intg_dk_n, 2);
    EXPECT_EQ_INT(intg_dv_n, 2);
    k = 1;
    v = 2;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v), 0);
    EXPECT_EQ_UINT(hashtbl_size(&ht), 1);
    hashtbl_fini(&ht);
    EXPECT_EQ_INT(intg_dk_n, 3);
    EXPECT_EQ_INT(intg_dv_n, 3);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {intg_hash_int, intg_cmp_int, NULL, NULL};
    int keys[15];
    int vals[15];
    int idx;
    int q;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    for (idx = 0; idx < 15; idx++) {
      keys[idx] = idx;
      vals[idx] = idx;
      EXPECT_EQ_INT(hashtbl_insert(&ht, &keys[idx], &vals[idx]), 0);
    }
    for (idx = 0; idx < 15; idx += 3)
      EXPECT_EQ_INT(hashtbl_remove(&ht, &keys[idx], NULL), 0);
    EXPECT_EQ_UINT(hashtbl_size(&ht), 10);
    for (idx = 1; idx < 15; idx += 3) {
      q = idx;
      EXPECT_EQ_INT(hashtbl_update(&ht, &q, &vals[idx], NULL), 0);
    }
    q = 1;
    EXPECT_EQ_PTR(hashtbl_find(&ht, &q), &vals[1]);
    hashtbl_clear(&ht);
    for (idx = 0; idx < 6; idx++) {
      keys[idx] = idx + 200;
      vals[idx] = idx;
      EXPECT_EQ_INT(hashtbl_insert(&ht, &keys[idx], &vals[idx]), 0);
      EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht),
                       (double)hashtbl_threshold(&ht));
    }
    EXPECT_EQ_UINT(hashtbl_size(&ht), 6);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {intg_hash_int, intg_cmp_int, NULL, NULL};
    int k1, k2, k3;
    double d1, d2a, d2b, d3;
    void *fv;
    void *oldv;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k1 = 101;
    d1 = 2.25;
    k2 = 102;
    d2a = -0.125;
    d2b = 3.5;
    k3 = 103;
    d3 = 1.0e9;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k1, &d1), 0);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k2, &d2a), 0);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k3, &d3), 0);
    fv = hashtbl_find(&ht, &k2);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_DOUBLE(*(double *)fv, -0.125);
    EXPECT_EQ_INT(hashtbl_update(&ht, &k2, &d2b, &oldv), 0);
    EXPECT_NOTNULL(oldv);
    EXPECT_EQ_PTR(oldv, &d2a);
    EXPECT_EQ_DOUBLE(*(double *)oldv, -0.125);
    fv = hashtbl_find(&ht, &k2);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_DOUBLE(*(double *)fv, 3.5);
    EXPECT_EQ_INT(hashtbl_remove(&ht, &k1, NULL), 0);
    EXPECT_NULL(hashtbl_findnode(&ht, &k1));
    EXPECT_NOTNULL(hashtbl_findnode(&ht, &k3));
    hashtbl_fini(&ht);
  }
}
