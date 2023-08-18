/** @file
---------------------------------------------------------------------------------
    Copyright ©2023-2025 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    lszlib.h
---------------------------------------------------------------------------------
--*/

#ifndef LSZ_LIB_H_
#define LSZ_LIB_H_

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef LSZ_BASE_H_
#define LSZ_BASE_H_

#define LSZ_RET_0_ERR                   0   // 功能正常
#define LSZ_RET_E_ARG                   1   // 非法参数
#define LSZ_RET_E_OUT                   2   // 资源耗尽
#define LSZ_RET_E_NFD                   3   // 目标未见
#define LSZ_RET_E_DUP                   4   // 目标重复
#define LSZ_RET_E_FMT                   5   // 格式问题
#define LSZ_RET_E_N_A                   6   // 尚未支持
#define LSZ_RET_E_SEC                   7   // 安全问题

#define stdout_support(__boolean) \
do {                                        \
    if (__boolean) {                        \
        freopen("/dev/tty" , "w", stdout);  \
    } else {                                \
        freopen("/dev/null", "w", stdout);  \
    }                                       \
} while(0)

#define void_of(ptr, index, size) \
    ((char *) (ptr) + (index) * (size))

#define base_of(ptr, type, field) \
    ((type *) ((char *) (ptr) - offsetof(type, field)))

#define of_str(a)       #a
#define str_of(a) of_str(a)

typedef
int
(* lsz_compare_callback_t) (
    void const          *a,
    void const          *b
    );

int
lsz_swap_val (
    void                *a,
    void                *b,
    size_t              unit
);

int
lsz_swap_ptr (
    void                **a,
    void                **b
);

int
lsz_compare_int (
    void const          *a,
    void const          *b
);

int
lsz_compare_str (
    void const          *a,
    void const          *b
);

size_t
lsz_n_align_bit (
    size_t              data,
    size_t              bits
);

size_t
lsz_p_align_bit (
    size_t              data,
    size_t              bits
);

#endif


#ifndef LSZ_TESTCASE_H_
#define LSZ_TESTCASE_H_

typedef void *testcase_t;

testcase_t
testcase_init (
    void
);

testcase_t
testcase_priv (
    void
);

void
testcase_free (
    testcase_t          priv
);

int
testcase_report (
    testcase_t          priv
);

int
testcase_assert (
    testcase_t          priv,
    void               *expect,
    void               *target,
    size_t              size,
    char const         *file,
    int  const          line,
    char const         *func,
    char const         *format,
    ...
);

#define lsz_testcase_do                                         \
do {                                                            \
    testcase_t __tc__ = NULL;                                   \
    __tc__ = testcase_init();                                   \
    stdout_support(0);

#define TESTCASE_ASSERT_DATA(expect, target, size, format, ...) \
    testcase_assert(testcase_priv(), expect, target, size, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)

#define TESTCASE_ASSERT_BOOL(boolex, format, ...)               \
    testcase_assert(testcase_priv(), NULL, NULL, (boolex), __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)

#define lsz_testcase_done                                       \
    stdout_support(1);                                          \
    testcase_report(testcase_priv());                           \
    testcase_free(__tc__);                                      \
} while(0)

#endif


#ifndef LSZ_TESTPERF_H_
#define LSZ_TESTPERF_H_
#include <time.h>

#define LSZ_TESTPERF_MAX                                        1024

#define lsz_testperf_do                                         \
{                                                               \
    clock_t     __lo;                                           \
    clock_t     __hi;                                           \
    double      __re[LSZ_TESTPERF_MAX];                         \
    double      __max;                                          \
    double      __min;                                          \
    double      __avr;                                          \
    stdout_support(0);                                          \
    for (int i = 0; i < LSZ_TESTPERF_MAX; i++) {                \
        __lo = clock();

// to do benchmark between lsz_testperf_do & lsz_testperf_done

#define lsz_testperf_done                                       \
        __hi = clock();                                         \
        __re[i] = ((double) (__hi - __lo));                     \
    }                                                           \
    stdout_support(1);                                          \
    __max = __re[0];                                            \
    __min = __re[0];                                            \
    __avr = 0;                                                  \
    for (int i = 0; i < LSZ_TESTPERF_MAX; i++) {                \
        __avr += __re[i];                                       \
        if (__min > __re[i]) {                                  \
            __min = __re[i];                                    \
        }                                                       \
        if (__max < __re[i]) {                                  \
            __max  = __re[i];                                   \
        }                                                       \
    }                                                           \
    __avr /= LSZ_TESTPERF_MAX;                                  \
    printf("-----------------------------------------------\n");\
    printf("benchmark:\n");                                     \
    printf("--------------- --------------- ---------------\n");\
    printf("min (us)        max (us)        avr (us)       \n");\
    printf("--------------- --------------- ---------------\n");\
    printf("%15.4f %15.4f %15.4f\n", __min, __max, __avr);      \
    printf("-----------------------------------------------\n");\
}

#endif


#ifndef LSZ_SORT_H_
#define LSZ_SORT_H_
//
// quick sort of vec with n units and each size is unit
//
int quick_sort(void *data, size_t unit, int n, lsz_compare_callback_t fn);

#endif


#ifndef LSZ_P_C__H_
#define LSZ_P_C__H_
//
// callback to handle each result of combination
//
typedef void (*lsz_combination_callback_t)(void *m_sub_set, int m);
//
// C(n, m) = n! / (n - m)! / m! = P(n, m) / m!
//
int combination(void *vector, size_t unit, int n, int m, lsz_combination_callback_t callback, void *context);
//
// callback to handle each result of permutation
//
typedef void (*lsz_permutation_callback_t)(void *m_sub_set, int m);
//
// P(n, m) = n! / (n - m)!      = C(n, m) * m!
//
int permutation(void *vector, size_t unit, int n, int m, lsz_permutation_callback_t callback, void *context);

#endif


#ifndef LSZ_LIST_H_
#define LSZ_LIST_H_

typedef struct _lsz_list_t lsz_list_t;

#define LSZ_LINK_SIGNATURE              ('l' << 24 | 'i' << 16 | 's' << 8 | 't')
struct _lsz_list_t {
    lsz_list_t          *prev;
    lsz_list_t          *next;
};

typedef int (* lsz_list_callback_t) (
    lsz_list_t          *list,
    lsz_list_t          *link,
    void                *data
);

int list_init (
    lsz_list_t          *list
);

void
list_free (
    lsz_list_t          *list,
    lsz_list_callback_t fn
);

int is_list_valid (
    lsz_list_t          *list
);

int is_list_empty (
    lsz_list_t          *list
);

int list_insert_tail (
    lsz_list_t          *list,
    lsz_list_t          *link
);

int list_insert_head (
    lsz_list_t          *list,
    lsz_list_t          *link
);

int list_delete_link (
    lsz_list_t          *link
);

int list_replace_link (
    lsz_list_t          *be_replace,
    lsz_list_t          *to_replace
);

int list_for_each (
    lsz_list_t          *list,
    lsz_list_callback_t callback,
    void                *data
);

#endif


#ifndef LSZ_DICT_H_
#define LSZ_DICT_H_

typedef void *                                   dict_t;

typedef struct _lsz_dict_node_t         lsz_dict_node_t;

#define LSZ_DICT_NODE_COLOR_RED         0
#define LSZ_DICT_NODE_COLOR_BLK         1

#define LSZ_DICT_NODE_SIGNATURE         ('r' << 8 | 'b')
#pragma pack(1)
struct _lsz_dict_node_t {
    uint16_t         _;
    uint16_t         c;
    lsz_dict_node_t *p;
    lsz_dict_node_t *l;
    lsz_dict_node_t *r;
};
#pragma pack( )
//
// 签名验证
//
int is_dict_node_signature(lsz_dict_node_t const *node);
int is_dict_tree_null_node(lsz_dict_node_t const *node);
//
// 回调函数：键值获取函数
//
typedef void * (* lsz_dict_k_pointer_t) (
    lsz_dict_node_t const               *node
);
typedef void * (* lsz_dict_v_pointer_t) (
    lsz_dict_node_t const               *node
);
//
// 回调函数：键的比较函数
//
typedef int  (* lsz_dict_k_compare_t) (
    void            const               *a,
    void            const               *b
);
//
// 回调函数：节点资源释放
//
typedef void (* lsz_dict_tree_free_t) (
    lsz_dict_node_t const               *node
);

/*@fn: 新建字典
--------------------------------------------------------------------------------
[I]:
    sz_k                                : 键的单位大小
                                             0: 代表字串
                                            !0: 代表字串以外的其它值类型
    sz_v                                : 值的单位大小
                                             0: 代表字串
                                            !0: 代表字串以外的其它值类型
    fn_kptr                             : 键的指针函数
    fn_vptr                             : 值的指针函数
    fn_kcmp                             : 键的比较函数

[O]:
    dict_t                              : 字典（指针需要使用者释放）
--------------------------------------------------------------------------------
*/
dict_t
dict_new (
    lsz_dict_k_pointer_t                fn_kptr,
    lsz_dict_v_pointer_t                fn_vptr,
    lsz_dict_k_compare_t                fn_kcmp,
    lsz_dict_tree_free_t                fn_free
);

/*@fn: 注销字典
--------------------------------------------------------------------------------
[I]:
    dict                                : 目标字典

[O]:
    void                                : 空
--------------------------------------------------------------------------------
*/
void dict_free (
    dict_t                              dict
);

/*@fn: 字典是否为空?
--------------------------------------------------------------------------------
[I]:
    dict                                : 目标字典

[O]:
    bool                                : 0->否 1->是
--------------------------------------------------------------------------------
*/
int is_dict_empty (
    dict_t          const               dict
);

/*@fn: 插入附属键值对
--------------------------------------------------------------------------------
[I]:
    dict                                : 目标字典
    node                                : 目标节点

[O]:
    int                                 : 错误代码
                                            LSZ_RET_0_ERR: 成功
                                            LSZ_RET_E_ARG: 函数参数错误
                                            LSZ_RET_E_DUP: 目标已经存在
                                            LSZ_RET_E_OUT: 主存资源耗尽
--------------------------------------------------------------------------------
*/
int dict_insert (
    dict_t                              dict,
    lsz_dict_node_t                    *node
);

/*@fn: 删除附属键值对
--------------------------------------------------------------------------------
[I]:
    dict                                : 目标字典
    k                                   : 键

[O]:
    void                                : 空
--------------------------------------------------------------------------------
*/
void dict_delete (
    dict_t                              dict,
    void                                *k
);

/*@fn: 获取最小键的值
--------------------------------------------------------------------------------
[I]:
    dict                                : 目标字典

[O]:
    void *                              : 最小键的值的指针
                                            NULL: 失败
                                           !NULL: 成功
--------------------------------------------------------------------------------
*/
void * dict_min (
    dict_t          const               dict
);

/*@fn: 获取最大键的值
--------------------------------------------------------------------------------
[I]:
    dict                                : 目标字典

[O]:
    void *                              : 最大键的值的指针
                                            NULL: 失败
                                           !NULL: 成功
--------------------------------------------------------------------------------
*/
void * dict_max (
    dict_t          const               dict
);

/*@fn: 获取目标键的值
--------------------------------------------------------------------------------
[I]:
    dict                                : 目标字典
    k                                   : 键

[O]:
    void *                              : 目标键的值的指针
                                            NULL: 失败
                                           !NULL: 成功
--------------------------------------------------------------------------------
*/
void * dict_search (
    dict_t          const               dict,
    void            const               *k
);

/*@fn: 获取上一个节点的键值对
--------------------------------------------------------------------------------
[I]:
    dict                                : 目标字典
    k                                   : [I] 当前键 [O] 目标键

[O]:
    void *                              : 目标键的值的指针
                                            NULL: 终点
                                           !NULL: 成功
--------------------------------------------------------------------------------
*/
void * dict_prev (
    dict_t          const               dict,
    void                                **k
);

/*@fn: 获取下一个节点的键值对
--------------------------------------------------------------------------------
[I]:
    dict                                : 目标字典
    k                                   : [I] 当前键 [O] 目标键

[O]:
    void *                              : 目标键的值的指针
                                            NULL: 终点
                                           !NULL: 成功
--------------------------------------------------------------------------------
*/
void * dict_next (
    dict_t          const               dict,
    void                                **k
);

/*@fn: 字典的遍历
--------------------------------------------------------------------------------
[I]:
    dict                                : 目标字典
    callback_func                       : 回调函数
    callback_data                       : 回调数据

[O]:
    void                                : 空
--------------------------------------------------------------------------------
*/

//
//! 遍历回调函数
//
typedef void (* lsz_dict_for_each_callback_t) (
    lsz_dict_node_t const               *node,
    void                                *data
);

void dict_for_each (
    dict_t          const               dict,
    lsz_dict_for_each_callback_t        callback_func,
    void                               *callback_data
);

#endif


#ifndef LSZ_PATH_H_
#define LSZ_PATH_H_

#define PATH_NAME_UNIT  128

typedef char path_node_t[PATH_NAME_UNIT];

int is_path_valid(char const *path);
int path_from(path_node_t const *src_node, size_t  src_len, char **dst_path);
int path_into(char const *src_path, path_node_t **dst_node, size_t *dst_len);

#endif


#ifndef LSZ_JSON_H_
#define LSZ_JSON_H_

typedef void * json_t;

json_t
json_new (
    void
    );

int
json_free (
    json_t              json
    );

int
jboo_get (
    json_t              json,
    char               *path,
    long               *data
    );

int
jnum_get (
    json_t              json,
    char               *path,
    long               *data
    );

int
jnul_get (
    json_t              json,
    char               *path
    );

int
jstr_get (
    json_t              json,
    char               *path,
    char              **data
    );

int
jobj_get (
    json_t              json,
    char               *path,
    json_t             *data
    );

int
jboo_set (
    json_t              json,
    char               *path,
    long                data
    );

int
jnum_set (
    json_t              json,
    char               *path,
    long                data
    );

int
jnul_set (
    json_t              json,
    char               *path
    );

int
jstr_set (
    json_t              json,
    char               *path,
    char               *data
    );

int
jobj_set (
    json_t              json,
    char               *path,
    json_t              data
    );

int
jboo_add (
    json_t              json,
    char               *path,
    long                data
    );

int
jnum_add (
    json_t              json,
    char               *path,
    long                data
    );

int
jnul_add (
    json_t              json,
    char               *path
    );

int
jstr_add (
    json_t              json,
    char               *path,
    char               *data
    );

int
jobj_add (
    json_t              json,
    char               *path,
    json_t              data
    );

int
json_del (
    json_t              json,
    char               *path
    );

char *
json_decode (
    json_t              json
    );

json_t
json_encode (
    char               *text
    );

#endif

#endif // LSZ_LIB_H_
