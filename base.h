/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    基础定义
---------------------------------------------------------------------------------
--*/
#ifndef LSZ_BASE
#define LSZ_BASE
#include <stddef.h>
#include <stdint.h>
#include <assert.h>

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

#define of_str(a) #a
#define str_of(a) of_str(a)

typedef int (* lsz_compare_t) (const void *a, const void *b);

void lsz_swap(void *a, void *b, size_t unit);
int  lsz_icmp(const void *a, const void *b);
int  lsz_scmp(const void *a, const void *b);

#endif
