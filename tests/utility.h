#ifndef PNK_RB_TREE_TESTS_UTILITY_H
#define PNK_RB_TREE_TESTS_UTILITY_H

#include <pnk/rb_tree.h>

struct Data
{
    struct pnk_rb_node node;
    int v;
};

void pretty_print  (struct pnk_rb_node const* node,
                    int                       depth);
bool less          (struct pnk_rb_node const* lhs,
                    struct pnk_rb_node const* rhs);
bool is_valid_tree (struct pnk_rb_tree const* tree);

#endif // PNK_RB_TREE_TESTS_UTILITY_H