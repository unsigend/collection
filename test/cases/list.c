/* list test suite */

#include <list.h>
#include <utest.h>

struct item {
    int v;
    struct list_head node;
};

UTEST_CASE(LIST_HEAD_INIT)
{
    struct list_head head = LIST_HEAD_INIT(head);

    EXPECT_EQ_PTR(head.next, &head);
    EXPECT_EQ_PTR(head.prev, &head);
    EXPECT_EQ_INT(list_empty(&head), 1);
}

UTEST_CASE(list_init)
{
    struct list_head head;

    list_init(&head);
    EXPECT_EQ_PTR(head.next, &head);
    EXPECT_EQ_PTR(head.prev, &head);
    EXPECT_EQ_INT(list_empty(&head), 1);
}

UTEST_CASE(list_empty)
{
    struct list_head head;
    struct item a = {.v = 1};

    list_init(&head);
    EXPECT_EQ_INT(list_empty(&head), 1);
    list_add_head(&head, &a.node);
    EXPECT_EQ_INT(list_empty(&head), 0);
    list_del_init(&a.node);
    EXPECT_EQ_INT(list_empty(&head), 1);
}

UTEST_CASE(list_entry)
{
    struct item a = {.v = 42};

    list_init(&a.node);
    EXPECT_EQ_PTR(list_entry(&a.node, struct item, node), &a);
    EXPECT_EQ_INT(list_entry(&a.node, struct item, node)->v, 42);
}

UTEST_CASE(list_first_entry)
{
    struct list_head head;
    struct item a = {.v = 1}, b = {.v = 2};

    list_init(&head);
    list_add_tail(&head, &a.node);
    list_add_tail(&head, &b.node);
    EXPECT_EQ_PTR(list_first_entry(&head, struct item, node), &a);
    EXPECT_EQ_INT(list_first_entry(&head, struct item, node)->v, 1);
}

UTEST_CASE(list_last_entry)
{
    struct list_head head;
    struct item a = {.v = 1}, b = {.v = 2};

    list_init(&head);
    list_add_tail(&head, &a.node);
    list_add_tail(&head, &b.node);
    EXPECT_EQ_PTR(list_last_entry(&head, struct item, node), &b);
    EXPECT_EQ_INT(list_last_entry(&head, struct item, node)->v, 2);
}

UTEST_CASE(list_add_head)
{
    struct list_head head;
    struct item a = {.v = 1}, b = {.v = 2};

    list_init(&head);
    list_add_head(&head, &a.node);
    EXPECT_EQ_PTR(head.next, &a.node);
    EXPECT_EQ_PTR(head.prev, &a.node);
    list_add_head(&head, &b.node);
    EXPECT_EQ_PTR(list_first_entry(&head, struct item, node), &b);
    EXPECT_EQ_PTR(list_last_entry(&head, struct item, node), &a);
}

UTEST_CASE(list_add_tail)
{
    struct list_head head;
    struct item a = {.v = 1}, b = {.v = 2};

    list_init(&head);
    list_add_tail(&head, &a.node);
    list_add_tail(&head, &b.node);
    EXPECT_EQ_PTR(list_first_entry(&head, struct item, node), &a);
    EXPECT_EQ_PTR(list_last_entry(&head, struct item, node), &b);
}

UTEST_CASE(list_del)
{
    struct list_head head;
    struct item a = {.v = 1}, b = {.v = 2}, c = {.v = 3};

    list_init(&head);
    list_add_tail(&head, &a.node);
    list_add_tail(&head, &b.node);
    list_add_tail(&head, &c.node);
    list_del(&b.node);
    EXPECT_EQ_PTR(b.node.next, LIST_POISON);
    EXPECT_EQ_PTR(b.node.prev, LIST_POISON);
    EXPECT_EQ_PTR(list_first_entry(&head, struct item, node), &a);
    EXPECT_EQ_PTR(list_last_entry(&head, struct item, node), &c);
    EXPECT_EQ_PTR(a.node.next, &c.node);
    EXPECT_EQ_PTR(c.node.prev, &a.node);
}

UTEST_CASE(list_del_init)
{
    struct list_head head, other;
    struct item a = {.v = 1};

    list_init(&head);
    list_init(&other);
    list_add_tail(&head, &a.node);
    list_del_init(&a.node);
    EXPECT_EQ_INT(list_empty(&head), 1);
    EXPECT_EQ_PTR(a.node.next, &a.node);
    EXPECT_EQ_PTR(a.node.prev, &a.node);
    list_add_tail(&other, &a.node);
    EXPECT_EQ_PTR(list_first_entry(&other, struct item, node), &a);
}

UTEST_CASE(list_del_head)
{
    struct list_head head, *n;
    struct item a = {.v = 1}, b = {.v = 2};

    list_init(&head);
    EXPECT_NULL(list_del_head(&head));
    list_add_tail(&head, &a.node);
    list_add_tail(&head, &b.node);
    n = list_del_head(&head);
    EXPECT_EQ_PTR(n, &a.node);
    EXPECT_EQ_PTR(n->next, LIST_POISON);
    EXPECT_EQ_PTR(list_first_entry(&head, struct item, node), &b);
    n = list_del_head(&head);
    EXPECT_EQ_PTR(n, &b.node);
    EXPECT_EQ_INT(list_empty(&head), 1);
}

UTEST_CASE(list_del_tail)
{
    struct list_head head, *n;
    struct item a = {.v = 1}, b = {.v = 2};

    list_init(&head);
    EXPECT_NULL(list_del_tail(&head));
    list_add_tail(&head, &a.node);
    list_add_tail(&head, &b.node);
    n = list_del_tail(&head);
    EXPECT_EQ_PTR(n, &b.node);
    EXPECT_EQ_PTR(n->next, LIST_POISON);
    EXPECT_EQ_PTR(list_last_entry(&head, struct item, node), &a);
}

UTEST_CASE(list_del_head_init)
{
    struct list_head head, other, *n;
    struct item a = {.v = 1};

    list_init(&head);
    list_init(&other);
    EXPECT_NULL(list_del_head_init(&head));
    list_add_tail(&head, &a.node);
    n = list_del_head_init(&head);
    EXPECT_EQ_PTR(n, &a.node);
    EXPECT_EQ_PTR(n->next, n);
    EXPECT_EQ_INT(list_empty(&head), 1);
    list_add_tail(&other, n);
    EXPECT_EQ_PTR(list_first_entry(&other, struct item, node), &a);
}

UTEST_CASE(list_del_tail_init)
{
    struct list_head head, other, *n;
    struct item a = {.v = 1}, b = {.v = 2};

    list_init(&head);
    list_init(&other);
    EXPECT_NULL(list_del_tail_init(&head));
    list_add_tail(&head, &a.node);
    list_add_tail(&head, &b.node);
    n = list_del_tail_init(&head);
    EXPECT_EQ_PTR(n, &b.node);
    EXPECT_EQ_PTR(n->next, n);
    EXPECT_EQ_PTR(list_last_entry(&head, struct item, node), &a);
    list_add_tail(&other, n);
    EXPECT_EQ_PTR(list_first_entry(&other, struct item, node), &b);
}

UTEST_CASE(list_for_each)
{
    struct list_head head, *pos;
    struct item a = {.v = 1}, b = {.v = 2}, c = {.v = 3};
    int n = 0, sum = 0;

    list_init(&head);
    list_add_tail(&head, &a.node);
    list_add_tail(&head, &b.node);
    list_add_tail(&head, &c.node);
    list_for_each (&head, pos) {
        n++;
        sum += list_entry(pos, struct item, node)->v;
    }
    EXPECT_EQ_INT(n, 3);
    EXPECT_EQ_INT(sum, 6);
}

UTEST_CASE(list_for_each_entry)
{
    struct list_head head;
    struct item a = {.v = 1}, b = {.v = 2}, c = {.v = 3};
    struct item *pos;
    int n = 0, sum = 0;

    list_init(&head);
    list_add_tail(&head, &a.node);
    list_add_tail(&head, &b.node);
    list_add_tail(&head, &c.node);
    list_for_each_entry (&head, pos, struct item, node) {
        n++;
        sum += pos->v;
    }
    EXPECT_EQ_INT(n, 3);
    EXPECT_EQ_INT(sum, 6);
}

UTEST_CASE(list_for_each_safe)
{
    struct list_head head, *pos, *tmp;
    struct item a = {.v = 1}, b = {.v = 2}, c = {.v = 3};
    int n = 0;

    list_init(&head);
    list_add_tail(&head, &a.node);
    list_add_tail(&head, &b.node);
    list_add_tail(&head, &c.node);
    list_for_each_safe (&head, pos, tmp) {
        n++;
        list_del(pos);
    }
    EXPECT_EQ_INT(n, 3);
    EXPECT_EQ_INT(list_empty(&head), 1);
}

UTEST_CASE(list_for_each_entry_safe)
{
    struct list_head head;
    struct item a = {.v = 1}, b = {.v = 2}, c = {.v = 3};
    struct item *pos, *tmp;
    int n = 0, sum = 0;

    list_init(&head);
    list_add_tail(&head, &a.node);
    list_add_tail(&head, &b.node);
    list_add_tail(&head, &c.node);
    list_for_each_entry_safe (&head, pos, tmp, struct item, node) {
        n++;
        sum += pos->v;
        list_del(&pos->node);
    }
    EXPECT_EQ_INT(n, 3);
    EXPECT_EQ_INT(sum, 6);
    EXPECT_EQ_INT(list_empty(&head), 1);
}

UTEST_CASE(list_replace)
{
    struct list_head head;
    struct item a = {.v = 1}, b = {.v = 2}, c = {.v = 3}, d = {.v = 4};

    list_init(&head);
    list_add_tail(&head, &a.node);
    list_add_tail(&head, &b.node);
    list_add_tail(&head, &c.node);
    list_replace(&b.node, &d.node);
    EXPECT_EQ_PTR(list_first_entry(&head, struct item, node), &a);
    EXPECT_EQ_PTR(a.node.next, &d.node);
    EXPECT_EQ_PTR(d.node.next, &c.node);
    EXPECT_EQ_PTR(list_last_entry(&head, struct item, node), &c);
    EXPECT_EQ_PTR(list_entry(d.node.prev, struct item, node), &a);
}

UTEST_CASE(list_replace_init)
{
    struct list_head head, other;
    struct item a = {.v = 1}, b = {.v = 2}, c = {.v = 3};

    list_init(&head);
    list_init(&other);
    list_add_tail(&head, &a.node);
    list_add_tail(&head, &b.node);
    list_replace_init(&a.node, &c.node);
    EXPECT_EQ_PTR(list_first_entry(&head, struct item, node), &c);
    EXPECT_EQ_PTR(list_last_entry(&head, struct item, node), &b);
    EXPECT_EQ_PTR(a.node.next, &a.node);
    EXPECT_EQ_PTR(a.node.prev, &a.node);
    list_add_tail(&other, &a.node);
    EXPECT_EQ_PTR(list_first_entry(&other, struct item, node), &a);
}

UTEST_CASE(list_is_head)
{
    struct list_head head;
    struct item a = {.v = 1};

    list_init(&head);
    EXPECT_EQ_INT(list_is_head(&head, &head), 1);
    list_add_tail(&head, &a.node);
    EXPECT_EQ_INT(list_is_head(&head, &head), 1);
    EXPECT_EQ_INT(list_is_head(&head, &a.node), 0);
}

UTEST_CASE(list_is_last)
{
    struct list_head head;
    struct item a = {.v = 1}, b = {.v = 2};

    list_init(&head);
    list_add_tail(&head, &a.node);
    EXPECT_EQ_INT(list_is_last(&head, &a.node), 1);
    list_add_tail(&head, &b.node);
    EXPECT_EQ_INT(list_is_last(&head, &a.node), 0);
    EXPECT_EQ_INT(list_is_last(&head, &b.node), 1);
}

UTEST_CASE(list_is_first)
{
    struct list_head head;
    struct item a = {.v = 1}, b = {.v = 2};

    list_init(&head);
    list_add_tail(&head, &a.node);
    EXPECT_EQ_INT(list_is_first(&head, &a.node), 1);
    list_add_tail(&head, &b.node);
    EXPECT_EQ_INT(list_is_first(&head, &a.node), 1);
    EXPECT_EQ_INT(list_is_first(&head, &b.node), 0);
}

UTEST_CASE(list_move_head)
{
    struct list_head a, b;
    struct item x = {.v = 1}, y = {.v = 2}, z = {.v = 3};

    list_init(&a);
    list_init(&b);
    list_add_tail(&a, &x.node);
    list_add_tail(&a, &y.node);
    list_add_tail(&b, &z.node);
    list_move_head(&b, &x.node);
    EXPECT_EQ_PTR(list_first_entry(&b, struct item, node), &x);
    EXPECT_EQ_PTR(list_last_entry(&b, struct item, node), &z);
    EXPECT_EQ_PTR(list_first_entry(&a, struct item, node), &y);
    list_move_head(&a, &y.node);
    EXPECT_EQ_PTR(list_first_entry(&a, struct item, node), &y);
}

UTEST_CASE(list_move_tail)
{
    struct list_head a, b;
    struct item x = {.v = 1}, y = {.v = 2}, z = {.v = 3};

    list_init(&a);
    list_init(&b);
    list_add_tail(&a, &x.node);
    list_add_tail(&a, &y.node);
    list_add_tail(&b, &z.node);
    list_move_tail(&b, &x.node);
    EXPECT_EQ_PTR(list_first_entry(&a, struct item, node), &y);
    EXPECT_EQ_PTR(list_first_entry(&b, struct item, node), &z);
    EXPECT_EQ_PTR(list_last_entry(&b, struct item, node), &x);
    list_move_tail(&b, &z.node);
    EXPECT_EQ_PTR(list_first_entry(&b, struct item, node), &x);
    EXPECT_EQ_PTR(list_last_entry(&b, struct item, node), &z);
}

UTEST_CASE(list_splice_head)
{
    struct list_head dest, src;
    struct item x = {.v = 1}, y = {.v = 2}, z = {.v = 3};

    list_init(&dest);
    list_init(&src);
    list_add_tail(&dest, &z.node);
    list_splice_head(&dest, &src);
    EXPECT_EQ_PTR(list_first_entry(&dest, struct item, node), &z);
    list_add_tail(&src, &x.node);
    list_add_tail(&src, &y.node);
    list_splice_head(&dest, &src);
    EXPECT_EQ_PTR(list_first_entry(&dest, struct item, node), &x);
    EXPECT_EQ_PTR(x.node.next, &y.node);
    EXPECT_EQ_PTR(y.node.next, &z.node);
    EXPECT_EQ_PTR(list_last_entry(&dest, struct item, node), &z);
}

UTEST_CASE(list_splice_tail)
{
    struct list_head dest, src;
    struct item x = {.v = 1}, y = {.v = 2}, z = {.v = 3};

    list_init(&dest);
    list_init(&src);
    list_add_tail(&dest, &z.node);
    list_add_tail(&src, &x.node);
    list_add_tail(&src, &y.node);
    list_splice_tail(&dest, &src);
    EXPECT_EQ_PTR(list_first_entry(&dest, struct item, node), &z);
    EXPECT_EQ_PTR(z.node.next, &x.node);
    EXPECT_EQ_PTR(x.node.next, &y.node);
    EXPECT_EQ_PTR(list_last_entry(&dest, struct item, node), &y);
}

UTEST_CASE(list_splice_head_init)
{
    struct list_head dest, src;
    struct item x = {.v = 1}, y = {.v = 2}, z = {.v = 3}, w = {.v = 4};

    list_init(&dest);
    list_init(&src);
    list_add_tail(&dest, &z.node);
    list_add_tail(&src, &x.node);
    list_add_tail(&src, &y.node);
    list_splice_head_init(&dest, &src);
    EXPECT_EQ_PTR(list_first_entry(&dest, struct item, node), &x);
    EXPECT_EQ_PTR(list_last_entry(&dest, struct item, node), &z);
    EXPECT_EQ_INT(list_empty(&src), 1);
    list_add_tail(&src, &w.node);
    EXPECT_EQ_PTR(list_first_entry(&src, struct item, node), &w);
}

UTEST_CASE(list_splice_tail_init)
{
    struct list_head dest, src;
    struct item x = {.v = 1}, y = {.v = 2}, z = {.v = 3};

    list_init(&dest);
    list_init(&src);
    list_add_tail(&dest, &z.node);
    list_add_tail(&src, &x.node);
    list_add_tail(&src, &y.node);
    list_splice_tail_init(&dest, &src);
    EXPECT_EQ_PTR(list_first_entry(&dest, struct item, node), &z);
    EXPECT_EQ_PTR(list_last_entry(&dest, struct item, node), &y);
    EXPECT_EQ_INT(list_empty(&src), 1);
    list_splice_tail_init(&dest, &src);
    EXPECT_EQ_PTR(list_last_entry(&dest, struct item, node), &y);
}

UTEST_SUITE(list)
{
    UTEST_RUNCASE(LIST_HEAD_INIT);
    UTEST_RUNCASE(list_init);
    UTEST_RUNCASE(list_empty);
    UTEST_RUNCASE(list_is_head);
    UTEST_RUNCASE(list_is_last);
    UTEST_RUNCASE(list_is_first);
    UTEST_RUNCASE(list_entry);
    UTEST_RUNCASE(list_first_entry);
    UTEST_RUNCASE(list_last_entry);
    UTEST_RUNCASE(list_add_head);
    UTEST_RUNCASE(list_add_tail);
    UTEST_RUNCASE(list_del);
    UTEST_RUNCASE(list_del_init);
    UTEST_RUNCASE(list_replace);
    UTEST_RUNCASE(list_replace_init);
    UTEST_RUNCASE(list_move_head);
    UTEST_RUNCASE(list_move_tail);
    UTEST_RUNCASE(list_splice_head);
    UTEST_RUNCASE(list_splice_tail);
    UTEST_RUNCASE(list_splice_head_init);
    UTEST_RUNCASE(list_splice_tail_init);
    UTEST_RUNCASE(list_del_head);
    UTEST_RUNCASE(list_del_tail);
    UTEST_RUNCASE(list_del_head_init);
    UTEST_RUNCASE(list_del_tail_init);
    UTEST_RUNCASE(list_for_each);
    UTEST_RUNCASE(list_for_each_entry);
    UTEST_RUNCASE(list_for_each_safe);
    UTEST_RUNCASE(list_for_each_entry_safe);
}
