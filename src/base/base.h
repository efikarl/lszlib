/** @file
---------------------------------------------------------------------------------
    Copyright ©2023-2024 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    基础定义
---------------------------------------------------------------------------------
--*/
#ifndef LSZ_BASE_H_
#define LSZ_BASE_H_
#include <stddef.h>
#include <stdint.h>
#include <assert.h>

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
