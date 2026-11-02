/* Tests for hash_wy and hash_sip. */

#include <hash.h>
#include <stdint.h>
#include <string.h>
#include <utest.h>

UTEST_CASE(hash_wy)
{
    const char *a = "hello";
    const char *b = "world";
    uint64_t h0, h1;

    h0 = hash_wy(a, strlen(a), 0);
    h1 = hash_wy(a, strlen(a), 0);
    EXPECT_EQ_UINT(h0, h1);
    EXPECT_NE_UINT(h0, hash_wy(a, strlen(a), 1));
    EXPECT_NE_UINT(h0, hash_wy(b, strlen(b), 0));
    EXPECT_EQ_UINT(hash_wy("", 0, 0), hash_wy(NULL, 0, 0));
}

UTEST_CASE(hash_sip)
{
    const char *a = "hello";
    const char *b = "world";
    uint8_t k0[16] = {0};
    uint8_t k1[16] = {1};
    uint64_t h0, h1;

    h0 = hash_sip(a, strlen(a), k0);
    h1 = hash_sip(a, strlen(a), k0);
    EXPECT_EQ_UINT(h0, h1);
    EXPECT_NE_UINT(h0, hash_sip(a, strlen(a), k1));
    EXPECT_NE_UINT(h0, hash_sip(b, strlen(b), k0));
    EXPECT_EQ_UINT(hash_sip("", 0, k0), hash_sip(NULL, 0, k0));
}

UTEST_SUITE(hash)
{
    UTEST_RUNCASE(hash_wy);
    UTEST_RUNCASE(hash_sip);
}
