#include <lszlib.h>
#include <assert.h>

int *tree_k_pointer(const lsz_tree_t *node);
int *tree_v_pointer(const lsz_tree_t *node);

int is_tree_balance(const lsz_tree_t *node)
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

    next = (lsz_tree_t *) node;
    while(!is_tree_null_node((const lsz_tree_t *) next)) {
        next = next->l;
        if (next && next->c == LSZ_TREE_BLK) {
            lbh++;
        }
    }
    lbh++;
    next = (lsz_tree_t *) node;
    while(!is_tree_null_node((const lsz_tree_t *) next)) {
        next = next->r;
        if (next->c == LSZ_TREE_BLK) {
            rbh++;
        }
    }
    rbh++;
    if (lbh != rbh) {
        printf("%02d(%02d): lbh:rbh = %02d:%02d\n", *tree_k_pointer(node), *tree_v_pointer(node), lbh, rbh);
    }

    return lbh == rbh;
}

int is_tree_rslrblk(const lsz_tree_t *node)
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

int is_balance = 1;
int is_rsonblk = 1;

void tree_for_each_callback(const lsz_tree_t *node, void *v)
{
    int k = *tree_k_pointer(node);

    if (!is_tree_balance(node)) {
        is_balance = 0;
        printf("k: %02d, is_tree_balance: fail\n", k);
    }
    if (!is_tree_rslrblk(node)) {
        printf("k: %02d, is_tree_rslrblk: fail\n", k);
        is_rsonblk = 0;
    }
}
