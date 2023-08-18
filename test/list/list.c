#include <lszlib.h>
#include <assert.h>
#include <string.h>

/** @file
---------------------------------------------------------------------------------
    游戏：循环报数，报三退出
---------------------------------------------------------------------------------
    [i]
        n: 人数
    [o]
        最后的人的最初编号
---------------------------------------------------------------------------------
--*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "lszlib.h"

typedef struct {
    lsz_list_t  link;
    int         initial_num;
} people_t;

struct _game_t {
    lsz_list_t  list;
    int         current_cnt;
    int         last_person;
} g_game_data;

void run_the_game(lsz_list_t *list)
{

    lsz_list_t  *this = NULL;
    lsz_list_t  *next = NULL;

    if (!list) {
        return;
    }
    if (is_list_empty(list)) {
        return;
    }
    for (this = list->next, next = this->next; this != list; this = next, next = this->next) {
        people_t *people = base_of(this, people_t, link);

        if (next == list) {
            g_game_data.last_person = people->initial_num;
        }
        g_game_data.current_cnt = g_game_data.current_cnt % 3 + 1;
        if (g_game_data.current_cnt == 3) {
            printf("no.%-4d: out\n", people->initial_num);
            list_delete_link(this);
            if (people) {
                free(people);
            }
        }
    }

    run_the_game(list);
}

int find_the_one(int n)
{
    people_t    *people;

    if (n <= 0) {
        return -1;
    } else {
        list_init(&g_game_data.list);
    }
    printf("------------\n");
    for (int i = 0; i < n; i++) {
        people = calloc(1, sizeof(people_t));
        people->initial_num = i+1;
        list_insert_tail(&g_game_data.list, &people->link);
    }
    run_the_game(&g_game_data.list);

    return g_game_data.last_person;
}

int main(int argc, char const *argv[])
{
    int ret = 0;

    lsz_benchmark_do;
    ret = find_the_one(1);
    assert(ret == 1);
    ret = find_the_one(2);
    assert(ret == 2);
    ret = find_the_one(3);
    assert(ret == 2);
    lsz_benchmark_done;

    return 0;
}
