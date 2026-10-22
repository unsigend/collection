#include <list.h>
#include <utest.h>

struct edg_item {
    int val;
    struct list_head node;
};

static int edg_count(const struct list_head *head)
{
    struct list_head *pos;
    int n = 0;

    list_for_each(head, pos)
        n++;
    return n;
}

static int edg_links_ok(const struct list_head *head)
{
    const struct list_head *pos;

    if (head->next->prev != head || head->prev->next != head)
        return 0;
    for (pos = head->next; pos != head; pos = pos->next) {
        if (pos->next->prev != pos || pos->prev->next != pos)
            return 0;
    }
    return 1;
}

UTEST_CASE(edge)
{
    {
        struct list_head head;

        list_init(&head);
        EXPECT_NULL(list_del_head(&head));
        EXPECT_NULL(list_del_tail(&head));
        EXPECT_NULL(list_del_head_init(&head));
        EXPECT_NULL(list_del_tail_init(&head));
        EXPECT_TRUE(list_empty(&head));
        EXPECT_EQ_PTR(head.next, &head);
        EXPECT_EQ_PTR(head.prev, &head);
    }

    {
        struct list_head head;
        struct edg_item a;
        struct list_head *n;

        list_init(&head);
        a.val = 1;
        list_add_head(&head, &a.node);
        n = list_del_head(&head);
        EXPECT_EQ_PTR(n, &a.node);
        EXPECT_TRUE(list_empty(&head));
        EXPECT_NULL(list_del_head(&head));
        EXPECT_NULL(list_del_tail(&head));
    }

    {
        struct list_head head;
        struct edg_item a;
        struct list_head *n;

        list_init(&head);
        a.val = 1;
        list_add_tail(&head, &a.node);
        n = list_del_tail(&head);
        EXPECT_EQ_PTR(n, &a.node);
        EXPECT_TRUE(list_empty(&head));
        EXPECT_EQ_INT(edg_count(&head), 0);
        EXPECT_TRUE(edg_links_ok(&head));
    }

    {
        struct list_head head;
        struct edg_item a;
        struct list_head *n;

        list_init(&head);
        a.val = 7;
        list_add_head(&head, &a.node);
        n = list_del_head_init(&head);
        EXPECT_EQ_PTR(n, &a.node);
        EXPECT_TRUE(list_empty(&head));
        EXPECT_TRUE(list_empty(&a.node));
        n = list_del_tail_init(&head);
        EXPECT_NULL(n);
    }

    {
        struct list_head head;
        struct edg_item a;

        list_init(&head);
        a.val = 1;
        list_add_tail(&head, &a.node);
        list_del(&a.node);
        EXPECT_NULL(a.node.next);
        EXPECT_NULL(a.node.prev);
        EXPECT_TRUE(list_empty(&head));
        EXPECT_TRUE(edg_links_ok(&head));
        list_add_tail(&head, &a.node);
        EXPECT_FALSE(list_empty(&head));
        EXPECT_EQ_PTR(list_first_entry(&head, struct edg_item, node), &a);
        EXPECT_TRUE(edg_links_ok(&head));
    }

    {
        struct list_head head;
        struct edg_item a, b;

        list_init(&head);
        a.val = 1;
        b.val = 2;
        list_add_tail(&head, &a.node);
        list_add_tail(&head, &b.node);
        list_del_init(&a.node);
        EXPECT_TRUE(list_empty(&a.node));
        EXPECT_EQ_INT(edg_count(&head), 1);
        list_del_init(&b.node);
        list_add_head(&a.node, &b.node);
        EXPECT_TRUE(list_empty(&head));
        EXPECT_EQ_INT(edg_count(&a.node), 1);
        EXPECT_EQ_PTR(list_first_entry(&a.node, struct edg_item, node), &b);
    }

    {
        struct list_head head;
        struct edg_item a, b, c;

        list_init(&head);
        a.val = 1;
        b.val = 2;
        c.val = 3;
        list_add_tail(&head, &a.node);
        list_add_tail(&head, &b.node);
        list_add_tail(&head, &c.node);
        list_del(&a.node);
        list_del(&c.node);
        EXPECT_EQ_INT(edg_count(&head), 1);
        EXPECT_EQ_PTR(list_first_entry(&head, struct edg_item, node), &b);
        EXPECT_EQ_PTR(list_last_entry(&head, struct edg_item, node), &b);
        EXPECT_EQ_PTR(b.node.next, &head);
        EXPECT_EQ_PTR(b.node.prev, &head);
        EXPECT_TRUE(edg_links_ok(&head));
    }

    {
        struct list_head head;
        struct edg_item a, b;

        list_init(&head);
        a.val = 1;
        b.val = 2;
        list_add_head(&head, &a.node);
        list_add_head(&head, &b.node);
        list_init(&head);
        EXPECT_TRUE(list_empty(&head));
        EXPECT_EQ_INT(edg_count(&head), 0);
    }

    {
        struct list_head head;
        struct edg_item nodes[8];
        int i;

        list_init(&head);
        for (i = 0; i < 8; i++) {
            nodes[i].val = i;
            list_add_tail(&head, &nodes[i].node);
        }
        EXPECT_EQ_INT(edg_count(&head), 8);
        EXPECT_TRUE(edg_links_ok(&head));
        list_del(&nodes[0].node);
        list_del(&nodes[7].node);
        list_del(&nodes[3].node);
        EXPECT_EQ_INT(edg_count(&head), 5);
        EXPECT_EQ_INT(list_first_entry(&head, struct edg_item, node)->val, 1);
        EXPECT_EQ_INT(list_last_entry(&head, struct edg_item, node)->val, 6);
        EXPECT_TRUE(edg_links_ok(&head));
    }

    {
        struct list_head head = LIST_HEAD_INIT(head);
        struct edg_item a;

        EXPECT_TRUE(list_empty(&head));
        a.val = 9;
        list_add_tail(&head, &a.node);
        EXPECT_FALSE(list_empty(&head));
        list_del_init(&a.node);
        EXPECT_TRUE(list_empty(&head));
        EXPECT_TRUE(list_empty(&a.node));
    }
}
