/** @file
---------------------------------------------------------------------------------
    Copyright ©2023-2025 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    数据 - JSON
---------------------------------------------------------------------------------
--*/
#include "json_prv.h"

/*
--------------------------------------------------------------------------------
by json_api.c
--------------------------------------------------------------------------------
*/

int
lsz_jarr_node_init (
    const  char        *path,
    lsz_jarr_node_t    *node
    )
{
    int                 r               = LSZ_RET_0_ERR;
    int                 scope           = 0;
    path_node_t         index;
    int                 is_all_space    = 0;

    if (!is_path_valid(path) || !node) {
        return LSZ_RET_E_ARG;
    }

    node->is_valid  = 0;
    node->is_empty  = 0;
    node->level     = 0;
    for (int i = 0, j = 0; (i < strlen(path) + 1) && (i < PATH_NAME_UNIT); i++) {
        if (!scope) {
            if (path[i] != '[') {
                node->name[i] = path[i];
                continue;
            } else {
                if (node->is_empty) {
                    printf("expect: %s\n", "path[x]..[x]..[x] or path[x]..[x]..[]");
                    r = LSZ_RET_E_ARG;
                    goto eofn;
                }
                node->name[i] = '\0';
                scope++;
                if (scope > 1) {
                    r = LSZ_RET_E_ARG;
                    goto eofn;
                }
                node->level++; // total of pair []

                is_all_space = 1;
            }
        } else {
            if (path[i] != ']') {
                index[j++] = path[i];
                if ((path[i] != ' ') && (path[i] != '\t')) {
                    is_all_space = 0;
                }
            } else {
                if (is_all_space) {
                    node->is_empty = 1;
                }

                index[j++] = '\0';
                scope--;
                if (scope < 0) {
                    r = LSZ_RET_E_ARG;
                    goto eofn;
                }
                // insert index of level
                if (node->level >= LSZ_JARR_LEVEL) {
                    r = LSZ_RET_E_ARG;
                    goto eofn;
                }
                node->index[node->level-1] = strtol(index, NULL, 0);
                // zero j for next index
                j = 0;
            }
        }
    }
    if (node->level) {
        node->is_valid = 1;
    }

eofn:
    return r;
}

int
is_path_of_lsz_jarr (
    char               *path,
    lsz_jarr_node_t    *node
    )
{
    int                 r     = 0;
    size_t              n     = 0;
    path_node_t        *vect  = NULL;
    size_t              cntv  = 0;

    r = path_into(path, &vect, &cntv);

    r = lsz_jarr_node_init (
            vect[cntv-1],
            node);
    if (vect) {
        free(vect);
    }

    return node->is_valid;
}

/*
--------------------------------------------------------------------------------
end!
--------------------------------------------------------------------------------
*/
