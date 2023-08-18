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
type
--------------------------------------------------------------------------------
*/

typedef struct {
    char *              head;
    char *              tail;
    size_t              size;
} lsz_text_t;

/*
--------------------------------------------------------------------------------
impl
--------------------------------------------------------------------------------
*/

static
int
lsz_txtcatf (
    lsz_text_t         *text,
    size_t              bfsz,
    const      char    *format,
                        ...
    )
{
    int                 r   = LSZ_RET_0_ERR;
    char               *dst = NULL;
    char               *src = NULL;
    char               *new = NULL; 
    size_t              dst_strsz = 0;
    size_t              dst_bufsz = 0;
    size_t              src_strsz = 0;
    size_t              new_strsz = 0;
    size_t              new_bufsz = 0;

    if ((!text) || (text->tail < text->head) || (!bfsz)) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    dst       = text->head;
    dst_bufsz = text->size;
    dst_strsz = text->tail - text->head;

    src = calloc(1, bfsz + 1);
    if (!src) {
        r = LSZ_RET_E_OUT;
        goto eofn;
    }
    va_list args; va_start(args, format); src_strsz = vsnprintf(src, bfsz + 1, format, args); va_end(args);
    if (!src_strsz) {
        r = LSZ_RET_E_OUT;
        goto eofn;
    }

    new_strsz = dst_strsz + src_strsz + 1;
    if (dst_bufsz < new_strsz) {
        new_bufsz = lsz_p_align_bit(new_strsz, 8);
        new = calloc(1, new_bufsz);
        if (!new) {
            r = LSZ_RET_E_OUT;
            goto eofn;
        }
        strcat(new, dst);
        strcat(new, src);
        if (dst) {
            free (dst);
            dst = NULL;
        }
        text->head = new;
        text->tail = new + dst_strsz + src_strsz;
        text->size = new_bufsz;
    } else {
        strcat(dst, src);
        text->tail+= src_strsz;
    }

eofn:
    if (src) {
        free (src);
    }
    return r;
}

/*
--------------------------------------------------------------------------------
*/

void
lsz_jobj_decode (
    lsz_json_t         *json,
    lsz_text_t         *text
    );

void
lsz_jarr_decode (
    lsz_json_t         *json,
    lsz_text_t         *text
    );

char *
lsz_json_decode (
    lsz_json_t         *json
    )
{
    char               *rslt = NULL;
    lsz_text_t          text;

    if ((!json) || (json->signature != LSZ_JSON_SIGNATURE)) {
        goto eofn;
    }

    text.head = calloc(1, LSZ_JSON_BFSZ);
    text.tail = text.head;
    text.size = 0;
    if (!text.head) {
        goto eofn;
    }
    lsz_jobj_decode(json, &text);
    rslt = strdup(text.head);

eofn:
    if (text.head) {
        free(text.head);
    }
    return rslt;
}

void
lsz_jobj_decode (
    lsz_json_t         *json,
    lsz_text_t         *text
    )
{
    int                 r    = LSZ_RET_0_ERR;
    lsz_list_t         *link = NULL;
    lsz_json_t         *unit = NULL;

    lsz_txtcatf(text, 1, "{");
    for (link = json->data.obj.list.next; link != &json->data.obj.list; link = link->next) {
        unit = base_of(link, lsz_json_t, link);
        if ((!unit) || (unit->signature != LSZ_JSON_SIGNATURE)) {
            assert(0);
        }
        switch (unit->type) {
        case json_t_boo:
            lsz_txtcatf(text, LSZ_JSON_BFSZ,
                "\"%s\":%s",
                unit->name, unit->data.num ? "true" : "false");
            break;
        case json_t_num:
            lsz_txtcatf(text, LSZ_JSON_BFSZ,
                "\"%s\":%d",
                unit->name, unit->data.num);
            break;
        case json_t_nul:
            lsz_txtcatf(text, LSZ_JSON_BFSZ,
                "\"%s\":%s",
                unit->name, "null");
            break;
        case json_t_str:
            lsz_txtcatf(text, LSZ_JSON_BFSZ,
                "\"%s\":\"%s\"",
                unit->name, unit->data.str);
            break;
        case json_t_obj:
            lsz_txtcatf(text, LSZ_JSON_BFSZ, "\"%s\":", unit->name);
            lsz_jobj_decode(unit, text);
            break;
        case json_t_arr:
            lsz_txtcatf(text, LSZ_JSON_BFSZ, "\"%s\":", unit->name);
            lsz_jarr_decode(unit, text);
            break;
        default:
            r = LSZ_RET_E_ARG;
            break;
        }
        if (link->next != &json->data.obj.list) {
            // if not end of member, append ','
            lsz_txtcatf(text, 1, ",");
        }
    }
    lsz_txtcatf(text, 1, "}");
}

void
lsz_jarr_decode (
    lsz_json_t         *json,
    lsz_text_t         *text
    )
{
    lsz_list_t         *link = NULL;
    lsz_json_t         *unit = NULL;

    lsz_txtcatf(text, 1, "[");
    for (link = json->data.arr.list.next; link != &json->data.arr.list; link = link->next) {
        unit = base_of(link, lsz_json_t, link);
        if ((!unit) || (unit->signature != LSZ_JSON_SIGNATURE)) {
            assert(0);
        }
        switch (unit->type) {
        case json_t_boo:
            lsz_txtcatf(text, LSZ_JSON_BFSZ,
                "%s",
                unit->data.num ? "true" : "false");
            break;
        case json_t_num:
            lsz_txtcatf(text, LSZ_JSON_BFSZ,
                "%d",
                unit->data.num);
            break;
        case json_t_nul:
            lsz_txtcatf(text, LSZ_JSON_BFSZ,
                "%s",
                "null");
            break;
        case json_t_str:
            lsz_txtcatf(text, LSZ_JSON_BFSZ,
                "\"%s\"",
                unit->data.str);
            break;
        case json_t_obj:
            lsz_jobj_decode(unit, text);
            break;
        case json_t_arr:
            lsz_jarr_decode(unit, text);
            break;
        default:
            break;
        }
        if (link->next != &json->data.arr.list) {
            // if not end of member, append ','
            lsz_txtcatf(text, 1, ",");
        }
    }
    lsz_txtcatf(text, 1, "]");
}

/*
--------------------------------------------------------------------------------
apis
--------------------------------------------------------------------------------
*/

char *
json_decode (
    json_t              json
    )
{
    return lsz_json_decode(json);
}

/*
--------------------------------------------------------------------------------
end!
--------------------------------------------------------------------------------
*/
