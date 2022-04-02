#include <stddef.h>
#include <pnk/rb_tree.h>

static inline enum pnk_rb_node_color
color(struct pnk_rb_node const* node)
{
    if (node == NULL || node->color == PNK_RB_NODE_COLOR_BLACK)
        return PNK_RB_NODE_COLOR_BLACK;
    else
        return PNK_RB_NODE_COLOR_RED;
}

static inline bool
is_left_child(struct pnk_rb_node const* node)
{
    return node->parent != NULL && node == node->parent->left;
}

static inline bool
is_right_child(struct pnk_rb_node const* node)
{
    return node->parent != NULL && node == node->parent->right;
}

static inline void
transplant(
    struct pnk_rb_tree* tree,
    struct pnk_rb_node* u,
    struct pnk_rb_node* v)
{
    if ( is_left_child(u) )
        u->parent->left  = v;
    else if ( is_right_child(u) )
        u->parent->right = v;
    else
        tree->root = v;

    if (v != NULL)
        v->parent = u->parent;
}

static inline void
left_rotate(
    struct pnk_rb_tree* tree,
    struct pnk_rb_node* root)
{
    struct pnk_rb_node* pivot = root->right;
    root->right = pivot->left;

    if (pivot->left != NULL)
        pivot->left->parent = root;

    transplant(tree, root, pivot);

    pivot->left = root;
    root->parent = pivot;
}

static inline void
right_rotate(
    struct pnk_rb_tree* tree,
    struct pnk_rb_node* root)
{
    struct pnk_rb_node* pivot = root->left;
    root->left = pivot->right;

    if (pivot->right != NULL)
        pivot->right->parent = root;

    transplant(tree, root, pivot);

    pivot->right = root;
    root->parent = pivot;
}

static inline void
insert_fixup(
    struct pnk_rb_tree* tree,
    struct pnk_rb_node* node)
{
    while (color(node->parent) == PNK_RB_NODE_COLOR_RED)
    {
        if ( is_left_child(node->parent) )
        {
            struct pnk_rb_node* const uncle = node->parent->parent->right;

            // Case 1
            if (color(uncle) == PNK_RB_NODE_COLOR_RED)
            {
                node->parent->color = PNK_RB_NODE_COLOR_BLACK;
                uncle->color        = PNK_RB_NODE_COLOR_BLACK;
                node->parent->parent->color = PNK_RB_NODE_COLOR_RED;
                node = node->parent->parent;
            }
            else
            {
                // Case 2
                if ( is_right_child(node) )
                {
                    node = node->parent;
                    left_rotate(tree, node);
                }
                // Case 3
                node->parent->color = PNK_RB_NODE_COLOR_BLACK;
                node->parent->parent->color = PNK_RB_NODE_COLOR_RED;
                right_rotate(tree, node->parent->parent);
            }
        }
        else
        {
            struct pnk_rb_node* const uncle = node->parent->parent->left;

            // Case 4
            if (color(uncle) == PNK_RB_NODE_COLOR_RED)
            {
                node->parent->color = PNK_RB_NODE_COLOR_BLACK;
                uncle->color        = PNK_RB_NODE_COLOR_BLACK;
                node->parent->parent->color = PNK_RB_NODE_COLOR_RED;
                node = node->parent->parent;
            }
            // Case 5
            else
            {
                if ( is_left_child(node) )
                {
                    node = node->parent;
                    right_rotate(tree, node);
                }
                // Case 6
                node->parent->color = PNK_RB_NODE_COLOR_BLACK;
                node->parent->parent->color = PNK_RB_NODE_COLOR_RED;
                left_rotate(tree, node->parent->parent);
            }
        }
    }

    tree->root->color = PNK_RB_NODE_COLOR_BLACK;
}

static inline void
delete_fixup(
    struct pnk_rb_tree* tree,
    struct pnk_rb_node* node)
{
    while (node != NULL && node != tree->root && color(node) == PNK_RB_NODE_COLOR_BLACK)
    {
        if ( is_left_child(node) )
        {
            struct pnk_rb_node* brother = node->parent->right;

            if (color(brother) == PNK_RB_NODE_COLOR_RED)
            {
                brother->color = PNK_RB_NODE_COLOR_BLACK;
                node->parent->color = PNK_RB_NODE_COLOR_RED;
                left_rotate(tree, node->parent);
                brother = node->parent->right;
            }

            if (color(brother->left ) == PNK_RB_NODE_COLOR_BLACK &&
                color(brother->right) == PNK_RB_NODE_COLOR_BLACK)
            {
                brother->color = PNK_RB_NODE_COLOR_RED;
                node = node->parent;
            }
            else
            {
                if (color(brother->right) == PNK_RB_NODE_COLOR_BLACK)
                {
                    brother->left->color = PNK_RB_NODE_COLOR_BLACK;
                    brother->color = PNK_RB_NODE_COLOR_RED;
                    right_rotate(tree, brother);
                    brother = node->parent->right;
                }

                brother->color = node->parent->color;
                node->parent->color   = PNK_RB_NODE_COLOR_BLACK;
                brother->right->color = PNK_RB_NODE_COLOR_BLACK;
                left_rotate(tree, node->parent);
                node = tree->root;
            }
        }
        else
        {
            struct pnk_rb_node* brother = node->parent->left;

            if (color(brother) == PNK_RB_NODE_COLOR_RED)
            {
                brother->color = PNK_RB_NODE_COLOR_BLACK;
                node->parent->color = PNK_RB_NODE_COLOR_RED;
                right_rotate(tree, node->parent);
                brother = node->parent->left;
            }

            if (color(brother->left ) == PNK_RB_NODE_COLOR_BLACK &&
                color(brother->right) == PNK_RB_NODE_COLOR_BLACK)
            {
                brother->color = PNK_RB_NODE_COLOR_RED;
                node = node->parent;
            }
            else
            {
                if (color(brother->left) == PNK_RB_NODE_COLOR_BLACK)
                {
                    brother->right->color = PNK_RB_NODE_COLOR_BLACK;
                    brother->color = PNK_RB_NODE_COLOR_RED;
                    left_rotate(tree, brother);
                    brother = node->parent->left;
                }

                brother->color = node->parent->color;
                node->parent->color   = PNK_RB_NODE_COLOR_BLACK;
                brother->left->color = PNK_RB_NODE_COLOR_BLACK;
                right_rotate(tree, node->parent);
                node = tree->root;
            }
        }
    }

    if (node != NULL)
        node->color = PNK_RB_NODE_COLOR_BLACK;
}

bool
pnk_rb_tree_is_empty(struct pnk_rb_tree* tree)
{
    return tree->root == NULL;
}

struct pnk_rb_node*
pnk_rb_tree_minimum(struct pnk_rb_node* root)
{
    while (root->left != NULL)
        root = root->left;

    return root;
}

struct pnk_rb_node*
pnk_rb_tree_maximum(struct pnk_rb_node* root)
{
    while (root->right != NULL)
        root = root->right;

    return root;
}

struct pnk_rb_node*
pnk_rb_tree_predecessor(struct pnk_rb_node* node)
{
    if (node->left != NULL)
        return pnk_rb_tree_maximum(node->left);

    while ( is_left_child(node) )
        node = node->parent;

    return node->parent;
}

struct pnk_rb_node*
pnk_rb_tree_successor(struct pnk_rb_node* node)
{
    if (node->right != NULL)
        return pnk_rb_tree_minimum(node->right);

    while ( is_right_child(node) )
        node = node->parent;

    return node->parent;
}

void
pnk_rb_tree_preorder_nlr(
    struct pnk_rb_node* node,
    pnk_rb_tree_visitor visit)
{
    if (node != NULL)
    {
        visit(node);
        pnk_rb_tree_preorder_nlr(node->left , visit);
        pnk_rb_tree_preorder_nlr(node->right, visit);
    }
}

void
pnk_rb_tree_preorder_nrl(
    struct pnk_rb_node* node,
    pnk_rb_tree_visitor visit)
{
    if (node != NULL)
    {
        visit(node);
        pnk_rb_tree_preorder_nrl(node->right, visit);
        pnk_rb_tree_preorder_nrl(node->left , visit);
    }
}

void
pnk_rb_tree_postorder_lrn(
    struct pnk_rb_node* node,
    pnk_rb_tree_visitor visit)
{
    if (node != NULL)
    {
        pnk_rb_tree_postorder_lrn(node->left , visit);
        pnk_rb_tree_postorder_lrn(node->right, visit);
        visit(node);
    }
}

void
pnk_rb_tree_postorder_rln(
    struct pnk_rb_node* node,
    pnk_rb_tree_visitor visit)
{
    if (node != NULL)
    {
        pnk_rb_tree_postorder_rln(node->right, visit);
        pnk_rb_tree_postorder_rln(node->left , visit);
        visit(node);
    }
}

void
pnk_rb_tree_inorder_lnr(
    struct pnk_rb_node* node,
    pnk_rb_tree_visitor visit)
{
    if (node != NULL)
    {
        pnk_rb_tree_inorder_lnr(node->left , visit);
        visit(node);
        pnk_rb_tree_inorder_lnr(node->right, visit);
    }
}

void
pnk_rb_tree_inorder_rnl(
    struct pnk_rb_node* node,
    pnk_rb_tree_visitor visit)
{
    if (node != NULL)
    {
        pnk_rb_tree_inorder_rnl(node->right, visit);
        visit(node);
        pnk_rb_tree_inorder_rnl(node->left , visit);
    }
}

void
pnk_rb_tree_insert(
    struct pnk_rb_tree* tree,
    struct pnk_rb_node* node)
{
    if ( pnk_rb_tree_is_empty(tree) )
    {
        tree->root = node;
        return;
    }

    struct pnk_rb_node* lag = NULL;
    struct pnk_rb_node* lead = tree->root;

    while (lead != NULL)
    {
        lag = lead;

        if ( tree->less(node, lead) )
            lead = lead->left;
        else
            lead = lead->right;
    }

    node->parent = lag;
    node->color = PNK_RB_NODE_COLOR_RED;

    if ( tree->less(node, lag) )
        lag->left = node;
    else
        lag->right = node;

    insert_fixup(tree, node);
}

void
pnk_rb_tree_delete(
    struct pnk_rb_tree* tree,
    struct pnk_rb_node* node)
{
    struct pnk_rb_node* x = NULL;
    struct pnk_rb_node* y = node;
    enum pnk_rb_node_color y_original_color = y->color;

    if (node->left == NULL)
    {
        x = node->right;
        transplant(tree, node, node->right);
    }
    else if (node->right == NULL)
    {
        x = node->left;
        transplant(tree, node, node->left);
    }
    else
    {
        y = pnk_rb_tree_minimum(node->right);
        x = y->right;
        y_original_color = y->color;

        // Note: x != NULL added.
        if (x != NULL && y->parent == node)
        {
            x->parent = y;
        }
        else
        {
            transplant(tree, y, y->right);
            y->right = node->right;
            // Note: Added check.
            if (y->right != NULL)
                y->right->parent = y;
        }

        transplant(tree, node, y);
        y->left = node->left;
        y->left->parent = y;
        y->color = node->color;
    }

    if (y_original_color == PNK_RB_NODE_COLOR_BLACK)
        delete_fixup(tree, x);
}