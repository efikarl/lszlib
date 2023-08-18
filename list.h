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
--*/
#ifndef LSZ_LIST
#define LSZ_LIST
#include <stdint.h>
#include <stddef.h>

#define LSZ_MAX_LINK    UINT16_MAX
#define LSZ_LINK_SIG    ('l' << 24 | 'i' << 16 | 's' << 8 | 't')
#define LSZ_LINK_VLD    0

typedef struct _lsz_list_t lsz_list_t;

struct _lsz_list_t {
    lsz_list_t          *prev;
    lsz_list_t          *next;
};

#define LIST_INIT(list) { &(list) , &(list) }

//
//! callback(!lsz_compare_t): 0: 成功，!0: 失败
//
typedef int (* lsz_list_callback_t) (
    lsz_list_t          *list,
    lsz_list_t          *link,
    void                *data
);

int list_init (
    lsz_list_t          *list
);
int is_list_valid (
    lsz_list_t          *list
);
int is_list_empty (
    lsz_list_t          *list
);
int list_insert_tail (
    lsz_list_t          *list,
    lsz_list_t          *link
);
int list_insert_head (
    lsz_list_t          *list,
    lsz_list_t          *link
);
int list_delete_link (
    lsz_list_t          *link
);
int list_for_each (
    lsz_list_t          *list,
    lsz_list_callback_t callback,
    void                *data
);

#endif
