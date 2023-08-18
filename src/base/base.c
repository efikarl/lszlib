/** @file
---------------------------------------------------------------------------------
    Copyright ©2023-2025 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    基础功能
---------------------------------------------------------------------------------
--*/
#include "lszlib.h"

/*
--------------------------------------------------------------------------------
impl
--------------------------------------------------------------------------------
*/

int lsz_swap_val(void *a, void *b, size_t unit)
{
    int                 r = LSZ_RET_0_ERR;
    void               *t = NULL;

    if (!a || !b) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    if (!unit) {
        goto eofn;
    }

    t = calloc(1, unit);
    if (!t) {
        r = LSZ_RET_E_OUT;
        goto eofn;
    }

    memmove(t, a, unit);
    memmove(a, b, unit);
    memmove(b, t, unit);

    if (t) {
        free(t);
    }

eofn:
    return r;
}

int lsz_swap_ptr(void **a, void **b)
{
    int                 r = LSZ_RET_0_ERR;
    void               *t = NULL;

    if (!a || !b) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    t = *a; *a = *b; *b = t;

eofn:
    return r;
}

int lsz_compare_int(void const *a, void const *b)
{
    int *x = (int *)a;
    int *y = (int *)b;
    assert(x);
    assert(y);

    return (*x > *y) - (*x < *y);
}

int lsz_compare_str(void const *a, void const *b)
{
    char *x = (char *)a;
    char *y = (char *)b;
    assert(x);
    assert(y);

    return strcmp(x, y);
}

size_t lsz_n_align_bit(size_t data, size_t bits)
{
    return (~((((size_t)1) << bits) - 1)) & data;
}

size_t lsz_p_align_bit(size_t data, size_t bits)
{
    return (~((((size_t)1) << bits) - 1)) & (data + ((((size_t)1) << bits) - 1));
}

/*
--------------------------------------------------------------------------------
end!
--------------------------------------------------------------------------------
*/
