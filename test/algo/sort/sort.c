#include <lszlib.h>

void vout(int *v, int n)
{
    for (int i = 0; i < n; i++) {
        printf("%d", v[i]);
        if (i != n - 1) {
            printf(", ");
        } else {
            printf("\n");
        }
    }
}

void sout(char (*s)[4], int n)
{
    for (int i = 0; i < n; i++) {
        printf("%s", s[i]);
        if (i != n - 1) {
            printf(", ");
        } else {
            printf("\n");
        }
    }
}

void testcase_qsort_int(void)
{
    int a0[] = { 0, 1, 2, 3, 4, 5, 6, 7, };
    int a1[] = { 0, 1, 2, 3, 4, 5, 6, 7, };
    quick_sort(a1, sizeof(int), sizeof(a1)/sizeof(int), lsz_compare_int); vout(a1, sizeof(a1)/sizeof(int));
    TESTCASE_ASSERT_DATA(a0, a1, sizeof(a0), "int");

    int b0[] = { 0, 1, 2, 3, 4, 5, 6, 7, };
    int b1[] = { 7, 6, 5, 4, 3, 2, 1, 0, };
    quick_sort(b1, sizeof(int), sizeof(b1)/sizeof(int), lsz_compare_int); vout(b1, sizeof(b1)/sizeof(int));
    TESTCASE_ASSERT_DATA(b0, b1, sizeof(b0), "int");

    int c0[] = { 0, 0, 1, 1, 2, 2, 3, 3, };
    int c1[] = { 0, 0, 1, 1, 2, 2, 3, 3, };
    quick_sort(c1, sizeof(int), sizeof(c1)/sizeof(int), lsz_compare_int); vout(c1, sizeof(c1)/sizeof(int));
    TESTCASE_ASSERT_DATA(c0, c1, sizeof(c0), "int");

    int d0[] = { 0, 0, 1, 1, 2, 2, 3, 3, };
    int d1[] = { 3, 3, 2, 2, 1, 1, 0, 0, };
    quick_sort(d1, sizeof(int), sizeof(d1)/sizeof(int), lsz_compare_int); vout(d1, sizeof(d1)/sizeof(int));
    TESTCASE_ASSERT_DATA(d0, d1, sizeof(d0), "int");

    int e0[] = { 0, 0, 0, 0, 0, 0, 0, 0, };
    int e1[] = { 0, 0, 0, 0, 0, 0, 0, 0, };
    quick_sort(e1, sizeof(int), sizeof(e1)/sizeof(int), lsz_compare_int); vout(e1, sizeof(e1)/sizeof(int));
    TESTCASE_ASSERT_DATA(e0, e1, sizeof(e0), "int");

    int f0[] = { 0, 0, 0, 0, 1, 2, 3, 4, };
    int f1[] = { 0, 2, 0, 4, 0, 1, 0, 3, };
    quick_sort(f1, sizeof(int), sizeof(f1)/sizeof(int), lsz_compare_int); vout(f1, sizeof(f1)/sizeof(int));
    TESTCASE_ASSERT_DATA(f0, f1, sizeof(f0), "int");

    int g0[] = { 0, 0, 0, 0, 1, 2, 3, 4, };
    int g1[] = { 3, 0, 0, 1, 4, 0, 0, 2, };
    quick_sort(g1, sizeof(int), sizeof(g1)/sizeof(int), lsz_compare_int); vout(g1, sizeof(g1)/sizeof(int));
    TESTCASE_ASSERT_DATA(g0, g1, sizeof(g0), "int");
}

void testcase_qsort_str(void)
{
    char s0[][4] = { "___", "abc", "acb", "bac", "bca", "cab", "cba" };
    char s1[][4] = { "cba", "abc", "acb", "bca", "bac", "cab", "___" };
    quick_sort(s1, sizeof(char [4]), sizeof(s1)/sizeof(char [4]), lsz_compare_str);
    TESTCASE_ASSERT_DATA(s0, s1, sizeof(s0), "str");
}

void main(int argc, char const *argv[])
{
    lsz_testcase_do {
        testcase_qsort_int();
        testcase_qsort_str();
    } lsz_testcase_done;
}
