/** @file
--------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
--------------------------------------------------------------------------------
    数据 - 红黑二叉树
--------------------------------------------------------------------------------
    性质1：节点非红即黑
    性质2：根节点是黑色
    性质3：叶节点是黑色
    性质4：红色节点的子节点是黑色节点
    性质5：指定节点的子路径的黑高相同
--------------------------------------------------------------------------------
--*/
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "base.h"
#include "tree.h"
//
// 叶子节点
//
lsz_tree_t __lsz_tree_nil__ = {
    .s = LSZ_TREE_NODE_SIGNATURE,
    .c = LSZ_TREE_BLK,
};

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
// 签名验证
//
#define is_priv_signature(this) \
    ((this) && (lsz_tree_this(this, null) == &__lsz_tree_nil__))

/*
--------------------------------------------------------------------------------
*/

//
// 旋转操作：只改变树的指针，不改变树的次序属性
//
/*@fn: lsz_tree_rotate_l
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
void lsz_tree_rotate_l(lsz_tree_t **root, lsz_tree_t *l)
{
    lsz_tree_t *r = l->r;
    assert(l);
    assert(r);
    // r的父亲
    r->p = l->p;
    if (is_tree_null_node(l->p)) {
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
    if (!is_tree_null_node(r->l)) {
        r->l->p = l;
    }
    // r的左子
    r->l = l;
    l->p = r;
}

/*@fn: lsz_tree_rotate_r
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
void lsz_tree_rotate_r(lsz_tree_t **root, lsz_tree_t *r)
{
    lsz_tree_t *l = r->l;
    assert(r);
    assert(l);
    // l的父亲
    l->p = r->p;
    if (is_tree_null_node(r->p)) {
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
    if (!is_tree_null_node(l->r)) {
        l->r->p = r;
    }
    // l的右子
    l->r = r;
    r->p = l;
}

/*@fn: lsz_tree_transplant
--------------------------------------------------------------------------------
    目的：子树替换（以v替换u）
--------------------------------------------------------------------------------
*/
void lsz_tree_transplant(lsz_tree_t **root, lsz_tree_t *u, lsz_tree_t *v)
{
    assert(u);
    assert(v);
    assert(u != &__lsz_tree_nil__);

    if (is_tree_null_node(u->p)) {
        *root = v;
    } else if (u == u->p->l) {
        u->p->l = v;
    } else if (u == u->p->r) {
        u->p->r = v;
    }
    v->p = u->p;
}

/*@fn: lsz_tree_insert_fixup
--------------------------------------------------------------------------------
    目的：不删减节点，保持红黑树性质
--------------------------------------------------------------------------------
*/
void lsz_tree_insert_fixup(lsz_tree_t **root, lsz_tree_t *i)
{
    //
    // 插入操作可能破坏：性质2，性质4
    //
    lsz_tree_t *uofi = NULL;
    //
    // 红父条件下，i祖父必然存在
    //
    while (i->p->c == LSZ_TREE_RED) {
        //
        // 参考：算法导论
        //
        if (i->p == i->p->p->l) {
            uofi = i->p->p->r;
            if (uofi->c == LSZ_TREE_RED) {
                i->p->c    = LSZ_TREE_BLK;
                uofi->c    = LSZ_TREE_BLK;
                i->p->p->c = LSZ_TREE_RED;
                i = i->p->p;
            } else {
                if (i == i->p->r) {
                    i = i->p;
                    lsz_tree_rotate_l(root, i);
                }
                i->p->c    = LSZ_TREE_BLK;
                i->p->p->c = LSZ_TREE_RED;
                lsz_tree_rotate_r(root, i->p->p);
            }
        } else {
            uofi = i->p->p->l;
            if (uofi->c == LSZ_TREE_RED) {
                i->p->c    = LSZ_TREE_BLK;
                uofi->c    = LSZ_TREE_BLK;
                i->p->p->c = LSZ_TREE_RED;
                i = i->p->p;
            } else {
                if (i == i->p->l) {
                    i = i->p;
                    lsz_tree_rotate_r(root, i);
                }
                i->p->c    = LSZ_TREE_BLK;
                i->p->p->c = LSZ_TREE_RED;
                lsz_tree_rotate_l(root, i->p->p);
            }
        }
    }
    (*root)->c = LSZ_TREE_BLK;
}

/*@fn: lsz_tree_delete_fixup
--------------------------------------------------------------------------------
    目的：不删减节点，保持红黑树性质
--------------------------------------------------------------------------------
*/
void lsz_tree_delete_fixup(lsz_tree_t **root, lsz_tree_t *i)
{
    //
    // 删除操作可能破坏：性质1，性质2，性质4
    //
    lsz_tree_t *bofi = NULL;
    //
    // 双黑非根时，i兄弟必然存在
    //
    while ((!is_tree_null_node(i->p)) && (i->c == LSZ_TREE_BLK)) {
        //
        // 参考：算法导论
        //
        if (i == i->p->l) {
            bofi = i->p->r;
            if (bofi->c == LSZ_TREE_RED) {
                bofi->c = LSZ_TREE_BLK;
                i->p->c = LSZ_TREE_RED;
                lsz_tree_rotate_l(root, i->p); bofi = i->p->r;
            }
            if (((bofi->l->c == LSZ_TREE_BLK)) &&
                ((bofi->r->c == LSZ_TREE_BLK))) {
                bofi->c = LSZ_TREE_RED;
                i = i->p;
            } else {
                if (((bofi->l->c == LSZ_TREE_RED)) &&
                    ((bofi->r->c == LSZ_TREE_BLK))) {
                    bofi->l->c = LSZ_TREE_BLK;
                    bofi->c    = LSZ_TREE_RED;
                    lsz_tree_rotate_r(root, bofi); bofi = i->p->r;
                }
                if (bofi->r->c == LSZ_TREE_RED) {
                    bofi->c    = i->p->c;
                    i->p->c    = LSZ_TREE_BLK;
                    bofi->r->c = LSZ_TREE_BLK;
                    lsz_tree_rotate_l(root, i->p);
                    // 双黑解除，终止循环
                    i = *root;
                }
            }
        } else {
            bofi = i->p->l;
            if (bofi->c == LSZ_TREE_RED) {
                bofi->c = LSZ_TREE_BLK;
                i->p->c = LSZ_TREE_RED;
                lsz_tree_rotate_r(root, i->p); bofi = i->p->l;
            }
            if (((bofi->r->c == LSZ_TREE_BLK)) &&
                ((bofi->l->c == LSZ_TREE_BLK))) {
                bofi->c = LSZ_TREE_RED;
                i = i->p;
            } else {
                if (((bofi->r->c == LSZ_TREE_RED)) &&
                    ((bofi->l->c == LSZ_TREE_BLK))) {
                    bofi->r->c = LSZ_TREE_BLK;
                    bofi->c    = LSZ_TREE_RED;
                    lsz_tree_rotate_l(root, bofi); bofi = i->p->l;
                }
                if (bofi->l->c == LSZ_TREE_RED) {
                    bofi->c    = i->p->c;
                    i->p->c    = LSZ_TREE_BLK;
                    bofi->l->c = LSZ_TREE_BLK;
                    lsz_tree_rotate_r(root, i->p);
                    // 双黑解除，终止循环
                    i = *root;
                }
            }
        }
    }
    //
    // 情况1到情况2的补偿
    //
    if (i->c == LSZ_TREE_RED) {
        i->c =  LSZ_TREE_BLK;
    }
}

/*
--------------------------------------------------------------------------------
*/

lsz_tree_t *lsz_tree_new(const void *tree, const void *k, const void *v)
{
    if (!is_priv_signature(tree) || !k) {
        assert(!!0);
        return NULL;
    }

    lsz_tree_t *node = calloc(1, sizeof(lsz_tree_t));
    if (!node) {
        assert(!!0);
        return NULL;
    }
    if (k) {
        if (!lsz_tree_this(tree, sz_k)) {
            node->k = strdup(k);
        } else {
            node->k = malloc(lsz_tree_this(tree, sz_k)); if (node->k) memcpy(node->k, k, lsz_tree_this(tree, sz_k));
        }
    }
    if (v) {
        if (!lsz_tree_this(tree, sz_v)) {
            node->v = strdup(v);
        } else {
            node->v = malloc(lsz_tree_this(tree, sz_v)); if (node->v) memcpy(node->v, v, lsz_tree_this(tree, sz_v));
        }
    }
    node->s = LSZ_TREE_NODE_SIGNATURE;
    node->c = LSZ_TREE_BLK;
    node->p = &__lsz_tree_nil__;
    node->l = &__lsz_tree_nil__;
    node->r = &__lsz_tree_nil__;

    return node;
}

lsz_tree_t *lsz_tree_min(lsz_tree_t *node)
{
    if (!is_tree_signature(node)) {
        assert(!!0);
        return NULL;
    }

    lsz_tree_t *find = node;
    while (!is_tree_null_node(node)) {
        find = node;
        node = node->l;
    }
    return find;
}

lsz_tree_t *lsz_tree_max(lsz_tree_t *node)
{
    if (!is_tree_signature(node)) {
        assert(!!0);
        return NULL;
    }

    lsz_tree_t *find = node;
    while (!is_tree_null_node(node)) {
        find = node;
        node = node->r;
    }
    return find;
}

lsz_tree_t *lsz_tree_search(const void *tree, const void *k)
{
    if (!is_priv_signature(tree) || !k) {
        assert(!!0);
        return NULL;
    }

    int i = 0;
    lsz_tree_t *node = lsz_tree_this(tree, root);
    while ((node != lsz_tree_this(tree, null)) && (i = lsz_tree_this(tree, cb_k)(k, node->k))) {
        if (i < 0) {
            node = node->l;
        } else {
            node = node->r;
        }
    }
    return node;
}

lsz_tree_t *lsz_tree_prev(lsz_tree_t *node)
{
    if (!is_tree_signature(node)) {
        assert(!!0);
        return NULL;
    }

    if (is_tree_null_node(node)) {
        return node;
    }
    if (!is_tree_null_node(node->l)) {
        node = lsz_tree_max(node->l);
    } else {
        while (!is_tree_null_node(node->p) && (node == node->p->l)) {
            node = node->p;
        }
        node = node->p;
    }

    return node;
}

lsz_tree_t *lsz_tree_next(lsz_tree_t *node)
{
    if (!is_tree_signature(node)) {
        assert(!!0);
        return NULL;
    }

    if (is_tree_null_node(node)) {
        return node;
    }
    if (!is_tree_null_node(node->r)) {
        node = lsz_tree_min(node->r);
    } else {
        while (!is_tree_null_node(node->p) && (node == node->p->r)) {
            node = node->p;
        }
        node = node->p;
    }

    return node;
}

void lsz_tree_for_each(lsz_tree_t *node, lsz_tree_for_each_callback_t cb_func, void *cb_data)
{
    if (!is_tree_signature(node)) {
        assert(!!0);
    }

    if (!is_tree_null_node(node)) {
        lsz_tree_for_each(node->l, cb_func, cb_data);
        if (cb_func) cb_func((const void *) node->k, node->v, cb_data);
        lsz_tree_for_each(node->r, cb_func, cb_data);
    }
}

void lsz_tree_reset_nil(void *tree)
{
    if (!is_priv_signature(tree)) {
        assert(!!0);
    }
    lsz_tree_this(tree, null)->s = LSZ_TREE_NODE_SIGNATURE;
    lsz_tree_this(tree, null)->c = LSZ_TREE_BLK;
}

/*
--------------------------------------------------------------------------------
*/

void *tree_new(size_t sz_k, size_t sz_v, lsz_compare_t cb_k)
{
    lsz_tree_private_t *this = calloc(1, sizeof(lsz_tree_private_t));
    if (!cb_k) {
        printf("error: k_compare_callback is null\n");
        return NULL;
    }
    if (!this) {
        printf("error: out of resources of calloc\n");
        return NULL;
    }
    this->root  = &__lsz_tree_nil__;
    this->null  = &__lsz_tree_nil__;
    this->sz_k  = sz_k;
    this->sz_v  = sz_v;
    this->cb_k  = cb_k;
    pthread_mutex_init(&this->lock, NULL);

    return (void *) this;
}

int is_tree_empty(const void *tree)
{
    if (!tree) {
        printf("error: invalid parameter - tree (null)\n");
        return 0;
    }

    if (lsz_tree_this(tree, root) == lsz_tree_this(tree, null)) {
        return !!1;
    } else {
        return !!0;
    }
}

int tree_insert(void *tree, const void *k, const void *v)
{
    if (!tree) {
        return LSZ_RET_E_ARG;
    }
    if (!k) {
        return LSZ_RET_E_ARG;
    }

    lsz_tree_t *node = NULL;
    //
    // 如果节点存在，直接返回
    //
    node = lsz_tree_search(tree, k);
    if (node != lsz_tree_this(tree, null)) {
        return LSZ_RET_E_DUP;
    }

    pthread_mutex_lock(&lsz_tree_this(tree, lock));

    node = lsz_tree_new(tree, k, v);
    if (!node) {
        return LSZ_RET_E_OUT;
    }
    lsz_tree_t *n = lsz_tree_this(tree, root);
    lsz_tree_t *p = lsz_tree_this(tree, null);
    // 寻找其父
    while (n != lsz_tree_this(tree, null)) {
        p = n;
        if (lsz_tree_this(tree, cb_k)(node->k, n->k) < 0) {
            n = n->l;
        } else {
            n = n->r;
        }
    }
    node->p = p;
    // 插入节点
    if (p == lsz_tree_this(tree, null)) {
        lsz_tree_this(tree, root) = node;
    } else {
        if (lsz_tree_this(tree, cb_k)(node->k, p->k) < 0) {
            p->l = node;
        } else {
            p->r = node;
        }
    }
    node->c = LSZ_TREE_RED;
    // 红黑平衡
    lsz_tree_insert_fixup(&lsz_tree_this(tree, root), node);

    pthread_mutex_unlock(&lsz_tree_this(tree, lock));

    return LSZ_RET_OK;
}

void tree_delete(void *tree, void *k)
{
    if (!tree) {
        printf("error: invalid parameter - tree (null)\n");
        return;
    }

    lsz_tree_t *node = NULL;
    lsz_tree_t *succ = NULL;
    //
    // 如果没此节点，直接返回
    //
    node = lsz_tree_search(tree, k);
    if (node == lsz_tree_this(tree, null)) {
        return;
    }

    pthread_mutex_lock(&lsz_tree_this(tree, lock));

    lsz_tree_t *node_to_fixup = NULL;
    uint8_t     delete_colour = node->c;

    if ((node->l == lsz_tree_this(tree, null)) && (node->r == lsz_tree_this(tree, null))) {
        //
        // [1] 叶子节点
        //
        node_to_fixup = lsz_tree_this(tree, null);
        lsz_tree_transplant(&lsz_tree_this(tree, root), node, lsz_tree_this(tree, null));
    } else if ((node->l != lsz_tree_this(tree, null)) && (node->r == lsz_tree_this(tree, null))) {
        //
        // [2] 存在左子（必然红色）：直接移植
        //
        node->l->c = node->c;
        lsz_tree_transplant(&lsz_tree_this(tree, root), node, node->l);
    } else if ((node->l == lsz_tree_this(tree, null)) && (node->r != lsz_tree_this(tree, null))) {
        //
        // [3] 存在右子（必然红色）：直接移植
        //
        node->r->c = node->c;
        lsz_tree_transplant(&lsz_tree_this(tree, root), node, node->r);
    } else {
        //
        // [4] 存在双子：使用后继置换所删（+颜色），那么等效于删除了后继
        //
        // 寻找节点后继（节点右子最小者即为后继）| 存在右子即存在后继节点
        succ = lsz_tree_min(node->r);
        node_to_fixup = succ->r;
        delete_colour = succ->c;
        //
        // 移植后继右子到后继
        //
        if (node == succ->p) {
            succ->r->p = succ;
        } else {
            lsz_tree_transplant(&lsz_tree_this(tree, root), succ, succ->r);
            succ->r = node->r;
            succ->r->p = succ;
        }
        //
        // 移植后继到所删节点
        //
        succ->c = node->c;
        lsz_tree_transplant(&lsz_tree_this(tree, root), node, succ);
        succ->l = node->l;
        succ->l->p = succ;
    }

    if (node_to_fixup && (delete_colour == LSZ_TREE_BLK)) {
        // 红黑平衡
        lsz_tree_delete_fixup(&lsz_tree_this(tree, root), node_to_fixup);
        lsz_tree_reset_nil(tree);
    }
    //
    // 释放资源
    //
    if (node->k) {
        free(node->k);
    }
    if (node->v) {
        free(node->v);
    }
    if (node) {
        free(node);
    }

    pthread_mutex_unlock(&lsz_tree_this(tree, lock));
}

void *tree_min(const void *tree)
{
    if (!tree) {
        printf("error: invalid parameter - tree (null)\n");
        return NULL;
    }

    lsz_tree_t *node = lsz_tree_min(lsz_tree_this(tree, root));
    if (node == lsz_tree_this(tree, null)) {
        return NULL;
    } else {
        return node->v;
    }
}

void *tree_max(const void *tree)
{
    if (!tree) {
        printf("error: invalid parameter - tree (null)\n");
        return NULL;
    }

    lsz_tree_t *node = lsz_tree_max(lsz_tree_this(tree, root));
    if (node == lsz_tree_this(tree, null)) {
        return NULL;
    } else {
        return node->v;
    }
}

void *tree_search(const void *tree, const void *k)
{
    if (!tree) {
        printf("error: invalid parameter - tree (null)\n");
        return NULL;
    }
    if (!k) {
        printf("error: invalid parameter - k    (null)\n");
        return NULL;
    }

    lsz_tree_t *node = lsz_tree_search(tree, k);
    if (node == lsz_tree_this(tree, null)) {
        return NULL;
    } else {
        return node->v;
    }
}

void *tree_prev(const void *tree, void **k)
{
    if (!tree) {
        printf("error: invalid parameter - tree (null)\n");
        return NULL;
    }
    if (!k) {
        printf("error: invalid parameter - k    (null)\n");
        return NULL;
    }

    lsz_tree_t *node;
    if (!*k) {
        node = lsz_tree_max (lsz_tree_this(tree, root));
    } else {
        node = lsz_tree_prev(lsz_tree_search(tree, *k));
    }
    *k = node->k;

    return node->v;
}

void *tree_next(const void *tree, void **k)
{
    if (!tree) {
        printf("error: invalid parameter - tree (null)\n");
        return NULL;
    }
    if (!k) {
        printf("error: invalid parameter - k    (null)\n");
        return NULL;
    }

    lsz_tree_t *node;
    if (!*k) {
        node = lsz_tree_min (lsz_tree_this(tree, root));
    } else {
        node = lsz_tree_next(lsz_tree_search(tree, *k));
    }
    *k = node->k;

    return node->v;
}

void tree_for_each(const void *tree, lsz_tree_for_each_callback_t cb_func, void *cb_data)
{
    if (!tree) {
        printf("error: invalid parameter - tree (null)\n");
        return;
    }

    lsz_tree_for_each(lsz_tree_this(tree, root), cb_func, cb_data);
}

void tree_free(void *tree)
{
    if (!tree) {
        return;
    }
    lsz_tree_t *node = NULL;
    while ((node = lsz_tree_min(lsz_tree_this(tree, root))) != lsz_tree_this(tree, null)) {
        tree_delete(tree, node->k);
    }
    free(tree);
}

/*
--------------------------------------------------------------------------------
*/
