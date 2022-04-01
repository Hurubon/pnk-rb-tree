#include <stdio.h>
#include <stdbool.h>
#include "rb_tree.h"

struct example
{
    struct pnk_rb_node node;
    int v;
};

void pretty_print(struct pnk_rb_node* root, int depth)
{
    if (root == NULL)
        return;

    struct example* e = (struct example*)root;

    for (int i = 0; i < depth - 1; ++i)
        printf("    ");

    if (depth > 0)
        printf(" |_ ");

    printf("%i\n", e->v);

    pretty_print(root->left, depth + 1);
    pretty_print(root->right, depth + 1);
}

int main(void)
{
    struct example examples[] = {
        { {}, 1 }, { {}, 2 }, { {}, 3 }, { {}, 4 },
        { {}, 5 }, { {}, 6 }, { {}, 7 }, { {}, 8 },
        { {}, 9 }, { {}, 10 }, { {}, 11}, { {}, 12},
        { {}, 13}, { {}, 14}, { {}, 15},
    };

    // Manually hooking up the tree.
    struct pnk_rb_tree tree = { .root = &examples[7] };
    examples[7].parent = NULL;
    examples[7].left  = &examples[3];
    examples[7].right = &examples[11]

    examples[3].parent = &examples[7];
    examples[3].left  = &examples[1];
    examples[3].right = &examples[5];

    examples[1].parent = &examples[3];
    examples[1].left  = &examples[0];
    examples[1].right = &examples[2];

    examples[0].parent = &examples[1];
    examples[2].parent = &examples[1];

    examples[5].parent = &examples[3];
    examples[5].left  = &examples[4];
    examples[5].right = &examples[6];

    examples[4].parent = &examples[5];
    examples[6].parent = &examples[5];

    examples[11].parent = &examples[7];
    examples[11].left  = &examples[9];
    examples[11].right = &examples[13];

    examples[9].parent = &examples[11];
    examples[9].left  = &examples[8];
    examples[9].right = &examples[10];

    examples[8].parent = examples[9];
    examples[10].parent = examples[9];

    examples[13].parent = &examples[11];
    examples[13].left  = &examples[12];
    examples[13].right = &examples[14];

    examples[12].parent = &examples[13];
    examples[14].parent = &examples[13];

    pretty_print(tree.root, 0);
}