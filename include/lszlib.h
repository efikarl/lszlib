/** @file
---------------------------------------------------------------------------------
    Copyright ©2023-2024 Liu Yi, efikarl@yeah.net

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
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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

#define void_of(ptr, index, size)       \
            ((char *) (ptr) + (index) * (size))

#define base_of(ptr, type, field)       \
            ((type *) ((char *) (ptr) - offsetof(type, field)))

#define of_str(a) #a
#define str_of(a) of_str(a)

typedef int (* lsz_compare_t) (const void *a, const void *b);

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
lsz_cmp_int (
    const void          *a,
    const void          *b
);

int
lsz_cmp_str (
    const void          *a,
    const void          *b
);

size_t
size_n_align (
    size_t              bitn,
    size_t              size
);

size_t
size_p_align (
    size_t              bitn,
    size_t              size
);

#endif


#ifndef LSZ_BENCHMARK_H_
#define LSZ_BENCHMARK_H_
#include <time.h>

#define LSZ_BENCHMARK_MAX                                   1024

#define lsz_benchmark_do                                        \
{                                                               \
    clock_t     lo;                                             \
    clock_t     hi;                                             \
    double      re[LSZ_BENCHMARK_MAX];                          \
    double      max;                                            \
    double      min;                                            \
    double      avr;                                            \
    freopen("/dev/null", "w", stdout);                          \
    for (int i = 0; i < LSZ_BENCHMARK_MAX; i++) {               \
        lo = clock();

// to do benchmark between lsz_benchmark_do & lsz_benchmark_done

#define lsz_benchmark_done                                      \
        hi = clock();                                           \
        re[i] = ((double) (hi - lo));                           \
    }                                                           \
    freopen("/dev/tty",  "w", stdout);                          \
    max = re[0];                                                \
    min = re[0];                                                \
    avr = 0;                                                    \
    for (int i = 0; i < LSZ_BENCHMARK_MAX; i++) {               \
        avr += re[i];                                           \
        if (min > re[i]) {                                      \
            min = re[i];                                        \
        }                                                       \
        if (max < re[i]) {                                      \
            max  = re[i];                                       \
        }                                                       \
    }                                                           \
    avr /= LSZ_BENCHMARK_MAX;                                   \
    printf("-----------------------------------------------\n");\
    printf("benchmark:\n");                                     \
    printf("--------------- --------------- ---------------\n");\
    printf("min (us)        max (us)        avr (us)       \n");\
    printf("--------------- --------------- ---------------\n");\
    printf("%15.4f %15.4f %15.4f\n", min, max, avr);            \
    printf("-----------------------------------------------\n");\
}

#endif


#ifndef LSZ_TESTCASE_H_
#define LSZ_TESTCASE_H_

typedef void *testcase_t;

typedef enum {
    testcase_record_fail,
    testcase_record_pass,
} testcase_rslt_t;

typedef void (* testcase_func_t) (void);

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
    const char         *file,
    const char         *func,
    const char         *info
);

#define lsz_testcase_do                 \
do {                                    \
    testcase_t __tc__ = NULL;           \
    __tc__ = testcase_init();           \
    freopen("/dev/null", "w", stdout);

#define TESTCASE_ASSERT_DATA(expect, target, size, info) \
    testcase_assert(testcase_priv(), expect, target, size, __FILE__, __FUNCTION__, info)

#define TESTCASE_ASSERT_BOOL(boolex, info)               \
    testcase_assert(testcase_priv(), NULL, NULL, (boolex), __FILE__, __FUNCTION__, info)

#define lsz_testcase_done               \
    freopen("/dev/tty", "w", stdout);   \
    testcase_report(testcase_priv());   \
    testcase_free(__tc__);              \
} while(0);

#endif


#ifndef LSZ_LIST_H_
#define LSZ_LIST_H_

#define LSZ_LINK_SIG    ('l' << 24 | 'i' << 16 | 's' << 8 | 't')
#define LSZ_LINK_VLD    0

typedef struct _lsz_list_t lsz_list_t;

struct _lsz_list_t {
    lsz_list_t          *prev;
    lsz_list_t          *next;
};

#define LIST_INIT(list) { &(list) , &(list) }

//
//! callback(!lsz_compare_t): 0: 成功，!0: 失败
//
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

int list_owns (
    lsz_list_t          *dst_list,
    lsz_list_t          *src_list
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

int list_for_each (
    lsz_list_t          *list,
    lsz_list_callback_t callback,
    void                *data
);

#endif


#ifndef LSZ_TREE_H_
#define LSZ_TREE_H_

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

typedef void * (* lsz_tree_k_pointer_t) (
    const lsz_tree_t    *node
);

typedef void * (* lsz_tree_v_pointer_t) (
    const lsz_tree_t    *node
);

typedef int  (* lsz_tree_k_compare_t) (
    const void          *a,
    const void          *b
);

typedef void (* lsz_tree_node_free_t) (
    const lsz_tree_t    *node
);

int is_tree_signature(const lsz_tree_t *node);
int is_tree_null_node(const lsz_tree_t *node);

tree_t tree_new (
    lsz_tree_k_pointer_t                fn_kptr,
    lsz_tree_v_pointer_t                fn_vptr,
    lsz_tree_k_compare_t                fn_kcmp,
    lsz_tree_node_free_t                fn_free
);

void tree_free (
    tree_t                              tree
);

int is_tree_empty (
    const tree_t        tree
);

int tree_insert (
    tree_t              tree,
    lsz_tree_t         *node
);

void tree_delete (
    void                *tree,
    void                *k
);

void * tree_min (
    const tree_t        tree
);

void * tree_max (
    const tree_t        tree
);

void * tree_search (
    const tree_t        tree,
    const void          *k
);

void * tree_prev (
    const tree_t        tree,
    void                **k
);

void * tree_next (
    const tree_t        tree,
    void                **k
);

typedef void (* lsz_tree_for_each_callback_t) (
    const lsz_tree_t                    *node,
    void                                *data
);

void tree_for_each (
    const tree_t                        tree,
    lsz_tree_for_each_callback_t        callback_func,
    void                                *callback_data
);

#endif


#ifndef LSZ_SORT_H_
#define LSZ_SORT_H_
//
// callback to compare two unit of vector
//
typedef int (* lsz_compare_t) (const void *a, const void *b);
//
// quick sort of vec with n units and each size is unit
//
void lsz_qsort(void *vec, size_t unit, int n, lsz_compare_t cmpfn);

#endif


#ifndef LSZ_COMB_H_
#define LSZ_COMB_H_
//
// callback to handle each result of combination
//
typedef void (*lsz_comb_callback_t)(void *m_sub_set, int m);
//
// C(n, m)
//
int lsz_comb(void *vec, size_t unit, int n, int m, lsz_comb_callback_t fn);

#endif


#ifndef LSZ_PERM_H_
#define LSZ_PERM_H_
//
// callback to handle each result of permutation
//
typedef void (*lsz_perm_callback_t)(void *m_sub_set, int m);
//
// P(n, m) (now just support m == n)
//
int lsz_perm(void *vec, size_t unit, int n, int m, lsz_perm_callback_t fn);

#endif


#ifndef LSZ_PATH_H_
#define LSZ_PATH_H_

#define PATH_NODE_UNIT  64
#define PATH_NAME_UNIT  64

typedef char path_node_t[PATH_NODE_UNIT];

char *path_normalize (
    const char          *src,
    size_t              len
);

bool is_path_valid (
    const char          *path
);

int path_from(const path_node_t *src_node, size_t  src_len, char **dst_path);
int path_into(const char *src_path, path_node_t **dst_node, size_t *dst_len);

int path_index (
    const char          *path,
    path_node_t          name,
    size_t              *index
);

#endif


#endif // LSZ_LIB_H_
