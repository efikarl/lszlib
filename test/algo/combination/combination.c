#include <lszlib.h>

static void callback_print(void *sub, int m)
{
    int *set = (int *)sub;
    for (int i = 0; i < m; i++) {
        printf("%04d", set[i]);
        if (i != m - 1) {
            printf(", ");
        } else {
            printf("\n");
        }
    }
}

void testcase_combination()
{
    int set[] = { 0, 1, 2 };
    int r = 0;
    int n = 0;
    int m = 0;
    int v = 0;

    n = sizeof(set)/sizeof(int);
    for (m = 0; m <= n; m++) {
        r = combination(set, sizeof(int), n, m, callback_print);
        v = 1;
        for (int i = n; i > (n - m); i--) {
            v *= i;
        }
        for (int i = m; i > 0; i--) {
            v /= i;
        }
        TESTCASE_ASSERT_BOOL(r == v, "combination");
    }
}

void main(int argc, char const *argv[])
{
    lsz_testcase_do {
        testcase_combination();
    } lsz_testcase_done;
}