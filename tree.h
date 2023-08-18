/** @file
---------------------------------------------------------------------------------
    Copyright ©2023-2024 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    值 - 红黑二叉树
---------------------------------------------------------------------------------
    性质1：节点非红即黑
    性质2：根节点是黑色
    性质3：叶节点是黑色
    性质4：红色节点的子节点是黑色节点
    性质5：指定节点的子路径的黑高相同
---------------------------------------------------------------------------------
--*/
#ifndef LSZ_TREE_H_
#define LSZ_TREE_H_
#include <stdint.h>
#include <stddef.h>

typedef void *  tree_t;

typedef struct _lsz_tree_t              lsz_tree_t;

#define LSZ_TREE_RED                    0
#define LSZ_TREE_BLK                    1
#define LSZ_TREE_NODE_SIGNATURE         ('t' << 8 | 'r')

#pragma pack(1)
struct _lsz_tree_t {
    uint16_t             _;
    uint16_t             c;
    lsz_tree_t          *p;
    lsz_tree_t          *l;
    lsz_tree_t          *r;
};
#pragma pack( )
//
// 签名验证
//
int is_tree_signature(const lsz_tree_t *node);
int is_tree_null_node(const lsz_tree_t *node);
//
// 回调函数：键值获取函数
//
typedef void * (* lsz_tree_k_pointer_t) (
    const lsz_tree_t    *node
);
typedef void * (* lsz_tree_v_pointer_t) (
    const lsz_tree_t    *node
);
//
// 回调函数：键的比较函数
//
typedef int  (* lsz_tree_k_compare_t) (
    const void          *a,
    const void          *b
);
//
// 回调函数：节点资源释放
//
typedef void (* lsz_tree_node_free_t) (
    const lsz_tree_t    *node
);

/*@fn: 植树
--------------------------------------------------------------------------------
[I]:
    sz_k                : 键的单位大小
                             0: 代表字串
                            !0: 代表字串以外的其它值类型
    sz_v                : 值的单位大小
                             0: 代表字串
                            !0: 代表字串以外的其它值类型
    fn_kptr             : 键的指针函数
    fn_vptr             : 值的指针函数
    fn_kcmp             : 键的比较函数

[O]:
    tree_t              : 树（指针需要使用者释放）
--------------------------------------------------------------------------------
*/
tree_t tree_new (
    lsz_tree_k_pointer_t     fn_kptr,
    lsz_tree_v_pointer_t     fn_vptr,
    lsz_tree_k_compare_t     fn_kcmp,
    lsz_tree_node_free_t     fn_free
);

/*@fn: 伐树
--------------------------------------------------------------------------------
[I]:
    tree                : 目标红黑树

[O]:
    void                : 空
--------------------------------------------------------------------------------
*/
void tree_free (
    tree_t                              tree
);

/*@fn: 是否空树?
--------------------------------------------------------------------------------
[I]:
    tree                : 目标红黑树

[O]:
    bool                : 0->否 1->是
--------------------------------------------------------------------------------
*/
int is_tree_empty (
    const tree_t        tree
);

/*@fn: 插入附属键值对
--------------------------------------------------------------------------------
[I]:
    tree                : 目标红黑树
    node                : 目标节点

[O]:
    int                 : 错误代码
                            LSZ_RET_0_ERR: 成功
                            LSZ_RET_E_ARG: 函数参数错误
                            LSZ_RET_E_DUP: 目标已经存在
                            LSZ_RET_E_OUT: 主存资源耗尽
--------------------------------------------------------------------------------
*/
int tree_insert (
    tree_t              tree,
    lsz_tree_t         *node
);

/*@fn: 删除附属键值对
--------------------------------------------------------------------------------
[I]:
    tree                : 目标红黑树
    k                   : 键

[O]:
    void                : 空
--------------------------------------------------------------------------------
*/
void tree_delete (
    void                *tree,
    void                *k
);

/*@fn: 获取最小键的值
--------------------------------------------------------------------------------
[I]:
    tree                : 目标红黑树

[O]:
    void *              : 最小键的值的指针
                            NULL: 失败
                           !NULL: 成功
--------------------------------------------------------------------------------
*/
void * tree_min (
    const tree_t        tree
);

/*@fn: 获取最大键的值
--------------------------------------------------------------------------------
[I]:
    tree                : 目标红黑树

[O]:
    void *              : 最大键的值的指针
                            NULL: 失败
                           !NULL: 成功
--------------------------------------------------------------------------------
*/
void * tree_max (
    const tree_t        tree
);

/*@fn: 获取目标键的值
--------------------------------------------------------------------------------
[I]:
    tree                : 目标红黑树
    k                   : 键

[O]:
    void *              : 目标键的值的指针
                            NULL: 失败
                           !NULL: 成功
--------------------------------------------------------------------------------
*/
void * tree_search (
    const tree_t        tree,
    const void          *k
);

/*@fn: 获取上一个节点的键值对
--------------------------------------------------------------------------------
[I]:
    tree                : 目标红黑树
    k                   : [I] 当前键 [O] 目标键

[O]:
    void *              : 目标键的值的指针
                            NULL: 终点
                           !NULL: 成功
--------------------------------------------------------------------------------
*/
void * tree_prev (
    const tree_t        tree,
    void                **k
);

/*@fn: 获取下一个节点的键值对
--------------------------------------------------------------------------------
[I]:
    tree                : 目标红黑树
    k                   : [I] 当前键 [O] 目标键

[O]:
    void *              : 目标键的值的指针
                            NULL: 终点
                           !NULL: 成功
--------------------------------------------------------------------------------
*/
void * tree_next (
    const tree_t        tree,
    void                **k
);

//
//! 遍历回调函数
//
typedef void (* lsz_tree_for_each_callback_t) (
    const lsz_tree_t                    *node,
    void                                *data
);

/*@fn: 树的遍历
--------------------------------------------------------------------------------
[I]:
    tree                : 目标红黑树
    callback_func       : 回调函数
    callback_data       : 回调数据

[O]:
    void                : 空
--------------------------------------------------------------------------------
*/
void tree_for_each (
    const tree_t                        tree,
    lsz_tree_for_each_callback_t        callback_func,
    void                                *callback_data
);

#endif
