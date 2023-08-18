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
apis
--------------------------------------------------------------------------------
*/

int
jboo_add (
    json_t              json,
    char               *path,
    long                data
    )
{
    int                 r = LSZ_RET_0_ERR;
    lsz_json_t          unit;

    unit.type = json_t_boo;
    unit.data.num   = data;

    r = json_add(json, path, &unit);
    if (r) {
        goto eofn;
    }

eofn:
    return r;
}

int
jnum_add (
    json_t              json,
    char               *path,
    long                data
    )
{
    int                 r = LSZ_RET_0_ERR;
    lsz_json_t          unit;

    unit.type = json_t_num;
    unit.data.num   = data;

    r = json_add(json, path, &unit);
    if (r) {
        goto eofn;
    }

eofn:
    return r;
}

int
jnul_add (
    json_t              json,
    char               *path
    )
{
    int                 r = LSZ_RET_0_ERR;
    lsz_json_t          unit;

    unit.type = json_t_nul;
    unit.data.str   = NULL;

    r = json_add(json, path, &unit);
    if (r) {
        goto eofn;
    }

eofn:
    return r;
}

int
jstr_add (
    json_t              json,
    char               *path,
    char               *data
    )
{
    int                 r = LSZ_RET_0_ERR;
    lsz_json_t          unit;

    unit.type = json_t_str;
    unit.data.str   = data;

    r = json_add(json, path, &unit);
    if (r) {
        goto eofn;
    }

eofn:
    return r;
}

int
jobj_add (
    json_t              json,
    char               *path,
    json_t              data
    )
{
    int                 r    = LSZ_RET_0_ERR;
    lsz_json_t         *unit = data ? data : json_new();

    if (!unit) {
        r = LSZ_RET_E_OUT;
        goto eofn;
    }
    if (unit->signature != LSZ_JSON_SIGNATURE) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }
    r = json_add(json, path, unit);
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
jboo_get (
    json_t              json,
    char               *path,
    long               *data
    )
{
    int                 r    = LSZ_RET_0_ERR;
    lsz_json_t         *unit = NULL;

    r = json_get(json, path, &unit);
    if (r) {
        goto eofn;
    }
    if (unit->type != json_t_boo) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    *data = unit->data.num;

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
    int                 r    = LSZ_RET_0_ERR;
    lsz_json_t         *unit = NULL;

    r = json_get(json, path, &unit);
    if (r) {
        goto eofn;
    }
    if (unit->type != json_t_num) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    *data = unit->data.num;

eofn:
    return r;
}

int
jnul_get (
    json_t              json,
    char               *path
    )
{
    int                 r    = LSZ_RET_0_ERR;
    lsz_json_t         *unit = NULL;

    r = json_get(json, path, &unit);
    if (r) {
        goto eofn;
    }
    if (unit->type != json_t_nul) {
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
    int                 r    = LSZ_RET_0_ERR;
    lsz_json_t         *unit = NULL;

    r = json_get(json, path, &unit);
    if (r) {
        goto eofn;
    }
    if (unit->type != json_t_str) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    *data = strdup(unit->data.str); // free by caller

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
    int                 r    = LSZ_RET_0_ERR;
    lsz_json_t         *unit = NULL;

    r = json_get(json, path, &unit);
    if (r) {
        goto eofn;
    }
    if (unit->type != json_t_obj) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    *data = unit; // note: it is not a copy of json_t

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
    lsz_json_t          unit;

    unit.type = json_t_boo;
    unit.data.num   = data;

    r = json_set(json, path, &unit);
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
    lsz_json_t          unit;

    unit.type = json_t_num;
    unit.data.num   = data;

    r = json_set(json, path, &unit);
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
    lsz_json_t          unit;

    unit.type = json_t_nul;
    unit.data.str   = NULL;

    r = json_set(json, path, &unit);
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
    lsz_json_t          unit;

    unit.type = json_t_str;
    unit.data.str   = data;

    r = json_set(json, path, &unit);
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
    int                 r    = LSZ_RET_0_ERR;
    lsz_json_t         *unit = data ? data : json_new();

    if (!unit) {
        r = LSZ_RET_E_OUT;
        goto eofn;
    }
    if (unit->signature != LSZ_JSON_SIGNATURE) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }
    r = json_set(json, path, unit);
    if (r) {
        goto eofn;
    }

eofn:
    return r;
}

/*
--------------------------------------------------------------------------------
end!
--------------------------------------------------------------------------------
*/
