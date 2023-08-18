/** @file
---------------------------------------------------------------------------------
    Copyright ©2023-2025 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    简单接口测试
---------------------------------------------------------------------------------
--*/
#include "lszlib.h"

/*
--------------------------------------------------------------------------------
type
--------------------------------------------------------------------------------
*/

typedef enum {
    testcase_record_fail,
    testcase_record_pass,
} testcase_rslt_t;

typedef struct {
    char const                         *file;
    int                                 line;
    char const                         *func;
    char                               *info;
    testcase_rslt_t                     rslt;
    lsz_list_t                          link;
} testcase_case_t;

typedef struct {
    int                                 cnts;
    int                                 pass;
    int                                 fail;
    lsz_list_t                          list;
} testcase_test_t;

static testcase_test_t *__lsz_test__;

/*
--------------------------------------------------------------------------------
impl
--------------------------------------------------------------------------------
*/

testcase_t
testcase_priv (
    void
)
{
    return __lsz_test__;
}

int
testcase_for_each_free (
    lsz_list_t         *list,
    lsz_list_t         *link,
    void               *data
)
{
    testcase_test_t    *tc_test = NULL;
    testcase_case_t    *tc_case = NULL;

    if (!list || !link) {
        return LSZ_RET_E_ARG;
    }
    tc_test = base_of(list, testcase_test_t, list);
    tc_case = base_of(link, testcase_case_t, link);

    if (tc_case->info) {
        free (tc_case->info);
        tc_case->info = NULL;
    }
    switch (tc_case->rslt) {
    case testcase_record_fail:
        tc_test->fail--;
        break;
    case testcase_record_pass:
        tc_test->pass--;
        break;
    default:
        break;
    }
    tc_test->cnts--;

    list_delete_link(link);
    if (tc_case) {
        free (tc_case);
        tc_case = NULL;
    }

    return LSZ_RET_0_ERR;
}


testcase_t
testcase_init (
    void
)
{
    testcase_test_t    *tc_test  = NULL;

    tc_test = calloc(1, sizeof(testcase_test_t));
    if (!tc_test) {
        return tc_test;
    }
    list_init(&tc_test->list);

    return __lsz_test__ = tc_test;
}

void
testcase_free (
    testcase_t          priv
)
{
    testcase_test_t    *tc_test  = priv;

    list_free(&tc_test->list, testcase_for_each_free);
    __lsz_test__ = NULL;
}

int
testcase_record (
    testcase_t          priv,
    testcase_rslt_t     rslt,
    char const         *file,
    int  const          line,
    char const         *func,
    char               *info
)
{
    int                 r       = 0;
    testcase_test_t    *tc_test = priv;
    testcase_case_t    *tc_case = NULL;
    lsz_list_t         *this    = NULL;
    lsz_list_t         *next    = NULL;
    int                 found   = 0;

    if (!priv || !file || !line || !func) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    for (this = tc_test->list.next, next = this->next; this != &tc_test->list; this = next, next = this->next) {
        tc_case = base_of(this, testcase_case_t, link);
        if (!strcmp(tc_case->file, file) && (tc_case->line == line) && !strcmp(tc_case->func, func)) {
            found = 1;
            break;
        }
    }
    if (found) {
        r = LSZ_RET_E_DUP;
        goto eofn;
    }

    tc_case = calloc(1, sizeof(testcase_case_t));
    if (!tc_case) {
        r = LSZ_RET_E_OUT;
        goto eofn;
    }
    tc_case->rslt = rslt;
    tc_case->file = file;
    tc_case->line = line;
    tc_case->func = func;
    tc_case->info = strdup(info);
    list_insert_tail(&tc_test->list, &tc_case->link);

    tc_test->cnts++;
    switch (tc_case->rslt) {
    case testcase_record_fail:
        tc_test->fail++;
        break;
    case testcase_record_pass:
        tc_test->pass++;
        break;
    default:
        break;
    }

eofn:
    return r;
}

void
testcase_data_print (void *data, size_t size)
{
    uint8_t line = 0;

    for (size_t i = 0; i < 16; i++) {
        if (i == 0) {
            printf("  ");
        }
        if (i == 8) {
            printf("  ");
        }
        printf(" %02x", i);
    }
    printf("\n");
    for (size_t i = 0; i < 16; i++) {
        if (i == 0) {
            printf("  ");
        }
        if (i == 8) {
            printf(" |");
        }
        printf(" --", i);
    }
    printf("\n");
    for (size_t i = 0; i < size; i++) {
        if (i % 16 == 0) {
            if (i != 0) {
                printf("\n");
            }
            printf("%02x", (line++) * 16);
        } else {
            if (i % 8 == 0) {
                printf("  ");
            }
        }
        printf(" %02x", ((uint8_t *)data)[i]);
    }
    printf("\n");
}

int
testcase_assert (
    testcase_t          priv,
    void               *expect,
    void               *target,
    size_t              size,
    char const         *file,
    int  const          line,
    char const         *func,
    char const         *format,
    ...
)
{
    int                 r       = 0;
    testcase_rslt_t     rslt    = 0;
    char                info [1024];

    if (!file || !line || !func) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }
 
    if (expect && target) {
        if (size) {
            rslt = !memcmp(expect, target, size);
            if (rslt == testcase_record_fail) {
                // print expect and target
                printf("expect:\n");
                testcase_data_print(expect, size);
                printf("target:\n");
                testcase_data_print(target, size);
            }
        } else {
            rslt = !strcmp(expect, target);
            if (rslt == testcase_record_fail) {
                printf("expect: %s\n", expect);
                printf("target: %s\n", target);
            }
        }
    } else {
        rslt = (testcase_rslt_t) (size!=0);
    }

    info[0] = '\0';
    if (format) {
        va_list args; va_start(args, format);  vsnprintf(info, sizeof(info), format, args);  va_end(args);
    }

    if (priv) {
        r = testcase_record(priv, rslt, file, line, func, (char *) &info);
    } else {
        printf("[00] (function)@(file:line):(description) = (%s)@(%s:%d):(%s)\n", func, file, line, info);
    }

eofn:
    return r;
}

int
testcase_report (
    testcase_t          priv
)
{
    testcase_test_t    *tc_test = priv;
    testcase_case_t    *tc_case = NULL;
    lsz_list_t         *this    = NULL;
    lsz_list_t         *next    = NULL;
    int                 i       = 0;

    printf("total: %4d tests, %4d passed, %4d failed\n", tc_test->cnts, tc_test->pass, tc_test->fail);

    for (this = tc_test->list.next, next = this->next; this != &tc_test->list; this = next, next = this->next) {
        tc_case = base_of(this, testcase_case_t, link);
        if (tc_case->rslt == testcase_record_fail) {
            printf("[%02d] (function)@(file:line):(description) = (%s)@(%s:%d):(%s)\n", ++i, tc_case->func, tc_case->file, tc_case->line, tc_case->info);
        }
    }

    return LSZ_RET_0_ERR;
}

/*
--------------------------------------------------------------------------------
end!
--------------------------------------------------------------------------------
*/
