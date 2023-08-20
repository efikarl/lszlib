#include <lszlib.h>
#include <assert.h>
#include <string.h>

void main(int argc, char const *argv[])
{
    char *path = NULL;
    int   retn = 0;

    {
        char            *pexp           = "user/Pass_w0rd";
        path_node_t      veci[]         = { "user", "Pass_w0rd" };
        size_t           cnti           = sizeof(veci)/sizeof(path_node_t);
        
        retn = path_from(veci, cnti, &path);
        assert(!memcmp(path, pexp, strlen(pexp)));


        path_node_t      vexp[]         = { "user", "Pass_w0rd" };
        path_node_t     *veco           = NULL;
        size_t           cnto           = strlen(path);
        
        retn = path_into(path, &veco, &cnto);
        for (int i = 0; i < cnto; i++) {
            assert(!memcmp(veco[i], vexp[i], sizeof(path_node_t)));
        }
    }

    {
        char            *pexp           = "user/Pass_w0rd";
        path_node_t      veci[]         = { "user/", "/Pass_w0rd" };
        size_t           cnti           = sizeof(veci)/sizeof(path_node_t);
        
        retn = path_from(veci, cnti, &path);
        assert(!memcmp(path, pexp, strlen(pexp)));


        path_node_t      vexp[]         = { "user", "Pass_w0rd" };
        path_node_t     *veco           = NULL;
        size_t           cnto           = strlen(path);
        
        retn = path_into(path, &veco, &cnto);
        for (int i = 0; i < cnto; i++) {
            assert(!memcmp(veco[i], vexp[i], sizeof(path_node_t)));
        }
    }

    {
        char            *pexp           = "user/Pass_w0rd";
        path_node_t      veci[]         = { "user//", "//Pass_w0rd/" };
        size_t           cnti           = sizeof(veci)/sizeof(path_node_t);
        
        retn = path_from(veci, cnti, &path);
        assert(!memcmp(path, pexp, strlen(pexp)));


        path_node_t      vexp[]         = { "user", "Pass_w0rd" };
        path_node_t     *veco           = NULL;
        size_t           cnto           = strlen(path);
        
        retn = path_into(path, &veco, &cnto);
        for (int i = 0; i < cnto; i++) {
            assert(!memcmp(veco[i], vexp[i], sizeof(path_node_t)));
        }
    }

    {
        char            *pexp           = "/user/Pass_w0rd";
        path_node_t      veci[]         = { "/user", "Pass_w0rd/" };
        size_t           cnti           = sizeof(veci)/sizeof(path_node_t);
        
        retn = path_from(veci, cnti, &path);
        assert(!memcmp(path, pexp, strlen(pexp)));


        path_node_t      vexp[]         = { "", "user", "Pass_w0rd" };
        path_node_t     *veco           = NULL;
        size_t           cnto           = strlen(path);
        
        retn = path_into(path, &veco, &cnto);
        for (int i = 0; i < cnto; i++) {
            assert(!memcmp(veco[i], vexp[i], sizeof(path_node_t)));
        }
    }

    {
        char            *pexp           = "/user/Pass_w0rd";
        path_node_t      veci[]         = { "/user//", "//Pass_w0rd/" };
        size_t           cnti           = sizeof(veci)/sizeof(path_node_t);
        
        retn = path_from(veci, cnti, &path);
        assert(!memcmp(path, pexp, strlen(pexp)));


        path_node_t      vexp[]         = { "", "user", "Pass_w0rd" };
        path_node_t     *veco           = NULL;
        size_t           cnto           = strlen(path);
        
        retn = path_into(path, &veco, &cnto);
        for (int i = 0; i < cnto; i++) {
            assert(!memcmp(veco[i], vexp[i], sizeof(path_node_t)));
        }
    }

    {
        char            *pexp           = "/user/Pass_w0rd";
        path_node_t      veci[]         = { "", "user", "Pass_w0rd" };
        size_t           cnti           = sizeof(veci)/sizeof(path_node_t);
        
        retn = path_from(veci, cnti, &path);
        assert(!memcmp(path, pexp, strlen(pexp)));


        path_node_t      vexp[]         = { "", "user", "Pass_w0rd" };
        path_node_t     *veco           = NULL;
        size_t           cnto           = strlen(path);
        
        retn = path_into(path, &veco, &cnto);
        for (int i = 0; i < cnto; i++) {
            assert(!memcmp(veco[i], vexp[i], sizeof(path_node_t)));
        }
    }

    {
        char            *pexp           = "/user/Pass_w0rd";
        path_node_t      veci[]         = { "/", "user", "Pass_w0rd/" };
        size_t           cnti           = sizeof(veci)/sizeof(path_node_t);
        
        retn = path_from(veci, cnti, &path);
        assert(!memcmp(path, pexp, strlen(pexp)));


        path_node_t      vexp[]         = { "", "user", "Pass_w0rd" };
        path_node_t     *veco           = NULL;
        size_t           cnto           = strlen(path);
        
        retn = path_into(path, &veco, &cnto);
        for (int i = 0; i < cnto; i++) {
            assert(!memcmp(veco[i], vexp[i], sizeof(path_node_t)));
        }
    }

    {
        char            *pexp           = "/user/Pass_w0rd";
        path_node_t      veci[]         = { "/", "/user/", "/Pass_w0rd/" };
        size_t           cnti           = sizeof(veci)/sizeof(path_node_t);
        
        retn = path_from(veci, cnti, &path);
        assert(!memcmp(path, pexp, strlen(pexp)));


        path_node_t      vexp[]         = { "", "user", "Pass_w0rd" };
        path_node_t     *veco           = NULL;
        size_t           cnto           = strlen(path);
        
        retn = path_into(path, &veco, &cnto);
        for (int i = 0; i < cnto; i++) {
            assert(!memcmp(veco[i], vexp[i], sizeof(path_node_t)));
        }
    }

    {
        char            *pexp           = "/user[5]/Pass_w0rd[4]";
        path_node_t      veci[]         = { "/", "/user[5]/", "/Pass_w0rd[4]/" };
        size_t           cnti           = sizeof(veci)/sizeof(path_node_t);
        
        retn = path_from(veci, cnti, &path);
        assert(!memcmp(path, pexp, strlen(pexp)));


        path_node_t      vexp[]         = { "", "user[5]", "Pass_w0rd[4]" };
        path_node_t     *veco           = NULL;
        size_t           cnto           = 0;

        retn = path_into(path, &veco, &cnto);
        for (int i = 0; i < cnto; i++) {
            assert(!memcmp(veco[i], vexp[i], sizeof(path_node_t)));
        }
    }

    {
        char            *pexp           = "/user[0xf0]/Pass_w0rd[0x55]";
        path_node_t      veci[]         = { "/", "/user[0xf0]/", "/Pass_w0rd[0x55]/" };
        size_t           cnti           = sizeof(veci)/sizeof(path_node_t);
        
        retn = path_from(veci, cnti, &path);
        printf("retn = %d, path = %s\n", retn, path);
        assert(!memcmp(path, pexp, strlen(pexp)));


        path_node_t      vexp[]         = { "", "user[0xf0]", "Pass_w0rd[0x55]" };
        path_node_t     *veco           = NULL;
        size_t           cnto           = 0;

        retn = path_into(path, &veco, &cnto);
        printf("retn = %d, path = %s\n", retn, path);
        for (int i = 0; i < cnto; i++) {
            assert(!memcmp(veco[i], vexp[i], sizeof(path_node_t)));
            printf("veco[%d] = %s\n", i, veco[i]);
        }
    }
}
