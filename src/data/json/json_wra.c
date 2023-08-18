/** @file
---------------------------------------------------------------------------------
    Copyright ©2023-2024 Liu Yi, efikarl@yeah.net

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
apis
--------------------------------------------------------------------------------
*/

int
jboo_get (
    json_t              json,
    char               *path,
    long               *data
    )
{
    int                 r = LSZ_RET_0_ERR;
    lsz_json_type_t     type0;
    lsz_json_data_t     data0;

    r = json_get(json, path, &type0, &data0);
    if (r) {
        goto eofn;
    }
    if (type0 != json_t_boo) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    *data = data0.num;

eofn:
    return r;
}

int
jnum_get (
    json_t              json,
    char               *path,
    long               *data
    )
{
    int                 r = LSZ_RET_0_ERR;
    lsz_json_type_t     type0;
    lsz_json_data_t     data0;

    r = json_get(json, path, &type0, &data0);
    if (r) {
        goto eofn;
    }
    if (type0 != json_t_num) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    *data = data0.num;

eofn:
    return r;
}

int
jnul_get (
    json_t              json,
    char               *path
    )
{
    int                 r = LSZ_RET_0_ERR;
    lsz_json_type_t     type0;
    lsz_json_data_t     data0;

    r = json_get(json, path, &type0, &data0);
    if (r) {
        goto eofn;
    }
    if (type0 != json_t_nul) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

eofn:
    return r;
}

int
jstr_get (
    json_t              json,
    char               *path,
    char              **data
    )
{
    int                 r = LSZ_RET_0_ERR;
    lsz_json_type_t     type0;
    lsz_json_data_t     data0;

    r = json_get(json, path, &type0, &data0);
    if (r) {
        goto eofn;
    }
    if (type0 != json_t_str) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    *data = strdup(data0.str); // free by caller

eofn:
    return r;
}

int
jobj_get (
    json_t              json,
    char               *path,
    json_t             *data
    )
{
    int                 r = LSZ_RET_0_ERR;
    lsz_json_type_t     type0;
    lsz_json_data_t     data0;

    r = json_get(json, path, &type0, &data0);
    if (r) {
        goto eofn;
    }
    if (type0 != json_t_obj) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    *data = data0.obj.json; // note: it is object itself (others is just a copy)

eofn:
    return r;
}

/*
--------------------------------------------------------------------------------
*/

int
jboo_set (
    json_t              json,
    char               *path,
    long                data
    )
{
    int                 r = LSZ_RET_0_ERR;
    lsz_json_data_t     data0 = { .num = data };

    r = json_set(json, path, json_t_boo, data0);
    if (r) {
        goto eofn;
    }

eofn:
    return r;
}

int
jnum_set (
    json_t              json,
    char               *path,
    long                data
    )
{
    int                 r = LSZ_RET_0_ERR;
    lsz_json_data_t     data0 = { .num = data };

    r = json_set(json, path, json_t_num, data0);
    if (r) {
        goto eofn;
    }

eofn:
    return r;
}

int
jnul_set (
    json_t              json,
    char               *path
    )
{
    int                 r = LSZ_RET_0_ERR;
    lsz_json_data_t     data0;

    r = json_set(json, path, json_t_nul, data0);
    if (r) {
        goto eofn;
    }

eofn:
    return r;
}

int
jstr_set (
    json_t              json,
    char               *path,
    char               *data
    )
{
    int                 r = LSZ_RET_0_ERR;
    lsz_json_data_t     data0 = { .str = data };

    r = json_set(json, path, json_t_str, data0);
    if (r) {
        goto eofn;
    }

eofn:
    return r;
}

int
jobj_set (
    json_t              json,
    char               *path,
    json_t              data
    )
{
    int                 r = LSZ_RET_0_ERR;
    lsz_json_data_t     data0 = { .obj.json = data };

    r = json_set(json, path, json_t_obj, data0);
    if (r) {
        goto eofn;
    }

eofn:
    return r;
}

/*
--------------------------------------------------------------------------------
*/

int
jboo_add (
    json_t              json,
    char               *path,
    long                data
    )
{
    lsz_json_data_t     data0;

    data0.num = (data != 0);
    return json_add(json, path, json_t_boo, &data0);
}

int
jnum_add (
    json_t              json,
    char               *path,
    long                data
    )
{
    lsz_json_data_t     data0;

    data0.num = data;
    return json_add(json, path, json_t_num, &data0);
}

int
jnul_add (
    json_t              json,
    char               *path
    )
{
    return json_add(json, path, json_t_nul, NULL);
}

int
jstr_add (
    json_t              json,
    char               *path,
    char               *data
    )
{
    lsz_json_data_t     data0;

    data0.str = data;
    return json_add(json, path, json_t_str, &data0);
}

int
jobj_add (
    json_t              json,
    char               *path,
    json_t             *data
    )
{
    lsz_json_data_t     data0;

    data0.obj.json = data;
    return json_add(json, path, json_t_obj, &data0);
}

/*
--------------------------------------------------------------------------------
end!
--------------------------------------------------------------------------------
*/
