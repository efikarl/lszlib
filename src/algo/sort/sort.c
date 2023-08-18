/** @file
---------------------------------------------------------------------------------
    Copyright ©2023-2025 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    算法 - 快速排序
---------------------------------------------------------------------------------
--*/
#include "lszlib.h"

/*
--------------------------------------------------------------------------------
type
--------------------------------------------------------------------------------
*/

typedef struct {
    void *                              data;
    size_t                              unit;
    int                                 n;
    lsz_compare_callback_t              compare;
} __lsz_quick_sort_t;

/*
--------------------------------------------------------------------------------
impl
--------------------------------------------------------------------------------
*/

void lsz_fn_quick_sort(__lsz_quick_sort_t *this, int l, int r)
{
    int i = l, j = r - 1; // [-1] is pivot

    if (l >= r) {
        return;
    }
    // partition: [i] < [r] <= [j]
    while (1)
    {
        while ((this->compare(void_of(this->data, i, this->unit), void_of(this->data, r, this->unit)) <  0))
            i++;
        while ((this->compare(void_of(this->data, j, this->unit), void_of(this->data, r, this->unit)) >= 0) && (j >= 0))
            j--;
        if (i < j) {
            lsz_swap_val(void_of(this->data, i, this->unit), void_of(this->data, j, this->unit), this->unit);
        } else {
            break;
        }
    }
    // move pivot from [-1] to [i]
    if (i != r) {
        lsz_swap_val(void_of(this->data, i, this->unit), void_of(this->data, r, this->unit), this->unit);
    }

    lsz_fn_quick_sort(this, l, i - 1);
    lsz_fn_quick_sort(this, i + 1, r);
}

/*
--------------------------------------------------------------------------------
apis
--------------------------------------------------------------------------------
*/

int quick_sort(void *data, size_t unit, int n, lsz_compare_callback_t compare)
{
    int                 r    = LSZ_RET_0_ERR;
    __lsz_quick_sort_t *this = NULL;

    if ((!data) || (!unit) && (n <= 0)) {
        r = LSZ_RET_E_ARG;
        printf("LSZ_RET_E_ARG: data = %08p, unit = %zu, n = %d\n", data, unit, n);
        goto eofn;
    }

    this = calloc(1, sizeof(__lsz_quick_sort_t));
    if (!this) {
        r = LSZ_RET_E_OUT;
        printf("LSZ_RET_E_OUT: this = %08p\n", this);
        goto eofn;
    }

    this->data      = data;
    this->unit      = unit;
    this->n         = n;
    this->compare   = compare;

    lsz_fn_quick_sort(this, 0, n - 1);

eofn:
    if (this) {
        free(this);
    }
    return r;
}

/*
--------------------------------------------------------------------------------
end!
--------------------------------------------------------------------------------
*/
