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
lsz_jobj_free (
    json_t              json
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

void *
lsz_json_k_pointer (const lsz_rb_node_t *node)
{
    return base_of(node, lsz_json_t, node)->name;
}

void *
lsz_json_v_pointer (const lsz_rb_node_t *node)
{
    return base_of(node, lsz_json_t, node);
}

int
lsz_json_k_compare (void const *a, void const *b)
{
    return lsz_compare_str(a, b);
}

void lsz_json_for_each_free (const lsz_rb_node_t *node)
{
    lsz_json_t *unit = base_of(node, lsz_json_t, node);
    if (!unit) {
        goto eofn;
    }

    switch (unit->type) {
    case json_t_str:
        if (unit->data.str) {
            free(unit->data.str);
        }
        break;
    case json_t_obj:
        lsz_jobj_free(unit->data.obj.json);
        break;
    case json_t_arr:
        lsz_jarr_free(&unit->data);
        break;
    default:
        break;
    }
    if (unit->name) {
        free(unit->name);
    }

    free(unit);

eofn:
    return;
}

json_t
json_new (
    void
    )
{
    return rb_tree_new(lsz_json_k_pointer, lsz_json_v_pointer, lsz_json_k_compare, lsz_json_for_each_free);
}

/*
--------------------------------------------------------------------------------
*/

void
lsz_jobj_free (
    json_t              json
    )
{
    rb_tree_free(json);
}

void
lsz_jarr_free (
    lsz_json_data_t    *data
    )
{
    if (!data) {
        goto eofn;
    }

    lsz_list_t         *this = NULL;
    lsz_list_t         *next = NULL;
    lsz_jarr_t         *jarr = NULL;

    for (this = data->arr.list.next, next = this->next; this != &data->arr.list; this = next, next = this->next) {
        jarr = base_of(this, lsz_jarr_t, link);
        assert(jarr);
        switch (data->arr.type) {
        case json_t_str:
            if (jarr->data.str) {
                free(jarr->data.str);
            }
            break;
        case json_t_obj:
            lsz_jobj_free(jarr->data.obj.json);
            break;
        case json_t_arr:
            lsz_jarr_free(&jarr->data);
            break;
        default:
            break;
        }
        list_delete_link(this);
        free(jarr);
        data->arr.cnta--;
    }

eofn:
    return;
}


void
json_free (
    json_t              json
    )
{
    lsz_jobj_free(json);
}

/*
--------------------------------------------------------------------------------
*/

int
lsz_jarr_get (
    lsz_jarr_node_t    *path,       // [I]: .
    lsz_json_type_t   **type,       // [I]: type of array; [O]: type of array data.
    lsz_json_data_t   **data        // [I]: data of array; [O]: data of array data.
)
{
    int                 r     = 0;
    lsz_jarr_t         *jarr0 = NULL;
    lsz_list_t         *link  = NULL;
    size_t              idst  = 0;
    size_t              isrc  = 0;
    int                 found = 0;

    if (!type || !data) {
        return LSZ_RET_E_ARG;
    }

    lsz_json_type_t    *type0 = *type;
    lsz_json_data_t    *data0 = *data;

    for (size_t i = 0; i < path->level; i++) {
        if (*type0 != json_t_arr) {
            r = LSZ_RET_E_ARG;
            goto eofn;
        }
        //
        // match the index
        //
        found = 0;
        idst  = path->index[i];
        for (link = data0->arr.list.next, isrc = 0; link != &data0->arr.list; link = link->next, isrc++) {
            if (idst == isrc) {
                found = 1;
                break;
            }
        }
        if (!found) {
            r = LSZ_RET_E_NFD;
            goto eofn;
        }
        //
        // next level data
        //
        jarr0 = base_of(link, lsz_jarr_t, link);
        if (!jarr0) {
            r = LSZ_RET_E_NFD;
            goto eofn;
        }
        type0 = &data0->arr.type;
        data0 = &jarr0->data;
    }

    if (type) *type = type0;
    if (data) *data = data0;

eofn:
    return r;
}

int
lsz_json_get (
    json_t             *json, // [I]
    char              **path, // [I] full path, [O] rest path
    size_t              plvl, // [I]
    lsz_json_type_t   **type, // [O] plvl.type
    lsz_json_data_t   **data  // [O] plvl.data
    )
{
    if (!json || !path) {
        return LSZ_RET_E_ARG;
    }

    int                 r     = 0;
    path_node_t        *vect  = NULL;
    size_t              cntv  = 0;
    size_t              i     = 0;
    size_t              count = 0;
    lsz_json_t         *unit  = NULL;
    lsz_jarr_node_t     node  ;
    json_t              json0 = (json) ? *json : NULL;
    lsz_json_type_t    *type0 = (type) ? *type : NULL;
    lsz_json_data_t    *data0 = (data) ? *data : NULL;

    r = path_into(*path, &vect, &cntv);
    if (r) {
        goto eofn;
    }
    if (plvl >= cntv) {
        //
        // >= is illegal, however when 0, = is current
        //
        count = 0;
    } else {
        count = cntv - plvl;
    }

    for (i = 0; i < count; i++) {
        assert(json0);
        r = jarr_node_from_path_node(vect[i], &node);
        if (r) {
            goto eofn;
        }
        unit = rb_tree_search(json0, node.name);
        if (!unit) {
            r = LSZ_RET_E_NFD;
            goto eofn;
        }
        type0 = &unit->type;
        data0 = &unit->data;
        if (i == count - 1) {
            switch (unit->type) {
            case json_t_boo:
            case json_t_num:
            case json_t_nul:
            case json_t_str:
                break;
            case json_t_obj:
                json0 = unit->data.obj.json;
                break;
            case json_t_arr:
                r = lsz_jarr_get(&node, &type0, &data0);
                if (r) {
                    goto eofn;
                }
                if (*type0 == json_t_obj) {
                    json0 = data0->obj.json;
                }
                break;
            default:
                r = LSZ_RET_E_ARG;
                goto eofn;
            }
        } else {
            switch (unit->type) {
            case json_t_obj:
                json0 = unit->data.obj.json;
                break;
            case json_t_arr:
                r = lsz_jarr_get(&node, &type0, &data0);
                if (r) {
                    goto eofn;
                }
                if (*type0 != json_t_obj) {
                    r = LSZ_RET_E_NFD;
                    goto eofn;
                }
                json0 = data0->obj.json;
                break;
            default:
                r = LSZ_RET_E_ARG;
                goto eofn;
            }
        }
    }

    if (json) *json = json0;
    if (type) *type = type0;
    if (data) *data = data0;

    *path = strstr(*path, vect[cntv-1]);

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
int
json_data_init (
    lsz_json_type_t     type,
    lsz_json_data_t     *dst,
    lsz_json_data_t     *src
    )
{
    int r = LSZ_RET_0_ERR;
    if (!dst) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    switch (type) {
    case json_t_boo:
        if (!src) {
            r = LSZ_RET_E_ARG;
            goto eofn;
        }
        dst->num = !!src->num;
        break;
    case json_t_num:
        if (!src) {
            r = LSZ_RET_E_ARG;
            goto eofn;
        }
        dst->num =   src->num;
        break;
    case json_t_nul:
        dst->str = NULL;
        break;
    case json_t_str:
        if (!src) {
            r = LSZ_RET_E_ARG;
            goto eofn;
        }
        if (!src->str) {
            r = LSZ_RET_E_ARG;
            goto eofn;
        }
        if (dst->str) {
            free (dst->str);
            dst->str = NULL;
        }
        dst->str = strdup(src->str);
        break;
    case json_t_obj:
        if (dst->obj.json) {
            json_free(dst->obj.json);
        }
        dst->obj.json = (src && src->obj.json) ? src->obj.json : json_new();
        break;
    case json_t_arr:
        if (dst->arr.type) {
            lsz_jarr_free(dst);
        }
        dst->arr.type = (src && src->arr.type) ? src->arr.type : json_t_non;
        if (src && src->arr.type && src->arr.cnta) {
            dst->arr.type = src->arr.type;
            dst->arr.cnta = src->arr.cnta;
            list_owns(&dst->arr.list, &src->arr.list);
        } else {
            list_init(&dst->arr.list);
        }
        break;
    default:
        r = LSZ_RET_E_ARG;
        goto eofn;
    }
    r = LSZ_RET_0_ERR;

eofn:
    return r;
}

/*
--------------------------------------------------------------------------------
*/

int
lsz_json_add (
    json_t              json,
    lsz_jarr_node_t    *node,
    lsz_json_type_t     type,
    lsz_json_data_t    *data
    )
{
    int                 r    = LSZ_RET_0_ERR;
    lsz_json_t         *unit = NULL;

    if (!json || !node) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    unit = rb_tree_search(json, node->name);
    if (unit) {
        r = LSZ_RET_E_DUP;
        goto eofn;
    }

    unit = calloc(1, sizeof(lsz_json_t));
    if (!unit) {
        r = LSZ_RET_E_OUT;
        goto eofn;
    }
    unit->type = type;
    unit->name = strdup(node->name);

    r = json_data_init(unit->type, &unit->data, data);
    if (r) {
        if (unit->name) {
            free(unit->name);
        }
        if (unit) {
            free(unit);
        }
        goto eofn;
    }

    r = rb_tree_insert(json, &unit->node);

eofn:
    return r;
}

int
lsz_jarr_add (
    json_t              json,
    lsz_jarr_node_t    *node,
    lsz_json_type_t     type,
    lsz_json_data_t    *data
    )
{
    int                 r     = LSZ_RET_0_ERR;
    lsz_json_t         *unit  = NULL;
    lsz_jarr_t         *jarr0 = NULL;
    lsz_json_type_t    *type0 = NULL;
    lsz_json_data_t    *data0 = NULL;
    lsz_json_data_t    *data1 = NULL;
    size_t              i     = 0;
    lsz_list_t         *link  = NULL;
    size_t              idst  = 0;
    size_t              isrc  = 0;
    int                 found = 0;

    if (!json || !node) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }

    unit = rb_tree_search(json, node->name);
    if (!unit) {
        r = lsz_json_add(json, node, json_t_arr, NULL);
        if (r) {
            goto eofn;
        }
        unit = rb_tree_search(json, node->name);
        if (!unit) {
            r = LSZ_RET_E_NFD;
            goto eofn;
        }
        data0 = &unit->data;
        i = 0;
    } else {
        type0 = &unit->type;
        data0 = &unit->data;
        for (i = 0; i < node->level; i++) {
            if (*type0 != json_t_arr) {
                r = LSZ_RET_E_ARG;
                goto eofn;
            }
            if ((node->is_empty) && (i == (node->level - 1))) {
                node->index[i] = data0->arr.cnta;
            }
            //
            // match the index
            //
            found = 0;
            idst  = node->index[i];
            for (link = data0->arr.list.next, isrc = 0; link != &data0->arr.list; link = link->next, isrc++) {
                if (idst == isrc) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                if (idst == isrc) { // idst not exist, idst - 1 exist
                    node->found = i;
                    break;
                } else {
                    r = LSZ_RET_E_ARG;
                    goto eofn;
                }
            }
            //
            // next level data
            //
            jarr0 = base_of(link, lsz_jarr_t, link);
            if (!jarr0) {
                r = LSZ_RET_E_NFD;
                goto eofn;
            }
            type0 = &data0->arr.type;
            data0 = &jarr0->data;
        }
    }
    for (i = i; i < node->level; i++) {
        if (i != node->level - 1) {
            data0->arr.type = json_t_arr;
            data1           = NULL;
        } else {
            data0->arr.type = type;
            data1           = data;
        }
        jarr0 = calloc(1, sizeof(lsz_jarr_t));
        if (!jarr0) {
            r = LSZ_RET_E_OUT;
            goto eofn;
        }
        r = json_data_init(data0->arr.type, &jarr0->data, data1);
        if (r) {
            if (jarr0) {
                free (jarr0);
                jarr0 = NULL;
            }
            goto eofn;
        }
        r = list_insert_tail(&data0->arr.list, &jarr0->link);
        if (r) {
            if (jarr0) {
                free (jarr0);
                jarr0 = NULL;
            }
            goto eofn;
        } else {
            data0->arr.cnta++;
        }
        data0 = &jarr0->data;
    }

eofn:
    return r;
}

/*
--------------------------------------------------------------------------------
*/

int
lsz_jobj_del (
    json_t             *json,
    lsz_jarr_node_t    *node
    )
{
    int                 r    = LSZ_RET_0_ERR;

    rb_tree_delete(*json, node->name);

eofn:
    return r;
}

int
lsz_jarr_del (
    json_t             *json,
    lsz_jarr_node_t        *node
    )
{
    int                 r    = LSZ_RET_0_ERR;
    lsz_json_t         *unit = NULL;
    lsz_json_type_t    *type = NULL;
    lsz_json_data_t    *data = NULL;
    lsz_jarr_t         *jarr = NULL;

    unit = rb_tree_search(*json, node->name);
    if (!unit) {
        r = LSZ_RET_E_NFD;
        goto eofn;
    }
    type = &unit->type;
    data = &unit->data;

    r = lsz_jarr_get(node, &type, &data);
    if (r) {
        goto eofn;
    }

    jarr = base_of(data, lsz_jarr_t, data);
    list_delete_link(& jarr->link);
    switch (*type) {
    case json_t_str:
        if (jarr->data.str) {
            free(jarr->data.str);
        }
        break;
    case json_t_obj:
        lsz_jobj_free(jarr->data.obj.json);
        break;
    case json_t_arr:
        lsz_jarr_free(&jarr->data);
        break;
    default:
        break;
    }
    free(jarr);

    base_of(type, lsz_json_child_arr_t, type)->cnta--;

eofn:
    return r;
}

/*
--------------------------------------------------------------------------------
apis
--------------------------------------------------------------------------------
*/

int
json_get (
    json_t              json,
    char               *path,
    lsz_json_type_t    *type,
    lsz_json_data_t    *data
    )
{
    int                 r       = LSZ_RET_0_ERR;
    json_t             *json0 = &json;
    lsz_json_type_t    *type0 = NULL;
    lsz_json_data_t    *data0 = NULL;

    if (!json || !path) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }
    r = lsz_json_get(json0, &path, 0, &type0, &data0);
    if (r) {
        goto eofn;
    }

    *type = *type0;
    *data = *data0;
eofn:
    return r;
}

int
json_set (
    json_t              json,
    char               *path,
    lsz_json_type_t     type,
    lsz_json_data_t     data
    )
{
    int                 r       = LSZ_RET_0_ERR;
    json_t             *json0 = &json;
    lsz_json_type_t    *type0 = NULL;
    lsz_json_data_t    *data0 = NULL;

    if (!json || !path) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }
    r = lsz_json_get(json0, &path, 0, &type0, &data0);
    if (r) {
        goto eofn;
    }
    // note: json_t_nul shall be able set to any type
    if (type != *type0) {
        if (*type0 != json_t_nul) {
            r = LSZ_RET_E_ARG;
            goto eofn;
        } else {
            *type0 = type;
        }
    }
    r = json_data_init(*type0, data0, &data);

eofn:
    return r;
}

int
json_add (
    json_t              json,
    char               *path,
    lsz_json_type_t     type,
    lsz_json_data_t    *data
    )
{
    int                 r       = LSZ_RET_0_ERR;
    json_t             *json0   = &json;
    lsz_jarr_node_t     node0;

    if (!json || !path) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }
    r = lsz_json_get(json0, &path, 1, NULL, NULL);
    if (r) {
        goto eofn;
    }

    if (is_path_of_jarr(path, &node0)) {
        r = lsz_jarr_add(*json0, &node0, type, data);
    } else {
        r = lsz_json_add(*json0, &node0, type, data);
    }

eofn:
    return r;
}

int
json_del (
    json_t              json,
    char               *path
    )
{
    int                 r       = LSZ_RET_0_ERR;
    json_t             *json0   = &json;
    lsz_json_type_t    *type0   = NULL;
    lsz_json_data_t    *data0   = NULL;
    lsz_jarr_node_t     node0;

    if (!json || !path) {
        r = LSZ_RET_E_ARG;
        goto eofn;
    }
    r = lsz_json_get(json0, &path, 1, NULL, NULL);
    if (r) {
        goto eofn;
    }

    if (is_path_of_jarr(path, &node0)) {
        r = lsz_jarr_del(json0, &node0);
    } else {
        r = lsz_jobj_del(json0, &node0);
    }

eofn:
    return r;
}

/*
--------------------------------------------------------------------------------
end!
--------------------------------------------------------------------------------
*/
