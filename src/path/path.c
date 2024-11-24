/** @file
---------------------------------------------------------------------------------
    Copyright ©2023-2024 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    数据结构 - 路径
---------------------------------------------------------------------------------
--*/

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "base.h"
#include "path.h"

char *path_normalize(const char *src, size_t len)
{
    if (!src) {
        return NULL;
    }

    if (!len) {
        len = strlen(src);
    }

    char *dst = calloc(1, len + 1);
    if (!dst) {
        return NULL;
    }

    int t = 0;
    for (int i = 0; i < len; i++) {
        if ((i > 0) && (src[i-1] == '/') && (src[i] == '/')) {
            continue;
        }
        dst[t++] = src[i];
    }

    return dst;
}

bool is_path_end_node(const char *path, size_t len, int *offset)
{
    int i = 0;

    if (!path || !len || (*offset >= len)) {
        return true;
    }

    for (i = *offset; i < len; i++) {
        if (path[i] == '/') {
            break;
        }
    }
    *offset = i;

    if (*offset < len - 1) {
        return false;
    }

    return true;
}

bool is_path_valid(const char *path)
{
    size_t path_len = strlen(path);

    if (!path || !path_len) {
        return false;
    }

    char * path_nor = path_normalize(path, path_len);
    char * node_nor = path_nor;
    int    in_pair  = 0;
    int    is_xnum  = 0;
    int    do_once  = 0;
    int    s, e, i, t, p;

    for (i = 0; i < path_len; i++) {
        s = i;
        is_path_end_node(path_nor, path_len, &i);
        e = i;
        node_nor = &path_nor[s];
        p = 0;
        for (t = 0; t < e - s; t++) {
            if (!in_pair) {
                if (isalnum(node_nor[t]) || (node_nor[t] == '_')) continue;
                if (node_nor[t] == '[') {
                    in_pair++;
                    if (in_pair > 1) {
                        printf ("err: adjacent '['\n");
                        goto eofn;
                    }
                } else {
                    goto eofn;
                }
            } else {
                if (!do_once) {
                    do_once++;
                    if ((e - s - t > 2)
                    &&  (node_nor[t] == '0') && (node_nor[t + 1] == 'x')) {
                        is_xnum++;
                        t = t + 1;  // skip leading "0x"
                        continue;
                    }
                    if (do_once > 1) {
                        printf ("err: more than 1.\n");
                        goto eofn;
                    }
                }
                if (is_xnum) {
                    if (isxdigit(node_nor[t])) continue;
                } else {
                    if ( isdigit(node_nor[t])) continue;
                }
                if (node_nor[t] == ']') {
                    in_pair--;
                    if (in_pair < 0) {
                        printf ("err: adjacent ']'\n");
                        goto eofn;
                    }
                    do_once--;
                    if (in_pair < 0) {
                        printf ("err: more than 0.\n");
                        goto eofn;
                    }
                    p = t;
                    is_xnum--;
                } else {
                    goto eofn;
                }
            }
        }
        if (in_pair) {
            printf ("err: expect but not in pair '[]'\n");
            goto eofn;
        }
        if ((p) && (p != e - s - 1)) {
            printf ("err: ']' shall be end of string\n");
            goto eofn;
        }
    }
    if (path_nor) {
        free(path_nor);
    }

    return true;

eofn:
    printf("path fail at node: ");
    for (i = 0; i < e - s; i++) {
        printf("%c", node_nor[i]);
    }
    printf(" (at [%d] char '%c')\n", t, node_nor[t]);
    if (path_nor) {
        free(path_nor);
    }

    return false;
}

size_t path_node_count(const char *path)
{
    size_t path_len = 0;
    size_t node_cnt = 0;

    if (!is_path_valid(path)) {
        return 0;
    }

    path_len = strlen(path);

    for (int i = 0; i < path_len; i++) {
        if (!is_path_end_node(path, path_len, &i)) {
            node_cnt++;
        }
    }

    return  ++node_cnt;
}

int path_from(const path_node_t *src_node, size_t src_len, char **dst_path)
{
    //
    // args's check
    //
    if (!src_node || !src_len | !dst_path) {
        return LSZ_RET_E_ARG;
    }
    //
    // calculate path len 
    //
    size_t  path_len = 0;
    bool    has_root = 0;
    for (int i = 0; i < src_len; i++) {
        size_t l = strlen((char *) src_node[i]);
        if (l > sizeof(path_node_t)) {
            assert(!!0);
            return LSZ_RET_E_SEC;
        }
        if (i == 0) {
            if (l == 0) {
                path_len += 1;
                has_root  = 1;
            } else {
                path_len += l;
            }
        } else {
            if ((i != 1) || (!has_root)) {
                path_len += 1;
            }
            if (l) {
                path_len += l;
            }
        }
    }
    //
    // calculate path str
    //
    char *path = calloc(1, path_len+1);
    if (!path) {
        assert(!!0);
        return LSZ_RET_E_OUT;
    }
    for (int i = 0; i < src_len; i++) {
        size_t l = strlen((char *) src_node[i]);
        if (i == 0) {
            if (l == 0) {
                strcat(path, "/");
            } else {
                strcat(path, (char *) src_node[i]);
            }
        } else {
            if ((i != 1) || (!has_root)) {
                strcat(path, "/");
            }
            if (l) {
                strcat(path, (char *) src_node[i]);
            }
        }
    }
    //
    // normalize
    //
    *dst_path = path_normalize(path, path_len);
    if (path) free(path);

    if (!is_path_valid(*dst_path)) {
        if (*dst_path) {
            free (*dst_path);
            *dst_path = NULL;
        }
        return LSZ_RET_E_ARG;
    }

    return LSZ_RET_0_ERR;
}

int path_into(const char *src_path, path_node_t **dst_node, size_t *dst_len)
{
    //
    // args's check
    //
    if (!src_path || !dst_node || !dst_len) {
        return LSZ_RET_E_ARG;
    }
    //
    // normalize
    //
    size_t path_len = strlen(src_path);
    char * path_nor = NULL;

    path_nor = path_normalize(src_path, path_len);
    if (!path_nor) {
        return LSZ_RET_E_OUT;
    }

    if (!is_path_valid(path_nor)) {
        if (path_nor) {
            free (path_nor);
            path_nor = NULL;
        }
        return LSZ_RET_E_ARG;
    }
    //
    // calculate node cnt
    //
    size_t node_cnt = path_node_count(path_nor);

    *dst_len = node_cnt;
    if (!node_cnt) {
        return LSZ_RET_E_ARG;
    }
    //
    // calculate node vec
    //
    path_node_t *node_vec = calloc(node_cnt, sizeof(path_node_t));

    *dst_node = node_vec;
    if (!node_vec) {
        return LSZ_RET_E_OUT;
    }

    int s;
    int e;
    for (int i = 0; i < path_len; i++) {
        s = i;
        is_path_end_node(path_nor, path_len, &i);
        e = i;
        memcpy(node_vec, &path_nor[s], e - s);
        // next
        node_vec++;
    }

    if (path_nor) {
        free(path_nor);
    }

    return LSZ_RET_0_ERR;
}
