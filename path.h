/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    数据结构 - 路径
---------------------------------------------------------------------------------
--*/

#ifndef LSZ_PATH
#define LSZ_PATH
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PATH_NODE_UNIT  64
#define PATH_NAME_UNIT  64

typedef char path_node_t[PATH_NODE_UNIT];

char *path_normalize (
    const char          *src,
    size_t              len
);

bool is_path_valid (
    const char          *path
);

int path_from(const path_node_t *src_node, size_t  src_len, char **dst_path);
int path_into(const char *src_path, path_node_t **dst_node, size_t *dst_len);

#endif
