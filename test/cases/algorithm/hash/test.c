#include "unit/hash_int.h"
#include "unit/hash_str.h"

UTEST_SUITE(hash)
{
  UTEST_RUNCASE(hash_str);
  UTEST_RUNCASE(hash_int);
}