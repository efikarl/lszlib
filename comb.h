/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    算法 - 组合(combination)
---------------------------------------------------------------------------------
--*/
#ifndef LSZ_COMB
#define LSZ_COMB
//
// callback to handle each result of combination
//
typedef void (*comb_callback_t)(int *comb, int m);
//
// combination of vec from element n to element m
//
int lsz_comb(void *vec, size_t unit, int n, int m, comb_callback_t fn);

#endif
