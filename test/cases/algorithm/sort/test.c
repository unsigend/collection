#include "unit/basic.h"
#include "unit/edge.h"
#include "unit/large.h"

sortfunc sort_under_test = sortins;

UTEST_SUITE(sort)
{
  UTEST_RUNCASE(basic);
  UTEST_RUNCASE(edge);
  UTEST_RUNCASE(large);
}
