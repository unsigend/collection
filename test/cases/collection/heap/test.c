#include "unit/basic.h"
#include "unit/edge.h"
#include "unit/integration.h"

UTEST_SUITE(heap)
{
  UTEST_RUNCASE(basic);
  UTEST_RUNCASE(edge);
  UTEST_RUNCASE(integration);
}