#ifndef __morz__tree__
#define __morz__tree__

#include <stdio.h>

struct Node {
    char symb;

    Node *left;
    Node *right;

    Node();
};

struct Tree {
    Node *root;

    Tree();
    void createTree();
};

#endif
