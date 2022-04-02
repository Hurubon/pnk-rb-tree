#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include "utility.h"

static inline enum pnk_rb_node_color
color(struct pnk_rb_node const* node)
{
    if (node == NULL || node->color == PNK_RB_NODE_COLOR_BLACK)
        return PNK_RB_NODE_COLOR_BLACK;
    else
        return PNK_RB_NODE_COLOR_RED;
}

static inline int
black_height(struct pnk_rb_node const* node)
{
    if (node == NULL)
        return 1;

    int const bhl = black_height(node->left );
    int const bhr = black_height(node->right);

    if (bhl == -1 || bhr == -1 || bhl != bhr)
        return -1;
    else
        return (color(node) == PNK_RB_NODE_COLOR_BLACK) + bhl;
}

static inline bool
satisfies_property_2(struct pnk_rb_node const* root)
{
    return color(root) == PNK_RB_NODE_COLOR_BLACK;
}

static inline bool
satisfies_property_4(struct pnk_rb_node const* root)
{
    if (root == NULL)
    {
        return true;
    }
    else
    {
        bool result = color(root) == PNK_RB_NODE_COLOR_BLACK || (
            color(root->left ) == PNK_RB_NODE_COLOR_BLACK &&
            color(root->right) == PNK_RB_NODE_COLOR_BLACK
        );
        return result &&
           satisfies_property_4(root->left ) &&
           satisfies_property_4(root->right);
    }
}

static inline bool
satisfies_property_5(struct pnk_rb_node const* node)
{
    return black_height(node) != -1;
}

bool
is_valid_tree(struct pnk_rb_tree const* tree)
{
    return satisfies_property_2(tree->root) &&
           satisfies_property_4(tree->root) &&
           satisfies_property_5(tree->root);
}

bool
less(
    struct pnk_rb_node const* lhs,
    struct pnk_rb_node const* rhs)
{
    struct Data const* L = (struct Data const*)lhs;
    struct Data const* R = (struct Data const*)rhs;
    return L->v < R->v;
}

void
pretty_print(
    struct pnk_rb_node const* node,
    int                       depth)
{
    if (node == NULL)
        return;

    for (int i = 0; i < depth - 1; ++i)
        printf("    ");

    if (depth > 0)
        printf(" |_ ");

    struct Data const* e = (struct Data const*)node;

    printf("%i (%s)\n", e->v, e->node.color == PNK_RB_NODE_COLOR_BLACK? "BLACK" : "RED");

    pretty_print(node->left , depth + 1);
    pretty_print(node->right, depth + 1);
}
