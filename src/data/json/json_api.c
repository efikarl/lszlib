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

void
lsz_jobj_free (
    lsz_json_data_t    *data
    );

void
lsz_jarr_free (
    lsz_json_data_t    *data
    );

/*
--------------------------------------------------------------------------------
impl
--------------------------------------------------------------------------------
*/

json_t
lsz_json_new (
    void
    )
{
    int                 r     = LSZ_RET_0_ERR;
    lsz_json_t         *json  = NULL;

    json = calloc(1, sizeof(lsz_json_t));
    if (!json) {
        r = LSZ_RET_E_OUT;
        goto eofn;
    }
    json->signature = LSZ_JSON_SIGNATURE;
    json->type      = json_t_obj;
    list_init(&json->data.obj.list);

eofn:
    return json;
}

/*
--------------------------------------------------------------------------------
*/

void
lsz_json_free (
    lsz_json_t         *json
    )
{
    //
    // free name
    //
    if (json->name) {
        free (json->name);
        json->name = NULL;
    }
    //
    // free data
    //
    switch (json->type) {
    case json_t_str:
        if (json->data.str) {
            free (json->data.str);
            json->data.str = NULL;
        };
        break;
    case json_t_obj:
        lsz_jobj_free(&json->data);
        break;
    case json_t_arr:
        lsz_jarr_free(&json->data);
        break;
    default:
        break;
    }
    //
    // free json
    //
    free (json);

eofn:
    return;
}


void
lsz_jobj_free (
    lsz_json_data_t    *data
    )
{
    lsz_list_t         *this = NULL;
    lsz_list_t         *next = NULL;
    lsz_json_t         *unit = NULL;

    for (this = data->obj.list.next, next = this->next; this != &data->obj.list; this = next, next = this->next) {
        unit = base_of(this, lsz_json_t, link);
        if ((!unit) || (unit->signature != LSZ_JSON_SIGNATURE)) {
            assert(0);
            goto eofn;
        }
        list_delete_link(&unit->link);
        lsz_json_free(unit);
    }

eofn:
    return;
}

void
lsz_jarr_free (
    lsz_json_data_t    *data
    )
{
    lsz_list_t         *this = NULL;
    lsz_list_t         *next = NULL;
    lsz_json_t         *unit = NULL;

    for (this = data->arr.list.next, next = this->next; this != &data->arr.list; this = next, next = this->next) {
        unit = base_of(this, lsz_json_t, link);
        if ((!unit) || (unit->signature != LSZ_JSON_SIGNATURE)) {
            assert(0);
            goto eofn;
        }
        list_delete_link(&unit->link);
        lsz_json_free(unit);
    }

eofn:
    return;
}

/*
--------------------------------------------------------------------------------
*/

int
lsz_jobj_get (
    lsz_json_t         *json,
    lsz_jarr_node_t    *node,
    lsz_json_t        **unit
    )
{
    int                 r     = LSZ_RET_0_ERR;
    int                 found = 0;
    lsz_json_t         *this  = NULL;
    lsz_list_t         *link  = NULL;

    found = 0;
    for (link = json->data.obj.list.next; link != &json->data.obj.list; link = link->next) {
        this = base_of(link, lsz_json_t, link);
        if ((!this) || (this->signature != LSZ_JSON_SIGNATURE)) {
            assert(0);
            goto eofn;
        }
        if (strcmp(this->name, node->name) == 0) {
            found = 1;
            break;
        }
    }
    if (!found) {
        r = LSZ_RET_E_NFD;
        goto eofn;
    }
    if (unit) *unit = this;

eofn:
    return r;
}

int
lsz_jarr_get (
    lsz_json_t         *json,
    lsz_jarr_node_t    *node,
    lsz_json_t        **unit
)
{
    int                 r     = LSZ_RET_0_ERR;
    int                 found = 0;
    lsz_json_t         *this  = json;
    lsz_list_t         *link  = NULL;
    size_t              idst  = 0;
    size_t              isrc  = 0;

    for (size_t i = 0; i < node->level; i++) {
        if ((json->type) != json_t_arr) {
            r = LSZ_RET_E_ARG;
            goto eofn;
        }
        //
        // match the index
        //
        found = 0;
        idst  = node->index[i];
        for (link = json->data.arr.list.next, isrc = 0; link != &json->data.arr.list; link = link->next, isrc++) {
            this = base_of(link, lsz_json_t, link);
            if ((!this) || (this->signature != LSZ_JSON_SIGNATURE)) {
                assert(0);
                goto eofn;
            }
            if (idst == isrc) {
                found = 1;
                break;
            }
        }
        if (!found) {
            r = LSZ_RET_E_NFD;
            goto eofn;
        }
        json = this;
    }
    if (unit) *unit = this;

eofn:
    return r;
}

int
lsz_json_get (
    lsz_json_t         *json, // [I]
    size_t              plvl, // [I]
    char              **path, // [O] full path, [O] rest path
    lsz_json_t        **unit  // [O] plvl unit
    )
{
    int                 r     = LSZ_RET_0_ERR;
    path_node_t        *vect  = NULL;
    size_t              cntv  = 0;
    size_t              cntp  = 0;
    size_t              i     = 0;
    lsz_jarr_node_t     node;
    lsz_json_t         *this  = json;

    r = path_into(*path, &vect, &cntv);
    if (r) {
        goto eofn;
    }
    if (plvl >= cntv) {
        goto eofn;
    }
    cntp = cntv - plvl;

    for (i = 0; i < cntp; i++) {
        r = lsz_jarr_node_init(vect[i], &node);
        if (r) {
            goto eofn;
        }
        r = lsz_jobj_get(json, &node, &this);
        if (r) {
            r = LSZ_RET_E_NFD;
            goto eofn;
        }
        if (i != cntp - 1) {
            switch (this->type) {
            case json_t_obj:
                json = this;
                break;
            case json_t_arr:
                json = this;
                r = lsz_jarr_get(json, &node, &this);
                if (r) {
                    goto eofn;
                }
                if (this->type == json_t_obj) {
                    json = this;
                } else {
                    r = LSZ_RET_E_NFD;
                    goto eofn;
                }
                break;
            default:
                r = LSZ_RET_E_ARG;
                goto eofn;
            }
        } else {
            switch (this->type) {
            case json_t_boo:
            case json_t_num:
            case json_t_nul:
            case json_t_str:
                break;
            case json_t_obj:
                json = this;
                break;
            case json_t_arr:
                json = this;
                r = lsz_jarr_get(json, &node, &this);
                if (r) {
                    goto eofn;
                }
                break;
            default:
                r = LSZ_RET_E_FMT;
                goto eofn;
            }
        }
    }
    *path = strstr(*path, vect[cntv-1]);
    if (unit) {
        *unit = this;
    }

eofn:
    if (vect) {
        free(vect);
    }

    return r;
}

/*
--------------------------------------------------------------------------------
*/

static
void
lsz_json_data_init (
    int               exist,
    lsz_json_t         *dst,
    lsz_json_t         *src
    )
{
    assert (dst);
    switch (dst->type) {
    case json_t_boo:
        dst->data.num = src ? src->data.num : 0;
        break;
    case json_t_num:
        dst->data.num = src ? src->data.num : 0;
        break;
    case json_t_nul:
        dst->data.str = NULL;
        break;
    case json_t_str:
        if (dst->data.str) {
            free(dst->data.str);
        }
        dst->data.str = src ? strdup(src->data.str) : NULL;
        break;
    case json_t_obj:
        if (exist) {
            if (src) {
                list_replace_link(&dst->link, &src->link);
                lsz_json_free(dst);
            }
        } else {
            list_init(&dst->data.obj.list);
        }
        break;
    case json_t_arr:
        if (exist) {
            if (src) {
                list_replace_link(&dst->link, &src->link);
                lsz_json_free(dst);
            }
        } else {
            list_init(&dst->data.arr.list);
        }
        break;
    default:
        break;
    }
}

/*
--------------------------------------------------------------------------------
*/

int
lsz_json_add (
    lsz_json_t         *json,
    lsz_jarr_node_t    *node,
    lsz_json_t         *unit
    )
{
    int                 r     = LSZ_RET_0_ERR;
    lsz_json_t         *next  = NULL;

    r = lsz_jobj_get(json, node, NULL);
    if (!r) {
        r = LSZ_RET_E_DUP;
        goto eofn;
    }

    if (unit->type == json_t_obj && unit->signature == LSZ_JSON_SIGNATURE) {
        next = unit;
        if (!next->name) {
            next->name = strdup(node->name);
        }
    } else {
        next = calloc(1, sizeof(lsz_json_t));
        if (!next) {
            r = LSZ_RET_E_OUT;
            goto eofn;
        }
        next->signature = LSZ_JSON_SIGNATURE;
        next->type      = unit->type;
        next->name      = strdup(node->name);
        lsz_json_data_init(0, next, unit);
    }

    r = list_insert_tail(&json->data.obj.list, &next->link);
    if (r) {
        goto eofn;
    }

eofn:
    return r;
}

int
lsz_jarr_add (
    lsz_json_t         *json,
    lsz_jarr_node_t    *node,
    lsz_json_t         *unit
    )
{
    int                 r     = LSZ_RET_0_ERR;
    lsz_json_t         *this  = NULL;
    lsz_json_t         *next  = NULL;
    int                 found = 0;
    size_t              i     = 0;
    lsz_list_t         *link  = NULL;
    size_t              idst  = 0;
    size_t              isrc  = 0;

    r = lsz_jobj_get(json, node, &this);
    if ((r) && (r != LSZ_RET_E_NFD)) {
        goto eofn;
    }
    //
    // find the index of the target
    //
    if (r == LSZ_RET_E_NFD) {
        this = calloc(1, sizeof(lsz_json_t));
        if (!this) {
            r = LSZ_RET_E_OUT;
            goto eofn;
        }
        this->signature = LSZ_JSON_SIGNATURE;
        this->type      = json_t_arr;
        this->name      = strdup(node->name);
        list_init(&this->data.arr.list);
        r = list_insert_tail(&json->data.obj.list, &this->link);
        if (r) {
            goto eofn;
        }

        i = 0;
    } else {
        for (i = 0; i < node->level; i++) {
            if (i != node->level - 1) {
                if (this->type != json_t_arr) {
                    r = LSZ_RET_E_ARG;
                    goto eofn;
                }
            } else {
                if (node->is_empty) {
                    node->index[i] = (size_t)-1;
                }
            }
            //
            // match the index
            //
            found = 0;
            idst  = node->index[i];
            for (link = this->data.arr.list.next, isrc = 0; link != &this->data.arr.list; link = link->next, isrc++) {
                next = base_of(link, lsz_json_t, link);
                if ((!next) || (next->signature != LSZ_JSON_SIGNATURE)) {
                    assert(0);
                    goto eofn;
                }
                if (idst == isrc) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                if (idst == isrc) {
                    break;
                }
                if ((node->is_empty) && (i == node->level - 1)) {
                    break;
                }
                r = LSZ_RET_E_ARG;
                goto eofn;
            }
            //
            // next level data
            //
            this = next;
        }
        if (i == node->level) {
            r = LSZ_RET_E_DUP;
            goto eofn;
        }
    }
    //
    // create the index of the rest
    //
    for (i = i; i < node->level; i++) {
        if (i != node->level - 1) {
            next = calloc(1, sizeof(lsz_json_t));
            if (!next) {
                r = LSZ_RET_E_OUT;
                break;
            }
            next->signature = LSZ_JSON_SIGNATURE;
            next->type      = json_t_arr;
            list_init(&next->data.arr.list);
        } else {
            if (unit->type == json_t_obj && unit->signature == LSZ_JSON_SIGNATURE) {
                next = unit;
            } else {
                next = calloc(1, sizeof(lsz_json_t));
                if (!next) {
                    r = LSZ_RET_E_OUT;
                    goto eofn;
                }
                next->signature = LSZ_JSON_SIGNATURE;
                next->type      = unit->type;
                lsz_json_data_init(0, next, unit);
            }
        }

        r = list_insert_tail(&this->data.arr.list, &next->link);
        if (r) {
            break;
        }
        this = next;
    }

eofn:
    return r;
}

/*
--------------------------------------------------------------------------------
*/

int
lsz_jobj_del (
    lsz_json_t         *json,
    lsz_jarr_node_t    *node
    )
{
    int                 r     = LSZ_RET_0_ERR;
    lsz_json_t         *unit  = NULL;

    r = lsz_jobj_get(json, node, &unit);
    if (r) {
        goto eofn;
    }
    list_delete_link(&unit->link);
    lsz_json_free(unit);

eofn:
    return r;
}

int
lsz_jarr_del (
    lsz_json_t         *json,
    lsz_jarr_node_t    *node
    )
{
    int                 r     = LSZ_RET_0_ERR;
    lsz_json_t         *jarr  = NULL;
    lsz_json_t         *unit  = NULL;

    r = lsz_jobj_get(json, node, &jarr);
    if (r) {
        goto eofn;
    }

    r = lsz_jarr_get(jarr, node, &unit);
    if (r) {
        goto eofn;
    }
    list_delete_link(&unit->link);
    lsz_json_free(unit);

eofn:
    return r;
}

/*
--------------------------------------------------------------------------------
apis
--------------------------------------------------------------------------------
*/

json_t
json_api_new (
    void
    )
{
    return lsz_json_new();
}

int
json_api_add (
    lsz_json_t         *json,
    char               *path,
    lsz_json_t         *unit
    )
{
    int                 r = LSZ_RET_0_ERR;
    lsz_jarr_node_t     node;

    if (!json || json->signature != LSZ_JSON_SIGNATURE) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }
    if (!path) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }
    if (!unit) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }
    if (((unit->type <= json_t_non) || (unit->type >= json_t_end))) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    r = lsz_json_get(json, 1, &path, &json);
    if (r) {
        goto eofn;
    }

    if (is_path_of_lsz_jarr(path, &node)) {
        r = lsz_jarr_add(json, &node, unit);
    } else {
        r = lsz_json_add(json, &node, unit);
    }

eofn:
    return r;
}

int
json_api_get (
    lsz_json_t         *json,
    char               *path,
    lsz_json_t        **unit
    )
{
    int                 r = LSZ_RET_0_ERR;
    lsz_json_t         *this = NULL;

    if (!json || json->signature != LSZ_JSON_SIGNATURE) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }
    if (!path) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    r = lsz_json_get(json, 0, &path, &this);
    if (r) {
        goto eofn;
    }
    if (unit) *unit = this;

eofn:
    return r;
}

int
json_api_set (
    lsz_json_t         *json,
    char               *path,
    lsz_json_t         *unit
    )
{
    int                 r = LSZ_RET_0_ERR;
    lsz_json_t         *this = NULL;

    if (!json || json->signature != LSZ_JSON_SIGNATURE) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }
    if (!path) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }
    if (!unit) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }
    if (((unit->type <= json_t_non) || (unit->type >= json_t_end))) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    r = lsz_json_get(json, 0, &path, &this);
    if (r) {
        goto eofn;
    }
    // note: json_t_nul shall be able set to any type
    if (unit->type != this->type) {
        if (this->type != json_t_nul) {
            r = LSZ_RET_E_ARG;
            goto eofn;
        } else {
            this->type = unit->type;
        }
    }
    lsz_json_data_init(1, this, unit);

eofn:
    return r;
}

int
json_api_del (
    lsz_json_t         *json,
    char               *path
    )
{
    int                 r = LSZ_RET_0_ERR;
    lsz_jarr_node_t     node;

    if (!json || json->signature != LSZ_JSON_SIGNATURE) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }
    if (!path) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }
    r = lsz_json_get(json, 1, &path, &json);
    if (r) {
        goto eofn;
    }

    if (is_path_of_lsz_jarr(path, &node)) {
        r = lsz_jarr_del(json, &node);
    } else {
        r = lsz_jobj_del(json, &node);
    }

eofn:
    return r;
}

int
json_api_free (
    lsz_json_t         *json
    )
{
    int                 r = LSZ_RET_0_ERR;

    if (!json || json->signature != LSZ_JSON_SIGNATURE) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    lsz_json_free(json);

eofn:
    return r;
}

/*
--------------------------------------------------------------------------------
apis
--------------------------------------------------------------------------------
*/

json_t
json_new (
    void
    )
{
    return json_api_new();
}

int
json_add (
    json_t              json,
    char               *path,
    lsz_json_t         *unit
    )
{
    return json_api_add(json, path, unit);
}

int
json_get (
    json_t              json,
    char               *path,
    lsz_json_t        **unit
    )
{
    return json_api_get(json, path, unit);
}

int
json_set (
    json_t              json,
    char               *path,
    lsz_json_t         *unit
    )
{
    return json_api_set(json, path, unit);
}

int
json_del (
    json_t              json,
    char               *path
    )
{
    return json_api_del(json, path);
}

int
json_free (
    json_t              json
    )
{
    return json_api_free(json);
}

/*
--------------------------------------------------------------------------------
end!
--------------------------------------------------------------------------------
*/
