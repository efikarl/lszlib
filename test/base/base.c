#include <lszlib.h>
#include <assert.h>
#include <string.h>

typedef struct {
    char user[8];
    char pswd[8];
} data_t;

void main(int argc, char const *argv[])
{
    data_t data0 = {
        .user = "   ",
        .pswd = "~1q",
    };
    data_t data1 = {
        .user = "hello",
        .pswd = "world",
    };

    lsz_swap(&data0, &data1, sizeof(data_t));
    assert(!strcmp(data0.user, "hello"));
    assert(!strcmp(data0.pswd, "world"));
    assert(!strcmp(data1.user, "   "  ));
    assert(!strcmp(data1.pswd, "~1q"  ));
}
