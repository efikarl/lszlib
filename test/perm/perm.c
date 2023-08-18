#include <lszlib.h>
#include <assert.h>
#include <string.h>

void pout(void *sub, int m)
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

void testcase_lsz_perm()
{
    int set[] = { 0, 1, 2 };
    int r = 0;
    int n = 0;
    int v = 0;

    n = sizeof(set)/sizeof(int);
    r = lsz_perm(set, sizeof(int), n, n, pout);
    v = 1;
    for (int i = n; i > 0; i--) {
        v *= i;
    }
    TESTCASE_ASSERT_BOOL(r == v, "comb.value");
}

void main(int argc, char const *argv[])
{
    lsz_testcase_do {
        testcase_lsz_perm();
    } lsz_testcase_done;
}
