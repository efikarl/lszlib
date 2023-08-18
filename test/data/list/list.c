#include <lszlib.h>

/** @file
---------------------------------------------------------------------------------
    游戏：循环报数，报M退出
---------------------------------------------------------------------------------
    [i]
        n: 人数
        M: 出局
    [o]
        最后的人的最初编号
---------------------------------------------------------------------------------
--*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lszlib.h"

#ifndef DEBUG_IT
#define DEBUG_IT 0
#endif

typedef struct {
    lsz_list_t  link;
    int         initial_num;
} people_t;

typedef struct {
    lsz_list_t  list;
    int         m;
    int         current_cnt;
    int         last_person;
} a_game_t;

a_game_t *
new_the_game(int n, int m)
{
    a_game_t *game = calloc(1, sizeof(a_game_t));
    people_t *people;

    list_init(&game->list);
    for (int i = 0; i < n; i++) {
        people = calloc(1, sizeof(people_t));
        people->initial_num = i+1;
        list_insert_tail(&game->list, &people->link);
    }
    game->m = m;

    return game;
}

int run_the_game(a_game_t *game)
{

    lsz_list_t  *this = NULL;
    lsz_list_t  *next = NULL;

    if (!game) {
        return -1;
    }
    if (is_list_empty(&game->list)) {
        int last_person = game->last_person;
        if (game) {
            free(game);
        }
        return last_person;
    }
    for (this = game->list.next, next = this->next; this != &game->list; this = next, next = this->next) {
        people_t *people = base_of(this, people_t, link);
        if (next == &game->list) {
            game->last_person = people->initial_num;
        }
        game->current_cnt = game->current_cnt % game->m + 1;
        if (game->current_cnt == game->m) {
#if DEBUG_IT
            printf("no.%-4d: out\n", people->initial_num);
#endif
            list_delete_link(this);
            if (people) {
                free(people);
            }
        }
    }

    run_the_game(game);
}

int find_the_one(int n, int m)
{
    if (n <= 0) {
        return -1;
    }
#if DEBUG_IT
    printf("------------\n");
#endif
    return run_the_game(new_the_game(n, m));
}

void testcase_find_the_one()
{
    TESTCASE_ASSERT_BOOL(1 == find_the_one(1, 3), "find_the_one(1, 3)");
    TESTCASE_ASSERT_BOOL(2 == find_the_one(2, 3), "find_the_one(2, 3)");
    TESTCASE_ASSERT_BOOL(2 == find_the_one(3, 3), "find_the_one(3, 3)");
    TESTCASE_ASSERT_BOOL(1 == find_the_one(4, 3), "find_the_one(4, 3)");
}

int main(int argc, char const *argv[])
{
    int ret = 0;

    lsz_testperf_do;
    for (int i = 0; i < 16; i++) {
        ret = find_the_one(i, 4);
#if DEBUG_IT
        printf("last person: [%03d] %03d\n", i, ret);
#endif
    }
    lsz_testperf_done;

    lsz_testcase_do {
        testcase_find_the_one();
    } lsz_testcase_done;

    return 0;
}
