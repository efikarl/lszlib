#include <lszlib.h>

void testcase_path_node_1(void)
{
    int             retn               = 0;
    size_t          path_arr_length    = 0;

    char           *target_path_str    = NULL;
    char           *expect_path_str    = "user/Pass_w0rd";
    path_node_t     source_path_arr[]  = { "user", "Pass_w0rd" };
                    path_arr_length    = sizeof(source_path_arr)/sizeof(path_node_t);
    
    retn = path_from(source_path_arr, path_arr_length, &target_path_str);
    TESTCASE_ASSERT_DATA(
        target_path_str, expect_path_str, strlen(expect_path_str),
    "path_from.1");


    char           *source_path_str    = target_path_str;
    path_node_t     expect_path_arr[]  = { "user", "Pass_w0rd" };
    path_node_t    *target_path_arr    = NULL;
                    path_arr_length    = strlen(source_path_str);
    
    retn = path_into(source_path_str, &target_path_arr, &path_arr_length);
    for (int i = 0; i < path_arr_length; i++) {
    TESTCASE_ASSERT_DATA(
        target_path_arr[i], expect_path_arr[i], sizeof(path_node_t),
    "path_into.1");
    }
}

void testcase_path_node_2(void)
{
    int             retn               = 0;
    size_t          path_arr_length    = 0;

    char           *target_path_str    = NULL;
    char           *expect_path_str    = "user/Pass_w0rd";
    path_node_t     source_path_arr[]  = { "user/", "/Pass_w0rd" };
                    path_arr_length    = sizeof(source_path_arr)/sizeof(path_node_t);
    
    retn = path_from(source_path_arr, path_arr_length, &target_path_str);
    TESTCASE_ASSERT_DATA(
        target_path_str, expect_path_str, strlen(expect_path_str),
    "path_from.2");


    char           *source_path_str    = target_path_str;
    path_node_t     expect_path_arr[]  = { "user", "Pass_w0rd" };
    path_node_t    *target_path_arr    = NULL;
                    path_arr_length    = strlen(source_path_str);
    
    retn = path_into(source_path_str, &target_path_arr, &path_arr_length);
    for (int i = 0; i < path_arr_length; i++) {
    TESTCASE_ASSERT_DATA(
        target_path_arr[i], expect_path_arr[i], sizeof(path_node_t),
    "path_into.2");
    }
}

void testcase_path_node_3(void)
{
    int             retn               = 0;
    size_t          path_arr_length    = 0;

    char           *target_path_str    = NULL;
    char           *expect_path_str    = "user/Pass_w0rd";
    path_node_t     source_path_arr[]  = { "user//", "//Pass_w0rd/" };
                    path_arr_length    = sizeof(source_path_arr)/sizeof(path_node_t);
    
    retn = path_from(source_path_arr, path_arr_length, &target_path_str);
    TESTCASE_ASSERT_DATA(
        target_path_str, expect_path_str, strlen(expect_path_str),
    "path_from.3");


    char           *source_path_str    = target_path_str;
    path_node_t     expect_path_arr[]  = { "user", "Pass_w0rd" };
    path_node_t    *target_path_arr    = NULL;
                    path_arr_length    = strlen(source_path_str);
    
    retn = path_into(source_path_str, &target_path_arr, &path_arr_length);
    for (int i = 0; i < path_arr_length; i++) {
    TESTCASE_ASSERT_DATA(
        target_path_arr[i], expect_path_arr[i], sizeof(path_node_t),
    "path_into.3");
    }
}

void testcase_path_node_4(void)
{
    int             retn               = 0;
    size_t          path_arr_length    = 0;

    char           *target_path_str    = NULL;
    char           *expect_path_str    = "/user/Pass_w0rd";
    path_node_t     source_path_arr[]  = { "/user", "Pass_w0rd/" };
                    path_arr_length    = sizeof(source_path_arr)/sizeof(path_node_t);
    
    retn = path_from(source_path_arr, path_arr_length, &target_path_str);
    TESTCASE_ASSERT_DATA(
        target_path_str, expect_path_str, strlen(expect_path_str),
    "path_from.4");


    char           *source_path_str    = target_path_str;
    path_node_t     expect_path_arr[]  = { "/", "user", "Pass_w0rd" };
    path_node_t    *target_path_arr    = NULL;
                    path_arr_length    = strlen(source_path_str);
    
    retn = path_into(source_path_str, &target_path_arr, &path_arr_length);
    for (int i = 0; i < path_arr_length; i++) {
    TESTCASE_ASSERT_DATA(
        target_path_arr[i], expect_path_arr[i], sizeof(path_node_t),
    "path_into.4");
    }
}

void testcase_path_node_5(void)
{
    int             retn               = 0;
    size_t          path_arr_length    = 0;

    char           *target_path_str    = NULL;
    char           *expect_path_str    = "/user/Pass_w0rd";
    path_node_t     source_path_arr[]  = { "/user//", "//Pass_w0rd/" };
                    path_arr_length    = sizeof(source_path_arr)/sizeof(path_node_t);
    
    retn = path_from(source_path_arr, path_arr_length, &target_path_str);
    TESTCASE_ASSERT_DATA(
        target_path_str, expect_path_str, strlen(expect_path_str),
    "path_from.5");


    char           *source_path_str    = target_path_str;
    path_node_t     expect_path_arr[]  = { "/", "user", "Pass_w0rd" };
    path_node_t    *target_path_arr    = NULL;
                    path_arr_length    = strlen(source_path_str);
    
    retn = path_into(source_path_str, &target_path_arr, &path_arr_length);
    for (int i = 0; i < path_arr_length; i++) {
    TESTCASE_ASSERT_DATA(
        target_path_arr[i], expect_path_arr[i], sizeof(path_node_t),
    "path_into.5");
    }
}

void testcase_path_node_6(void)
{
    int             retn               = 0;
    size_t          path_arr_length    = 0;

    char           *target_path_str    = NULL;
    char           *expect_path_str    = "/user/Pass_w0rd";
    path_node_t     source_path_arr[]  = { "/", "user", "Pass_w0rd" };
                    path_arr_length    = sizeof(source_path_arr)/sizeof(path_node_t);
    
    retn = path_from(source_path_arr, path_arr_length, &target_path_str);
    TESTCASE_ASSERT_DATA(
        target_path_str, expect_path_str, strlen(expect_path_str),
    "path_from.6");


    char           *source_path_str    = target_path_str;
    path_node_t     expect_path_arr[]  = { "/", "user", "Pass_w0rd" };
    path_node_t    *target_path_arr    = NULL;
                    path_arr_length    = strlen(source_path_str);
    
    retn = path_into(source_path_str, &target_path_arr, &path_arr_length);
    for (int i = 0; i < path_arr_length; i++) {
    TESTCASE_ASSERT_DATA(
        target_path_arr[i], expect_path_arr[i], sizeof(path_node_t),
    "path_into.6");
    }
}

void testcase_path_node_7(void)
{
    int             retn               = 0;
    size_t          path_arr_length    = 0;

    char           *target_path_str    = NULL;
    char           *expect_path_str    = "/user/Pass_w0rd";
    path_node_t     source_path_arr[]  = { "/", "user", "Pass_w0rd/" };
                    path_arr_length    = sizeof(source_path_arr)/sizeof(path_node_t);
    
    retn = path_from(source_path_arr, path_arr_length, &target_path_str);
    TESTCASE_ASSERT_DATA(
        target_path_str, expect_path_str, strlen(expect_path_str),
    "path_from.7");


    char           *source_path_str    = target_path_str;
    path_node_t     expect_path_arr[]  = { "/", "user", "Pass_w0rd" };
    path_node_t    *target_path_arr    = NULL;
                    path_arr_length    = strlen(source_path_str);
    
    retn = path_into(source_path_str, &target_path_arr, &path_arr_length);
    for (int i = 0; i < path_arr_length; i++) {
    TESTCASE_ASSERT_DATA(
        target_path_arr[i], expect_path_arr[i], sizeof(path_node_t),
    "path_into.7");
    }
}

void testcase_path_node_8(void)
{
    int             retn               = 0;
    size_t          path_arr_length    = 0;

    char           *target_path_str    = NULL;
    char           *expect_path_str    = "/user/Pass_w0rd";
    path_node_t     source_path_arr[]  = { "/", "/user/", "/Pass_w0rd/" };
                    path_arr_length    = sizeof(source_path_arr)/sizeof(path_node_t);
    
    retn = path_from(source_path_arr, path_arr_length, &target_path_str);
    TESTCASE_ASSERT_DATA(
        target_path_str, expect_path_str, strlen(expect_path_str),
    "path_from.8");


    char           *source_path_str    = target_path_str;
    path_node_t     expect_path_arr[]  = { "/", "user", "Pass_w0rd" };
    path_node_t    *target_path_arr    = NULL;
                    path_arr_length    = strlen(source_path_str);
    
    retn = path_into(source_path_str, &target_path_arr, &path_arr_length);
    for (int i = 0; i < path_arr_length; i++) {
    TESTCASE_ASSERT_DATA(
        target_path_arr[i], expect_path_arr[i], sizeof(path_node_t),
    "path_into.8");
    }
}

void testcase_path_node_9(void)
{
    int             retn               = 0;
    size_t          path_arr_length    = 0;

    char           *target_path_str    = NULL;
    char           *expect_path_str    = "/user[5]/Pass_w0rd[4]";
    path_node_t     source_path_arr[]  = { "/", "/user[5]/", "/Pass_w0rd[4]/" };
                    path_arr_length    = sizeof(source_path_arr)/sizeof(path_node_t);
    
    retn = path_from(source_path_arr, path_arr_length, &target_path_str);
    TESTCASE_ASSERT_DATA(
        target_path_str, expect_path_str, strlen(expect_path_str),
    "path_from.9");


    char           *source_path_str    = target_path_str;
    path_node_t     expect_path_arr[]  = { "/", "user[5]", "Pass_w0rd[4]" };
    path_node_t    *target_path_arr    = NULL;
                    path_arr_length    = strlen(source_path_str);
    
    retn = path_into(source_path_str, &target_path_arr, &path_arr_length);
    for (int i = 0; i < path_arr_length; i++) {
    TESTCASE_ASSERT_DATA(
        target_path_arr[i], expect_path_arr[i], sizeof(path_node_t),
    "path_into.9");
    }
}

void testcase_path_node_a(void)
{
    int             retn               = 0;
    size_t          path_arr_length    = 0;

    char           *target_path_str    = NULL;
    char           *expect_path_str    = "/user[0xf0][1]/Pass_w0rd[2][0x55]";
    path_node_t     source_path_arr[]  = { "/", "/user[0xf0][1]/", "/Pass_w0rd[2][0x55]/" };
                    path_arr_length    = sizeof(source_path_arr)/sizeof(path_node_t);
    
    retn = path_from(source_path_arr, path_arr_length, &target_path_str);
    TESTCASE_ASSERT_DATA(
        target_path_str, expect_path_str, strlen(expect_path_str),
    "path_from.a");


    char           *source_path_str    = target_path_str;
    path_node_t     expect_path_arr[]  = { "/", "user[0xf0][1]", "Pass_w0rd[2][0x55]" };
    path_node_t    *target_path_arr    = NULL;
                    path_arr_length    = strlen(source_path_str);
    
    retn = path_into(source_path_str, &target_path_arr, &path_arr_length);
    for (int i = 0; i < path_arr_length; i++) {
    TESTCASE_ASSERT_DATA(
        target_path_arr[i], expect_path_arr[i], sizeof(path_node_t),
    "path_into.a");
    }
}

void main(int argc, char const *argv[])
{
    lsz_testcase_do {
        testcase_path_node_1();
        testcase_path_node_2();
        testcase_path_node_3();
        testcase_path_node_4();
        testcase_path_node_5();
        testcase_path_node_6();
        testcase_path_node_7();
        testcase_path_node_8();
        testcase_path_node_9();
        testcase_path_node_a();
    } lsz_testcase_done;
}
