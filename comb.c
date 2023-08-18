/** @file
---------------------------------------------------------------------------------
    Copyright ©2023-2024 Liu Yi, efikarl@yeah.net

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

typedef struct {
    size_t              unit;
    int                 n;
    int                 m;
    int                 i;
    int                 r;
    lsz_comb_callback_t fn;
    void                *n_set;
    void                *m_set;
} __lsz_comb_t;

//
// C(N,M) = P(N,M) / P(M,M)
//
void __lsz_comb_t_fn_combination(__lsz_comb_t *this, int next)
{
    //  当前子集元素 + 集合剩余元素 < 子集元素总数 => 不可构造
    if (this->i + (this->n - next) < this->m) {
        return;
    }
    if (this->i >= this->m) {
        this->r++;
        // 排列完成时，执行：callback()
        if (this->fn) {
            this->fn(this->m_set, this->i);
        }
    } else {
        // m_set[i] = n_set[next]
        memcpy((char *)this->m_set + this->unit * this->i, (char *)this->n_set + this->unit * next, this->unit);
        this->i++; //  当前位置
        __lsz_comb_t_fn_combination(this, next + 1);
        this->i--; // !当前位置
        __lsz_comb_t_fn_combination(this, next + 1);
    }
}
//
// API
//
int lsz_comb(void *n_set, size_t unit, int n, int m, lsz_comb_callback_t fn)
{
    __lsz_comb_t *this = NULL;

    if (!m) {
        return 1;
    }
    if (!((n_set) && (n > 0) && (m >= 0) && (n >= m))) {
        printf("LSZ_RET_E_ARG: !((n_set) && (n > 0) && (m > 0) && (n >= m))\n");
        return 0;
    }

    this = calloc(1, sizeof(__lsz_comb_t));
    if (!this) {
        printf("LSZ_RET_E_OUT: this\n");
        goto eofn;
    }
    this->n_set = calloc(n, unit);
    if (!this->n_set) {
        printf("LSZ_RET_E_OUT: this->n_set\n");
        goto eofn;
    }
    this->m_set = calloc(m, unit);
    if (!this->m_set) {
        printf("LSZ_RET_E_OUT: this->m_set\n");
        goto eofn;
    }
    this->unit = unit;
    this->n    = n;
    this->m    = m;
    this->fn   = fn;
    this->i    = 0;
    this->r    = 0;
    memcpy(this->n_set, n_set, n * unit);
    __lsz_comb_t_fn_combination(this, 0);

eofn:
    if (this) {
        if (this->n_set) free(this->n_set);
        if (this->m_set) free(this->m_set);
        free(this);
    }

    return this->r;
}
