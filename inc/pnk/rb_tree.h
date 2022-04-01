#ifndef PNK_RB_TREE_H
#define PNK_RB_TREE_H

#include <stdbool.h>

enum pnk_rb_node_color
{
    PNK_RB_NODE_COLOR_BLACK,
    PNK_RB_NODE_COLOR_RED,
    PNK_RB_NODE_COLOR_COUNT,
};

struct pnk_rb_node
{
    struct pnk_rb_node* parent;
    struct pnk_rb_node* left;
    struct pnk_rb_node* right;
    enum pnk_rb_node_color color;
};

typedef bool(*pnk_rb_tree_less_fn)(
    struct pnk_rb_node const*,
    struct pnk_rb_node const*);

typedef void(*pnk_rb_tree_visitor)(
    struct pnk_rb_node*
);

struct pnk_rb_tree
{
    struct pnk_rb_node* root;
    pnk_rb_tree_less_fn less;
};

bool                pnk_rb_tree_is_empty      (struct pnk_rb_tree* tree);

struct pnk_rb_node* pnk_rb_tree_minimum       (struct pnk_rb_node* root);
struct pnk_rb_node* pnk_rb_tree_maximum       (struct pnk_rb_node* root);

struct pnk_rb_node* pnk_rb_tree_predecessor   (struct pnk_rb_node* node);
struct pnk_rb_node* pnk_rb_tree_successor     (struct pnk_rb_node* node);

void                pnk_rb_tree_preorder_nlr  (struct pnk_rb_node* node,
                                               pnk_rb_tree_visitor visit);
void                pnk_rb_tree_preorder_nrl  (struct pnk_rb_node* node,
                                               pnk_rb_tree_visitor visit);
void                pnk_rb_tree_postorder_lrn (struct pnk_rb_node* node,
                                               pnk_rb_tree_visitor visit);
void                pnk_rb_tree_postorder_rln (struct pnk_rb_node* node,
                                               pnk_rb_tree_visitor visit);
void                pnk_rb_tree_inorder_lnr   (struct pnk_rb_node* node,
                                               pnk_rb_tree_visitor visit);
void                pnk_rb_tree_inorder_rnl   (struct pnk_rb_node* node,
                                               pnk_rb_tree_visitor visit);

void                pnk_rb_tree_insert        (struct pnk_rb_tree* tree,
                                               struct pnk_rb_node* node);
void                pnk_rb_tree_delete        (struct pnk_rb_tree* tree,
                                               struct pnk_rb_node* node);

#endif // PNK_RB_TREE_H