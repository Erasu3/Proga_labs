
#include <iostream>
#include "tree.h"
#include <string.h>

void code(char line[1000], char result[1000]) {
    for(int i=0; i<strlen(line); i++) {
        FILE *f = fopen("morze.txt", "r+");

        while(!feof(f)) {
            char get[10];
            fgets(get, 10, f);
            if(get[0] == line[i] || get[0] + 32 == line[i]) {
                for(int i=2; get[i] != '\0'; i++) {
                    if(get[i] == '.') sprintf(result, "%s%s", result, "10");
                    else if(get[i] == '-') sprintf(result, "%s%s", result, "110");
                }
                //printf(" ");
            }
        }
        if(line[i] == ' ') {
            sprintf(result, "%s%s", result, "00");
        } else {
            sprintf(result, "%s%s", result, "0");
        }
    }
    sprintf(result, "%s%s", result, "0");
}

void decode(char line[1000], Tree *T) {
    Node *curr = T->root;
    for(int i=0; i<strlen(line);) {
        if(line[i] == '1' && line[i+1] == '0') {
            curr = curr->left;
            i+=2;
        }
        if(line[i] == '1' && line[i+1] == '1') {
            curr = curr->right;
            i+=3;
        }
        if(line[i] == '0' && line[i-1] == '0') {
            printf("%c", curr->symb + 32);
            i+=1;
            curr = T->root;
        }
        if(line[i] == '0' && line[i-1] == '0' && line[i+1] == '0') {
            printf(" ");
            i+=2;
        }
    }
}

int main(int argc, const char * argv[]) {
    Tree *T = new Tree;

    T->createTree();

    char line[1000], result[1000];
    FILE *f = fopen("input.txt", "r+");
    fgets(line, 100, f);

    code(line, result);
    printf("Input : %s\nMorze :%s\nDecoded : ", line, result);
    decode(result, T);
    printf("\n");
    //011010101001011010110000101100
    return 0;
}



