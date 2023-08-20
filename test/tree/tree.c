#include <lszlib.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int k;
    int v;
    lsz_tree_t node;
} test_t;

void *tree_k_pointer(const lsz_tree_t *node)
{
    return &(base_of(node, test_t, node)->k);
}

void *tree_v_pointer(const lsz_tree_t *node)
{
    return &(base_of(node, test_t, node)->v);
}

int tree_k_compare(const void *a, const void *b)
{
    return lsz_icmp(a, b);
}

void tree_node_free(const lsz_tree_t *node)
{
    free(base_of(node, test_t, node));
}

void tree_for_each_callback(const lsz_tree_t *node, void *v);

extern int is_balance;
extern int is_rsonblk;

/*
--------------------------------------------------------------------------------
*/

#define MAX_TEST_HANDLE 32

int main(int argc, char const *argv[])
{
    tree_t  tree = tree_new(tree_k_pointer, tree_v_pointer, tree_k_compare, tree_node_free);
    test_t *test = NULL;
    test_t  temp;

    srand(time(0));
    {
        for (int i = 0; i < MAX_TEST_HANDLE; i++) {
            // 模拟随机插入
            test = malloc(sizeof(test_t));
            test->v = i;
            do {
                test->k = rand()%MAX_TEST_HANDLE;
            } while (tree_search(tree, &test->k));
            tree_insert(tree, &test->node);
            printf("tree_insert: %02d(%02d)\n", test->k, temp.v = *(int *) tree_search(tree, &test->k));
            if (temp.v != test->v) {
                printf("error: k(v): %02d(%02d)\n", test->k, test->v);
            }
        }
        printf("+++++++++++++++++++++++++++++++++\n");
        printf("result:\n");
        tree_for_each(tree, tree_for_each_callback, NULL);
        printf("---------------------------------\n");
        printf("  balance: %s\n", is_balance?"pass":"fail");
        printf("  rsonblk: %s\n", is_rsonblk?"pass":"fail");
    }
    {
        printf("+++++++++++++++++++++++++++++++++\n");
        int *min   = NULL;
        int *max   = NULL;
        int *kptr  = NULL;
        int *vptr  = NULL;
        printf("tree_min: %02d\n", *(min = tree_min(tree)));
        printf("tree_max: %02d\n", *(max = tree_max(tree)));
        printf("+++++++++++++++++++++++++++++++++\n");
        kptr = NULL;
        while((vptr = tree_next(tree, (void **) &kptr))) {
            printf("tree_next: %02d(%02d)\n", *kptr, *vptr);
        }
        kptr = NULL;
        while((vptr = tree_prev(tree, (void **) &kptr))) {
            printf("tree_prev: %02d(%02d)\n", *kptr, *vptr);
        }
        printf("+++++++++++++++++++++++++++++++++\n");
    }
    {
        for (int i = 0; i < MAX_TEST_HANDLE; i++) {
            // 模拟随机删除
            do {
                temp.k = rand()%MAX_TEST_HANDLE;
            } while (!tree_search(tree, &temp.k));
            printf("tree_delete: %02d(%02d)\n", temp.k, temp.v = *(int *) tree_search(tree, &temp.k));
            tree_delete(tree, &temp.k);
            if (tree_search(tree, &temp.k)) {
                printf("error: k(v): %02d(%02d)\n", temp.k, temp.v);
            }
        }
        printf("+++++++++++++++++++++++++++++++++\n");
        printf("result:\n");
        tree_for_each(tree, tree_for_each_callback, NULL);
        printf("---------------------------------\n");
        printf("  balance: %s\n", is_balance?"pass":"fail");
        printf("  rsonblk: %s\n", is_rsonblk?"pass":"fail");
        printf("+++++++++++++++++++++++++++++++++\n");
    }

    tree_free(tree);

    return 0;
}
