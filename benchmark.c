/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    简单性能测试
---------------------------------------------------------------------------------
--*/
#include "benchmark.h"

typedef void (* lsz_benchmark_function_t) (void);

double lsz_benchmark(lsz_benchmark_function_t fn)
{
    lsz_benchmark_do;
        fn();
    lsz_benchmark_done;
}
