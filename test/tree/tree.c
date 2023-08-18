#include <lszlib.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

extern  lsz_tree_t __lsz_tree_nil__;

#define is_tree_null_node(node) \
    ((node) && ((node) == &__lsz_tree_nil__))

//
// 私有数据
//
typedef struct _lsz_tree_private_t {
    lsz_tree_t                 *root;
    lsz_tree_t                 *null;
    size_t                      sz_k;
    size_t                      sz_v;
    lsz_compare_t               cb_k;
    pthread_mutex_t             lock;
} lsz_tree_private_t;
//
// 私有成员
//
#define lsz_tree_this(tree, field) \
    (((lsz_tree_private_t *) (tree))->field)

//
//! 遍历回调函数（危险）
//
typedef void (* lsz_tree_for_each_unsafe_callback_t) (
    lsz_tree_t                 *node,
    void                       *data
);

void temp_for_each_unsafe(lsz_tree_t *node, lsz_tree_for_each_unsafe_callback_t cb_func, void *cb_data)
{
    assert(node);

    if (!is_tree_null_node(node)) {
        temp_for_each_unsafe(node->l, cb_func, cb_data);
        if (cb_func) cb_func(node, cb_data);
        temp_for_each_unsafe(node->r, cb_func, cb_data);
    }
}

void tree_for_each_unsafe(const void *tree, lsz_tree_for_each_unsafe_callback_t cb_func, void *cb_data)
{
    assert(tree);

    temp_for_each_unsafe(lsz_tree_this(tree, root), cb_func, cb_data);
}

int is_tree_balance(lsz_tree_t *node)
{
    assert(node);

    if (is_tree_null_node((const lsz_tree_t *) node)) {
        return 1;
    }
    //
    // 对每个节点，只检最左最右黑高
    //
    lsz_tree_t *next = NULL;
    int lbh = 0;
    int rbh = 0;

    next = node;
    while(!is_tree_null_node((const lsz_tree_t *) next)) {
        next = next->l;
        if (next && next->c == LSZ_TREE_BLK) {
            lbh++;
        }
    }
    lbh++;
    next = node;
    while(!is_tree_null_node((const lsz_tree_t *) next)) {
        next = next->r;
        if (next->c == LSZ_TREE_BLK) {
            rbh++;
        }
    }
    rbh++;
    if (lbh != rbh) {
        printf("%02d(%02d): lbh:rbh = %02d:%02d\n", *(int *) node->k, *(int *) node->v, lbh, rbh);
    }

    return lbh == rbh;
}

int is_tree_rslrblk(lsz_tree_t *node)
{
    assert(node);
    //
    // 对每个红色节点，双子必须黑色
    //
    if (is_tree_null_node((const lsz_tree_t *) node)) {
        return 1;
    }
    if (node->c == LSZ_TREE_BLK) {
        return 1;
    }
    if (node->c == LSZ_TREE_RED) {
        if (node->l->c != LSZ_TREE_BLK) {
            return 0;
        }
        if (node->r->c != LSZ_TREE_BLK) {
            return 0;
        }
        return 1;
    }
    return 0;
}

static int is_balance = 1;
static int is_rslrblk = 1;
#define MAX_TEST_HANDLE 32

void callback(lsz_tree_t *node, void *v)
{
    int k = *(int *) node->k;

    if (!is_tree_balance(node)) {
        is_balance = 0;
        printf("k: %02d, is_tree_balance: fail\n", k);
    }
    if (!is_tree_rslrblk(node)) {
        printf("k: %02d, is_tree_rslrblk: fail\n", k);
        is_rslrblk = 0;
    }
}

int main(int argc, char const *argv[])
{
    void *tree = tree_new(sizeof(int), sizeof(int), lsz_icmp);

    srand(time(0));

    int hint = 0;
    int dint = 0;
    int dget = 0;
    {
        for (int i = 0; i < MAX_TEST_HANDLE; i++) {
            // 模拟随机插入
            dint = i;
            do {
                hint = rand()%MAX_TEST_HANDLE;
            } while (tree_search(tree, &hint));
            tree_insert(tree, &hint, &dint);
            printf("tree_insert: %02d(%02d)\n", hint, dget = *(int *) tree_search(tree, &hint));
            if (dget != dint) {
                printf("tree_search: %02d(%02d)\n", hint, dget);
            }
        }
        printf("+++++++++++++++++++++++++++++++++\n");
        printf("result:\n");
        tree_for_each_unsafe(tree, callback, NULL);
        printf("---------------------------------\n");
        printf("  is_balance: %s\n", is_balance?"pass":"fail");
        printf("  is_rslrblk: %s\n", is_rslrblk?"pass":"fail");
    }
    {
        printf("+++++++++++++++++++++++++++++++++\n");
        int *min   = NULL;
        int *max   = NULL;
        int *hptr  = NULL;
        int *dptr  = NULL;
        printf("tree_min: %02d\n", *(min = tree_min(tree)));
        printf("tree_max: %02d\n", *(max = tree_max(tree)));
        hptr = NULL;
        while((dptr = tree_next(tree, (void **) &hptr))) {
            printf("tree_next: %02d(%02d)\n", *hptr, *dptr);
        }
        hptr = NULL;
        while((dptr = tree_prev(tree, (void **) &hptr))) {
            printf("tree_prev: %02d(%02d)\n", *hptr, *dptr);
        }
        printf("+++++++++++++++++++++++++++++++++\n");
    }
    {
        for (int i = 0; i < MAX_TEST_HANDLE; i++) {
            // 模拟随机删除
            do {
                hint = rand()%MAX_TEST_HANDLE;
            } while (!tree_search(tree, &hint));
            printf("tree_delete: %02d(%02d)\n", hint, dint = *(int *) tree_search(tree, &hint));
            tree_delete(tree, &hint);
            if (tree_search(tree, &hint)) {
                printf("tree_search: %02d(%02d)\n", hint, dint);
            }
        }
        printf("+++++++++++++++++++++++++++++++++\n");
        printf("result:\n");
        tree_for_each_unsafe(tree, callback, NULL);
        printf("---------------------------------\n");
        printf("  is_balance: %s\n", is_balance?"pass":"fail");
        printf("  is_rslrblk: %s\n", is_rslrblk?"pass":"fail");
        printf("+++++++++++++++++++++++++++++++++\n");
    }

    tree_free(tree);

    return 0;
}
