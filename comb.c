/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    算法 - 组合(combination)
---------------------------------------------------------------------------------
--*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "comb.h"

struct {
    void            *set;
    void            *sub;
    size_t          unit;
    int             n;
    int             m;
    int             i;
    int             r;
    comb_callback_t fn;
} __lsz_comb__;
//
// C(N,M) = A(N,M) / A(M,M)
//
void _lsz_comb(int next)
{
    //  当前子集元素 + 集合剩余元素 < 子集元素总数 => 不可构造
    if (__lsz_comb__.i + (__lsz_comb__.n - next) < __lsz_comb__.m) {
        return;
    }
    if (__lsz_comb__.i >= __lsz_comb__.m) {
        __lsz_comb__.r++;
        // 排列完成时，执行：callback()
        if (__lsz_comb__.fn) {
            __lsz_comb__.fn(__lsz_comb__.sub, __lsz_comb__.i);
        }
    } else {
        // sub[i] = set[next]
        memcpy((char *)__lsz_comb__.sub + __lsz_comb__.unit*__lsz_comb__.i, (char *)__lsz_comb__.set + __lsz_comb__.unit*next, __lsz_comb__.unit);
        __lsz_comb__.i++; //  当前位置
        _lsz_comb(next + 1);
        __lsz_comb__.i--; // !当前位置
        _lsz_comb(next + 1);
    }
}
//
// API
//
int lsz_comb(void *set, size_t unit, int n, int m, comb_callback_t fn)
{
    __lsz_comb__.set    = calloc(n, unit);
    __lsz_comb__.sub    = calloc(m, unit);
    __lsz_comb__.unit   = unit;
    __lsz_comb__.n      = n;
    __lsz_comb__.m      = m;
    __lsz_comb__.fn     = fn;
    __lsz_comb__.i      = 0;
    __lsz_comb__.r      = 0;

    memcpy(__lsz_comb__.set, set, n*unit);

    _lsz_comb(0);

    free(__lsz_comb__.set);
    free(__lsz_comb__.sub);

    return __lsz_comb__.r;
}
