/** @file
---------------------------------------------------------------------------------
    Copyright ©2023-2025 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    算法 - 排列(permutation)
---------------------------------------------------------------------------------
--*/
#include "lszlib.h"

/*
--------------------------------------------------------------------------------
type
--------------------------------------------------------------------------------
*/

typedef struct {
    size_t                              unit;
    int                                 n;
    int                                 m;
    int                                 i;
    int                                 r;
    lsz_permutation_callback_t          fn;
    void                               *co;
    void                               *n_set;
    void                               *m_set;
} __lsz_permutation_t;

/*
--------------------------------------------------------------------------------
impl: P(m, m) = m * P(m-1, m-1)
--------------------------------------------------------------------------------
*/

static void lsz_fn_permutation(
    __lsz_permutation_t                *this,
    int                                 next
    )
{
    if (next >= this->m) {
        this->r++;
        // 排列完成，执行回调
        if (this->fn) {
            this->fn(this->m_set, this->m);
        }
        return;
    } else {
        for (int i = next; i < this->m; i++) {
            if (next != i) {
                lsz_swap_val(void_of(this->m_set, next, this->unit), void_of(this->m_set, i, this->unit), this->unit);
            }
            lsz_fn_permutation(this, next + 1);
            if (next != i) {
                lsz_swap_val(void_of(this->m_set, next, this->unit), void_of(this->m_set, i, this->unit), this->unit);
            }
        }
    }
}

static void lsz_fn_combination(
    __lsz_permutation_t                *this,
    int                                 next
    )
{
    //  当前子集元素 + 集合剩余元素 < 子集元素总数 => 不可构造
    if (this->i + (this->n - next) < this->m) {
        return;
    }
    if (this->i >= this->m) {
        // 组合完成，执行排列
        if (this->fn) {
            lsz_fn_permutation(this, 0);
        }
    } else {
        // m_set[i] = n_set[next]
        memcpy((char *)this->m_set + this->unit * this->i, (char *)this->n_set + this->unit * next, this->unit);
        // 假设：I = i
        this->i++; // 情形1：C(n_set[next+1, n], m_set[I+1, m-(I+1]))
        lsz_fn_combination(this, next + 1);
        this->i--; // 情形2：C(n_set[next+1, n], m_set[I+0, m-(I+0)])
        lsz_fn_combination(this, next + 1);
    }
}

/*
--------------------------------------------------------------------------------
apis
--------------------------------------------------------------------------------
*/

int permutation(void *n_set, size_t unit, int n, int m, lsz_permutation_callback_t fn, void *co)
{
    __lsz_permutation_t *this = NULL;

    if (!m) {
        return 1;
    }
    if (!((n_set) && (n > 0) && (m >= 0) && (n >= m))) {
        printf("LSZ_RET_E_ARG: !((n_set) && (n > 0) && (m > 0) && (n >= m))\n");
        return 0;
    }

    this = calloc(1, sizeof(__lsz_permutation_t));
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
    this->co   = co;
    this->i    = 0;
    this->r    = 0;
    memcpy(this->n_set, n_set, n * unit);
    lsz_fn_combination(this, 0);

eofn:
    if (this) {
        if (this->n_set) free(this->n_set);
        if (this->m_set) free(this->m_set);
        free(this);
    }

    return this->r;
}

/*
--------------------------------------------------------------------------------
end!
--------------------------------------------------------------------------------
*/
