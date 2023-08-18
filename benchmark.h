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
#ifndef LSZ_BENCHMARK
#define LSZ_BENCHMARK
#include <time.h>
#include <stdio.h>

#define LSZ_BENCHMARK_MAX                                   1024

#define lsz_benchmark_do                                        \
{                                                               \
    clock_t     lo;                                             \
    clock_t     hi;                                             \
    double      re[LSZ_BENCHMARK_MAX];                          \
    double      max;                                            \
    double      min;                                            \
    double      avr;                                            \
    freopen("/dev/null", "w", stdout);                          \
    for (int i = 0; i < LSZ_BENCHMARK_MAX; i++) {               \
        lo = clock();

// to do benchmark between lsz_benchmark_do & lsz_benchmark_done

#define lsz_benchmark_done                                      \
        hi = clock();                                           \
        re[i] = ((double) (hi - lo));                           \
    }                                                           \
    freopen("/dev/tty",  "w", stdout);                          \
    max = re[0];                                                \
    min = re[0];                                                \
    avr = 0;                                                    \
    for (int i = 0; i < LSZ_BENCHMARK_MAX; i++) {               \
        avr += re[i];                                           \
        if (min > re[i]) {                                      \
            min = re[i];                                        \
        }                                                       \
        if (max < re[i]) {                                      \
            max  = re[i];                                       \
        }                                                       \
    }                                                           \
    avr /= LSZ_BENCHMARK_MAX;                                   \
    printf("-----------------------------------------------\n");\
    printf("benchmark:\n");                                     \
    printf("--------------- --------------- ---------------\n");\
    printf("min (us)        max (us)        avr (us)       \n");\
    printf("--------------- --------------- ---------------\n");\
    printf("%15.4f %15.4f %15.4f\n", min, max, avr);            \
    printf("-----------------------------------------------\n");\
}

#endif
