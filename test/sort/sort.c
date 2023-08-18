#include <lszlib.h>
#include <assert.h>
#include <string.h>

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

void main(int argc, char const *argv[])
{
    int v0[] = { 0, 0, 1, 1, 2, 2, 3, 3, };
    int v1[] = { 3, 3, 2, 2, 1, 1, 0, 0, };
    lsz_qsort(v1, sizeof(int), sizeof(v1)/sizeof(int), lsz_icmp); vout(v1, sizeof(v1)/sizeof(int));
    assert(!memcmp(v0, v1, sizeof(v0)));

    int a0[] = { 0, 0, 0, 1, 2, 3, 4, 5, };
    int a1[] = { 0, 0, 5, 0, 2, 1, 4, 3, };
    lsz_qsort(a1, sizeof(int), sizeof(a1)/sizeof(int), lsz_icmp); vout(a1, sizeof(a1)/sizeof(int));
    assert(!memcmp(a0, a1, sizeof(a0)));

    char s0[][4] = { "___", "abc", "acb", "bac", "bca", "cab", "cba" };
    char s1[][4] = { "cba", "abc", "acb", "bca", "bac", "cab", "___" };
    lsz_qsort(s1, sizeof(char [4]), sizeof(s1)/sizeof(char [4]), lsz_scmp); sout(s1, sizeof(s1)/sizeof(char [4]));
    assert(!memcmp(s0, s1, sizeof(s0)));
}
