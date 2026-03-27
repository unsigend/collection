#include "unit/basic.h"
#include "unit/edge.h"
#include "unit/large.h"

/* Options: sortins, sortqs, sortheap */
sortfunc sort_under_test = sortheap;

UTEST_SUITE(sort)
{
  UTEST_RUNCASE(basic);
  UTEST_RUNCASE(edge);
  UTEST_RUNCASE(large);
}
