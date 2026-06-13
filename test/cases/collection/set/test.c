#include "unit/basic.h"
#include "unit/edge.h"
#include "unit/integration.h"
#include "unit/iter.h"

UTEST_SUITE(set)
{
  UTEST_RUNCASE(basic);
  UTEST_RUNCASE(edge);
  UTEST_RUNCASE(integration);
  UTEST_RUNCASE(iter);
}
