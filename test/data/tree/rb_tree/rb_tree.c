#include <lszlib.h>

#define MAX_TEST_HANDLE                 32

typedef struct {
    int k;
    int v;
    lsz_rb_node_t node;
} test_t;

void *tree_k_pointer(const lsz_rb_node_t *node)
{
    return &(base_of(node, test_t, node)->k);
}

void *tree_v_pointer(const lsz_rb_node_t *node)
{
    return &(base_of(node, test_t, node)->v);
}

int tree_k_compare(void const *a, void const *b)
{
    return lsz_compare_int(a, b);
}

void tree_node_free(const lsz_rb_node_t *node)
{
    free(base_of(node, test_t, node));
}

/*
--------------------------------------------------------------------------------
*/

int is_balance = 1;
int is_red_blk = 1;

int is_tree_balance(const lsz_rb_node_t *node)
{
    assert(node);

    if (is_rb_tree_null_node((const lsz_rb_node_t *) node)) {
        return 1;
    }
    //
    // 对每个节点，只检最左最右黑高
    //
    lsz_rb_node_t *next = NULL;
    int lbh = 0;
    int rbh = 0;

    next = (lsz_rb_node_t *) node;
    while(!is_rb_tree_null_node((const lsz_rb_node_t *) next)) {
        next = next->l;
        if (next && next->c == LSZ_RB_TREE_BLK) {
            lbh++;
        }
    }
    lbh++;
    next = (lsz_rb_node_t *) node;
    while(!is_rb_tree_null_node((const lsz_rb_node_t *) next)) {
        next = next->r;
        if (next->c == LSZ_RB_TREE_BLK) {
            rbh++;
        }
    }
    rbh++;
    if (lbh != rbh) {
        printf("%02d(%02d): lbh:rbh = %02d:%02d\n", *(int *)tree_k_pointer(node), *(int *)tree_v_pointer(node), lbh, rbh);
    }

    return lbh == rbh;
}

int is_tree_rslrblk(const lsz_rb_node_t *node)
{
    assert(node);
    //
    // 对每个红色节点，双子必须黑色
    //
    if (is_rb_tree_null_node((const lsz_rb_node_t *) node)) {
        return 1;
    }
    if (node->c == LSZ_RB_TREE_BLK) {
        return 1;
    }
    if (node->c == LSZ_RB_TREE_RED) {
        if (node->l->c != LSZ_RB_TREE_BLK) {
            return 0;
        }
        if (node->r->c != LSZ_RB_TREE_BLK) {
            return 0;
        }
        return 1;
    }
    return 0;
}

void tree_for_each_callback(const lsz_rb_node_t *node, void *v)
{
    int k = *(int *)tree_k_pointer(node);

    if (!is_tree_balance(node)) {
        is_balance = 0;
        printf("k: %02d, is_tree_balance: fail\n", k);
    }
    if (!is_tree_rslrblk(node)) {
        printf("k: %02d, is_tree_rslrblk: fail\n", k);
        is_red_blk = 0;
    }
}

/*
--------------------------------------------------------------------------------
*/

void testcase_rb_tree(void)
{
    rb_tree_t   tree = rb_tree_new(tree_k_pointer, tree_v_pointer, tree_k_compare, tree_node_free);
    test_t     *test = NULL;
    test_t      temp;

    srand(time(0));
    {
        for (int i = 0; i < MAX_TEST_HANDLE; i++) {
            // 模拟随机插入
            test = malloc(sizeof(test_t));
            test->v = i;
            do {
                test->k = rand()%MAX_TEST_HANDLE;
            } while (rb_tree_search(tree, &test->k));
            rb_tree_insert(tree, &test->node);
            printf("rb_tree_insert: %02d(%02d)\n", test->k, temp.v = *(int *) rb_tree_search(tree, &test->k));
            if (temp.v != test->v) {
                printf("error: k(v): %02d(%02d)\n", test->k, test->v);
            }
        }
        rb_tree_for_each(tree, tree_for_each_callback, NULL);
        TESTCASE_ASSERT_BOOL(is_balance, "is_balance");
        TESTCASE_ASSERT_BOOL(is_red_blk, "is_red_blk");
    }
    {
        printf("+++++++++++++++++++++++++++++++++\n");
        int *min   = NULL;
        int *max   = NULL;
        int *kptr  = NULL;
        int *vptr  = NULL;
        printf("rb_tree_min: %02d\n", *(min = rb_tree_min(tree)));
        printf("rb_tree_max: %02d\n", *(max = rb_tree_max(tree)));
        printf("+++++++++++++++++++++++++++++++++\n");
        kptr = NULL;
        while((vptr = rb_tree_next(tree, (void **) &kptr))) {
            printf("rb_tree_next: %02d(%02d)\n", *kptr, *vptr);
        }
        kptr = NULL;
        while((vptr = rb_tree_prev(tree, (void **) &kptr))) {
            printf("rb_tree_prev: %02d(%02d)\n", *kptr, *vptr);
        }
        printf("+++++++++++++++++++++++++++++++++\n");
    }
    {
        for (int i = 0; i < MAX_TEST_HANDLE; i++) {
            // 模拟随机删除
            do {
                temp.k = rand()%MAX_TEST_HANDLE;
            } while (!rb_tree_search(tree, &temp.k));
            printf("rb_tree_delete: %02d(%02d)\n", temp.k, temp.v = *(int *) rb_tree_search(tree, &temp.k));
            rb_tree_delete(tree, &temp.k);
            if (rb_tree_search(tree, &temp.k)) {
                printf("error: k(v): %02d(%02d)\n", temp.k, temp.v);
            }
        }
        rb_tree_for_each(tree, tree_for_each_callback, NULL);
        TESTCASE_ASSERT_BOOL(is_balance, "is_balance");
        TESTCASE_ASSERT_BOOL(is_red_blk, "is_red_blk");
    }

    rb_tree_free(tree);
}

/*
--------------------------------------------------------------------------------
*/

int main(int argc, char const *argv[])
{
    lsz_testcase_do {
        testcase_rb_tree();
    } lsz_testcase_done;
}
