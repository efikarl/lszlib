/** @file
---------------------------------------------------------------------------------
    Copyright ©2023-2025 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    数据 - 路径
---------------------------------------------------------------------------------
--*/
#include "lszlib.h"

char *path_normalize(char const *src, size_t len)
{
    if (!src) {
        return NULL;
    }

    if (!len) {
        len = strlen(src);
    }

    int cnt = 0;
    for (int i = 0; i < len; i++) {
        if ((i > 0) && (src[i-1] == '/') && (src[i] == '/')) {
            continue;
        }
        cnt++;
    }

    char *dst = calloc(1, cnt + 1);
    if (!dst) {
        return NULL;
    }

    for (int i = 0, j = 0; i < len; i++) {
        if ((i > 0) && (src[i-1] == '/') && (src[i] == '/')) {
            continue;
        }
        dst[j++] = src[i];
    }

    return dst;
}

int is_path_end_node(char const *path, size_t len, int *offset, int *end)
{
    int i = 0;

    if (!path || !len || !offset || (*offset >= len)) {
        return 1;
    }

    while (
        path[*offset]
        && 
        path[*offset] == '/'
    ) {
        (*offset)++;
    }

    for (i = *offset; i < len; i++) {
        if (path[i] == '/') {
            break;
        }
    }
    if (end) {
       *end = i;
    }

    if (i < len) {
        return 0;
    } else {
        return 1;
    }
}

int is_path_valid(char const *path)
{
    size_t path_len = strlen(path);

    if (!path || !path_len) {
        return 0;
    }

    char * nor_path = path_normalize(path, path_len);
    char * nor_node = nor_path;
    int    in_pair  = 0;
    int    is_xnum  = 0;
    int    do_once  = 0;
    int    s, e, i, t, p;

    for (i = 0; i < path_len; i++) {
        s = i;
        is_path_end_node(nor_path, path_len, &s, &e);
        i = e;
        nor_node = &nor_path[s];
        p = 0;
        for (t = 0; t < e - s; t++) {
            if (!in_pair) {
                if (isalnum(nor_node[t]) || (nor_node[t] == '_') || (nor_node[t] == '-') || (nor_node[t] == '.')) continue;
                if (nor_node[t] == '[') {
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
                    &&  (nor_node[t] == '0') && (nor_node[t + 1] == 'x')) {
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
                    if (isxdigit(nor_node[t])) continue;
                } else {
                    if ( isdigit(nor_node[t])) continue;
                }
                if (nor_node[t] == ']') {
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
    if (nor_path) {
        free(nor_path);
    }

    return 1;

eofn:
    printf("path fail at node: ");
    for (i = 0; i < e - s; i++) {
        printf("%c", nor_node[i]);
    }
    printf(" (at [%d] char '%c')\n", t, nor_node[t]);
    if (nor_path) {
        free(nor_path);
    }

    return 0;
}

size_t path_node_count(char const *path)
{
    size_t path_len = 0;
    size_t node_cnt = 0;

    if (!is_path_valid(path)) {
        return 0;
    }

    path_len = strlen(path);

    if (path[0] == '/') {
        node_cnt++;
    }
    for (int i = 0; i < path_len; i++) {
        if (!is_path_end_node(path, path_len, &i, &i)) {
            node_cnt++;
        }
    }

    return  ++node_cnt;
}

int path_from(path_node_t const *src_node, size_t src_len, char **dst_path)
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
    int     has_root = 0;
    for (int i = 0; i < src_len; i++) {
        size_t l = strlen((char *) src_node[i]);
        if (l > sizeof(path_node_t)) {
            assert(!!0);
            return LSZ_RET_E_SEC;
        }
        path_len += l + 1;
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
        strcat(path, (char *) src_node[i]);
        strcat(path, "/");
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

int path_into(char const *src_path, path_node_t **dst_node, size_t *dst_len)
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
    size_t       path_len = strlen(src_path);
    char *       nor_path = NULL;
    size_t       node_cnt = 0;
    path_node_t *nor_node = NULL;

    nor_path = path_normalize(src_path, path_len);
    if (!nor_path) {
        return LSZ_RET_E_OUT;
    }

    if (!is_path_valid(nor_path)) {
        if (nor_path) {
            free (nor_path);
            nor_path = NULL;
        }
        return LSZ_RET_E_ARG;
    }
    //
    // calculate node cnt
    //
    node_cnt = path_node_count(nor_path);
    if (!node_cnt) {
        return LSZ_RET_E_ARG;
    }
    *dst_len = node_cnt;
    //
    // calculate node vec
    //
    nor_node = calloc(node_cnt, sizeof(path_node_t));
    if (!nor_node) {
        return LSZ_RET_E_OUT;
    }
    *dst_node = nor_node;

    if (nor_path[0] == '/') {
        strcpy(nor_node[0], "/");
        nor_node++;
    }
    int s;
    int e;
    for (int i = 0; i < path_len; i++) {
        s = i;
        is_path_end_node(nor_path, path_len, &s, &e);
        i = e;
        memcpy(nor_node, &nor_path[s], e - s);
        nor_node++;
    }

    if (nor_path) {
        free(nor_path);
    }

    return LSZ_RET_0_ERR;
}
