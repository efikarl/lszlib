/** @file
--------------------------------------------------------------------------------
    Copyright ©2023-2024 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
--------------------------------------------------------------------------------
    数据 - 红黑树
--------------------------------------------------------------------------------
--*/
#include <pthread.h>

#include "lszlib.h"

/*
--------------------------------------------------------------------------------
type
--------------------------------------------------------------------------------
*/

#define LSZ_RB_TREE_THIS_SIGNATURE ('t' << 24 | 'r' << 16 | 'e' << 8 | 'e')
//
// 私有数据
//
typedef struct _lsz_rb_tree_t {
    uint32_t                         _;
    pthread_mutex_t               lock;
    lsz_rb_node_t                *root;
    lsz_rb_tree_k_pointer_t    fn_kptr;
    lsz_rb_tree_v_pointer_t    fn_vptr;
    lsz_rb_tree_k_compare_t    fn_kcmp;
    lsz_rb_tree_node_free_t    fn_free;
} lsz_rb_tree_t;
//
// 私有成员
//
#define lsz_rb_tree_this(tree, field)  (((lsz_rb_tree_t *) (tree))->field)

//
// 叶子节点
//
lsz_rb_node_t __lsz_rb_tree_null_node__ = {
    ._ = LSZ_RB_TREE_NODE_SIGNATURE,
    .c = LSZ_RB_TREE_BLK,
};

/*
--------------------------------------------------------------------------------
impl
--------------------------------------------------------------------------------
*/

//
// 签名验证
//
int is_rb_tree_signature(lsz_rb_tree_t *this)
{
    return ((this) && (this->_ == LSZ_RB_TREE_THIS_SIGNATURE));
}

//
// 节点验证
//
int is_rb_node_signature(lsz_rb_node_t const *node)
{
    return ((node) && (node->_ == LSZ_RB_TREE_NODE_SIGNATURE));
}

int is_rb_tree_null_node(lsz_rb_node_t const *node)
{
    return (node == &__lsz_rb_tree_null_node__);
}

//
// 键值获取
//
void *k_of_rb_tree(lsz_rb_tree_t *this, lsz_rb_node_t const *node)
{
    if (!is_rb_tree_signature(this) || !is_rb_node_signature(node) || is_rb_tree_null_node(node)) {
        assert(!!0);
        return NULL;
    } else {
        return this->fn_kptr(node);
    }
}

void *v_of_rb_tree(lsz_rb_tree_t *this, lsz_rb_node_t const *node)
{
    if (!is_rb_tree_signature(this) || !is_rb_node_signature(node) || is_rb_tree_null_node(node)) {
        assert(!!0);
        return NULL;
    } else {
        return this->fn_vptr(node);
    }
}

/*
--------------------------------------------------------------------------------
*/

//
// 旋转操作：只改变树的指针，不改变树的次序属性
//
/*@fn: lsz_rb_tree_rotate_l
--------------------------------------------------------------------------------
     lp                    lp
      |                     |
      l                     r
     / \                   / \
    ll  r      =左旋=>     l  rr
       / \               /  \
      rl  rr            ll  rl
--------------------------------------------------------------------------------
*/
void lsz_rb_tree_rotate_l(lsz_rb_node_t **root, lsz_rb_node_t *l)
{
    if (!is_rb_node_signature(*root) || !is_rb_node_signature(l) || !is_rb_node_signature(l->r)) {
        assert(!!0);
    }

    lsz_rb_node_t *r = l->r;
    // r的父亲
    r->p = l->p;
    if (is_rb_tree_null_node(l->p)) {
        *root = r;
    } else {
        if (l == l->p->l) {
            l->p->l = r;
        } else {
            l->p->r = r;
        }
    }
    // l的右子
    l->r = r->l;
    if (!is_rb_tree_null_node(r->l)) {
        r->l->p = l;
    }
    // r的左子
    r->l = l;
    l->p = r;
}

/*@fn: lsz_rb_tree_rotate_r
--------------------------------------------------------------------------------
     rp                    rp
      |                     |
      l                     r
     / \                   / \
    ll  r      <=右旋=     l  rr
       / \               /  \
      lr  rr            ll  lr
--------------------------------------------------------------------------------
*/
void lsz_rb_tree_rotate_r(lsz_rb_node_t **root, lsz_rb_node_t *r)
{
    if (!is_rb_node_signature(*root) || !is_rb_node_signature(r) || !is_rb_node_signature(r->l)) {
        assert(!!0);
    }

    lsz_rb_node_t *l = r->l;
    // l的父亲
    l->p = r->p;
    if (is_rb_tree_null_node(r->p)) {
        *root = l;
    } else {
        if (r == r->p->r) {
            r->p->r = l;
        } else {
            r->p->l = l;
        }
    }
    // r的左子
    r->l = l->r;
    if (!is_rb_tree_null_node(l->r)) {
        l->r->p = r;
    }
    // l的右子
    l->r = r;
    r->p = l;
}

/*@fn: lsz_rb_tree_transplant
--------------------------------------------------------------------------------
    目的：子树替换（以v替换u）
--------------------------------------------------------------------------------
*/
void lsz_rb_tree_transplant(lsz_rb_node_t **root, lsz_rb_node_t *u, lsz_rb_node_t *v)
{
    if (!is_rb_node_signature(*root) || !is_rb_node_signature(u) || !is_rb_node_signature(v) || is_rb_tree_null_node(u)) {
        assert(!!0);
    }

    if (is_rb_tree_null_node(u->p)) {
        *root = v;
    } else if (u == u->p->l) {
        u->p->l = v;
    } else if (u == u->p->r) {
        u->p->r = v;
    }
    v->p = u->p;
}

/*@fn: lsz_rb_tree_insert_fixup
--------------------------------------------------------------------------------
    目的：不删减节点，保持红黑树性质
--------------------------------------------------------------------------------
*/
void lsz_rb_tree_insert_fixup(lsz_rb_node_t **root, lsz_rb_node_t *i)
{
    if (!is_rb_node_signature(*root) || !is_rb_node_signature(i)) {
        assert(!!0);
    }
    //
    // 插入操作可能破坏：性质2，性质4
    //
    lsz_rb_node_t *uofi = NULL;
    //
    // 红父条件下，i祖父必然存在
    //
    while (i->p->c == LSZ_RB_TREE_RED) {
        //
        // 参考：算法导论
        //
        if (i->p == i->p->p->l) {
            uofi = i->p->p->r;
            if (uofi->c == LSZ_RB_TREE_RED) {
                i->p->c    = LSZ_RB_TREE_BLK;
                uofi->c    = LSZ_RB_TREE_BLK;
                i->p->p->c = LSZ_RB_TREE_RED;
                i = i->p->p;
            } else {
                if (i == i->p->r) {
                    i = i->p;
                    lsz_rb_tree_rotate_l(root, i);
                }
                i->p->c    = LSZ_RB_TREE_BLK;
                i->p->p->c = LSZ_RB_TREE_RED;
                lsz_rb_tree_rotate_r(root, i->p->p);
            }
        } else {
            uofi = i->p->p->l;
            if (uofi->c == LSZ_RB_TREE_RED) {
                i->p->c    = LSZ_RB_TREE_BLK;
                uofi->c    = LSZ_RB_TREE_BLK;
                i->p->p->c = LSZ_RB_TREE_RED;
                i = i->p->p;
            } else {
                if (i == i->p->l) {
                    i = i->p;
                    lsz_rb_tree_rotate_r(root, i);
                }
                i->p->c    = LSZ_RB_TREE_BLK;
                i->p->p->c = LSZ_RB_TREE_RED;
                lsz_rb_tree_rotate_l(root, i->p->p);
            }
        }
    }
    (*root)->c = LSZ_RB_TREE_BLK;
}

/*@fn: lsz_rb_tree_delete_fixup
--------------------------------------------------------------------------------
    目的：不删减节点，保持红黑树性质
--------------------------------------------------------------------------------
*/
void lsz_rb_tree_delete_fixup(lsz_rb_node_t **root, lsz_rb_node_t *i)
{
    if (!is_rb_node_signature(*root) || !is_rb_node_signature(i)) {
        assert(!!0);
    }
    //
    // 删除操作可能破坏：性质1，性质2，性质4
    //
    lsz_rb_node_t *bofi = NULL;
    //
    // 双黑非根时，i兄弟必然存在
    //
    while ((!is_rb_tree_null_node(i->p)) && (i->c == LSZ_RB_TREE_BLK)) {
        //
        // 参考：算法导论
        //
        if (i == i->p->l) {
            bofi = i->p->r;
            if (bofi->c == LSZ_RB_TREE_RED) {
                bofi->c = LSZ_RB_TREE_BLK;
                i->p->c = LSZ_RB_TREE_RED;
                lsz_rb_tree_rotate_l(root, i->p); bofi = i->p->r;
            }
            if (((bofi->l->c == LSZ_RB_TREE_BLK)) &&
                ((bofi->r->c == LSZ_RB_TREE_BLK))) {
                bofi->c = LSZ_RB_TREE_RED;
                i = i->p;
            } else {
                if (((bofi->l->c == LSZ_RB_TREE_RED)) &&
                    ((bofi->r->c == LSZ_RB_TREE_BLK))) {
                    bofi->l->c = LSZ_RB_TREE_BLK;
                    bofi->c    = LSZ_RB_TREE_RED;
                    lsz_rb_tree_rotate_r(root, bofi); bofi = i->p->r;
                }
                if (bofi->r->c == LSZ_RB_TREE_RED) {
                    bofi->c    = i->p->c;
                    i->p->c    = LSZ_RB_TREE_BLK;
                    bofi->r->c = LSZ_RB_TREE_BLK;
                    lsz_rb_tree_rotate_l(root, i->p);
                    // 双黑解除，终止循环
                    i = *root;
                }
            }
        } else {
            bofi = i->p->l;
            if (bofi->c == LSZ_RB_TREE_RED) {
                bofi->c = LSZ_RB_TREE_BLK;
                i->p->c = LSZ_RB_TREE_RED;
                lsz_rb_tree_rotate_r(root, i->p); bofi = i->p->l;
            }
            if (((bofi->r->c == LSZ_RB_TREE_BLK)) &&
                ((bofi->l->c == LSZ_RB_TREE_BLK))) {
                bofi->c = LSZ_RB_TREE_RED;
                i = i->p;
            } else {
                if (((bofi->r->c == LSZ_RB_TREE_RED)) &&
                    ((bofi->l->c == LSZ_RB_TREE_BLK))) {
                    bofi->r->c = LSZ_RB_TREE_BLK;
                    bofi->c    = LSZ_RB_TREE_RED;
                    lsz_rb_tree_rotate_l(root, bofi); bofi = i->p->l;
                }
                if (bofi->l->c == LSZ_RB_TREE_RED) {
                    bofi->c    = i->p->c;
                    i->p->c    = LSZ_RB_TREE_BLK;
                    bofi->l->c = LSZ_RB_TREE_BLK;
                    lsz_rb_tree_rotate_r(root, i->p);
                    // 双黑解除，终止循环
                    i = *root;
                }
            }
        }
    }
    //
    // 情况1到情况2的补偿
    //
    if (i->c == LSZ_RB_TREE_RED) {
        i->c =  LSZ_RB_TREE_BLK;
    }
}

/*
--------------------------------------------------------------------------------
*/

lsz_rb_node_t *lsz_rb_tree_min(lsz_rb_node_t *node)
{
    if (!is_rb_node_signature(node)) {
        assert(!!0);
        return &__lsz_rb_tree_null_node__;
    }

    lsz_rb_node_t *find = node;
    while (!is_rb_tree_null_node(node)) {
        find = node;
        node = node->l;
    }
    return find;
}

lsz_rb_node_t *lsz_rb_tree_max(lsz_rb_node_t *node)
{
    if (!is_rb_node_signature(node)) {
        assert(!!0);
        return &__lsz_rb_tree_null_node__;
    }

    lsz_rb_node_t *find = node;
    while (!is_rb_tree_null_node(node)) {
        find = node;
        node = node->r;
    }
    return find;
}

lsz_rb_node_t *lsz_rb_tree_prev(lsz_rb_node_t *node)
{
    if (!is_rb_node_signature(node)) {
        assert(!!0);
        return &__lsz_rb_tree_null_node__;
    }

    if (is_rb_tree_null_node(node)) {
        return node;
    }
    if (!is_rb_tree_null_node(node->l)) {
        node = lsz_rb_tree_max(node->l);
    } else {
        while (!is_rb_tree_null_node(node->p) && (node == node->p->l)) {
            node = node->p;
        }
        node = node->p;
    }

    return node;
}

lsz_rb_node_t *lsz_rb_tree_next(lsz_rb_node_t *node)
{
    if (!is_rb_node_signature(node)) {
        assert(!!0);
        return &__lsz_rb_tree_null_node__;
    }

    if (is_rb_tree_null_node(node)) {
        return node;
    }
    if (!is_rb_tree_null_node(node->r)) {
        node = lsz_rb_tree_min(node->r);
    } else {
        while (!is_rb_tree_null_node(node->p) && (node == node->p->r)) {
            node = node->p;
        }
        node = node->p;
    }

    return node;
}

void lsz_rb_tree_for_each(lsz_rb_node_t *node, lsz_rb_tree_for_each_callback_t cb_func, void *cb_data)
{
    if (!is_rb_node_signature(node)) {
        assert(!!0);
    }

    if (!is_rb_tree_null_node(node)) {
        lsz_rb_tree_for_each(node->l, cb_func, cb_data);
        if (cb_func) cb_func(node, cb_data);
        lsz_rb_tree_for_each(node->r, cb_func, cb_data);
    }
}

void lsz_rb_tree_reset_nil(void)
{
    __lsz_rb_tree_null_node__._ = LSZ_RB_TREE_NODE_SIGNATURE;
    __lsz_rb_tree_null_node__.c = LSZ_RB_TREE_BLK;
}

lsz_rb_node_t *lsz_rb_tree_search(rb_tree_t const tree, void const *k)
{
    if (!is_rb_tree_signature(tree) || !k) {
        assert(!!0);
        return &__lsz_rb_tree_null_node__;
    }
    lsz_rb_node_t *node = lsz_rb_tree_this(tree, root);

    int i = 0;
    while ((!is_rb_tree_null_node(node)) && (i = lsz_rb_tree_this(tree, fn_kcmp)(k, k_of_rb_tree(tree, node)))) {
        if (i < 0) {
            node = node->l;
        } else {
            node = node->r;
        }
    }
    return node;
}

/*
--------------------------------------------------------------------------------
apis
--------------------------------------------------------------------------------
*/

rb_tree_t rb_tree_new(lsz_rb_tree_k_pointer_t fn_kptr, lsz_rb_tree_v_pointer_t fn_vptr, lsz_rb_tree_k_compare_t fn_kcmp, lsz_rb_tree_node_free_t fn_free)
{
    if (!fn_kptr) {
        printf("LSZ_RET_E_ARG: !fn_kptr\n");
        return NULL;
    }
    if (!fn_vptr) {
        printf("LSZ_RET_E_ARG: !fn_vptr\n");
        return NULL;
    }
    if (!fn_kcmp) {
        printf("LSZ_RET_E_ARG: !fn_kcmp\n");
        return NULL;
    }
    if (!fn_free) {
        printf("LSZ_RET_E_ARG: !fn_free\n");
        return NULL;
    }

    lsz_rb_tree_t *this = calloc(1, sizeof(lsz_rb_tree_t));
    if (!this) {
        printf("LSZ_RET_E_OUT: this\n");
        return NULL;
    }
    this->_       = LSZ_RB_TREE_THIS_SIGNATURE;
    this->root    = &__lsz_rb_tree_null_node__;
    this->fn_kptr = fn_kptr;
    this->fn_vptr = fn_vptr;
    this->fn_kcmp = fn_kcmp;
    this->fn_free = fn_free;
    pthread_mutex_init(&this->lock, NULL);

    return (rb_tree_t) this;
}

int is_rb_tree_empty(rb_tree_t const tree)
{
    if (!is_rb_tree_signature(tree)) {
        printf("LSZ_RET_E_ARG: tree\n");
        return !!1;
    }

    if (is_rb_tree_null_node(lsz_rb_tree_this(tree, root))) {
        return !!1;
    } else {
        return !!0;
    }
}

int rb_tree_insert(rb_tree_t tree, lsz_rb_node_t *node)
{
    int retn = LSZ_RET_0_ERR;

    if (!is_rb_tree_signature(tree)) {
        printf("LSZ_RET_E_ARG: tree\n");
        return LSZ_RET_E_ARG;
    }
    if (!node) {
        printf("LSZ_RET_E_ARG: node\n");
        return LSZ_RET_E_ARG;
    }
    //
    // 节点的初始化，强制执行
    //
    node->_ = LSZ_RB_TREE_NODE_SIGNATURE;
    node->c = LSZ_RB_TREE_BLK;
    node->p = &__lsz_rb_tree_null_node__;
    node->l = &__lsz_rb_tree_null_node__;
    node->r = &__lsz_rb_tree_null_node__;
    void *k = k_of_rb_tree(tree, node);
    if (!k) {
        printf("LSZ_RET_E_ARG: k_of_rb_tree is null\n");
        return LSZ_RET_E_ARG;
    }
    void *v = v_of_rb_tree(tree, node);
    if (!v) {
        printf("LSZ_RET_W_ARG: v_of_rb_tree is null\n");
    }

    pthread_mutex_lock(&lsz_rb_tree_this(tree, lock));
    //
    // 如果节点存在，直接返回
    //
    lsz_rb_node_t *temp = lsz_rb_tree_search(tree, k);
    if (!is_rb_tree_null_node(temp)) {
        retn = LSZ_RET_E_DUP;
        goto eofn;
    }

    lsz_rb_node_t *n = lsz_rb_tree_this(tree, root);
    lsz_rb_node_t *p = &  __lsz_rb_tree_null_node__;
    // 寻找其父
    while (!is_rb_tree_null_node(n)) {
        p = n;
        if (lsz_rb_tree_this(tree, fn_kcmp)(k, k_of_rb_tree(tree, n)) < 0) {
            n = n->l;
        } else {
            n = n->r;
        }
    }
    node->p = p;
    // 插入节点
    if (is_rb_tree_null_node(p)) {
        lsz_rb_tree_this(tree, root) = node;
    } else {
        if (lsz_rb_tree_this(tree, fn_kcmp)(k, k_of_rb_tree(tree, p)) < 0) {
            p->l = node;
        } else {
            p->r = node;
        }
    }
    node->c = LSZ_RB_TREE_RED;
    // 红黑平衡
    lsz_rb_tree_insert_fixup(&lsz_rb_tree_this(tree, root), node);

eofn:
    pthread_mutex_unlock(&lsz_rb_tree_this(tree, lock));

    return retn;
}

void rb_tree_delete(rb_tree_t tree, void *k)
{
    if (!is_rb_tree_signature(tree)) {
        printf("LSZ_RET_E_ARG: tree\n");
        return;
    }

    pthread_mutex_lock(&lsz_rb_tree_this(tree, lock));
    //
    // 如果没此节点，直接返回
    //
    lsz_rb_node_t *node = lsz_rb_tree_search(tree, k);
    if (is_rb_tree_null_node(node)) {
        goto eofn;
    }

    lsz_rb_node_t *succ          = NULL;
    lsz_rb_node_t *node_to_fixup = NULL;
    uint8_t     delete_colour = node->c;

    if (is_rb_tree_null_node(node->l) && is_rb_tree_null_node(node->r)) {
        //
        // [1] 叶子节点
        //
        node_to_fixup = &__lsz_rb_tree_null_node__;
        lsz_rb_tree_transplant(&lsz_rb_tree_this(tree, root), node, &__lsz_rb_tree_null_node__);
    } else if (!is_rb_tree_null_node(node->l) && is_rb_tree_null_node(node->r)) {
        //
        // [2] 存在左子（必然红色）：直接移植
        //
        node->l->c = node->c;
        lsz_rb_tree_transplant(&lsz_rb_tree_this(tree, root), node, node->l);
    } else if (is_rb_tree_null_node(node->l) && !is_rb_tree_null_node(node->r)) {
        //
        // [3] 存在右子（必然红色）：直接移植
        //
        node->r->c = node->c;
        lsz_rb_tree_transplant(&lsz_rb_tree_this(tree, root), node, node->r);
    } else {
        //
        // [4] 存在双子：使用后继置换所删（+颜色），那么等效于删除了后继
        //
        // 寻找节点后继（节点右子最小者即为后继）| 存在右子即存在后继节点
        succ = lsz_rb_tree_min(node->r);
        node_to_fixup = succ->r;
        delete_colour = succ->c;
        //
        // 移植后继右子到后继
        //
        if (node == succ->p) {
            succ->r->p = succ;
        } else {
            lsz_rb_tree_transplant(&lsz_rb_tree_this(tree, root), succ, succ->r);
            succ->r = node->r;
            succ->r->p = succ;
        }
        //
        // 移植后继到所删节点
        //
        succ->c = node->c;
        lsz_rb_tree_transplant(&lsz_rb_tree_this(tree, root), node, succ);
        succ->l = node->l;
        succ->l->p = succ;
    }

    if (node_to_fixup && (delete_colour == LSZ_RB_TREE_BLK)) {
        // 红黑平衡
        lsz_rb_tree_delete_fixup(&lsz_rb_tree_this(tree, root), node_to_fixup);
        lsz_rb_tree_reset_nil();
    }
    //
    // 释放资源
    //
    lsz_rb_tree_this(tree, fn_free)(node);

eofn:
    pthread_mutex_unlock(&lsz_rb_tree_this(tree, lock));
}

void *rb_tree_min(rb_tree_t const tree)
{
    if (!is_rb_tree_signature(tree)) {
        printf("LSZ_RET_E_ARG: tree\n");
        return NULL;
    }

    lsz_rb_node_t *node = lsz_rb_tree_min(lsz_rb_tree_this(tree, root));
    if (is_rb_tree_null_node(node)) {
        return NULL;
    } else {
        return v_of_rb_tree(tree, node);
    }
}

void *rb_tree_max(rb_tree_t const tree)
{
    if (!is_rb_tree_signature(tree)) {
        printf("LSZ_RET_E_ARG: tree\n");
        return NULL;
    }

    lsz_rb_node_t *node = lsz_rb_tree_max(lsz_rb_tree_this(tree, root));
    if (is_rb_tree_null_node(node)) {
        return NULL;
    } else {
        return v_of_rb_tree(tree, node);
    }
}

void *rb_tree_search(rb_tree_t const tree, void const *k)
{
    if (!is_rb_tree_signature(tree)) {
        printf("LSZ_RET_E_ARG: tree\n");
        return NULL;
    }
    if (!k) {
        printf("LSZ_RET_E_ARG: !k\n");
        return NULL;
    }

    lsz_rb_node_t *node = lsz_rb_tree_search(tree, k);
    if (is_rb_tree_null_node(node)) {
        return NULL;
    } else {
        return v_of_rb_tree(tree, node);
    }
}

void *rb_tree_prev(rb_tree_t const tree, void **k)
{
    if (!is_rb_tree_signature(tree)) {
        printf("LSZ_RET_E_ARG: tree\n");
        return NULL;
    }
    if (!k) {
        printf("LSZ_RET_E_ARG: !k\n");
        return NULL;
    }

    lsz_rb_node_t *node;
    if (!*k) {
        node = lsz_rb_tree_max (lsz_rb_tree_this(tree, root));
    } else {
        node = lsz_rb_tree_prev(lsz_rb_tree_search(tree, *k));
    }
    if (is_rb_tree_null_node(node)) {
        *k  =  NULL;
        return NULL;
    }

    *k  =  k_of_rb_tree(tree, node);
    return v_of_rb_tree(tree, node);
}

void *rb_tree_next(rb_tree_t const tree, void **k)
{
    if (!is_rb_tree_signature(tree)) {
        printf("LSZ_RET_E_ARG: tree\n");
        return NULL;
    }
    if (!k) {
        printf("LSZ_RET_E_ARG: !k\n");
        return NULL;
    }

    lsz_rb_node_t *node;
    if (!*k) {
        node = lsz_rb_tree_min (lsz_rb_tree_this(tree, root));
    } else {
        node = lsz_rb_tree_next(lsz_rb_tree_search(tree, *k));
    }
    if (is_rb_tree_null_node(node)) {
        *k  =  NULL;
        return NULL;
    }

    *k  =  k_of_rb_tree(tree, node);
    return v_of_rb_tree(tree, node);
}

void rb_tree_for_each(rb_tree_t const tree, lsz_rb_tree_for_each_callback_t cb_func, void *cb_data)
{
    if (!is_rb_tree_signature(tree)) {
        printf("LSZ_RET_E_ARG: tree\n");
        return;
    }

    lsz_rb_tree_for_each(lsz_rb_tree_this(tree, root), cb_func, cb_data);
}

void rb_tree_free(rb_tree_t tree)
{
    if (!is_rb_tree_signature(tree)) {
        printf("LSZ_RET_E_ARG: tree\n");
        return;
    }

    lsz_rb_node_t *node = NULL;
    while (!is_rb_tree_null_node(node = lsz_rb_tree_min(lsz_rb_tree_this(tree, root)))) {
        rb_tree_delete(tree, k_of_rb_tree(tree, node));
    }
    free(tree);
}

/*
--------------------------------------------------------------------------------
end!
--------------------------------------------------------------------------------
*/
