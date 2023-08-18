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

int
lsz_jobj_encode (
    char              **rest,
    json_t             *json,
    char               *path
    );

int
lsz_jarr_encode (
    char              **rest,
    json_t              json,
    char               *path
    );

/*
--------------------------------------------------------------------------------
impl
--------------------------------------------------------------------------------
*/

static
int
is_endof (
    char               *text
    )
{
    return (*text == '\0') || (*text == EOF);
}

static
int
is_space (
    char                this
    )
{
    return (this == ' ') || (this == '\t') || (this == '\r') || (this == '\n');
}

static
char *
skip_ws (
    char               *text
    )
{
    size_t              i;

    for (i = 0; is_space(text[i]); i++);

    return text + i;
}

/*
--------------------------------------------------------------------------------
*/

int
lsz_jtok_encode (
    char              **rest,
    char                token
    )
{
    int                 r       = LSZ_RET_0_ERR;
    char               *text    = *rest;

    text = skip_ws(text);

    if (text[0] != token) {
        r = LSZ_RET_E_FMT;
        goto eofn;
    } else {
        text++;
    }

    *rest = skip_ws(text);

eofn:
    return r;
}

/*
--------------------------------------------------------------------------------
*/

int
lsz_jkey_encode (
    char              **rest,
    char              **path
    )
{
    int                 r       = LSZ_RET_0_ERR;
    char               *text    = *rest;
    char               *s       = NULL;
    char               *e       = NULL;

    text = skip_ws(text);

    if (text[0] != '"') {
        r = LSZ_RET_E_FMT;
        goto eofn;
    }
    s = ++text;
    while(1) {
        while (!is_endof(text)) {
            if (*text == '"') {
                break;
            }
            text++;
        }
        if (is_endof(text)) {
            r = LSZ_RET_E_FMT;
            goto eofn;
        }
        if (text[-1] != '\\') {
            break;
        } else {
            text++;
        }
    };
    e = --text;

    *path = strndup(s, e - s + 1);
    *rest = skip_ws(text + 1 + 1);

eofn:
    return r;
}

int
lsz_jstr_encode (
    char              **rest,
    json_t              json,
    char               *path
    )
{
    int                 r       = LSZ_RET_0_ERR;
    char               *text    = *rest;
    char               *s       = NULL;
    char               *e       = NULL;
    char               *data    = NULL;

    text = skip_ws(text);

    if (text[0] != '"') {
        r = LSZ_RET_E_FMT;
        goto eofn;
    }
    s = ++text;
    while(1) {
        while (!is_endof(text)) {
            if (*text == '"') {
                break;
            }
            text++;
        }
        if (is_endof(text)) {
            r = LSZ_RET_E_FMT;
            goto eofn;
        }
        if (text[-1] != '\\') {
            break;
        } else {
            text++;
        }
    };
    e = --text;

    data = strndup(s, e - s + 1);
    r = jstr_add(json, path, data);
    if (r) {
        goto eofn;
    }

    *rest = skip_ws(text + 1 + 1);

eofn:
    if (data) {
        free (data);
        data = NULL;
    }
    return r;
}

int
lsz_jnul_encode (
    char              **rest,
    json_t              json,
    char               *path
    )
{
    int                 r       = LSZ_RET_0_ERR;
    char               *text    = *rest;

    text = skip_ws(text);

    if (memcmp(text, "null" , 4) != 0) {
        r = LSZ_RET_E_FMT;
        goto eofn;
    }

    r = jnul_add(json, path);
    text += 4;
    if (r) {
        goto eofn;
    }

    *rest = skip_ws(text);

eofn:
    return r;
}

int
lsz_jboo_encode (
    char              **rest,
    json_t              json,
    char               *path
    )
{
    int                 r       = LSZ_RET_0_ERR;
    char               *text    = *rest;

    text = skip_ws(text);

    if ((memcmp(text, "true", 4) != 0) && (memcmp(text, "false", 5) != 0)) {
        r = LSZ_RET_E_FMT;
        goto eofn;
    }
    if (text[0] == 't') {
        r = jboo_add(json, path, 1);
        text += 4;
    } else {
        r = jboo_add(json, path, 0);
        text += 5;
    }
    if (r) {
        goto eofn;
    }

    *rest = skip_ws(text);

eofn:
    return r;
}

int
lsz_jnum_encode (
    char              **rest,
    json_t              json,
    char               *path
    )
{
    int                 r       = LSZ_RET_0_ERR;
    char               *text    = *rest;
    char               *e       = NULL;
    long                rslt    = 0;

    text = skip_ws(text);

    if ((text[0] == '0') && (text[1] == 'x')) {
        rslt = strtol(text, &e, 16);
    } else {
        rslt = strtol(text, &e, 10);
    }
    if (e == text || is_endof(e)) {
        r = LSZ_RET_E_FMT;
        goto eofn;
    }
    r = jnum_add(json, path, rslt);
    if (r) {
        goto eofn;
    }

    *rest = skip_ws(e);

eofn:
    return r;
}

/*
--------------------------------------------------------------------------------
*/

int
lsz_jval_encode (
    char              **rest,
    json_t              json,
    char               *path,
    lsz_json_type_t    *type
    )
{
    int                 r       = LSZ_RET_0_ERR;

    *rest = skip_ws(*rest);

    switch ((*rest)[0]) {
    case '{':
        if (type) *type = json_t_obj;
        r = lsz_jobj_encode(rest,&json, path);
        break;
    case '[':
        if (type) *type = json_t_arr;
        r = lsz_jarr_encode(rest, json, path);
        break;
    case '"':
        if (type) *type = json_t_str;
        r = lsz_jstr_encode(rest, json, path);
        break;
    case 'n':
        if (type) *type = json_t_nul;
        r = lsz_jnul_encode(rest, json, path);
        break;
    case 'f':
    case 't':
        if (type) *type = json_t_boo;
        r = lsz_jboo_encode(rest, json, path);
        break;
    case '+':
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        if (type) *type = json_t_num;
        r = lsz_jnum_encode(rest, json, path);
        break;
    default:
        if (type) *type = json_t_non;
        if (is_endof(*rest)) {
            r = LSZ_RET_E_FMT;
            goto eofn;
        }
        break;
    }

    *rest = skip_ws(*rest);

eofn:
    return r;
}

/*
--------------------------------------------------------------------------------
*/

int
lsz_jobj_encode (
    char              **rest,
    json_t             *json,
    char               *path
    )
{
    int                 r       = LSZ_RET_0_ERR;
    char               *text    = *rest;
    json_t              this    = json_new();
    char               *name    = NULL;

    if ((!*json && path) || (*json && !path)) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    r = lsz_jtok_encode(&text, '{');
    if (r) {
        r = LSZ_RET_E_FMT;
        goto eofn;
    }
    if (text[0] == '}') {
        r = LSZ_RET_0_ERR;
        goto eofn;
    }
    if (is_endof(text)) {
        r = LSZ_RET_E_FMT;
        goto eofn;
    }
    while (1) {
        r = lsz_jkey_encode(&text, &name);
        if (r) {
            r = LSZ_RET_E_FMT;
            goto eofn;
        }
        r = lsz_jtok_encode(&text, ':');
        if (r) {
            r = LSZ_RET_E_FMT;
            goto eofn;
        }
        r = lsz_jval_encode(&text, this, name, NULL);
        if (r) {
            r = LSZ_RET_E_FMT;
            goto eofn;
        }
        if (name) {
            free (name);
            name = NULL;
        }
        r = lsz_jtok_encode(&text, ',');
        if (r != LSZ_RET_0_ERR) {
            break;
        }
    };
    r = lsz_jtok_encode(&text, '}');
    if (r) {
        r = LSZ_RET_E_FMT;
        goto eofn;
    }
    if (*json) {
        r = jobj_add(*json, path, this);
    } else {
        *json = this;
    }
    if (r) {
        goto eofn;
    }
    *rest = text;

eofn:
    if (r) {
        if (name) {
            free (name);
            name = NULL;
        }
        if (this) {
            json_free(this);
            this = NULL;
        }
    }
    return r;
}

int
lsz_jarr_encode (
    char              **rest,
    json_t              json,
    char               *path
    )
{
    int                 r       = LSZ_RET_0_ERR;
    char               *text    = *rest;
    char               *name    = path;
    lsz_json_type_t     type_a  = json_t_non;
    lsz_json_type_t     type_b  = json_t_non;
    size_t              i       = strlen(path);
    size_t              j       = 3; // "[0]"
    char                *end    = NULL;

    if (i + j + 1 >= PATH_NAME_UNIT) {
        return LSZ_RET_E_ARG;
    }
    path = calloc(1, PATH_NAME_UNIT);
    strcat(path, name);
    strcat(path,"[0]");

    r = lsz_jtok_encode(&text, '[');
    if (r) {
        r = LSZ_RET_E_FMT;
        goto eofn;
    }
    if (text[0] == ']') {
        r = LSZ_RET_0_ERR;
        goto eofn;
    }
    if (is_endof(text)) {
        r = LSZ_RET_E_FMT;
        goto eofn;
    }
    while (1) {
        r = lsz_jval_encode(&text, json, path, &type_b);
        if (r) {
            r = LSZ_RET_E_FMT;
            goto eofn;
        }
        if (type_a == json_t_non) {
            type_a = type_b;
        } else {
            if (type_a != type_b) {
                r = LSZ_RET_E_FMT;
                goto eofn;
            }
        }
        r = lsz_jtok_encode(&text, ',');
        if (r != LSZ_RET_0_ERR) {
            break;
        }
        // index++
        j = snprintf(&path[i], PATH_NAME_UNIT - i, "[%ld]", strtol(&path[i+1], &end, 10) + 1);
        if (i + j + 1 >= PATH_NAME_UNIT) {
            r = LSZ_RET_E_OUT;
            goto eofn;
        }
    };
    r = lsz_jtok_encode(&text, ']');
    if (r) {
        r = LSZ_RET_E_FMT;
        goto eofn;
    }
    *rest = text;

eofn:
    if (path) {
        free (path);
    }
    return r;
}

/*
--------------------------------------------------------------------------------
apis
--------------------------------------------------------------------------------
*/

json_t
json_encode (
    char               *text
    )
{
    int                 r       = LSZ_RET_0_ERR;
    json_t              json    = NULL;

    if (!text) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    r = lsz_jobj_encode(&text, &json, NULL);
    if (r) {
        goto eofn;
    }

eofn:
    if (r) {
        if (json) json_free(json);
    }
    return json;
}

/*
--------------------------------------------------------------------------------
end!
--------------------------------------------------------------------------------
*/
