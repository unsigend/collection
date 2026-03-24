#include <hash.h>
#include <utest.h>

UTEST_CASE(hash_int)
{
  {
    uint32_t h1, h2;

    h1 = hash_u32(0u);
    h2 = hash_u32(1u);
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_u32(12345u);
    h2 = hash_u32(12346u);
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_u32(0xffffffffu);
    h2 = hash_u32(0x7fffffffu);
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_u32(0xdeadbeefu);
    h2 = hash_u32(0xdeadbeefu);
    EXPECT_EQ_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_u32(0xaaaaaaaau);
    h2 = hash_u32(0x55555555u);
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_u32(0x0000ffffu);
    h2 = hash_u32(0xffff0000u);
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_u32(0x13579bdfu);
    h2 = hash_u32(0x13579bdeu);
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_u32(42u);
    h2 = hash_u32(42u);
    EXPECT_EQ_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_u64(0ull);
    h2 = hash_u64(1ull);
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_u64(0x0000000100000000ull);
    h2 = hash_u64(0x0000000200000000ull);
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_u64(0x00000000ffffffffull);
    h2 = hash_u64(0xffffffff00000000ull);
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_u64(0x0123456789abcdefull);
    h2 = hash_u64(0x0123456789abcdefull);
    EXPECT_EQ_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_u64(0xaaaaaaaaaaaaaaaaull);
    h2 = hash_u64(0x5555555555555555ull);
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_u64(0x000000000000ffffull);
    h2 = hash_u64(0x00000000ffff0000ull);
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_u64(0xffffffffffffffffull);
    h2 = hash_u64(0xfffffffffffffffeull);
    EXPECT_NE_UINT(h1, h2);
  }

  {
    uint32_t h1, h2;

    h1 = hash_u64(123456789ull);
    h2 = hash_u64(123456789ull);
    EXPECT_EQ_UINT(h1, h2);
  }
}
