#include <list.h>
#include <utest.h>

struct item {
    int val;
    struct list_head node;
};

static int basic_count(const struct list_head *head)
{
    struct list_head *pos;
    int n = 0;

    list_for_each(head, pos)
        n++;
    return n;
}

UTEST_CASE(basic)
{
    {
        struct list_head head = LIST_HEAD_INIT(head);

        EXPECT_EQ_PTR(head.next, &head);
        EXPECT_EQ_PTR(head.prev, &head);
        EXPECT_TRUE(list_empty(&head));
        EXPECT_EQ_INT(basic_count(&head), 0);
    }

    {
        struct list_head head;

        list_init(&head);
        EXPECT_EQ_PTR(head.next, &head);
        EXPECT_EQ_PTR(head.prev, &head);
        EXPECT_TRUE(list_empty(&head));
    }

    {
        struct list_head head;
        struct item a;

        list_init(&head);
        a.val = 10;
        list_init(&a.node);
        list_add_head(&head, &a.node);
        EXPECT_FALSE(list_empty(&head));
        EXPECT_EQ_INT(basic_count(&head), 1);
        EXPECT_EQ_PTR(head.next, &a.node);
        EXPECT_EQ_PTR(head.prev, &a.node);
        EXPECT_EQ_PTR(a.node.next, &head);
        EXPECT_EQ_PTR(a.node.prev, &head);
        EXPECT_EQ_PTR(list_first_entry(&head, struct item, node), &a);
        EXPECT_EQ_PTR(list_last_entry(&head, struct item, node), &a);
        EXPECT_EQ_INT(list_first_entry(&head, struct item, node)->val, 10);
    }

    {
        struct list_head head;
        struct item a, b, c;

        list_init(&head);
        a.val = 1;
        b.val = 2;
        c.val = 3;
        list_add_tail(&head, &a.node);
        list_add_tail(&head, &b.node);
        list_add_tail(&head, &c.node);
        EXPECT_EQ_INT(basic_count(&head), 3);
        EXPECT_EQ_PTR(list_first_entry(&head, struct item, node), &a);
        EXPECT_EQ_PTR(list_last_entry(&head, struct item, node), &c);
        EXPECT_EQ_INT(list_entry(head.next, struct item, node)->val, 1);
        EXPECT_EQ_INT(list_entry(head.next->next, struct item, node)->val, 2);
        EXPECT_EQ_INT(list_entry(head.prev, struct item, node)->val, 3);
    }

    {
        struct list_head head;
        struct item a, b, c;

        list_init(&head);
        a.val = 1;
        b.val = 2;
        c.val = 3;
        list_add_head(&head, &a.node);
        list_add_head(&head, &b.node);
        list_add_head(&head, &c.node);
        EXPECT_EQ_INT(list_entry(head.next, struct item, node)->val, 3);
        EXPECT_EQ_INT(list_entry(head.next->next, struct item, node)->val, 2);
        EXPECT_EQ_INT(list_entry(head.prev, struct item, node)->val, 1);
    }

    {
        struct list_head head;
        struct item a, b;
        struct list_head *n;

        list_init(&head);
        a.val = 1;
        b.val = 2;
        list_add_tail(&head, &a.node);
        list_add_tail(&head, &b.node);
        n = list_del_head(&head);
        EXPECT_EQ_PTR(n, &a.node);
        EXPECT_NULL(a.node.next);
        EXPECT_NULL(a.node.prev);
        EXPECT_EQ_INT(basic_count(&head), 1);
        EXPECT_EQ_PTR(list_first_entry(&head, struct item, node), &b);
        n = list_del_head(&head);
        EXPECT_EQ_PTR(n, &b.node);
        EXPECT_TRUE(list_empty(&head));
    }

    {
        struct list_head head;
        struct item a, b;
        struct list_head *n;

        list_init(&head);
        a.val = 1;
        b.val = 2;
        list_add_tail(&head, &a.node);
        list_add_tail(&head, &b.node);
        n = list_del_tail(&head);
        EXPECT_EQ_PTR(n, &b.node);
        EXPECT_NULL(b.node.next);
        EXPECT_NULL(b.node.prev);
        EXPECT_EQ_PTR(list_last_entry(&head, struct item, node), &a);
        n = list_del_tail(&head);
        EXPECT_EQ_PTR(n, &a.node);
        EXPECT_TRUE(list_empty(&head));
    }

    {
        struct list_head head;
        struct item a, b;
        struct list_head *n;

        list_init(&head);
        a.val = 1;
        b.val = 2;
        list_add_tail(&head, &a.node);
        list_add_tail(&head, &b.node);
        n = list_del_head_init(&head);
        EXPECT_EQ_PTR(n, &a.node);
        EXPECT_TRUE(list_empty(&a.node));
        EXPECT_EQ_PTR(a.node.next, &a.node);
        EXPECT_EQ_PTR(a.node.prev, &a.node);
        EXPECT_EQ_INT(basic_count(&head), 1);
        n = list_del_tail_init(&head);
        EXPECT_EQ_PTR(n, &b.node);
        EXPECT_TRUE(list_empty(&b.node));
        EXPECT_TRUE(list_empty(&head));
    }

    {
        struct list_head head;
        struct item a, b, c;

        list_init(&head);
        a.val = 1;
        b.val = 2;
        c.val = 3;
        list_add_tail(&head, &a.node);
        list_add_tail(&head, &b.node);
        list_add_tail(&head, &c.node);
        list_del(&b.node);
        EXPECT_NULL(b.node.next);
        EXPECT_NULL(b.node.prev);
        EXPECT_EQ_INT(basic_count(&head), 2);
        EXPECT_EQ_PTR(a.node.next, &c.node);
        EXPECT_EQ_PTR(c.node.prev, &a.node);
        EXPECT_EQ_PTR(list_first_entry(&head, struct item, node), &a);
        EXPECT_EQ_PTR(list_last_entry(&head, struct item, node), &c);
    }

    {
        struct list_head head;
        struct item a, b, c;

        list_init(&head);
        a.val = 1;
        b.val = 2;
        c.val = 3;
        list_add_tail(&head, &a.node);
        list_add_tail(&head, &b.node);
        list_add_tail(&head, &c.node);
        list_del_init(&b.node);
        EXPECT_TRUE(list_empty(&b.node));
        EXPECT_EQ_INT(basic_count(&head), 2);
        EXPECT_EQ_PTR(a.node.next, &c.node);
        EXPECT_EQ_PTR(c.node.prev, &a.node);
        list_add_tail(&head, &b.node);
        EXPECT_EQ_INT(basic_count(&head), 3);
        EXPECT_EQ_PTR(list_last_entry(&head, struct item, node), &b);
    }

    {
        struct wrap {
            char tag;
            struct list_head node;
            int val;
        } a, b;
        struct list_head head;

        list_init(&head);
        a.tag = 'a';
        a.val = 11;
        b.tag = 'b';
        b.val = 22;
        list_add_tail(&head, &a.node);
        list_add_tail(&head, &b.node);
        EXPECT_EQ_PTR(list_entry(&a.node, struct wrap, node), &a);
        EXPECT_EQ_INT(list_first_entry(&head, struct wrap, node)->val, 11);
        EXPECT_EQ_CHAR(list_last_entry(&head, struct wrap, node)->tag, 'b');
    }
}
