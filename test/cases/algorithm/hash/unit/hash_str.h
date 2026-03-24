#include <hash.h>
#include <utest.h>

UTEST_CASE(hash_str)
{
  {
    uint32_t h1, h2;

    h1 = hash_str("a");
    h2 = hash_str("b");
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_str("abc");
    h2 = hash_str("abd");
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_str("collection");
    h2 = hash_str("collections");
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_str("Hello");
    h2 = hash_str("hello");
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_str("abc");
    h2 = hash_str("abc ");
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_str("ab c");
    h2 = hash_str("abc");
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_str("a-b-c");
    h2 = hash_str("a_b_c");
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_str("0123456789");
    h2 = hash_str("0123456788");
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_str("prefix_data");
    h2 = hash_str("prefix_date");
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_str("x");
    h2 = hash_str("xx");
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_str("key=42");
    h2 = hash_str("key=43");
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_str("path/to/file");
    h2 = hash_str("path\\to\\file");
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_str("aaab");
    h2 = hash_str("baaa");
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_str("A1b2C3");
    h2 = hash_str("A1b2C4");
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_str("line1\nline2");
    h2 = hash_str("line1\nline3");
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_str("repeatable value 123");
    h2 = hash_str("repeatable value 123");
    EXPECT_EQ_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_str("same-string-check");
    h2 = hash_str("same-string-check");
    EXPECT_EQ_UINT(h1, h2);
  }
}