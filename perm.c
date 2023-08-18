/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    算法 - 排列(permutation)
---------------------------------------------------------------------------------
--*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "base.h"
#include "perm.h"

struct {
    void            *vec;
    size_t          unit;
    int             m;
    int             n;
    int             r;
    perm_callback_t fn;
} __lsz_perm__;

//
// P(N) = N! => P(N) = N * P(N-1)
//
void _lsz_perm(int next)
{
    if (next >= __lsz_perm__.n) {
        __lsz_perm__.r++;
        // 排列完成时，执行：callback()
        if (__lsz_perm__.fn) {
            __lsz_perm__.fn(__lsz_perm__.vec, __lsz_perm__.m, __lsz_perm__.n);
        }
        return;
    } else {
        for (int i = next; i < __lsz_perm__.n; i++) {
            // vec[next] <=交换=> vec[i]
            if (next != i) {
                lsz_swap(void_of(__lsz_perm__.vec, next, __lsz_perm__.unit), void_of(__lsz_perm__.vec, i, __lsz_perm__.unit), __lsz_perm__.unit);
            }
            _lsz_perm(next + 1);
            // vec[next] <=复位=> vec[i]
            if (next != i) {
                lsz_swap(void_of(__lsz_perm__.vec, next, __lsz_perm__.unit), void_of(__lsz_perm__.vec, i, __lsz_perm__.unit), __lsz_perm__.unit);
            }
        }
    }
}
//
// API
//
int lsz_perm(void *vec, size_t unit, int m, int n, perm_callback_t fn)
{
    __lsz_perm__.vec    = calloc(n, unit);
    __lsz_perm__.unit   = unit;
    __lsz_perm__.m      = m;
    __lsz_perm__.n      = n;
    __lsz_perm__.fn     = fn;
    __lsz_perm__.r      = 0;

    memcpy(__lsz_perm__.vec, vec, n*unit);

    _lsz_perm(m);

    free(__lsz_perm__.vec);

    return __lsz_perm__.r;
}
