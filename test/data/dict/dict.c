#include <lszlib.h>

#define MAX_TEST_HANDLE                 32

typedef struct {
    int k;
    int v;
    lsz_dict_node_t node;
} test_t;

void *dict_k_pointer(const lsz_dict_node_t *node)
{
    return &(base_of(node, test_t, node)->k);
}

void *dict_v_pointer(const lsz_dict_node_t *node)
{
    return &(base_of(node, test_t, node)->v);
}

int dict_k_compare(void const *a, void const *b)
{
    return lsz_compare_int(a, b);
}

void dict_tree_free(const lsz_dict_node_t *node)
{
    free(base_of(node, test_t, node));
}

/*
--------------------------------------------------------------------------------
*/

int is_balance = 1;
int is_red_blk = 1;

int is_dict_tree_balance(const lsz_dict_node_t *node)
{
    assert(node);

    if (is_dict_tree_null_node((const lsz_dict_node_t *) node)) {
        return 1;
    }
    //
    // 对每个节点，只检最左最右黑高
    //
    lsz_dict_node_t *next = NULL;
    int lbh = 0;
    int rbh = 0;

    next = (lsz_dict_node_t *) node;
    while(!is_dict_tree_null_node((const lsz_dict_node_t *) next)) {
        next = next->l;
        if (next && next->c == LSZ_DICT_NODE_COLOR_BLK) {
            lbh++;
        }
    }
    lbh++;
    next = (lsz_dict_node_t *) node;
    while(!is_dict_tree_null_node((const lsz_dict_node_t *) next)) {
        next = next->r;
        if (next->c == LSZ_DICT_NODE_COLOR_BLK) {
            rbh++;
        }
    }
    rbh++;
    if (lbh != rbh) {
        printf("%02d(%02d): lbh:rbh = %02d:%02d\n", *(int *)dict_k_pointer(node), *(int *)dict_v_pointer(node), lbh, rbh);
    }

    return lbh == rbh;
}

int is_dict_tree_red_blk(const lsz_dict_node_t *node)
{
    assert(node);
    //
    // 对每个红色节点，双子必须黑色
    //
    if (is_dict_tree_null_node((const lsz_dict_node_t *) node)) {
        return 1;
    }
    if (node->c == LSZ_DICT_NODE_COLOR_BLK) {
        return 1;
    }
    if (node->c == LSZ_DICT_NODE_COLOR_RED) {
        if (node->l->c != LSZ_DICT_NODE_COLOR_BLK) {
            return 0;
        }
        if (node->r->c != LSZ_DICT_NODE_COLOR_BLK) {
            return 0;
        }
        return 1;
    }
    return 0;
}

void dict_for_each_callback(const lsz_dict_node_t *node, void *v)
{
    int k = *(int *)dict_k_pointer(node);

    if (!is_dict_tree_balance(node)) {
        is_balance = 0;
        printf("k: %02d, is_dict_tree_balance: fail\n", k);
    }
    if (!is_dict_tree_red_blk(node)) {
        printf("k: %02d, is_dict_tree_red_blk: fail\n", k);
        is_red_blk = 0;
    }
}

/*
--------------------------------------------------------------------------------
*/

void testcase_rb_tree(void)
{
    dict_t      tree = dict_new(dict_k_pointer, dict_v_pointer, dict_k_compare, dict_tree_free);
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
            } while (dict_search(tree, &test->k));
            dict_insert(tree, &test->node);
            printf("dict_insert: %02d(%02d)\n", test->k, temp.v = *(int *) dict_search(tree, &test->k));
            if (temp.v != test->v) {
                printf("error: k(v): %02d(%02d)\n", test->k, test->v);
            }
        }
        dict_for_each(tree, dict_for_each_callback, NULL);
        TESTCASE_ASSERT_BOOL(is_balance, "is_balance");
        TESTCASE_ASSERT_BOOL(is_red_blk, "is_red_blk");
    }
    {
        printf("+++++++++++++++++++++++++++++++++\n");
        int *min   = NULL;
        int *max   = NULL;
        int *kptr  = NULL;
        int *vptr  = NULL;
        printf("dict_min: %02d\n", *(min = dict_min(tree)));
        printf("dict_max: %02d\n", *(max = dict_max(tree)));
        printf("+++++++++++++++++++++++++++++++++\n");
        kptr = NULL;
        while((vptr = dict_next(tree, (void **) &kptr))) {
            printf("dict_next: %02d(%02d)\n", *kptr, *vptr);
        }
        kptr = NULL;
        while((vptr = dict_prev(tree, (void **) &kptr))) {
            printf("dict_prev: %02d(%02d)\n", *kptr, *vptr);
        }
        printf("+++++++++++++++++++++++++++++++++\n");
    }
    {
        for (int i = 0; i < MAX_TEST_HANDLE; i++) {
            // 模拟随机删除
            do {
                temp.k = rand()%MAX_TEST_HANDLE;
            } while (!dict_search(tree, &temp.k));
            printf("dict_delete: %02d(%02d)\n", temp.k, temp.v = *(int *) dict_search(tree, &temp.k));
            dict_delete(tree, &temp.k);
            if (dict_search(tree, &temp.k)) {
                printf("error: k(v): %02d(%02d)\n", temp.k, temp.v);
            }
        }
        dict_for_each(tree, dict_for_each_callback, NULL);
        TESTCASE_ASSERT_BOOL(is_balance, "is_balance");
        TESTCASE_ASSERT_BOOL(is_red_blk, "is_red_blk");
    }

    dict_free(tree);
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
