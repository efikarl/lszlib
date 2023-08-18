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
#ifndef LSZ_PERM
#define LSZ_PERM
//
// callback to handle each result of permutation
//
typedef void (*perm_callback_t)(int *perm, int m, int n);
//
// permutation of vec from element m to element n
//
int lsz_perm(void *vec, size_t unit, int m, int n, perm_callback_t fn);

#endif
