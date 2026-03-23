#include <slist.h>
#include <utest.h>

static int dtor_n;
static void dtor_inc(void *p)
{
  (void)p;
  dtor_n++;
}

UTEST_CASE(basic)
{
  {
    struct slist s;

    EXPECT_EQ_INT(slist_init(NULL, NULL), -1);
    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    slist_fini(&s);
  }

  {
    struct slist s;
    void *out;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    EXPECT_TRUE(slist_empty(&s));
    EXPECT_EQ_UINT(slist_size(&s), 0);
    EXPECT_NULL(slist_front(&s));
    EXPECT_NULL(slist_back(&s));
    out = (void *)1;
    EXPECT_EQ_INT(slist_popfront(&s, &out), -1);
    EXPECT_EQ_INT(slist_popfront(&s, NULL), -1);
    slist_fini(&s);
  }

  {
    struct slist s;
    int x;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    x = 10;
    EXPECT_EQ_INT(slist_pushfront(&s, &x), 0);
    EXPECT_FALSE(slist_empty(&s));
    EXPECT_EQ_UINT(slist_size(&s), 1);
    EXPECT_EQ_PTR(slist_front(&s), &x);
    EXPECT_EQ_PTR(slist_back(&s), &x);
    EXPECT_NOTNULL(s.head);
    EXPECT_EQ_PTR(slist_data(s.head), &x);
    EXPECT_NULL(slist_next(s.head));
    slist_fini(&s);
  }

  {
    struct slist s;
    int a, b, c;
    void *out;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    a = 1;
    b = 2;
    c = 3;
    EXPECT_EQ_INT(slist_pushback(&s, &a), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &b), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &c), 0);
    EXPECT_EQ_UINT(slist_size(&s), 3);
    EXPECT_EQ_PTR(slist_front(&s), &a);
    EXPECT_EQ_PTR(slist_back(&s), &c);
    out = NULL;
    EXPECT_EQ_INT(slist_popfront(&s, &out), 0);
    EXPECT_EQ_PTR(out, &a);
    out = NULL;
    EXPECT_EQ_INT(slist_popfront(&s, &out), 0);
    EXPECT_EQ_PTR(out, &b);
    out = NULL;
    EXPECT_EQ_INT(slist_popfront(&s, &out), 0);
    EXPECT_EQ_PTR(out, &c);
    EXPECT_TRUE(slist_empty(&s));
    EXPECT_EQ_INT(slist_popfront(&s, NULL), -1);
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
    EXPECT_EQ_INT(slist_pushback(&s, &a), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &c), 0);
    EXPECT_NOTNULL(s.head);
    EXPECT_EQ_INT(slist_insertn(&s, s.head, &b), 0);
    EXPECT_EQ_PTR(slist_data(s.head), &a);
    EXPECT_NOTNULL(slist_next(s.head));
    EXPECT_EQ_PTR(slist_data(slist_next(s.head)), &b);
    EXPECT_EQ_PTR(slist_back(&s), &c);
    EXPECT_EQ_INT(slist_insertn(&s, NULL, &d), 0);
    EXPECT_EQ_PTR(slist_front(&s), &d);
    EXPECT_EQ_UINT(slist_size(&s), 4);
    out = NULL;
    EXPECT_EQ_INT(slist_removen(&s, NULL, &out), 0);
    EXPECT_EQ_PTR(out, &d);
    EXPECT_EQ_PTR(slist_front(&s), &a);
    EXPECT_NOTNULL(s.head);
    EXPECT_EQ_INT(slist_removen(&s, s.head, &out), 0);
    EXPECT_EQ_PTR(out, &b);
    EXPECT_EQ_UINT(slist_size(&s), 2);
    EXPECT_EQ_PTR(slist_front(&s), &a);
    EXPECT_EQ_PTR(slist_back(&s), &c);
    slist_fini(&s);
  }

  {
    struct slist s;
    int a, b;
    void *out;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    a = 1;
    b = 2;
    EXPECT_EQ_INT(slist_pushback(&s, &a), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &b), 0);
    EXPECT_NOTNULL(s.head);
    out = NULL;
    EXPECT_EQ_INT(slist_removen(&s, s.head, &out), 0);
    EXPECT_EQ_PTR(out, &b);
    EXPECT_EQ_UINT(slist_size(&s), 1);
    EXPECT_EQ_PTR(slist_front(&s), &a);
    EXPECT_EQ_PTR(slist_back(&s), &a);
    EXPECT_NULL(slist_next(s.head));
    slist_fini(&s);
  }

  {
    struct slist s;
    int a, b, c;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    a = 5;
    b = 6;
    c = 7;
    slist_pushback(&s, &a);
    slist_pushback(&s, &b);
    slist_pushback(&s, &c);
    slist_clear(&s);
    EXPECT_TRUE(slist_empty(&s));
    EXPECT_EQ_UINT(slist_size(&s), 0);
    EXPECT_NULL(slist_front(&s));
    EXPECT_NULL(slist_back(&s));
    EXPECT_EQ_INT(slist_pushfront(&s, &a), 0);
    EXPECT_EQ_UINT(slist_size(&s), 1);
    slist_fini(&s);
  }

  {
    struct slist s;
    int a, b, c;
    struct slist_node *n0, *n1;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    a = 11;
    b = 22;
    c = 33;
    EXPECT_EQ_INT(slist_pushback(&s, &a), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &c), 0);
    n0 = s.head;
    EXPECT_NOTNULL(n0);
    EXPECT_EQ_INT(slist_insertn(&s, n0, &b), 0);
    n1 = slist_next(n0);
    EXPECT_NOTNULL(n1);
    EXPECT_EQ_PTR(slist_data(n0), &a);
    EXPECT_EQ_PTR(slist_data(n1), &b);
    EXPECT_EQ_PTR(slist_data(slist_next(n1)), &c);
    EXPECT_NULL(slist_next(slist_next(n1)));
    slist_fini(&s);
  }

  {
    struct slist s;
    int x;

    dtor_n = 0;
    EXPECT_EQ_INT(slist_init(&s, dtor_inc), 0);
    x = 7;
    EXPECT_EQ_INT(slist_pushfront(&s, &x), 0);
    EXPECT_EQ_INT(slist_popfront(&s, NULL), 0);
    EXPECT_EQ_INT(dtor_n, 1);
    slist_fini(&s);
  }

  {
    struct slist s;
    int a, b, c;

    dtor_n = 0;
    EXPECT_EQ_INT(slist_init(&s, dtor_inc), 0);
    a = 1;
    b = 2;
    c = 3;
    slist_pushback(&s, &a);
    slist_pushback(&s, &b);
    slist_pushback(&s, &c);
    slist_clear(&s);
    EXPECT_EQ_INT(dtor_n, 3);
    slist_fini(&s);
  }

  {
    struct slist s;
    int x;
    void *outp;

    dtor_n = 0;
    EXPECT_EQ_INT(slist_init(&s, dtor_inc), 0);
    x = 9;
    slist_pushfront(&s, &x);
    outp = NULL;
    EXPECT_EQ_INT(slist_popfront(&s, &outp), 0);
    EXPECT_EQ_PTR(outp, &x);
    EXPECT_EQ_INT(dtor_n, 0);
    slist_fini(&s);
  }

  {
    struct slist s;
    int a, b;
    void *out;

    dtor_n = 0;
    EXPECT_EQ_INT(slist_init(&s, dtor_inc), 0);
    a = 1;
    b = 2;
    slist_pushback(&s, &a);
    slist_pushback(&s, &b);
    EXPECT_NOTNULL(s.head);
    EXPECT_EQ_INT(slist_removen(&s, s.head, NULL), 0);
    EXPECT_EQ_INT(dtor_n, 1);
    out = NULL;
    EXPECT_EQ_INT(slist_popfront(&s, &out), 0);
    EXPECT_EQ_PTR(out, &a);
    slist_fini(&s);
  }

  {
    int x;
    struct slist_node n;
    struct slist_node *np;
    void *out;
    struct slist s;

    EXPECT_EQ_INT(slist_pushfront(NULL, &x), -1);
    EXPECT_EQ_INT(slist_pushback(NULL, &x), -1);
    out = NULL;
    EXPECT_EQ_INT(slist_popfront(NULL, &out), -1);
    EXPECT_EQ_INT(slist_insertn(NULL, &n, &x), -1);
    EXPECT_EQ_INT(slist_removen(NULL, &n, &out), -1);
    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    EXPECT_EQ_INT(slist_pushfront(&s, NULL), -1);
    EXPECT_EQ_INT(slist_pushback(&s, NULL), -1);
    EXPECT_EQ_INT(slist_insertn(&s, NULL, NULL), -1);
    EXPECT_EQ_INT(slist_removen(&s, NULL, &out), -1);
    slist_fini(&s);
    np = NULL;
    EXPECT_NULL(slist_next(np));
    EXPECT_NULL(slist_data(np));
  }

  {
    struct slist s;
    int a, b;
    void *out;

    dtor_n = 0;
    EXPECT_EQ_INT(slist_init(&s, dtor_inc), 0);
    a = 4;
    b = 5;
    EXPECT_EQ_INT(slist_pushback(&s, &a), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &b), 0);
    out = NULL;
    EXPECT_EQ_INT(slist_removen(&s, NULL, &out), 0);
    EXPECT_EQ_PTR(out, &a);
    EXPECT_EQ_INT(dtor_n, 0);
    EXPECT_EQ_INT(slist_removen(&s, NULL, NULL), 0);
    EXPECT_EQ_INT(dtor_n, 1);
    slist_fini(&s);
  }

  {
    slist_fini(NULL);
    slist_clear(NULL);
  }
}