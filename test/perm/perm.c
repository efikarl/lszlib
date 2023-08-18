#include <lszlib.h>
#include <assert.h>
#include <string.h>

void pout(int *perm, int m, int n)
{
    for (int i = m; i < n; i++) {
        printf("%04d", perm[i]);
        if (i != n - 1) {
            printf(", ");
        } else {
            printf("\n");
        }
    }
}

void main(int argc, char const *argv[])
{
    int vec[] = { 0, 1, 2 };
    int ret   = lsz_perm(vec, sizeof(int), 0, sizeof(vec)/sizeof(int), pout);

    assert(ret == (3*2*1));
}

/*@log
    ./a.out
    _lsz_perm(0)+
    _lsz_perm(0).lsz_swap(0,0)+
    0000, 0001, 0002
    _lsz_perm(1)+
    _lsz_perm(1).lsz_swap(1,1)+
    0000, 0001, 0002
    _lsz_perm(2)+
    _lsz_perm(2).lsz_swap(2,2)+
    0000, 0001, 0002
    _lsz_perm(3)+ # return
    0000, 0001, 0002
    _lsz_perm(2).lsz_swap(2,2)-
    0000, 0001, 0002
    _lsz_perm(2)-
    _lsz_perm(1).lsz_swap(1,1)-
    0000, 0001, 0002
    _lsz_perm(1).lsz_swap(1,2)+
    0000, 0002, 0001
    _lsz_perm(2)+
    _lsz_perm(2).lsz_swap(2,2)+
    0000, 0002, 0001
    _lsz_perm(3)+ # return
    0000, 0002, 0001
    _lsz_perm(2).lsz_swap(2,2)-
    0000, 0002, 0001
    _lsz_perm(2)-
    _lsz_perm(1).lsz_swap(1,2)-
    0000, 0001, 0002
    _lsz_perm(1)-
    _lsz_perm(0).lsz_swap(0,0)-
    0000, 0001, 0002
    _lsz_perm(0).lsz_swap(0,1)+
    0001, 0000, 0002
    _lsz_perm(1)+
    _lsz_perm(1).lsz_swap(1,1)+
    0001, 0000, 0002
    _lsz_perm(2)+
    _lsz_perm(2).lsz_swap(2,2)+
    0001, 0000, 0002
    _lsz_perm(3)+ # return
    0001, 0000, 0002
    _lsz_perm(2).lsz_swap(2,2)-
    0001, 0000, 0002
    _lsz_perm(2)-
    _lsz_perm(1).lsz_swap(1,1)-
    0001, 0000, 0002
    _lsz_perm(1).lsz_swap(1,2)+
    0001, 0002, 0000
    _lsz_perm(2)+
    _lsz_perm(2).lsz_swap(2,2)+
    0001, 0002, 0000
    _lsz_perm(3)+ # return
    0001, 0002, 0000
    _lsz_perm(2).lsz_swap(2,2)-
    0001, 0002, 0000
    _lsz_perm(2)-
    _lsz_perm(1).lsz_swap(1,2)-
    0001, 0000, 0002
    _lsz_perm(1)-
    _lsz_perm(0).lsz_swap(0,1)-
    0000, 0001, 0002
    _lsz_perm(0).lsz_swap(0,2)+
    0002, 0001, 0000
    _lsz_perm(1)+
    _lsz_perm(1).lsz_swap(1,1)+
    0002, 0001, 0000
    _lsz_perm(2)+
    _lsz_perm(2).lsz_swap(2,2)+
    0002, 0001, 0000
    _lsz_perm(3)+ # return
    0002, 0001, 0000
    _lsz_perm(2).lsz_swap(2,2)-
    0002, 0001, 0000
    _lsz_perm(2)-
    _lsz_perm(1).lsz_swap(1,1)-
    0002, 0001, 0000
    _lsz_perm(1).lsz_swap(1,2)+
    0002, 0000, 0001
    _lsz_perm(2)+
    _lsz_perm(2).lsz_swap(2,2)+
    0002, 0000, 0001
    _lsz_perm(3)+ # return
    0002, 0000, 0001
    _lsz_perm(2).lsz_swap(2,2)-
    0002, 0000, 0001
    _lsz_perm(2)-
    _lsz_perm(1).lsz_swap(1,2)-
    0002, 0001, 0000
    _lsz_perm(1)-
    _lsz_perm(0).lsz_swap(0,2)-
    0000, 0001, 0002
    _lsz_perm(0)-
*/
