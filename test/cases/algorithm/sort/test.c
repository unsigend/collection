#include "unit/basic.h"
#include "unit/edge.h"
#include "unit/large.h"

/* Options: sortins, sortqs, sortheap, sort */
sortfunc sort_under_test = sort;

UTEST_SUITE(sort)
{
  UTEST_RUNCASE(basic);
  UTEST_RUNCASE(edge);
  UTEST_RUNCASE(large);
}
