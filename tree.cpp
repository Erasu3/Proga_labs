#include "tree.h"
#include <iostream>

Node::Node() {
    this->left = NULL;
    this->right = NULL;
}

Tree::Tree() {
    this->root = new Node;
    this->root->left = NULL;
    this->root->right = NULL;
}

void Tree::createTree() {
    FILE *f = fopen("morze.txt", "r+");

    while(!feof(f)) {
        char line[10];
        char symb;
        fgets(line, 10, f);

        symb = line[0];
        Node* curr = this->root;
        for(int i=2; line[i] != '\0' && line[i] != '\n'; i++) {
            if(line[i] == '.') {
                if(curr->left == NULL) {
                    Node *tmp = new Node;
                    curr->left = tmp;
                }
                curr = curr->left;
            } else if(line[i] == '-') {
                if(curr->right == NULL) {
                    Node *tmp = new Node;
                    curr->right = tmp;
                }
                curr = curr->right;
            }
        }
        curr->symb = symb;
        //printf("%s\n", line);
    }
}
