/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    算法 - 快速排序
---------------------------------------------------------------------------------
--*/
#ifndef LSZ_SORT
#define LSZ_SORT
//
// callback to compare two unit of vector
//
typedef int (* lsz_compare_t) (const void *a, const void *b);
//
// quick sort of vec with n units and each size is unit
//
void lsz_qsort(void *vec, size_t unit, int n, lsz_compare_t cmpfn);

#endif
