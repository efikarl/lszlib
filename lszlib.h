/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    lszlib
---------------------------------------------------------------------------------
--*/

#ifndef LSZ_LIB_H_
#define LSZ_LIB_H_

#ifndef LSZ_BASE
#define LSZ_BASE
#include <stddef.h>
#include <stdint.h>

#define LSZ_RET_OK                      0    // 成功
#define LSZ_RET_E_ARG                   (-1) // 参数
#define LSZ_RET_E_OUT                   (-2) // 耗尽
#define LSZ_RET_E_DUP                   (-3) // 重复
#define LSZ_RET_E_NFD                   (-4) // 未见
#define LSZ_RET_E_MIN                   (-5) // 下限
#define LSZ_RET_E_MAX                   (-6) // 上限
#define LSZ_RET_E_SEC                   (-7) // 风险

#define void_of(ptr, index, size)       \
            ((char *) (ptr) + (index) * (size))

#define base_of(ptr, type, field)       \
            ((type *) ((char *) (ptr) - offsetof(type, field)))

typedef int (* lsz_compare_t) (const void *a, const void *b);

void lsz_swap(void *a, void *b, size_t unit);
int  lsz_icmp(const void *a, const void *b);
int  lsz_scmp(const void *a, const void *b);

#endif


#ifndef LSZ_BENCHMARK
#define LSZ_BENCHMARK
#include <time.h>
#include <stdio.h>

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


#ifndef LSZ_LIST
#define LSZ_LIST
#include <stdint.h>
#include <stddef.h>

#define LSZ_MAX_LINK    UINT16_MAX
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


#ifndef LSZ_TREE
#define LSZ_TREE
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


#ifndef LSZ_SORT
#define LSZ_SORT
//
// callback to compare two unit of vector
//
typedef int (* lsz_compare_t) (const void *a, const void *b);
//
// quick sort of vec with n units and each size is unit
//
void lsz_qsort(void *vec, size_t unit, int n, lsz_compare_t cmpfn);

#endif


#ifndef LSZ_COMB
#define LSZ_COMB
//
// callback to handle each result of combination
//
typedef void (*comb_callback_t)(int *comb, int m);
//
// combination of vec from element n to element m
//
int lsz_comb(void *vec, size_t unit, int n, int m, comb_callback_t fn);

#endif


#ifndef LSZ_PERM
#define LSZ_PERM
//
// callback to handle each result of permutation
//
typedef void (*perm_callback_t)(int *perm, int m, int n);
//
// permutation of vec from element m to element n
//
int lsz_perm(void *vec, size_t unit, int m, int n, perm_callback_t fn);

#endif


#ifndef LSZ_PATH
#define LSZ_PATH
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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

#endif


#endif // LSZ_LIB_H_
