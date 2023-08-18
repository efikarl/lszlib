#include <lszlib.h>

void testcase_lsz_swap_val()
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
    TESTCASE_ASSERT_DATA(&data0_expect, &data0_target, sizeof(data_t), "data0 swap(+)");
    TESTCASE_ASSERT_DATA(&data1_expect, &data1_target, sizeof(data_t), "data1 swap(+)");
    lsz_swap_val(&data0_target, &data1_target, sizeof(data_t));
    TESTCASE_ASSERT_DATA(&data1_expect, &data0_target, sizeof(data_t), "data0 swap(-)");
    TESTCASE_ASSERT_DATA(&data0_expect, &data1_target, sizeof(data_t), "data1 swap(-)");

    lsz_swap_val(&data1_target, &data0_target, sizeof(data_t));
    TESTCASE_ASSERT_DATA(&data0_expect, &data0_target, sizeof(data_t), "data0 swap(+)");
    TESTCASE_ASSERT_DATA(&data1_expect, &data1_target, sizeof(data_t), "data1 swap(+)");
    lsz_swap_val(&data1_target, &data0_target, sizeof(data_t));
    TESTCASE_ASSERT_DATA(&data1_expect, &data0_target, sizeof(data_t), "data0 swap(-)");
    TESTCASE_ASSERT_DATA(&data0_expect, &data1_target, sizeof(data_t), "data1 swap(-)");
}
void testcase_lsz_swap_ptr()
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

    data_t *pointer_a = &data0_expect;
    data_t *pointer_b = &data1_expect;

    lsz_swap_ptr((void **) &pointer_a, (void **)&pointer_b);
    TESTCASE_ASSERT_DATA(&data0_expect, pointer_b, sizeof(data_t), "data0 swap(+)");
    TESTCASE_ASSERT_DATA(&data1_expect, pointer_a, sizeof(data_t), "data1 swap(+)");
    lsz_swap_ptr((void **) &pointer_a, (void **)&pointer_b);
    TESTCASE_ASSERT_DATA(&data0_expect, pointer_a, sizeof(data_t), "data1 swap(-)");
    TESTCASE_ASSERT_DATA(&data1_expect, pointer_b, sizeof(data_t), "data0 swap(-)");

    lsz_swap_ptr((void **) &pointer_b, (void **) &pointer_a);
    TESTCASE_ASSERT_DATA(&data0_expect, pointer_b, sizeof(data_t), "data0 swap(+)");
    TESTCASE_ASSERT_DATA(&data1_expect, pointer_a, sizeof(data_t), "data1 swap(+)");
    lsz_swap_ptr((void **) &pointer_b, (void **) &pointer_a);
    TESTCASE_ASSERT_DATA(&data0_expect, pointer_a, sizeof(data_t), "data1 swap(-)");
    TESTCASE_ASSERT_DATA(&data1_expect, pointer_b, sizeof(data_t), "data0 swap(-)");
}

void testcase_lsz_compare_int()
{
    int8_t a[32];
    int8_t b[32];

    memset(a, 0x00, sizeof(a));
    memset(b, 0x00, sizeof(b));

    a[0] = 'a';
    b[0] = 'b';
    TESTCASE_ASSERT_BOOL(lsz_compare_int((int *) &a, (int *) &b) < 0, "less than");
    TESTCASE_ASSERT_BOOL(lsz_compare_int((int *) &b, (int *) &a) > 0, "more than");

    a[0] = 'e';
    b[0] = 'e';
    TESTCASE_ASSERT_BOOL(lsz_compare_int((int *) &a, (int *) &b) == 0, "equal");
}

void testcase_lsz_compare_str()
{
    int8_t a[32];
    int8_t b[32];

    memset(a, 0x00, sizeof(a));
    memset(b, 0x00, sizeof(b));

    a[0] = 'a';
    b[0] = 'b';
    TESTCASE_ASSERT_BOOL(lsz_compare_str((char *) &a, (char *) &b) < 0, "less than");
    TESTCASE_ASSERT_BOOL(lsz_compare_str((char *) &b, (char *) &a) > 0, "more than");

    a[0] = 'e';
    b[0] = 'e';
    TESTCASE_ASSERT_BOOL(lsz_compare_str((char *) &b, (char *) &a) == 0, "equal");
}

void testcase_lsz_n_align_bit()
{
    size_t v;

    v = lsz_n_align_bit(0x00, 4);
    TESTCASE_ASSERT_BOOL(v == 0x00, "lsz_n_align_bit(0x00, 4)");

    v = lsz_n_align_bit(0x0f, 4);
    TESTCASE_ASSERT_BOOL(v == 0x00, "lsz_n_align_bit(0x0f, 4)");

    v = lsz_n_align_bit(0x10, 4);
    TESTCASE_ASSERT_BOOL(v == 0x10, "lsz_n_align_bit(0x10, 4)");

    v = lsz_n_align_bit(0x11, 4);
    TESTCASE_ASSERT_BOOL(v == 0x10, "lsz_n_align_bit(0x11, 4)");

    v = lsz_n_align_bit(0x1f, 4);
    TESTCASE_ASSERT_BOOL(v == 0x10, "lsz_n_align_bit(0x1f, 4)");
}

void testcase_lsz_p_align_bit()
{
    size_t v;
    
    v = lsz_p_align_bit(0x00, 4);
    TESTCASE_ASSERT_BOOL(v == 0x00, "lsz_p_align_bit(0x00, 4)");

    v = lsz_p_align_bit(0x01, 4);
    TESTCASE_ASSERT_BOOL(v == 0x10, "lsz_p_align_bit(0x01, 4)");

    v = lsz_p_align_bit(0x0f, 4);
    TESTCASE_ASSERT_BOOL(v == 0x10, "lsz_p_align_bit(0x0f, 4)");

    v = lsz_p_align_bit(0x10, 4);
    TESTCASE_ASSERT_BOOL(v == 0x10, "lsz_p_align_bit(0x10, 4)");

    v = lsz_p_align_bit(0x11, 4);
    TESTCASE_ASSERT_BOOL(v == 0x20, "lsz_p_align_bit(0x11, 4)");
}

void main(int argc, char const *argv[])
{
    lsz_testcase_do {
        testcase_lsz_swap_val();
        testcase_lsz_swap_ptr();
        testcase_lsz_compare_int();
        testcase_lsz_compare_str();
        testcase_lsz_n_align_bit();
        testcase_lsz_p_align_bit();
    } lsz_testcase_done;
}
