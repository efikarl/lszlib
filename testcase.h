/** @file
---------------------------------------------------------------------------------
    Copyright ©2023-2024 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    简单接口测试
---------------------------------------------------------------------------------
--*/
#ifndef LSZ_TESTCASE_H_
#define LSZ_TESTCASE_H_
#include <stdio.h>

typedef void *testcase_t;

typedef enum {
    testcase_record_fail,
    testcase_record_pass,
} testcase_rslt_t;

typedef void (* testcase_func_t) (void);

testcase_t
testcase_init (
    void
);

testcase_t
testcase_priv (
    void
);

void
testcase_free (
    testcase_t          priv
);

int
testcase_report (
    testcase_t          priv
);

int
testcase_assert (
    testcase_t          priv,
    void               *expect,
    void               *target,
    size_t              size,
    const char         *file,
    const char         *func,
    const char         *info
);

#define lsz_testcase_do                 \
do {                                    \
    testcase_t __tc__ = NULL;           \
    __tc__ = testcase_init();           \
    freopen("/dev/null", "w", stdout);

#define TESTCASE_ASSERT_DATA(expect, target, size, info) \
    testcase_assert(testcase_priv(), expect, target, size, __FILE__, __FUNCTION__, info)

#define TESTCASE_ASSERT_BOOL(boolex, info)               \
    testcase_assert(testcase_priv(), NULL, NULL, (boolex), __FILE__, __FUNCTION__, info)

#define lsz_testcase_done               \
    freopen("/dev/tty", "w", stdout);   \
    testcase_report(testcase_priv());   \
    testcase_free(__tc__);              \
} while(0);

#endif
