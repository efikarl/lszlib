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
#ifndef LSZ_PERM_H_
#define LSZ_PERM_H_
//
// callback to handle each result of permutation
//
typedef void (*lsz_perm_callback_t)(void *m_sub_set, int m);
//
// P(n, m) (now just support m == n)
//
int lsz_perm(void *vec, size_t unit, int n, int m, lsz_perm_callback_t fn);

#endif
