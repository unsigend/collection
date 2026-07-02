#include "unit/accessors.h"
#include "unit/assign.h"
#include "unit/capacity.h"
#include "unit/compare.h"
#include "unit/edit.h"
#include "unit/init.h"
#include "unit/modify.h"
#include "unit/replace.h"
#include "unit/search.h"
#include "unit/split_join.h"
#include "unit/substr.h"
#include "unit/transform.h"

UTEST_SUITE(string)
{
    UTEST_RUNCASE(string_init);
    UTEST_RUNCASE(string_init_buf);
    UTEST_RUNCASE(string_init_bufn);
    UTEST_RUNCASE(string_dup);
    UTEST_RUNCASE(string_free);

    UTEST_RUNCASE(string_empty);
    UTEST_RUNCASE(string_len);
    UTEST_RUNCASE(string_capacity);
    UTEST_RUNCASE(string_raw);
    UTEST_RUNCASE(string_at);

    UTEST_RUNCASE(string_reserve);
    UTEST_RUNCASE(string_shrink);
    UTEST_RUNCASE(string_clear);

    UTEST_RUNCASE(string_assign);
    UTEST_RUNCASE(string_assignn);
    UTEST_RUNCASE(string_copy);

    UTEST_RUNCASE(string_pushback);
    UTEST_RUNCASE(string_popback);
    UTEST_RUNCASE(string_append);
    UTEST_RUNCASE(string_appendn);
    UTEST_RUNCASE(string_append_str);
    UTEST_RUNCASE(string_prepend);
    UTEST_RUNCASE(string_prependn);

    UTEST_RUNCASE(string_insert);
    UTEST_RUNCASE(string_erase);

    UTEST_RUNCASE(string_find);
    UTEST_RUNCASE(string_find_last);
    UTEST_RUNCASE(string_contains);
    UTEST_RUNCASE(string_starts_with);
    UTEST_RUNCASE(string_ends_with);

    UTEST_RUNCASE(string_eq);
    UTEST_RUNCASE(string_eq_buf);
    UTEST_RUNCASE(string_cmp);

    UTEST_RUNCASE(string_lowercase);
    UTEST_RUNCASE(string_uppercase);
    UTEST_RUNCASE(string_trim_start);
    UTEST_RUNCASE(string_trim_end);
    UTEST_RUNCASE(string_trim);
    UTEST_RUNCASE(string_reverse);

    UTEST_RUNCASE(string_replace);
    UTEST_RUNCASE(string_replace_all);

    UTEST_RUNCASE(string_substr);

    UTEST_RUNCASE(string_split);
    UTEST_RUNCASE(string_split_free);
    UTEST_RUNCASE(string_join);
    UTEST_RUNCASE(string_join_str);
}
