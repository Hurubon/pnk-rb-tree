#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <pnk/rb_tree.h>
#include "utility.h"

int main(void)
{
    srand(time(NULL));

    for (int tests = 0; tests < 256; ++tests)
    {
        int const max_examples = 256;
        int const num_examples = rand() % max_examples;
        int const to_remove    = rand() % num_examples;

        struct Data* const data = malloc(num_examples * sizeof *data);
        struct pnk_rb_tree tree = { .less = less };

        for (int i = 0; i < num_examples; ++i)
        {
            data[i] = (struct Data){ {}, rand() % 1024 };
            pnk_rb_tree_insert(&tree, &data[i].node);
        }

        pnk_rb_tree_delete(&tree, &data[to_remove].node);
        assert( is_valid_tree(&tree) );

        free(data);
    }
}