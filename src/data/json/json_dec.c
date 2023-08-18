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

void
lsz_json_for_each_decode_type (
    const lsz_rb_node_t   *node,
    void               *ctxt
    );

void
lsz_json_for_each_decode_data (
    const lsz_rb_node_t   *node,
    void               *ctxt
    );

/*
--------------------------------------------------------------------------------
type
--------------------------------------------------------------------------------
*/

typedef struct {
    char *              head;
    char *              tail;
    size_t              size;
} lsz_json_text_t;

/*
--------------------------------------------------------------------------------
impl
--------------------------------------------------------------------------------
*/

static
int
lsz_txtcatf (
    lsz_json_text_t    *text,
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
    va_list             args;

    if (!text) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }
    if (text->tail < text->head) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }
    if (!bfsz) {
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
    va_start(args, format); src_strsz = vsnprintf(src, bfsz + 1, format, args); va_end(args);
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
lsz_jobj_decode_type (
    json_t              json,
    lsz_json_text_t    *text
    )
{
    lsz_txtcatf(text, 1, "{");
    rb_tree_for_each(json, lsz_json_for_each_decode_type, text);
    //
    // if end of member, eat ','
    //
    if (text->tail[-1] == ',') {
        text->tail[-1] = '\0';
        text->tail--;
    }
    lsz_txtcatf(text, 1, "}");
}

void
lsz_jarr_decode_type (
    lsz_json_data_t    *data,
    lsz_json_text_t    *text
    )
{
    lsz_list_t         *link = NULL;
    lsz_jarr_t         *jarr = NULL;

    lsz_txtcatf(text, 1, "[");
    for (link = data->arr.list.next; link != &data->arr.list; link = link->next) {
        jarr = base_of(link, lsz_jarr_t, link);
        switch (data->arr.type) {
        case json_t_boo:
            lsz_txtcatf(text, LSZ_JSON_BFSZ,
                "%s",
                "true");
            break;
        case json_t_num:
            lsz_txtcatf(text, LSZ_JSON_BFSZ,
                "%d",
                0x0001);
            break;
        case json_t_nul:
            lsz_txtcatf(text, LSZ_JSON_BFSZ,
                "%s",
                "null");
            break;
        case json_t_str:
            lsz_txtcatf(text, LSZ_JSON_BFSZ,
                "\"%s\"",
                "****");
            break;
        case json_t_obj:
            lsz_jobj_decode_type (
                jarr->data.obj.json,
                text);
            break;
        case json_t_arr:
            lsz_jarr_decode_type (
                &jarr->data,
                text);
            break;
        default:
            break;
        }
        break; // the first member for type is ok
    }
    lsz_txtcatf(text, 1, "]");
}

void
lsz_json_for_each_decode_type (
    const lsz_rb_node_t   *node,
    void               *ctxt
    )
{
    int                 r    = LSZ_RET_0_ERR;
    lsz_json_text_t    *text = (lsz_json_text_t *) ctxt;
    lsz_json_t         *unit = base_of(node, lsz_json_t, node);

    switch (unit->type) {
    case json_t_boo:
        lsz_txtcatf(text, LSZ_JSON_BFSZ,
            "\"%s\":%s",
            unit->name, "true");
        break;
    case json_t_num:
        lsz_txtcatf(text, LSZ_JSON_BFSZ,
            "\"%s\":%d",
            unit->name, 0x0001);
        break;
    case json_t_nul:
        lsz_txtcatf(text, LSZ_JSON_BFSZ,
            "\"%s\":%s",
            unit->name, "null");
        break;
    case json_t_str:
        lsz_txtcatf(text, LSZ_JSON_BFSZ,
            "\"%s\":\"%s\"",
            unit->name, "****");
        break;
    case json_t_obj:
        lsz_txtcatf(text, LSZ_JSON_BFSZ, "\"%s\":", unit->name);
        lsz_jobj_decode_type (
            unit->data.obj.json,
            text);
        break;
    case json_t_arr:
        lsz_txtcatf(text, LSZ_JSON_BFSZ, "\"%s\":", unit->name);
        lsz_jarr_decode_type (
            &unit->data,
            text);
        break;
    default:
        r = LSZ_RET_E_ARG;
        break;
    }
    if (!r) {
        lsz_txtcatf(text, 1, ",");
    }
}

char *
lsz_json_decode_type (
    json_t              json
    )
{
    char               *rslt = NULL;
    lsz_json_text_t     text;

    if (!json) {
        goto eofn;
    }

    text.head = calloc(1, LSZ_JSON_BFSZ);
    text.tail = text.head;
    text.size = 0;
    if (!text.head) {
        return NULL;
    }
    lsz_jobj_decode_type(json, &text);
    rslt = strdup(text.head);
    if (text.head) {
        free(text.head);
    }

eofn:
    return rslt;
}

/*
--------------------------------------------------------------------------------
*/

void
lsz_jobj_decode_data (
    json_t              json,
    lsz_json_text_t    *text
    )
{
    lsz_txtcatf(text, 1, "{");
    rb_tree_for_each(json, lsz_json_for_each_decode_data, text);
    //
    // if end of member, eat ','
    //
    if (text->tail[-1] == ',') {
        text->tail[-1] = '\0';
        text->tail--;
    }
    lsz_txtcatf(text, 1, "}");
}

void
lsz_jarr_decode_data (
    lsz_json_data_t    *data,
    lsz_json_text_t    *text
    )
{
    lsz_list_t         *link = NULL;
    lsz_jarr_t         *jarr = NULL;

    lsz_txtcatf(text, 1, "[");
    for (link = data->arr.list.next; link != &data->arr.list; link = link->next) {
        jarr = base_of(link, lsz_jarr_t, link);
        switch (data->arr.type) {
        case json_t_boo:
            lsz_txtcatf(text, LSZ_JSON_BFSZ,
                "%s",
                jarr->data.num ? "true" : "false");
            break;
        case json_t_num:
            lsz_txtcatf(text, LSZ_JSON_BFSZ,
                "%d",
                jarr->data.num);
            break;
        case json_t_nul:
            lsz_txtcatf(text, LSZ_JSON_BFSZ,
                "%s",
                "null");
            break;
        case json_t_str:
            lsz_txtcatf(text, LSZ_JSON_BFSZ,
                "\"%s\"",
                jarr->data.str);
            break;
        case json_t_obj:
            lsz_jobj_decode_data (
                jarr->data.obj.json,
                text);
            break;
        case json_t_arr:
            lsz_jarr_decode_data (
                &jarr->data,
                text);
            break;
        default:
            break;
        }
        //
        // if not end of member, append ','
        //
        if (link->next != &data->arr.list) {
            lsz_txtcatf(text, 1, ",");
        }
    }
    lsz_txtcatf(text, 1, "]");
}

void
lsz_json_for_each_decode_data (
    const lsz_rb_node_t   *node,
    void               *ctxt
    )
{
    int                 r    = LSZ_RET_0_ERR;
    lsz_json_text_t    *text = (lsz_json_text_t *) ctxt;
    lsz_json_t         *unit = base_of(node, lsz_json_t, node);

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
        lsz_jobj_decode_data (
            unit->data.obj.json,
            text);
        break;
    case json_t_arr:
        lsz_txtcatf(text, LSZ_JSON_BFSZ, "\"%s\":", unit->name);
        lsz_jarr_decode_data (
            &unit->data,
            text);
        break;
    default:
        r = LSZ_RET_E_ARG;
        break;
    }
    if (!r) {
        lsz_txtcatf(text, 1, ",");
    }
}

char *
lsz_json_decode_data (
    json_t              json
    )
{
    char               *rslt = NULL;
    lsz_json_text_t     text;

    if (!json) {
        goto eofn;
    }

    text.head = calloc(1, LSZ_JSON_BFSZ);
    text.tail = text.head;
    text.size = 0;
    if (!text.head) {
        return NULL;
    }
    lsz_jobj_decode_data(json, &text);
    rslt = strdup(text.head);
    if (text.head) {
        free(text.head);
    }

eofn:
    return rslt;
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
    return lsz_json_decode_data(json);
}

/*
--------------------------------------------------------------------------------
end!
--------------------------------------------------------------------------------
*/
