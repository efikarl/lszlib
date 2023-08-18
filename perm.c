/** @file
---------------------------------------------------------------------------------
    Copyright ©2023-2024 Liu Yi, efikarl@yeah.net

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

typedef struct {
    size_t              unit;
    int                 n;
    int                 m;
    int                 i;
    int                 r;
    lsz_perm_callback_t fn;
    void                *n_set;
    void                *m_set;
} __lsz_perm_t;

//
// P(N) = N! => P(N) = N * P(N-1)
//
void __lsz_perm_t_fn_permutation(__lsz_perm_t *this, int next)
{
    if (next >= this->n) {
        this->r++;
        // 排列完成时，执行：callback()
        if (this->fn) {
            this->fn(this->n_set, this->m);
        }
        return;
    } else {
        for (int i = next; i < this->n; i++) {
            // vec[next] <=交换=> vec[i]
            if (next != i) {
                lsz_swap_val(void_of(this->n_set, next, this->unit), void_of(this->n_set, i, this->unit), this->unit);
            }
            __lsz_perm_t_fn_permutation(this, next + 1);
            // vec[next] <=复位=> vec[i]
            if (next != i) {
                lsz_swap_val(void_of(this->n_set, next, this->unit), void_of(this->n_set, i, this->unit), this->unit);
            }
        }
    }
}
//
// API
//
int lsz_perm(void *n_set, size_t unit, int n, int m, lsz_perm_callback_t fn)
{
    __lsz_perm_t *this = NULL;

    if (m != n) {
        printf("LSZ_RET_E_N_A: this\n");
        goto eofn;
    }
    if (!((n_set) && (n > 0))) {
        printf("LSZ_RET_E_ARG: !((n_set) && (n > 0))\n");
        return 0;
    }

    this = calloc(1, sizeof(__lsz_perm_t));
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
    this->m    = n;
    this->fn   = fn;
    this->i    = 0;
    this->r    = 0;
    memcpy(this->n_set, n_set, n * unit);
    __lsz_perm_t_fn_permutation(this, 0);

eofn:
    if (this) {
        if (this->n_set) free(this->n_set);
        if (this->m_set) free(this->m_set);
        free(this);
    }

    return this->r;
}
