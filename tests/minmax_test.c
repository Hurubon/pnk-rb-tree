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

        struct Data* const data = malloc(num_examples * sizeof *data);
        struct pnk_rb_tree tree = { .less = less };

        for (int i = 0; i < num_examples; ++i)
        {
            data[i] = (struct Data){ {}, rand() % 1024 };
            pnk_rb_tree_insert(&tree, &data[i].node);
        }

        int min = data[0].v;
        int max = data[0].v;

        for (int i = 1; i < num_examples; ++i)
        {
            if (data[i].v < min)
                min = data[i].v;

            if (data[i].v > max)
                max = data[i].v;
        }

        struct Data* min_data = (struct Data*)pnk_rb_tree_minimum(tree.root);
        struct Data* max_data = (struct Data*)pnk_rb_tree_maximum(tree.root);

        assert(min_data->v == min);
        assert(max_data->v == max);

        free(data);
    }
}