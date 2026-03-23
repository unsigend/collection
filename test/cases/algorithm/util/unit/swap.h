#include <stdlib.h>
#include <utest.h>
#include <util.h>

UTEST_CASE(swap)
{
  {
    int a, b;

    a = 5;
    b = 7;
    swap(&a, &b, 0);
    EXPECT_EQ_INT(a, 5);
    EXPECT_EQ_INT(b, 7);
  }

  {
    int a, b;

    a = 1;
    b = 2;
    swap(&a, &b, sizeof(int));
    EXPECT_EQ_INT(a, 2);
    EXPECT_EQ_INT(b, 1);
  }

  {
    double a, b;

    a = 3.25;
    b = 9.5;
    swap(&a, &b, sizeof(double));
    EXPECT_EQ_DOUBLE(a, 9.5);
    EXPECT_EQ_DOUBLE(b, 3.25);
  }

  {
    struct {
      int x;
      char c;
      double d;
    } s, t;

    s.x = 11;
    s.c = 'a';
    s.d = 0.125;
    t.x = 22;
    t.c = 'b';
    t.d = 2.0;
    swap(&s, &t, sizeof(s));
    EXPECT_EQ_INT(s.x, 22);
    EXPECT_EQ_CHAR(s.c, 'b');
    EXPECT_EQ_DOUBLE(s.d, 2.0);
    EXPECT_EQ_INT(t.x, 11);
    EXPECT_EQ_CHAR(t.c, 'a');
    EXPECT_EQ_DOUBLE(t.d, 0.125);
  }

  {
    unsigned char u[11];
    unsigned char v[11];
    size_t idx;

    for (idx = 0; idx < sizeof(u); idx++) {
      u[idx] = (unsigned char)idx;
      v[idx] = (unsigned char)(100 + idx);
    }
    swap(u, v, sizeof(u));
    for (idx = 0; idx < sizeof(u); idx++) {
      EXPECT_EQ_UCHAR(u[idx], (unsigned char)(100 + idx));
      EXPECT_EQ_UCHAR(v[idx], (unsigned char)idx);
    }
  }

  {
    int buf[5];

    buf[0] = 0;
    buf[1] = 10;
    buf[2] = 20;
    buf[3] = 30;
    buf[4] = 40;
    swap(&buf[1], &buf[3], sizeof(int));
    EXPECT_EQ_INT(buf[0], 0);
    EXPECT_EQ_INT(buf[1], 30);
    EXPECT_EQ_INT(buf[2], 20);
    EXPECT_EQ_INT(buf[3], 10);
    EXPECT_EQ_INT(buf[4], 40);
  }

  {
    int *p;
    int *q;
    size_t n;

    n = 4;
    p = malloc(n * sizeof(int));
    q = malloc(n * sizeof(int));
    EXPECT_NOTNULL(p);
    EXPECT_NOTNULL(q);
    p[0] = 1;
    p[1] = 2;
    p[2] = 3;
    p[3] = 4;
    q[0] = 100;
    q[1] = 200;
    q[2] = 300;
    q[3] = 400;
    swap(p, q, n * sizeof(int));
    EXPECT_EQ_INT(p[0], 100);
    EXPECT_EQ_INT(p[1], 200);
    EXPECT_EQ_INT(p[2], 300);
    EXPECT_EQ_INT(p[3], 400);
    EXPECT_EQ_INT(q[0], 1);
    EXPECT_EQ_INT(q[1], 2);
    EXPECT_EQ_INT(q[2], 3);
    EXPECT_EQ_INT(q[3], 4);
    free(p);
    free(q);
  }
}
