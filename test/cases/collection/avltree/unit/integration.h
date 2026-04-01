#include <avltree.h>
#include <stdio.h>
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

static void intg_dk_inc_free(void *p)
{
  intg_dk_n++;
  free(p);
}

static void intg_dv_inc_free(void *p)
{
  intg_dv_n++;
  free(p);
}

static int intg_cmp_int_key(void *a, void *b)
{
  int ka = *(int *)a;
  int kb = *(int *)b;
  if (ka < kb)
    return -1;
  if (ka > kb)
    return 1;
  return 0;
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

static int intg_cmp_str(void *a, void *b)
{
  return strcmp((const char *)a, (const char *)b);
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
    struct avltree t;
    struct avltree_fns fns = {intg_cmp_int_key, intg_free_key, intg_free_val};
    int idx;
    int q;
    int *pk;
    int *pv;
    int ins;
    void *fv;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    for (idx = 0; idx < 128; idx++) {
      pk = (int *)malloc(sizeof(int));
      EXPECT_NOTNULL(pk);
      pv = (int *)malloc(sizeof(int));
      if (!pv) {
        free(pk);
      }
      EXPECT_NOTNULL(pv);
      *pk = idx * 4 + 7;
      *pv = idx * 17 + 3;
      ins = avltree_insert(&t, pk, pv);
      if (ins != 0) {
        free(pk);
        free(pv);
      }
      EXPECT_EQ_INT(ins, 0);
    }
    EXPECT_EQ_UINT(avltree_size(&t), 128);
    for (idx = 0; idx < 128; idx++) {
      q = idx * 4 + 7;
      fv = avltree_find(&t, &q);
      EXPECT_NOTNULL(fv);
      EXPECT_EQ_INT(*(int *)fv, idx * 17 + 3);
    }
    q = 4 * 20 + 7;
    EXPECT_EQ_INT(avltree_remove(&t, &q, NULL), 0);
    q = 4 * 40 + 7;
    EXPECT_EQ_INT(avltree_remove(&t, &q, NULL), 0);
    q = 4 * 60 + 7;
    EXPECT_EQ_INT(avltree_remove(&t, &q, NULL), 0);
    EXPECT_EQ_UINT(avltree_size(&t), 125);
    q = 4 * 20 + 7;
    EXPECT_NULL(avltree_findnode(&t, &q));
    pk = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(pk);
    pv = (int *)malloc(sizeof(int));
    if (!pv) {
      free(pk);
    }
    EXPECT_NOTNULL(pv);
    *pk = 4 * 20 + 7;
    *pv = 90909;
    ins = avltree_insert(&t, pk, pv);
    if (ins != 0) {
      free(pk);
      free(pv);
    }
    EXPECT_EQ_INT(ins, 0);
    EXPECT_EQ_UINT(avltree_size(&t), 126);
    q = 4 * 20 + 7;
    fv = avltree_find(&t, &q);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_INT(*(int *)fv, 90909);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {intg_cmp_str, intg_free_key, intg_free_val};
    int idx;
    char *sk;
    int *pv;
    int ins;
    char qbuf[48];
    void *fv;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    for (idx = 0; idx < 120; idx++) {
      sk = (char *)malloc(40);
      EXPECT_NOTNULL(sk);
      snprintf(sk, 40, "pref_%d_suf", idx);
      pv = (int *)malloc(sizeof(int));
      if (!pv) {
        free(sk);
      }
      EXPECT_NOTNULL(pv);
      *pv = idx * 31 - 100;
      ins = avltree_insert(&t, sk, pv);
      if (ins != 0) {
        free(sk);
        free(pv);
      }
      EXPECT_EQ_INT(ins, 0);
    }
    EXPECT_EQ_UINT(avltree_size(&t), 120);
    for (idx = 0; idx < 120; idx++) {
      snprintf(qbuf, sizeof qbuf, "pref_%d_suf", idx);
      fv = avltree_find(&t, qbuf);
      EXPECT_NOTNULL(fv);
      EXPECT_EQ_INT(*(int *)fv, idx * 31 - 100);
    }
    snprintf(qbuf, sizeof qbuf, "pref_%d_suf", 44);
    EXPECT_EQ_INT(avltree_remove(&t, qbuf, NULL), 0);
    snprintf(qbuf, sizeof qbuf, "pref_%d_suf", 88);
    EXPECT_EQ_INT(avltree_remove(&t, qbuf, NULL), 0);
    EXPECT_EQ_UINT(avltree_size(&t), 118);
    snprintf(qbuf, sizeof qbuf, "pref_%d_suf", 44);
    EXPECT_NULL(avltree_findnode(&t, qbuf));
    sk = (char *)malloc(40);
    EXPECT_NOTNULL(sk);
    snprintf(sk, 40, "pref_%d_suf", 44);
    pv = (int *)malloc(sizeof(int));
    if (!pv) {
      free(sk);
    }
    EXPECT_NOTNULL(pv);
    *pv = 44444;
    ins = avltree_insert(&t, sk, pv);
    if (ins != 0) {
      free(sk);
      free(pv);
    }
    EXPECT_EQ_INT(ins, 0);
    snprintf(qbuf, sizeof qbuf, "pref_%d_suf", 44);
    fv = avltree_find(&t, qbuf);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_INT(*(int *)fv, 44444);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {intg_cmp_point, intg_free_key,
                              intg_destroy_val_box};
    int idx;
    struct intg_point lq;
    struct intg_point *pk;
    struct intg_val_box *box;
    int ins;
    void *fv;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    for (idx = 0; idx < 112; idx++) {
      pk = (struct intg_point *)malloc(sizeof(struct intg_point));
      EXPECT_NOTNULL(pk);
      pk->x = idx;
      pk->y = idx * 13 + 5;
      box = (struct intg_val_box *)malloc(sizeof(struct intg_val_box));
      if (!box) {
        free(pk);
      }
      EXPECT_NOTNULL(box);
      box->payload = (int *)malloc(sizeof(int));
      if (!box->payload) {
        free(pk);
        free(box);
      }
      EXPECT_NOTNULL(box->payload);
      *box->payload = idx * 101 + 1;
      ins = avltree_insert(&t, pk, box);
      if (ins != 0) {
        free(box->payload);
        free(box);
        free(pk);
      }
      EXPECT_EQ_INT(ins, 0);
    }
    EXPECT_EQ_UINT(avltree_size(&t), 112);
    for (idx = 0; idx < 112; idx++) {
      lq.x = idx;
      lq.y = idx * 13 + 5;
      fv = avltree_find(&t, &lq);
      EXPECT_NOTNULL(fv);
      EXPECT_EQ_INT(*(int *)((struct intg_val_box *)fv)->payload,
                    idx * 101 + 1);
    }
    lq.x = 33;
    lq.y = 33 * 13 + 5;
    {
      void *oldv;
      struct intg_val_box *bnew;

      bnew = (struct intg_val_box *)malloc(sizeof(struct intg_val_box));
      EXPECT_NOTNULL(bnew);
      bnew->payload = (int *)malloc(sizeof(int));
      if (!bnew->payload) {
        free(bnew);
      }
      EXPECT_NOTNULL(bnew->payload);
      *bnew->payload = 777777;
      EXPECT_EQ_INT(avltree_update(&t, &lq, bnew, &oldv), 0);
      EXPECT_NOTNULL(oldv);
      intg_destroy_val_box(oldv);
    }
    fv = avltree_find(&t, &lq);
    EXPECT_NOTNULL(fv);
    EXPECT_EQ_INT(*(int *)((struct intg_val_box *)fv)->payload, 777777);
    lq.x = 10;
    lq.y = 10 * 13 + 5;
    EXPECT_EQ_INT(avltree_remove(&t, &lq, NULL), 0);
    lq.x = 99;
    lq.y = 99 * 13 + 5;
    EXPECT_EQ_INT(avltree_remove(&t, &lq, NULL), 0);
    EXPECT_EQ_UINT(avltree_size(&t), 110);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {intg_cmp_int_key, intg_free_key, intg_free_val};
    int idx;
    int q;
    int *pk;
    int *pv;
    int ins;
    void *fv;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    for (idx = 0; idx < 144; idx++) {
      pk = (int *)malloc(sizeof(int));
      EXPECT_NOTNULL(pk);
      pv = (int *)malloc(sizeof(int));
      if (!pv) {
        free(pk);
      }
      EXPECT_NOTNULL(pv);
      *pk = 50000 - idx;
      *pv = idx ^ (idx << 1);
      ins = avltree_insert(&t, pk, pv);
      if (ins != 0) {
        free(pk);
        free(pv);
      }
      EXPECT_EQ_INT(ins, 0);
    }
    EXPECT_EQ_UINT(avltree_size(&t), 144);
    for (idx = 0; idx < 144; idx++) {
      q = 50000 - idx;
      fv = avltree_find(&t, &q);
      EXPECT_NOTNULL(fv);
      EXPECT_EQ_INT(*(int *)fv, idx ^ (idx << 1));
    }
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {intg_cmp_int_key, intg_free_key, intg_free_val};
    struct intg_rec *pr;
    int idx;
    int kodd;
    int kfour;
    int *pk;
    int ins;
    void *fv;
    void *oldv;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    for (idx = 0; idx < 100; idx++) {
      pk = (int *)malloc(sizeof(int));
      EXPECT_NOTNULL(pk);
      *pk = idx * 11 + 2;
      pr = (struct intg_rec *)malloc(sizeof(struct intg_rec));
      if (!pr) {
        free(pk);
      }
      EXPECT_NOTNULL(pr);
      pr->id = (long)(idx + 900);
      pr->w = (double)idx * 0.25 + 0.5;
      ins = avltree_insert(&t, pk, pr);
      if (ins != 0) {
        free(pk);
        free(pr);
      }
      EXPECT_EQ_INT(ins, 0);
    }
    EXPECT_EQ_UINT(avltree_size(&t), 100);
    for (idx = 0; idx < 100; idx += 2) {
      kfour = idx * 11 + 2;
      EXPECT_EQ_INT(avltree_remove(&t, &kfour, NULL), 0);
    }
    EXPECT_EQ_UINT(avltree_size(&t), 50);
    for (idx = 1; idx < 100; idx += 2) {
      kodd = idx * 11 + 2;
      pr = (struct intg_rec *)malloc(sizeof(struct intg_rec));
      EXPECT_NOTNULL(pr);
      pr->id = (long)(idx + 7000);
      pr->w = (double)idx * 3.14159;
      EXPECT_EQ_INT(avltree_update(&t, &kodd, pr, &oldv), 0);
      EXPECT_NOTNULL(oldv);
      free(oldv);
    }
    for (idx = 1; idx < 100; idx += 2) {
      kodd = idx * 11 + 2;
      fv = avltree_find(&t, &kodd);
      EXPECT_NOTNULL(fv);
      EXPECT_EQ_INT((int)((struct intg_rec *)fv)->id, idx + 7000);
    }
    for (idx = 1; idx < 100; idx += 4) {
      kodd = idx * 11 + 2;
      EXPECT_EQ_INT(avltree_remove(&t, &kodd, NULL), 0);
    }
    for (idx = 0; idx < 100; idx += 4) {
      kfour = idx * 11 + 2;
      pk = (int *)malloc(sizeof(int));
      EXPECT_NOTNULL(pk);
      *pk = kfour;
      pr = (struct intg_rec *)malloc(sizeof(struct intg_rec));
      if (!pr) {
        free(pk);
      }
      EXPECT_NOTNULL(pr);
      pr->id = (long)kfour;
      pr->w = 1.0;
      ins = avltree_insert(&t, pk, pr);
      if (ins != 0) {
        free(pk);
        free(pr);
      }
      EXPECT_EQ_INT(ins, 0);
    }
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {intg_cmp_point, intg_free_key, intg_free_val};
    int idx;
    struct intg_point lq;
    struct intg_point *pk;
    double *pd;
    int ins;

    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    for (idx = 0; idx < 105; idx++) {
      pk = (struct intg_point *)malloc(sizeof(struct intg_point));
      EXPECT_NOTNULL(pk);
      pk->x = -idx;
      pk->y = idx * idx;
      pd = (double *)malloc(sizeof(double));
      if (!pd) {
        free(pk);
      }
      EXPECT_NOTNULL(pd);
      *pd = (double)idx * 1.75 - 40.5;
      ins = avltree_insert(&t, pk, pd);
      if (ins != 0) {
        free(pk);
        free(pd);
      }
      EXPECT_EQ_INT(ins, 0);
    }
    EXPECT_EQ_UINT(avltree_size(&t), 105);
    for (idx = 0; idx < 105; idx++) {
      lq.x = -idx;
      lq.y = idx * idx;
      pd = (double *)avltree_find(&t, &lq);
      EXPECT_NOTNULL(pd);
      EXPECT_EQ_DOUBLE(*pd, (double)idx * 1.75 - 40.5);
    }
    avltree_clear(&t);
    EXPECT_TRUE(avltree_empty(&t));
    for (idx = 0; idx < 16; idx++) {
      pk = (struct intg_point *)malloc(sizeof(struct intg_point));
      EXPECT_NOTNULL(pk);
      pk->x = idx;
      pk->y = -idx;
      pd = (double *)malloc(sizeof(double));
      if (!pd) {
        free(pk);
      }
      EXPECT_NOTNULL(pd);
      *pd = (double)idx;
      ins = avltree_insert(&t, pk, pd);
      if (ins != 0) {
        free(pk);
        free(pd);
      }
      EXPECT_EQ_INT(ins, 0);
    }
    EXPECT_EQ_UINT(avltree_size(&t), 16);
    avltree_fini(&t);
  }

  {
    struct avltree t;
    struct avltree_fns fns = {intg_cmp_int_key, intg_dk_inc_free,
                              intg_dv_inc_free};
    int idx;
    int q;
    int *pk;
    int *pv;
    int ins;

    intg_dk_n = 0;
    intg_dv_n = 0;
    EXPECT_EQ_INT(avltree_init(&t, &fns), 0);
    for (idx = 0; idx < 110; idx++) {
      pk = (int *)malloc(sizeof(int));
      EXPECT_NOTNULL(pk);
      pv = (int *)malloc(sizeof(int));
      if (!pv) {
        free(pk);
      }
      EXPECT_NOTNULL(pv);
      *pk = idx + 6000;
      *pv = idx;
      ins = avltree_insert(&t, pk, pv);
      if (ins != 0) {
        free(pk);
        free(pv);
      }
      EXPECT_EQ_INT(ins, 0);
    }
    avltree_clear(&t);
    EXPECT_EQ_INT(intg_dk_n, 110);
    EXPECT_EQ_INT(intg_dv_n, 110);
    intg_dk_n = 0;
    intg_dv_n = 0;
    pk = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(pk);
    pv = (int *)malloc(sizeof(int));
    if (!pv) {
      free(pk);
    }
    EXPECT_NOTNULL(pv);
    *pk = 1;
    *pv = 2;
    ins = avltree_insert(&t, pk, pv);
    if (ins != 0) {
      free(pk);
      free(pv);
    }
    EXPECT_EQ_INT(ins, 0);
    q = 1;
    EXPECT_NOTNULL(avltree_find(&t, &q));
    avltree_fini(&t);
    EXPECT_EQ_INT(intg_dk_n, 1);
    EXPECT_EQ_INT(intg_dv_n, 1);
  }
}
