#include <slist.h>
#include <utest.h>

static int edg_dtor_n;
static void edg_dtor_inc(void *p)
{
  (void)p;
  edg_dtor_n++;
}

UTEST_CASE(edge)
{
  {
    struct slist s;
    void *out;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    out = (void *)1;
    EXPECT_EQ_INT(slist_popfront(&s, &out), -1);
    EXPECT_EQ_INT(slist_removen(&s, NULL, &out), -1);
    EXPECT_TRUE(slist_empty(&s));
    EXPECT_NULL(slist_front(&s));
    EXPECT_NULL(slist_back(&s));
    slist_fini(&s);
  }

  {
    struct slist s;
    int x;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    x = 1;
    EXPECT_EQ_INT(slist_pushfront(&s, &x), 0);
    EXPECT_EQ_INT(slist_popfront(&s, NULL), 0);
    EXPECT_TRUE(slist_empty(&s));
    EXPECT_NULL(slist_front(&s));
    EXPECT_NULL(slist_back(&s));
    EXPECT_EQ_INT(slist_popfront(&s, NULL), -1);
    slist_fini(&s);
  }

  {
    struct slist s;
    int x;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    x = 1;
    EXPECT_EQ_INT(slist_pushback(&s, &x), 0);
    slist_clear(&s);
    EXPECT_TRUE(slist_empty(&s));
    EXPECT_EQ_INT(slist_popfront(&s, NULL), -1);
    slist_fini(&s);
  }

  {
    struct slist s;
    int x;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    x = 1;
    EXPECT_EQ_INT(slist_pushback(&s, &x), 0);
    slist_clear(&s);
    slist_clear(&s);
    EXPECT_TRUE(slist_empty(&s));
    EXPECT_EQ_UINT(slist_size(&s), 0);
    slist_fini(&s);
  }

  {
    struct slist s;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    slist_clear(&s);
    slist_clear(&s);
    EXPECT_TRUE(slist_empty(&s));
    slist_fini(&s);
  }

  {
    struct slist s;
    int a, b;
    void *p0, *p1;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    a = 1;
    b = 2;
    EXPECT_EQ_INT(slist_pushback(&s, &a), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &b), 0);
    p0 = slist_front(&s);
    p1 = slist_front(&s);
    EXPECT_EQ_PTR(p0, p1);
    EXPECT_EQ_PTR(slist_back(&s), &b);
    slist_fini(&s);
  }

  {
    struct slist s;
    int a, b, c;
    struct slist_node *n0, *n1;
    void *out;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    a = 10;
    b = 20;
    c = 30;
    EXPECT_EQ_INT(slist_pushback(&s, &a), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &b), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &c), 0);
    n0 = s.head;
    EXPECT_NOTNULL(n0);
    n1 = slist_next(n0);
    EXPECT_NOTNULL(n1);
    out = NULL;
    EXPECT_EQ_INT(slist_removen(&s, n1, &out), 0);
    EXPECT_EQ_PTR(out, &c);
    EXPECT_EQ_PTR(slist_back(&s), &b);
    EXPECT_NULL(slist_next(n1));
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
    out = NULL;
    EXPECT_EQ_INT(slist_removen(&s, NULL, &out), 0);
    EXPECT_EQ_PTR(out, &a);
    EXPECT_EQ_PTR(slist_front(&s), &b);
    EXPECT_EQ_PTR(slist_back(&s), &b);
    EXPECT_EQ_INT(slist_removen(&s, NULL, &out), 0);
    EXPECT_EQ_PTR(out, &b);
    EXPECT_TRUE(slist_empty(&s));
    slist_fini(&s);
  }

  {
    struct slist s;
    int a, b, c;
    struct slist_node *n0;
    void *out;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    a = 1;
    b = 2;
    c = 3;
    EXPECT_EQ_INT(slist_pushback(&s, &a), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &c), 0);
    n0 = s.head;
    EXPECT_NOTNULL(n0);
    out = NULL;
    EXPECT_EQ_INT(slist_insertn(&s, n0, &b), 0);
    EXPECT_EQ_UINT(slist_size(&s), 3);
    EXPECT_EQ_INT(slist_removen(&s, n0, &out), 0);
    EXPECT_EQ_PTR(out, &b);
    EXPECT_EQ_UINT(slist_size(&s), 2);
    EXPECT_EQ_PTR(slist_front(&s), &a);
    EXPECT_EQ_PTR(slist_back(&s), &c);
    slist_fini(&s);
  }

  {
    struct slist s;
    int x;
    void *out;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    x = 1;
    EXPECT_EQ_INT(slist_pushfront(&s, &x), 0);
    out = NULL;
    EXPECT_EQ_INT(slist_popfront(&s, &out), 0);
    EXPECT_EQ_PTR(out, &x);
    EXPECT_TRUE(slist_empty(&s));
    EXPECT_EQ_INT(slist_removen(&s, NULL, &out), -1);
    EXPECT_EQ_INT(slist_insertn(&s, NULL, &x), 0);
    EXPECT_EQ_PTR(slist_front(&s), &x);
    slist_fini(&s);
  }

  {
    struct slist s;
    int a, b;
    void *out;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(slist_init(&s, edg_dtor_inc), 0);
    a = 1;
    b = 2;
    EXPECT_EQ_INT(slist_pushback(&s, &a), 0);
    EXPECT_EQ_INT(slist_pushback(&s, &b), 0);
    out = NULL;
    EXPECT_EQ_INT(slist_popfront(&s, &out), 0);
    EXPECT_EQ_PTR(out, &a);
    EXPECT_EQ_INT(edg_dtor_n, 0);
    EXPECT_EQ_INT(slist_popfront(&s, NULL), 0);
    EXPECT_EQ_INT(edg_dtor_n, 1);
    slist_fini(&s);
  }

  {
    struct slist s;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(slist_init(&s, edg_dtor_inc), 0);
    slist_clear(&s);
    EXPECT_EQ_INT(edg_dtor_n, 0);
    slist_fini(&s);
  }

  {
    struct slist s;
    int x;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(slist_init(&s, edg_dtor_inc), 0);
    x = 3;
    EXPECT_EQ_INT(slist_pushback(&s, &x), 0);
    slist_fini(&s);
    EXPECT_EQ_INT(edg_dtor_n, 1);
  }

  {
    int x;
    struct slist_node n;
    struct slist_node *np;
    void *out;

    EXPECT_EQ_INT(slist_init(NULL, edg_dtor_inc), -1);
    EXPECT_EQ_INT(slist_pushfront(NULL, &x), -1);
    EXPECT_EQ_INT(slist_pushback(NULL, &x), -1);
    out = NULL;
    EXPECT_EQ_INT(slist_popfront(NULL, &out), -1);
    EXPECT_EQ_INT(slist_insertn(NULL, &n, &x), -1);
    EXPECT_EQ_INT(slist_removen(NULL, &n, &out), -1);
    np = NULL;
    EXPECT_NULL(slist_next(np));
    EXPECT_NULL(slist_data(np));
  }

  {
    struct slist s;
    int x;
    void *out;

    EXPECT_EQ_INT(slist_init(&s, NULL), 0);
    x = 1;
    out = NULL;
    EXPECT_EQ_INT(slist_pushfront(&s, NULL), -1);
    EXPECT_EQ_INT(slist_pushback(&s, NULL), -1);
    EXPECT_EQ_INT(slist_insertn(&s, NULL, NULL), -1);
    EXPECT_EQ_INT(slist_removen(&s, NULL, &out), -1);
    EXPECT_EQ_INT(slist_insertn(&s, NULL, &x), 0);
    EXPECT_EQ_INT(slist_removen(&s, s.head, &out), -1);
    slist_fini(&s);
  }

  {
    slist_fini(NULL);
    slist_clear(NULL);
  }
}