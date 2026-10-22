#include <list.h>
#include <utest.h>

struct intg_item {
    int val;
    struct list_head node;
};

struct intg_pair {
    int x;
    int y;
    struct list_head node;
};

static int intg_count(const struct list_head *head)
{
    struct list_head *pos;
    int n = 0;

    list_for_each(head, pos)
        n++;
    return n;
}

static int intg_sum(const struct list_head *head)
{
    struct intg_item *pos;
    int s = 0;

    list_for_each_entry(head, pos, struct intg_item, node)
        s += pos->val;
    return s;
}

UTEST_CASE(integration)
{
    {
        struct list_head head;
        struct intg_item a, b, c, d;
        struct list_head *n;

        list_init(&head);
        a.val = 0;
        b.val = 1;
        c.val = 2;
        d.val = 3;
        list_add_tail(&head, &a.node);
        list_add_tail(&head, &b.node);
        list_add_tail(&head, &c.node);
        list_add_head(&head, &d.node);
        EXPECT_EQ_INT(intg_count(&head), 4);
        EXPECT_EQ_INT(list_first_entry(&head, struct intg_item, node)->val, 3);
        EXPECT_EQ_INT(list_last_entry(&head, struct intg_item, node)->val, 2);
        n = list_del_head(&head);
        EXPECT_EQ_PTR(n, &d.node);
        n = list_del_tail(&head);
        EXPECT_EQ_PTR(n, &c.node);
        EXPECT_EQ_INT(intg_count(&head), 2);
        EXPECT_EQ_INT(intg_sum(&head), 1);
        list_del(&a.node);
        EXPECT_EQ_PTR(list_first_entry(&head, struct intg_item, node), &b);
        list_add_tail(&head, &c.node);
        list_add_head(&head, &d.node);
        EXPECT_EQ_INT(intg_count(&head), 3);
        EXPECT_EQ_INT(list_first_entry(&head, struct intg_item, node)->val, 3);
        EXPECT_EQ_INT(list_last_entry(&head, struct intg_item, node)->val, 2);
    }

    {
        struct list_head q1, q2;
        struct intg_item a, b, c;
        struct list_head *n;

        list_init(&q1);
        list_init(&q2);
        a.val = 10;
        b.val = 20;
        c.val = 30;
        list_add_tail(&q1, &a.node);
        list_add_tail(&q1, &b.node);
        list_add_tail(&q1, &c.node);
        n = list_del_head_init(&q1);
        list_add_tail(&q2, n);
        n = list_del_tail_init(&q1);
        list_add_head(&q2, n);
        EXPECT_EQ_INT(intg_count(&q1), 1);
        EXPECT_EQ_INT(list_first_entry(&q1, struct intg_item, node)->val, 20);
        EXPECT_EQ_INT(intg_count(&q2), 2);
        EXPECT_EQ_INT(list_first_entry(&q2, struct intg_item, node)->val, 30);
        EXPECT_EQ_INT(list_last_entry(&q2, struct intg_item, node)->val, 10);
    }

    {
        struct list_head head;
        struct intg_item nodes[16];
        int i;
        struct list_head *n;

        list_init(&head);
        for (i = 0; i < 16; i++) {
            nodes[i].val = i;
            if (i % 2 == 0)
                list_add_tail(&head, &nodes[i].node);
            else
                list_add_head(&head, &nodes[i].node);
        }
        EXPECT_EQ_INT(intg_count(&head), 16);
        EXPECT_EQ_INT(intg_sum(&head), 120);
        for (i = 0; i < 8; i++) {
            n = list_del_head(&head);
            EXPECT_NOTNULL(n);
            n = list_del_tail(&head);
            EXPECT_NOTNULL(n);
        }
        EXPECT_TRUE(list_empty(&head));
        EXPECT_NULL(list_del_head(&head));
    }

    {
        struct list_head head;
        struct intg_pair a, b, c;

        list_init(&head);
        a.x = 1;
        a.y = 10;
        b.x = 2;
        b.y = 20;
        c.x = 3;
        c.y = 30;
        list_add_tail(&head, &a.node);
        list_add_tail(&head, &b.node);
        list_add_tail(&head, &c.node);
        EXPECT_EQ_INT(list_first_entry(&head, struct intg_pair, node)->x, 1);
        EXPECT_EQ_INT(list_entry(head.next->next, struct intg_pair, node)->y,
                      20);
        list_del_init(&b.node);
        list_add_head(&head, &b.node);
        EXPECT_EQ_INT(list_first_entry(&head, struct intg_pair, node)->x, 2);
        EXPECT_EQ_INT(list_last_entry(&head, struct intg_pair, node)->x, 3);
        EXPECT_EQ_INT(intg_count(&head), 3);
    }

    {
        struct list_head head;
        struct intg_item a, b, c;
        struct intg_item *pos;
        int seen[3];
        int n;

        list_init(&head);
        a.val = 4;
        b.val = 5;
        c.val = 6;
        list_add_tail(&head, &a.node);
        list_add_tail(&head, &b.node);
        list_add_tail(&head, &c.node);
        list_del(&b.node);
        list_add_tail(&head, &b.node);
        n = 0;
        list_for_each_entry(&head, pos, struct intg_item, node) {
            EXPECT_LT_INT(n, 3);
            seen[n++] = pos->val;
        }
        EXPECT_EQ_INT(n, 3);
        EXPECT_EQ_INT(seen[0], 4);
        EXPECT_EQ_INT(seen[1], 6);
        EXPECT_EQ_INT(seen[2], 5);
    }

    {
        struct list_head head;
        struct intg_item a, b;

        list_init(&head);
        a.val = 1;
        b.val = 2;
        list_add_tail(&head, &a.node);
        list_del_init(&a.node);
        list_add_tail(&head, &a.node);
        list_add_tail(&head, &b.node);
        list_del_head_init(&head);
        list_add_tail(&head, &a.node);
        EXPECT_EQ_INT(intg_count(&head), 2);
        EXPECT_EQ_INT(list_first_entry(&head, struct intg_item, node)->val, 2);
        EXPECT_EQ_INT(list_last_entry(&head, struct intg_item, node)->val, 1);
        EXPECT_EQ_INT(intg_sum(&head), 3);
    }
}
