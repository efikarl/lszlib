/** @file
---------------------------------------------------------------------------------
    Copyright ©2023-2024 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
---------------------------------------------------------------------------------
    license: refer to https://github.com/tianocore/edk2
---------------------------------------------------------------------------------
    数据 - 双向循环链表
---------------------------------------------------------------------------------
     ________________________
    |                        |
    '-|    |<-|prev|<-|    |<-
      |link|  |list|  |link|
    ->|    |  |next|->|    |-.
    |________________________|
---------------------------------------------------------------------------------
--*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "base.h"
#include "list.h"

int list_init(lsz_list_t *list)
{
    if (!list) {
        return LSZ_RET_E_ARG;
    }
    list->prev = list;
    list->next = list;

    return LSZ_RET_0_ERR;
}

int is_list_valid(lsz_list_t *list)
{
    if (!list) {
        return 0;
    }
#if LSZ_LINK_VLD
    volatile lsz_list_t *this = NULL;

    for (this = list->next; (this) && (this != list); this = this->next);
    if (this == list) {
        return 1;
    } else {
        return 0;
    }
#else
    return 1;
#endif
}

int is_list_empty(lsz_list_t *list)
{
    assert(list);

    if ((list->next == list) && (list->prev == list)) {
        return 1;
    } else {
        return 0;
    }
}

int list_insert_tail(lsz_list_t *list, lsz_list_t *link)
{
    if (!is_list_valid(list)) {
        return LSZ_RET_E_ARG;
    }
    if (!link) {
        return LSZ_RET_E_ARG;
    }
    link->next = list;
    link->prev = list->prev;
    list->prev->next = link;
    list->prev = link;

    return LSZ_RET_0_ERR;
}

int list_insert_head(lsz_list_t *list, lsz_list_t *link)
{
    if (!is_list_valid(list)) {
        return LSZ_RET_E_ARG;
    }
    if (!link) {
        return LSZ_RET_E_ARG;
    }
    link->prev = list;
    link->next = list->next;
    link->next->prev = link;
    list->next = link;

    return LSZ_RET_0_ERR;
}

int list_delete_link(lsz_list_t *link)
{
    if (!link) {
        return LSZ_RET_E_ARG;
    }
    link->next->prev = link->prev;
    link->prev->next = link->next;

    return LSZ_RET_0_ERR;
}

int list_for_each(lsz_list_t *list, lsz_list_callback_t fn, void *data)
{
    lsz_list_t *this = NULL;
    lsz_list_t *next = NULL;

    if (!list) {
        return LSZ_RET_E_ARG;
    }
    for (this = list->next, next = this->next; this != list; this = next, next = this->next) {
        if (fn) fn(list, this, data);
    }
    return LSZ_RET_0_ERR;
}

int list_owns(lsz_list_t *dst_list, lsz_list_t *src_list)
{
    if (!dst_list) {
        return LSZ_RET_E_ARG;
    }

    if (is_list_empty(src_list) || !is_list_valid(src_list)) {
        return list_init(dst_list);
    }
    dst_list->prev = src_list->prev;
    dst_list->next = src_list->next;
    src_list->prev->next = dst_list;
    src_list->next->prev = dst_list;

    return LSZ_RET_0_ERR;
}

void
list_free(lsz_list_t *list, lsz_list_callback_t fn)
{
    list_for_each(list, fn, NULL);
}
