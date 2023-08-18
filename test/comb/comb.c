#include <lszlib.h>
#include <assert.h>
#include <string.h>

void cout(int *sub, int m)
{
    for (int i = 0; i < m; i++) {
        printf("%04d", sub[i]);
        if (i != m - 1) {
            printf(", ");
        } else {
            printf("\n");
        }
    }
}

void main(int argc, char const *argv[])
{
    int set[] = { 0, 1, 2, 3, 4 };
    int ret   = lsz_comb(set, sizeof(int), sizeof(set)/sizeof(int), 2, cout);

    assert(ret == ((5*4)/(2*1)));
}
