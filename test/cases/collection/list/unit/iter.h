#include <list.h>
#include <utest.h>

struct iter_item {
    int val;
    struct list_head node;
};

UTEST_CASE(iter)
{
    {
        struct list_head head = LIST_HEAD_INIT(head);
        struct list_head *pos;
        struct iter_item *epos;
        int n;

        n = 0;
        list_for_each(&head, pos)
            n++;
        EXPECT_EQ_INT(n, 0);

        n = 0;
        list_for_each_entry(&head, epos, struct iter_item, node)
            n++;
        EXPECT_EQ_INT(n, 0);
    }

    {
        struct list_head head;
        struct list_head *pos, *tmp;
        struct iter_item *epos, *etmp;
        int n;

        list_init(&head);
        n = 0;
        list_for_each_safe(&head, pos, tmp)
            n++;
        EXPECT_EQ_INT(n, 0);

        n = 0;
        list_for_each_entry_safe(&head, epos, etmp, struct iter_item, node)
            n++;
        EXPECT_EQ_INT(n, 0);
    }

    {
        struct list_head head;
        struct iter_item a;
        struct list_head *pos;
        int n;

        list_init(&head);
        a.val = 42;
        list_add_tail(&head, &a.node);
        n = 0;
        list_for_each(&head, pos) {
            EXPECT_EQ_PTR(pos, &a.node);
            n++;
        }
        EXPECT_EQ_INT(n, 1);
    }

    {
        struct list_head head;
        struct iter_item a, b, c;
        struct list_head *pos;
        int vals[3];
        int n;

        list_init(&head);
        a.val = 2;
        b.val = 4;
        c.val = 6;
        list_add_tail(&head, &a.node);
        list_add_tail(&head, &b.node);
        list_add_tail(&head, &c.node);
        n = 0;
        list_for_each(&head, pos) {
            EXPECT_LT_INT(n, 3);
            vals[n++] = list_entry(pos, struct iter_item, node)->val;
        }
        EXPECT_EQ_INT(n, 3);
        EXPECT_EQ_INT(vals[0], 2);
        EXPECT_EQ_INT(vals[1], 4);
        EXPECT_EQ_INT(vals[2], 6);
    }

    {
        struct list_head head;
        struct iter_item a, b, c;
        struct iter_item *pos;
        int sum;

        list_init(&head);
        a.val = 2;
        b.val = 4;
        c.val = 6;
        list_add_tail(&head, &a.node);
        list_add_tail(&head, &b.node);
        list_add_tail(&head, &c.node);
        sum = 0;
        list_for_each_entry(&head, pos, struct iter_item, node)
            sum += pos->val;
        EXPECT_EQ_INT(sum, 12);
    }

    {
        struct list_head head;
        struct iter_item a, b, c;
        struct list_head *pos, *tmp;
        int n;

        list_init(&head);
        a.val = 1;
        b.val = 2;
        c.val = 3;
        list_add_tail(&head, &a.node);
        list_add_tail(&head, &b.node);
        list_add_tail(&head, &c.node);
        n = 0;
        list_for_each_safe(&head, pos, tmp) {
            list_del(pos);
            n++;
        }
        EXPECT_EQ_INT(n, 3);
        EXPECT_TRUE(list_empty(&head));
        EXPECT_NULL(a.node.next);
        EXPECT_NULL(c.node.prev);
    }

    {
        struct list_head head;
        struct iter_item a, b, c;
        struct iter_item *pos, *tmp;
        int n;

        list_init(&head);
        a.val = 1;
        b.val = 2;
        c.val = 3;
        list_add_tail(&head, &a.node);
        list_add_tail(&head, &b.node);
        list_add_tail(&head, &c.node);
        n = 0;
        list_for_each_entry_safe(&head, pos, tmp, struct iter_item, node) {
            list_del(&pos->node);
            n++;
        }
        EXPECT_EQ_INT(n, 3);
        EXPECT_TRUE(list_empty(&head));
    }

    {
        struct list_head head;
        struct iter_item a, b, c;
        struct iter_item *pos, *tmp;
        int n;

        list_init(&head);
        a.val = 1;
        b.val = 2;
        c.val = 3;
        list_add_tail(&head, &a.node);
        list_add_tail(&head, &b.node);
        list_add_tail(&head, &c.node);
        n = 0;
        list_for_each_entry_safe(&head, pos, tmp, struct iter_item, node) {
            if (pos->val == 2)
                list_del_init(&pos->node);
            else
                n++;
        }
        EXPECT_EQ_INT(n, 2);
        EXPECT_EQ_INT(list_first_entry(&head, struct iter_item, node)->val, 1);
        EXPECT_EQ_INT(list_last_entry(&head, struct iter_item, node)->val, 3);
        EXPECT_TRUE(list_empty(&b.node));
    }

    {
        struct list_head head;
        struct iter_item a, b, c;
        struct iter_item *pos, *tmp;

        list_init(&head);
        a.val = 1;
        b.val = 2;
        c.val = 3;
        list_add_tail(&head, &a.node);
        list_add_tail(&head, &b.node);
        list_add_tail(&head, &c.node);
        list_for_each_entry_safe(&head, pos, tmp, struct iter_item, node) {
            if (pos == &a)
                list_del(&pos->node);
        }
        EXPECT_EQ_PTR(list_first_entry(&head, struct iter_item, node), &b);
        list_for_each_entry_safe(&head, pos, tmp, struct iter_item, node) {
            if (pos == &c)
                list_del(&pos->node);
        }
        EXPECT_EQ_PTR(list_first_entry(&head, struct iter_item, node), &b);
        EXPECT_EQ_PTR(list_last_entry(&head, struct iter_item, node), &b);
    }

    {
        struct list_head head;
        struct iter_item nodes[5];
        struct iter_item *pos, *tmp;
        int i;
        int sum;

        list_init(&head);
        for (i = 0; i < 5; i++) {
            nodes[i].val = i + 1;
            list_add_tail(&head, &nodes[i].node);
        }
        list_for_each_entry_safe(&head, pos, tmp, struct iter_item, node) {
            if (pos->val % 2 == 0)
                list_del(&pos->node);
        }
        sum = 0;
        list_for_each_entry(&head, pos, struct iter_item, node)
            sum += pos->val;
        EXPECT_EQ_INT(sum, 9);
    }

    {
        struct wrap {
            unsigned id;
            struct list_head node;
        } a, b;
        struct list_head head;
        struct wrap *pos;
        unsigned ids[2];
        int n;

        list_init(&head);
        a.id = 100;
        b.id = 200;
        list_add_head(&head, &b.node);
        list_add_head(&head, &a.node);
        n = 0;
        list_for_each_entry(&head, pos, struct wrap, node) {
            EXPECT_LT_INT(n, 2);
            ids[n++] = pos->id;
        }
        EXPECT_EQ_INT(n, 2);
        EXPECT_EQ_UINT(ids[0], 100);
        EXPECT_EQ_UINT(ids[1], 200);
    }
}
