#include <slist.h>
#include <stdlib.h>
#include <utest.h>

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

UTEST_CASE(integration)
{
  {
    struct slist s;
    int a, b, c, d, e;
    struct slist_node *n0, *n1;
    void *out;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    a = 10;
    b = 20;
    c = 30;
    d = 40;
    e = 50;
    EXPECT_EQ_INT(slist_pushback(&s, &a), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &c), 0);
    n0 = s.head;
    EXPECT_NOTNULL(n0);
    EXPECT_EQ_INT(slist_insertn(&s, n0, &b), 0);
    EXPECT_EQ_INT(slist_pushfront(&s, &d), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &e), 0);
    EXPECT_EQ_UINT(slist_size(&s), 5);
    EXPECT_EQ_PTR(slist_front(&s), &d);
    EXPECT_EQ_PTR(slist_back(&s), &e);
    n0 = s.head;
    EXPECT_NOTNULL(n0);
    n1 = slist_next(n0);
    EXPECT_NOTNULL(n1);
    EXPECT_EQ_PTR(slist_data(n0), &d);
    EXPECT_EQ_PTR(slist_data(n1), &a);
    EXPECT_EQ_PTR(slist_data(slist_next(n1)), &b);
    out = NULL;
    EXPECT_EQ_INT(slist_popfront(&s, &out), 0);
    EXPECT_EQ_PTR(out, &d);
    n0 = s.head;
    EXPECT_NOTNULL(n0);
    EXPECT_EQ_INT(slist_removen(&s, n0, &out), 0);
    EXPECT_EQ_PTR(out, &b);
    EXPECT_EQ_UINT(slist_size(&s), 3);
    EXPECT_EQ_PTR(slist_front(&s), &a);
    EXPECT_EQ_PTR(slist_back(&s), &e);
    slist_fini(&s);
  }

  {
    struct slist s;
    int a, b, c, d;
    void *out;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    a = 1;
    b = 2;
    c = 3;
    d = 4;
    EXPECT_EQ_INT(slist_pushfront(&s, &b), 0);
    EXPECT_EQ_INT(slist_pushfront(&s, &a), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &d), 0);
    EXPECT_EQ_INT(slist_insertn(&s, s.head, &c), 0);
    EXPECT_EQ_UINT(slist_size(&s), 4);
    EXPECT_EQ_PTR(slist_front(&s), &a);
    EXPECT_EQ_PTR(slist_back(&s), &d);
    out = NULL;
    EXPECT_EQ_INT(slist_removen(&s, NULL, &out), 0);
    EXPECT_EQ_PTR(out, &a);
    EXPECT_EQ_INT(slist_removen(&s, NULL, &out), 0);
    EXPECT_EQ_PTR(out, &c);
    EXPECT_EQ_UINT(slist_size(&s), 2);
    EXPECT_EQ_PTR(slist_front(&s), &b);
    EXPECT_EQ_PTR(slist_back(&s), &d);
    slist_clear(&s);
    EXPECT_TRUE(slist_empty(&s));
    EXPECT_EQ_INT(slist_pushback(&s, &a), 0);
    EXPECT_EQ_INT(slist_pushfront(&s, &d), 0);
    EXPECT_EQ_UINT(slist_size(&s), 2);
    EXPECT_EQ_PTR(slist_front(&s), &d);
    EXPECT_EQ_PTR(slist_back(&s), &a);
    slist_fini(&s);
  }

  {
    struct slist s;
    int a, b, c;
    struct slist_node *n0;
    void *out;

    intg_dtor_n = 0;
    EXPECT_EQ_INT(slist_init(&s, intg_dtor_inc), 0);
    a = 7;
    b = 8;
    c = 9;
    EXPECT_EQ_INT(slist_pushback(&s, &a), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &b), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &c), 0);
    out = NULL;
    EXPECT_EQ_INT(slist_popfront(&s, &out), 0);
    EXPECT_EQ_PTR(out, &a);
    EXPECT_EQ_INT(intg_dtor_n, 0);
    n0 = s.head;
    EXPECT_NOTNULL(n0);
    EXPECT_EQ_INT(slist_removen(&s, n0, NULL), 0);
    EXPECT_EQ_INT(intg_dtor_n, 1);
    EXPECT_EQ_UINT(slist_size(&s), 1);
    EXPECT_EQ_PTR(slist_front(&s), &b);
    EXPECT_EQ_PTR(slist_back(&s), &b);
    slist_clear(&s);
    EXPECT_EQ_INT(intg_dtor_n, 2);
    EXPECT_TRUE(slist_empty(&s));
    slist_fini(&s);
    EXPECT_EQ_INT(intg_dtor_n, 2);
  }

  {
    struct slist s;
    int *p0, *p1, *p2;
    void *out;

    intg_free_n = 0;
    EXPECT_EQ_INT(slist_init(&s, intg_free_ptr), 0);
    p0 = (int *)malloc(sizeof(int));
    p1 = (int *)malloc(sizeof(int));
    p2 = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(p0);
    EXPECT_NOTNULL(p1);
    EXPECT_NOTNULL(p2);
    *p0 = 100;
    *p1 = 200;
    *p2 = 300;
    EXPECT_EQ_INT(slist_pushback(&s, p0), 0);
    EXPECT_EQ_INT(slist_pushback(&s, p1), 0);
    EXPECT_EQ_INT(slist_pushback(&s, p2), 0);
    out = NULL;
    EXPECT_EQ_INT(slist_popfront(&s, &out), 0);
    EXPECT_EQ_PTR(out, p0);
    EXPECT_EQ_INT(*(int *)out, 100);
    EXPECT_EQ_INT(intg_free_n, 0);
    free(out);
    EXPECT_EQ_INT(slist_removen(&s, NULL, NULL), 0);
    EXPECT_EQ_INT(intg_free_n, 1);
    EXPECT_EQ_PTR(slist_front(&s), p2);
    slist_fini(&s);
    EXPECT_EQ_INT(intg_free_n, 2);
  }

  {
    struct slist s;
    int a, b, c, d;
    struct slist_node *n0, *n1;
    void *out;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    a = 1;
    b = 2;
    c = 3;
    d = 4;
    EXPECT_EQ_INT(slist_pushback(&s, &a), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &d), 0);
    n0 = s.head;
    EXPECT_NOTNULL(n0);
    EXPECT_EQ_INT(slist_insertn(&s, n0, &b), 0);
    n1 = slist_next(n0);
    EXPECT_NOTNULL(n1);
    EXPECT_EQ_INT(slist_insertn(&s, n1, &c), 0);
    EXPECT_EQ_UINT(slist_size(&s), 4);
    EXPECT_EQ_PTR(slist_data(s.head), &a);
    EXPECT_EQ_PTR(slist_data(slist_next(s.head)), &b);
    EXPECT_EQ_PTR(slist_data(slist_next(slist_next(s.head))), &c);
    EXPECT_EQ_PTR(slist_back(&s), &d);
    out = NULL;
    EXPECT_EQ_INT(slist_removen(&s, s.head, &out), 0);
    EXPECT_EQ_PTR(out, &b);
    EXPECT_EQ_INT(slist_removen(&s, s.head, &out), 0);
    EXPECT_EQ_PTR(out, &c);
    EXPECT_EQ_UINT(slist_size(&s), 2);
    EXPECT_EQ_PTR(slist_front(&s), &a);
    EXPECT_EQ_PTR(slist_back(&s), &d);
    slist_fini(&s);
  }

  {
    struct slist s;
    int a, b, c;
    struct slist_node *n0;
    void *out;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    a = 10;
    b = 20;
    c = 30;
    EXPECT_EQ_INT(slist_pushback(&s, &a), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &b), 0);
    n0 = s.head;
    EXPECT_NOTNULL(n0);
    out = NULL;
    EXPECT_EQ_INT(slist_insertn(&s, n0, &c), 0);
    EXPECT_EQ_UINT(slist_size(&s), 3);
    EXPECT_EQ_INT(slist_removen(&s, n0, &out), 0);
    EXPECT_EQ_PTR(out, &c);
    EXPECT_EQ_INT(slist_popfront(&s, &out), 0);
    EXPECT_EQ_PTR(out, &a);
    EXPECT_EQ_INT(slist_popfront(&s, &out), 0);
    EXPECT_EQ_PTR(out, &b);
    EXPECT_EQ_INT(slist_popfront(&s, &out), -1);
    EXPECT_TRUE(slist_empty(&s));
    slist_fini(&s);
  }

  {
    struct slist s;
    int a, b, c, d, e;
    void *out;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    a = 1;
    b = 2;
    c = 3;
    d = 4;
    e = 5;
    EXPECT_EQ_INT(slist_pushfront(&s, &c), 0);
    EXPECT_EQ_INT(slist_pushfront(&s, &b), 0);
    EXPECT_EQ_INT(slist_pushfront(&s, &a), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &d), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &e), 0);
    EXPECT_EQ_UINT(slist_size(&s), 5);
    EXPECT_EQ_PTR(slist_front(&s), &a);
    EXPECT_EQ_PTR(slist_back(&s), &e);
    out = NULL;
    EXPECT_EQ_INT(slist_popfront(&s, &out), 0);
    EXPECT_EQ_PTR(out, &a);
    EXPECT_EQ_INT(slist_removen(&s, s.head, &out), 0);
    EXPECT_EQ_PTR(out, &c);
    EXPECT_EQ_INT(slist_removen(&s, s.head, &out), 0);
    EXPECT_EQ_PTR(out, &d);
    EXPECT_EQ_UINT(slist_size(&s), 2);
    EXPECT_EQ_PTR(slist_front(&s), &b);
    EXPECT_EQ_PTR(slist_back(&s), &e);
    EXPECT_EQ_INT(slist_popfront(&s, &out), 0);
    EXPECT_EQ_PTR(out, &b);
    EXPECT_EQ_INT(slist_popfront(&s, &out), 0);
    EXPECT_EQ_PTR(out, &e);
    EXPECT_TRUE(slist_empty(&s));
    slist_fini(&s);
  }

  {
    struct slist s;
    int a, b, c, d;
    struct slist_node *n0, *n1;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    a = 11;
    b = 22;
    c = 33;
    d = 44;
    EXPECT_EQ_INT(slist_pushback(&s, &a), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &d), 0);
    n0 = s.head;
    EXPECT_NOTNULL(n0);
    EXPECT_EQ_INT(slist_insertn(&s, n0, &b), 0);
    n1 = slist_next(n0);
    EXPECT_NOTNULL(n1);
    EXPECT_EQ_INT(slist_insertn(&s, n1, &c), 0);
    EXPECT_EQ_UINT(slist_size(&s), 4);
    EXPECT_EQ_PTR(slist_data(s.head), &a);
    EXPECT_EQ_PTR(slist_data(slist_next(s.head)), &b);
    EXPECT_EQ_PTR(slist_data(slist_next(slist_next(s.head))), &c);
    EXPECT_EQ_PTR(slist_data(slist_next(slist_next(slist_next(s.head)))), &d);
    EXPECT_NULL(slist_next(slist_next(slist_next(slist_next(s.head)))));
    slist_fini(&s);
  }

  {
    struct slist s;
    int a, b, c;
    void *out;

    intg_dtor_n = 0;
    EXPECT_EQ_INT(slist_init(&s, intg_dtor_inc), 0);
    a = 101;
    b = 202;
    c = 303;
    EXPECT_EQ_INT(slist_pushback(&s, &a), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &b), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &c), 0);
    out = NULL;
    EXPECT_EQ_INT(slist_removen(&s, NULL, &out), 0);
    EXPECT_EQ_PTR(out, &a);
    EXPECT_EQ_INT(intg_dtor_n, 0);
    EXPECT_EQ_INT(slist_removen(&s, NULL, NULL), 0);
    EXPECT_EQ_INT(intg_dtor_n, 1);
    EXPECT_EQ_INT(slist_popfront(&s, NULL), 0);
    EXPECT_EQ_INT(intg_dtor_n, 2);
    EXPECT_TRUE(slist_empty(&s));
    slist_fini(&s);
    EXPECT_EQ_INT(intg_dtor_n, 2);
  }
}
