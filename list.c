/** @file
---------------------------------------------------------------------------------
    Copyright ©2022 Liu Yi, efikarl@yeah.net

    This program is just made available under the terms and conditions of the
    MIT license: http://www.efikarl.com/mit-license.html

    THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
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
#include <stdbool.h>

#include "base.h"
#include "list.h"

int list_init(lsz_list_t *list)
{
    if (list) {
        list->prev = list;
        list->next = list;
    } else {
        return LSZ_RET_E_ARG;
    }

    return LSZ_RET_OK;
}

int is_list_valid(lsz_list_t *list)
{
#if LSZ_LINK_VLD
    lsz_list_t *this = NULL;

    if (!list) {
        return LSZ_RET_E_ARG;
    }
    int list_link_count = 0;
    for (this = list->next; this != list; this = this->next) {
        list_link_count++;
        if (list_link_count == LSZ_MAX_LINK) {
            return LSZ_RET_E_MAX;
        }
    }
    return LSZ_RET_OK;
#else
    return LSZ_RET_OK;
#endif
}

int is_list_empty(lsz_list_t *list)
{
    if (!list) {
        return LSZ_RET_E_ARG;
    }
    if ((list->next == list) && (list->prev == list)) {
        return 1;
    } else {
        return 0;
    }
}

int list_insert_tail(lsz_list_t *list, lsz_list_t *link)
{
    int status = is_list_valid(list);
    if (status < 0) {
        return status;
    }
    if (!link) {
        return LSZ_RET_E_ARG;
    }
    link->next = list;
    link->prev = list->prev;
    list->prev->next = link;
    list->prev = link;

    return LSZ_RET_OK;
}

int list_insert_head(lsz_list_t *list, lsz_list_t *link)
{
    int status = is_list_valid(list);
    if (status < 0) {
        return status;
    }
    if (!link) {
        return LSZ_RET_E_ARG;
    }
    link->prev = list;
    link->next = list->next;
    link->next->prev = link;
    list->next = link;

    return LSZ_RET_OK;
}

int list_delete_link(lsz_list_t *link)
{
    if (!link) {
        return LSZ_RET_E_ARG;
    }
    link->next->prev = link->prev;
    link->prev->next = link->next;

    return LSZ_RET_OK;
}

int list_for_each(lsz_list_t *list, lsz_list_callback_t fn, void *data)
{
    int status;
    lsz_list_t *this = NULL;
    lsz_list_t *next = NULL;

    if (!list) {
        return LSZ_RET_E_ARG;
    }
    for (this = list->next, next = this->next; this != list; this = next, next = this->next) {
        if (fn) fn(list, this, data);
    }
    return 0;
}
