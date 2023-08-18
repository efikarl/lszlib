#include <lszlib.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

void testcase_lsz_swap()
{
    typedef struct {
        char    user[8] ;
        int     year    ;
    } data_t;

    data_t data0_expect = {
        .user = "hi"    ,
        .year = 2023    ,
    };
    data_t data1_expect = {
        .user = "world" ,
        .year = - 1024  ,
    };

    data_t data1_target = {
        .user = "hi"    ,
        .year = 2023    ,
    };
    data_t data0_target = {
        .user = "world" ,
        .year = - 1024  ,
    };

    lsz_swap_val(&data0_target, &data1_target, sizeof(data_t));
    TESTCASE_ASSERT_DATA(&data0_expect, &data0_target, sizeof(data_t), "data0 +swap 1");
    TESTCASE_ASSERT_DATA(&data1_expect, &data1_target, sizeof(data_t), "data1 +swap 1");
    lsz_swap_val(&data0_target, &data1_target, sizeof(data_t));
    TESTCASE_ASSERT_DATA(&data1_expect, &data0_target, sizeof(data_t), "data0 -swap 1");
    TESTCASE_ASSERT_DATA(&data0_expect, &data1_target, sizeof(data_t), "data1 -swap 1");

    lsz_swap_val(&data1_target, &data0_target, sizeof(data_t));
    TESTCASE_ASSERT_DATA(&data0_expect, &data0_target, sizeof(data_t), "data0 +swap 2");
    TESTCASE_ASSERT_DATA(&data1_expect, &data1_target, sizeof(data_t), "data1 +swap 2");
    lsz_swap_val(&data1_target, &data0_target, sizeof(data_t));
    TESTCASE_ASSERT_DATA(&data1_expect, &data0_target, sizeof(data_t), "data0 -swap 2");
    TESTCASE_ASSERT_DATA(&data0_expect, &data1_target, sizeof(data_t), "data1 -swap 2");
}

void testcase_lsz_icmp()
{
    int8_t a[32];
    int8_t b[32];

    memset(a, 0x00, sizeof(a));
    memset(b, 0x00, sizeof(b));

    a[0] = 'a';
    b[0] = 'b';
    TESTCASE_ASSERT_BOOL(lsz_cmp_int((int *) &a, (int *) &b) < 0, "less than");
    TESTCASE_ASSERT_BOOL(lsz_cmp_int((int *) &b, (int *) &a) > 0, "more than");

    a[0] = 'e';
    b[0] = 'e';
    TESTCASE_ASSERT_BOOL(lsz_cmp_int((int *) &a, (int *) &b) == 0, "equal");
}

void testcase_lsz_scmp()
{
    int8_t a[32];
    int8_t b[32];

    memset(a, 0x00, sizeof(a));
    memset(b, 0x00, sizeof(b));

    a[0] = 'a';
    b[0] = 'b';
    TESTCASE_ASSERT_BOOL(lsz_cmp_str((char *) &a, (char *) &b) < 0, "less than");
    TESTCASE_ASSERT_BOOL(lsz_cmp_str((char *) &b, (char *) &a) > 0, "more than");

    a[0] = 'e';
    b[0] = 'e';
    TESTCASE_ASSERT_BOOL(lsz_cmp_str((char *) &b, (char *) &a) == 0, "equal");
}

void testcase_size_n_align()
{
    size_t v;

    v = size_n_align(4, 0x00);
    TESTCASE_ASSERT_BOOL(v == 0x00, "size_n_align(4, 0x00)");

    v = size_n_align(4, 0x0f);
    TESTCASE_ASSERT_BOOL(v == 0x00, "size_n_align(4, 0x0f)");

    v = size_n_align(4, 0x10);
    TESTCASE_ASSERT_BOOL(v == 0x10, "size_n_align(4, 0x10)");

    v = size_n_align(4, 0x11);
    TESTCASE_ASSERT_BOOL(v == 0x10, "size_n_align(4, 0x11)");

    v = size_n_align(4, 0x1f);
    TESTCASE_ASSERT_BOOL(v == 0x10, "size_n_align(4, 0x1f)");
}

void testcase_size_p_align()
{
    size_t v;
    
    v = size_p_align(4, 0x00);
    TESTCASE_ASSERT_BOOL(v == 0x00, "size_p_align(4, 0x00)");

    v = size_p_align(4, 0x01);
    TESTCASE_ASSERT_BOOL(v == 0x10, "size_p_align(4, 0x01)");

    v = size_p_align(4, 0x0f);
    TESTCASE_ASSERT_BOOL(v == 0x10, "size_p_align(4, 0x0f)");

    v = size_p_align(4, 0x10);
    TESTCASE_ASSERT_BOOL(v == 0x10, "size_p_align(4, 0x10)");

    v = size_p_align(4, 0x11);
    TESTCASE_ASSERT_BOOL(v == 0x20, "size_p_align(4, 0x11)");
}

void main(int argc, char const *argv[])
{
    lsz_testcase_do {
        testcase_lsz_swap();
        testcase_lsz_icmp();
        testcase_lsz_scmp();
        testcase_size_n_align();
        testcase_size_p_align();
    } lsz_testcase_done;
}
