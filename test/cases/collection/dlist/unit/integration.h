#include <dlist.h>
#include <stdlib.h>
#include <utest.h>

struct intg_pair {
  int x;
  int y;
};

struct intg_nested {
  short tag;
  int id;
};

struct intg_box {
  int *p;
};

struct intg_wrap {
  struct intg_box *box;
};

static int intg_dtor_n;
static void intg_dtor_inc(void *p)
{
  (void)p;
  intg_dtor_n++;
}

static int intg_free_n;
static void intg_free_ptr(void *p)
{
  free(p);
  intg_free_n++;
}

static int intg_wrap_dtor_n;
static void intg_wrap_dtor(void *p)
{
  struct intg_wrap *w = (struct intg_wrap *)p;
  if (!w)
    return;
  if (w->box) {
    free(w->box->p);
    w->box->p = NULL;
    free(w->box);
    w->box = NULL;
  }
  free(w);
  intg_wrap_dtor_n++;
}

UTEST_CASE(integration)
{
  {
    struct dlist d;
    int a, b, c, e;
    struct dlist_node *n0, *n1;
    void *out;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    a = 1;
    b = 2;
    c = 3;
    e = 5;
    EXPECT_EQ_INT(dlist_pushback(&d, &a), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, &c), 0);
    n0 = d.tail;
    EXPECT_NOTNULL(n0);
    EXPECT_EQ_INT(dlist_insert(&d, n0, &b), 0);
    EXPECT_EQ_INT(dlist_pushfront(&d, &e), 0);
    EXPECT_EQ_UINT(dlist_size(&d), 4);
    EXPECT_EQ_PTR(dlist_front(&d), &e);
    EXPECT_EQ_PTR(dlist_back(&d), &c);
    n0 = d.head;
    n1 = dlist_next(n0);
    EXPECT_NOTNULL(n1);
    EXPECT_EQ_PTR(dlist_data(n0), &e);
    EXPECT_EQ_PTR(dlist_data(n1), &a);
    EXPECT_EQ_PTR(dlist_data(dlist_next(n1)), &b);
    EXPECT_EQ_PTR(dlist_data(d.tail), &c);
    out = NULL;
    EXPECT_EQ_INT(dlist_popfront(&d, &out), 0);
    EXPECT_EQ_PTR(out, &e);
    EXPECT_EQ_INT(dlist_remove(&d, d.tail, &out), 0);
    EXPECT_EQ_PTR(out, &c);
    EXPECT_EQ_UINT(dlist_size(&d), 2);
    EXPECT_EQ_PTR(dlist_front(&d), &a);
    EXPECT_EQ_PTR(dlist_back(&d), &b);
    dlist_fini(&d);
  }

  {
    struct dlist d;
    int a, b, c, d0;
    struct dlist_node *n0, *n1;
    void *out;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    a = 10;
    b = 20;
    c = 30;
    d0 = 40;
    EXPECT_EQ_INT(dlist_pushfront(&d, &c), 0);
    EXPECT_EQ_INT(dlist_pushfront(&d, &b), 0);
    EXPECT_EQ_INT(dlist_pushfront(&d, &a), 0);
    n0 = d.tail;
    EXPECT_NOTNULL(n0);
    EXPECT_EQ_INT(dlist_insert(&d, n0, &d0), 0);
    EXPECT_EQ_UINT(dlist_size(&d), 4);
    EXPECT_EQ_PTR(dlist_front(&d), &a);
    EXPECT_EQ_PTR(dlist_back(&d), &c);
    n1 = dlist_prev(d.tail);
    EXPECT_NOTNULL(n1);
    EXPECT_EQ_PTR(dlist_data(n1), &d0);
    out = NULL;
    EXPECT_EQ_INT(dlist_remove(&d, n1, &out), 0);
    EXPECT_EQ_PTR(out, &d0);
    EXPECT_EQ_INT(dlist_popback(&d, &out), 0);
    EXPECT_EQ_PTR(out, &c);
    EXPECT_EQ_INT(dlist_popfront(&d, &out), 0);
    EXPECT_EQ_PTR(out, &a);
    EXPECT_EQ_PTR(dlist_front(&d), &b);
    EXPECT_EQ_PTR(dlist_back(&d), &b);
    dlist_fini(&d);
  }

  {
    struct dlist d;
    int a, b, c;
    void *out;

    intg_dtor_n = 0;
    EXPECT_EQ_INT(dlist_init(&d, intg_dtor_inc), 0);
    a = 7;
    b = 8;
    c = 9;
    EXPECT_EQ_INT(dlist_pushback(&d, &a), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, &b), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, &c), 0);
    out = NULL;
    EXPECT_EQ_INT(dlist_popfront(&d, &out), 0);
    EXPECT_EQ_PTR(out, &a);
    EXPECT_EQ_INT(intg_dtor_n, 0);
    EXPECT_EQ_INT(dlist_remove(&d, d.head, NULL), 0);
    EXPECT_EQ_INT(intg_dtor_n, 1);
    EXPECT_EQ_INT(dlist_popback(&d, NULL), 0);
    EXPECT_EQ_INT(intg_dtor_n, 2);
    EXPECT_TRUE(dlist_empty(&d));
    dlist_fini(&d);
    EXPECT_EQ_INT(intg_dtor_n, 2);
  }

  {
    struct dlist d;
    int *p0, *p1, *p2;
    void *out;

    intg_free_n = 0;
    EXPECT_EQ_INT(dlist_init(&d, intg_free_ptr), 0);
    p0 = (int *)malloc(sizeof(int));
    p1 = (int *)malloc(sizeof(int));
    p2 = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(p0);
    EXPECT_NOTNULL(p1);
    EXPECT_NOTNULL(p2);
    *p0 = 100;
    *p1 = 200;
    *p2 = 300;
    EXPECT_EQ_INT(dlist_pushback(&d, p0), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, p1), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, p2), 0);
    out = NULL;
    EXPECT_EQ_INT(dlist_popfront(&d, &out), 0);
    EXPECT_EQ_PTR(out, p0);
    EXPECT_EQ_INT(*(int *)out, 100);
    EXPECT_EQ_INT(intg_free_n, 0);
    free(out);
    EXPECT_EQ_INT(dlist_remove(&d, d.tail, NULL), 0);
    EXPECT_EQ_INT(intg_free_n, 1);
    EXPECT_EQ_PTR(dlist_front(&d), p1);
    EXPECT_EQ_PTR(dlist_back(&d), p1);
    dlist_fini(&d);
    EXPECT_EQ_INT(intg_free_n, 2);
  }

  {
    struct dlist d;
    int a, b, c, d0, e;
    struct dlist_node *n0;
    void *out;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    a = 11;
    b = 22;
    c = 33;
    d0 = 44;
    e = 55;
    EXPECT_EQ_INT(dlist_pushback(&d, &c), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, &e), 0);
    EXPECT_EQ_INT(dlist_insert(&d, d.head, &b), 0);
    EXPECT_EQ_INT(dlist_insert(&d, d.head, &a), 0);
    EXPECT_EQ_INT(dlist_insert(&d, d.tail, &d0), 0);
    EXPECT_EQ_UINT(dlist_size(&d), 5);
    n0 = d.head;
    EXPECT_EQ_PTR(dlist_data(n0), &a);
    EXPECT_EQ_PTR(dlist_data(dlist_next(n0)), &b);
    EXPECT_EQ_PTR(dlist_data(dlist_next(dlist_next(n0))), &c);
    EXPECT_EQ_PTR(dlist_data(dlist_next(dlist_next(dlist_next(n0)))), &d0);
    EXPECT_EQ_PTR(dlist_data(d.tail), &e);
    out = NULL;
    EXPECT_EQ_INT(dlist_remove(&d, d.head, &out), 0);
    EXPECT_EQ_PTR(out, &a);
    EXPECT_EQ_INT(dlist_remove(&d, d.tail, &out), 0);
    EXPECT_EQ_PTR(out, &e);
    EXPECT_EQ_INT(dlist_popback(&d, &out), 0);
    EXPECT_EQ_PTR(out, &d0);
    EXPECT_EQ_PTR(dlist_front(&d), &b);
    EXPECT_EQ_PTR(dlist_back(&d), &c);
    dlist_fini(&d);
  }

  {
    struct dlist d;
    struct intg_pair a, b, c;
    void *out;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    a.x = 1;
    a.y = 10;
    b.x = 2;
    b.y = 20;
    c.x = 3;
    c.y = 30;
    EXPECT_EQ_INT(dlist_pushback(&d, &a), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, &c), 0);
    EXPECT_EQ_INT(dlist_insert(&d, d.tail, &b), 0);
    EXPECT_EQ_UINT(dlist_size(&d), 3);
    EXPECT_EQ_INT(((struct intg_pair *)dlist_front(&d))->x, 1);
    EXPECT_EQ_INT(((struct intg_pair *)dlist_data(dlist_next(d.head)))->x, 2);
    EXPECT_EQ_INT(((struct intg_pair *)dlist_back(&d))->x, 3);
    out = NULL;
    EXPECT_EQ_INT(dlist_remove(&d, dlist_next(d.head), &out), 0);
    EXPECT_NOTNULL(out);
    EXPECT_EQ_INT(((struct intg_pair *)out)->x, 2);
    EXPECT_EQ_INT(((struct intg_pair *)out)->y, 20);
    EXPECT_EQ_UINT(dlist_size(&d), 2);
    EXPECT_EQ_INT(((struct intg_pair *)dlist_front(&d))->x, 1);
    EXPECT_EQ_INT(((struct intg_pair *)dlist_back(&d))->x, 3);
    dlist_fini(&d);
  }

  {
    struct dlist d;
    struct intg_nested n0, n1, n2;
    void *out;

    EXPECT_EQ_INT(dlist_init(&d, NULL), 0);
    n0.tag = 7;
    n0.id = 700;
    n1.tag = 8;
    n1.id = 800;
    n2.tag = 9;
    n2.id = 900;
    EXPECT_EQ_INT(dlist_pushfront(&d, &n1), 0);
    EXPECT_EQ_INT(dlist_insert(&d, d.head, &n0), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, &n2), 0);
    EXPECT_EQ_UINT(dlist_size(&d), 3);
    EXPECT_EQ_INT((int)((struct intg_nested *)dlist_front(&d))->tag, 7);
    EXPECT_EQ_INT(((struct intg_nested *)dlist_data(dlist_next(d.head)))->id,
                  800);
    EXPECT_EQ_INT(((struct intg_nested *)dlist_back(&d))->id, 900);
    out = NULL;
    EXPECT_EQ_INT(dlist_popback(&d, &out), 0);
    EXPECT_EQ_INT(((struct intg_nested *)out)->id, 900);
    EXPECT_EQ_INT(dlist_popfront(&d, &out), 0);
    EXPECT_EQ_INT(((struct intg_nested *)out)->id, 700);
    EXPECT_EQ_PTR(dlist_front(&d), &n1);
    dlist_fini(&d);
  }

  {
    struct dlist d;
    struct intg_wrap *w0, *w1, *w2;
    void *out;

    intg_wrap_dtor_n = 0;
    EXPECT_EQ_INT(dlist_init(&d, intg_wrap_dtor), 0);
    w0 = (struct intg_wrap *)malloc(sizeof(struct intg_wrap));
    w1 = (struct intg_wrap *)malloc(sizeof(struct intg_wrap));
    w2 = (struct intg_wrap *)malloc(sizeof(struct intg_wrap));
    EXPECT_NOTNULL(w0);
    EXPECT_NOTNULL(w1);
    EXPECT_NOTNULL(w2);
    w0->box = (struct intg_box *)malloc(sizeof(struct intg_box));
    w1->box = (struct intg_box *)malloc(sizeof(struct intg_box));
    w2->box = (struct intg_box *)malloc(sizeof(struct intg_box));
    EXPECT_NOTNULL(w0->box);
    EXPECT_NOTNULL(w1->box);
    EXPECT_NOTNULL(w2->box);
    w0->box->p = (int *)malloc(sizeof(int));
    w1->box->p = (int *)malloc(sizeof(int));
    w2->box->p = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(w0->box->p);
    EXPECT_NOTNULL(w1->box->p);
    EXPECT_NOTNULL(w2->box->p);
    *w0->box->p = 11;
    *w1->box->p = 22;
    *w2->box->p = 33;
    EXPECT_EQ_INT(dlist_pushback(&d, w0), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, w1), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, w2), 0);
    out = NULL;
    EXPECT_EQ_INT(dlist_popfront(&d, &out), 0);
    EXPECT_EQ_INT(intg_wrap_dtor_n, 0);
    EXPECT_NOTNULL(out);
    EXPECT_EQ_INT(*((struct intg_wrap *)out)->box->p, 11);
    free(((struct intg_wrap *)out)->box->p);
    free(((struct intg_wrap *)out)->box);
    free(out);
    EXPECT_EQ_INT(dlist_remove(&d, d.tail, NULL), 0);
    EXPECT_EQ_INT(intg_wrap_dtor_n, 1);
    dlist_fini(&d);
    EXPECT_EQ_INT(intg_wrap_dtor_n, 2);
  }

  {
    struct dlist d;
    struct intg_wrap *w0, *w1, *w2;
    struct dlist_node *mid;
    void *out;

    intg_wrap_dtor_n = 0;
    EXPECT_EQ_INT(dlist_init(&d, intg_wrap_dtor), 0);
    w0 = (struct intg_wrap *)malloc(sizeof(struct intg_wrap));
    w1 = (struct intg_wrap *)malloc(sizeof(struct intg_wrap));
    w2 = (struct intg_wrap *)malloc(sizeof(struct intg_wrap));
    EXPECT_NOTNULL(w0);
    EXPECT_NOTNULL(w1);
    EXPECT_NOTNULL(w2);
    w0->box = (struct intg_box *)malloc(sizeof(struct intg_box));
    w1->box = (struct intg_box *)malloc(sizeof(struct intg_box));
    w2->box = (struct intg_box *)malloc(sizeof(struct intg_box));
    EXPECT_NOTNULL(w0->box);
    EXPECT_NOTNULL(w1->box);
    EXPECT_NOTNULL(w2->box);
    w0->box->p = (int *)malloc(sizeof(int));
    w1->box->p = (int *)malloc(sizeof(int));
    w2->box->p = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(w0->box->p);
    EXPECT_NOTNULL(w1->box->p);
    EXPECT_NOTNULL(w2->box->p);
    *w0->box->p = 101;
    *w1->box->p = 202;
    *w2->box->p = 303;
    EXPECT_EQ_INT(dlist_pushback(&d, w0), 0);
    EXPECT_EQ_INT(dlist_pushback(&d, w2), 0);
    EXPECT_EQ_INT(dlist_insert(&d, d.tail, w1), 0);
    mid = dlist_next(d.head);
    EXPECT_NOTNULL(mid);
    EXPECT_EQ_INT(*((struct intg_wrap *)dlist_data(mid))->box->p, 202);
    out = NULL;
    EXPECT_EQ_INT(dlist_remove(&d, mid, &out), 0);
    EXPECT_EQ_INT(intg_wrap_dtor_n, 0);
    EXPECT_EQ_INT(*((struct intg_wrap *)out)->box->p, 202);
    free(((struct intg_wrap *)out)->box->p);
    free(((struct intg_wrap *)out)->box);
    free(out);
    dlist_clear(&d);
    EXPECT_EQ_INT(intg_wrap_dtor_n, 2);
    EXPECT_TRUE(dlist_empty(&d));
    dlist_fini(&d);
    EXPECT_EQ_INT(intg_wrap_dtor_n, 2);
  }
}
