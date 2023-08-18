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

#ifndef LSZ_JSON_PRV_H_
#define LSZ_JSON_PRV_H_

#include "lszlib.h"

/*
--------------------------------------------------------------------------------
type (json)
--------------------------------------------------------------------------------
*/

typedef struct _lsz_json_t lsz_json_t;

typedef enum   _lsz_json_type_t {
    json_t_non = 0,
    json_t_boo,
    json_t_num,
    json_t_nul,
    json_t_str,
    json_t_obj,
    json_t_arr,
    json_t_end,
} lsz_json_type_t;

typedef struct _lsz_json_child_obj_t {
    lsz_list_t          list;
} lsz_json_child_obj_t;

typedef struct _lsz_json_child_arr_t {
    lsz_list_t          list;
} lsz_json_child_arr_t;

typedef union  _lsz_json_data_t {
    long                 num;
    char                *str;
    lsz_json_child_obj_t obj;
    lsz_json_child_arr_t arr;
} lsz_json_data_t;

#define LSZ_JSON_SIGNATURE    ('j' << 0 | 's' << 8 | 'o' << 16 | 'n' << 24)
struct _lsz_json_t {
    uint32_t            signature;
    lsz_json_type_t     type;
    char               *name;
    lsz_json_data_t     data;
    lsz_list_t          link;
};

#define LSZ_JSON_ROOT   "/"
#define LSZ_JSON_BFSZ   256

json_t
json_new (
    void
    );

int
json_add (
    json_t              json,
    char               *path,
    lsz_json_t         *unit
    );

int
json_get (
    json_t              json,
    char               *path,
    lsz_json_t        **unit
    );

int
json_set (
    json_t              json,
    char               *path,
    lsz_json_t         *unit
    );

int
json_del (
    json_t              json,
    char               *path
    );

int
json_free (
    json_t              json
    );

/*
--------------------------------------------------------------------------------
type (node)
--------------------------------------------------------------------------------
*/

#define LSZ_JARR_LEVEL  32

typedef struct {
    path_node_t         name;
    int                 is_valid;
    int                 is_empty;
    size_t              level;
    size_t              index[LSZ_JARR_LEVEL];
} lsz_jarr_node_t;

int
lsz_jarr_node_init (
    const  char        *vect,
    lsz_jarr_node_t    *path
    );

int
is_path_of_lsz_jarr (
    char               *path,
    lsz_jarr_node_t    *node
    );

#endif
