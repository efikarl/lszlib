/** @file
---------------------------------------------------------------------------------
    Copyright ©2023-2024 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    算法 - 快速排序
---------------------------------------------------------------------------------
--*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "base.h"
#include "sort.h"

typedef int (* lsz_compare_t) (const void *a, const void *b);

struct {
    void *          data;
    size_t          unit;
    int             n;
    lsz_compare_t   fn;
} __lsz_qsort__;

void _lsz_qsort(int *data, int l, int r)
{
    int i = l, j = r -1;

    if (l >= r) {
        return;
    }
    // let pivot = data[r], p = i
    // partition: find where pivot should be(p)
    while (1)
    {
        while ((__lsz_qsort__.fn(void_of(data, i, __lsz_qsort__.unit), void_of(data, r, __lsz_qsort__.unit)) <  0))
            i++;
        while ((__lsz_qsort__.fn(void_of(data, j, __lsz_qsort__.unit), void_of(data, r, __lsz_qsort__.unit)) >= 0) && (j >= 0))
            j--;
        if (i < j) {
            lsz_swap_val(void_of(data, i, __lsz_qsort__.unit), void_of(data, j, __lsz_qsort__.unit), __lsz_qsort__.unit);
        } else {
            break;
        }
    }
    if (i != r) {
        lsz_swap_val(void_of(data, i, __lsz_qsort__.unit), void_of(data, r, __lsz_qsort__.unit), __lsz_qsort__.unit);
    }

    _lsz_qsort(data, l, i - 1);
    _lsz_qsort(data, i + 1, r);
}

void lsz_qsort(void *data, size_t unit, int n, lsz_compare_t fn)
{
    __lsz_qsort__.data  = data;
    __lsz_qsort__.unit  = unit;
    __lsz_qsort__.n     = n;
    __lsz_qsort__.fn    = fn;

    _lsz_qsort(data, 0, n - 1);
}
