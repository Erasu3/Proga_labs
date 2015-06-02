#include <stdio.h>
#include <vector>
#include <map>
#include <list>
#include <fstream>


using namespace std;

const int MAX_NAME = 50;
bool isOver = false;

class Node{
public:
        char chr;
        int num;
        Node *right, *left;
        Node();
        Node(Node *One, Node *Two);
        Node(char _char, int _num);
        ~Node();
};

Node::Node(){
        num = 0;
        chr = NULL;
        right = NULL;
        left = NULL;
}

Node::Node(Node *One, Node *Two){
        left = One;
        right = Two;
        num = One->num + Two->num;
        chr = NULL;
}

Node::Node(char c, int num){
        left = NULL;
        right = NULL;
        this->num = num;
        chr = c;
}

Node::~Node(){
}



struct cmp{
        bool operator()(const Node* l, const Node* r) const { return l->num < r->num; }
};

bool pairCompare(const std::pair<char, int> firstElem, const std::pair<char, int> secondElem) {
        return firstElem.second > secondElem.second;

}

bool pCmp(const std::pair<char, double> firstElem, const std::pair<char, double > secondElem) {
        return firstElem.second > secondElem.second;

}

void HuffmanEncode(char *iName, char *oName, char *dictName);
void HuffmanDecode(char *iName, char *oName, char *dictName);


void getCode(Node *Head, vector <int> &code, map <char, vector <int> > &voc);
void getText(map <char, int> &ch, char *iName);
void out(map <char, vector <int> > voc, map <char, int> &ch, char *iName, char *oName, char *dictName);


int main() {
        int choice;
        printf("1. Encode\n"
               "2. Decode\n");

        scanf("%d", &choice);

        switch (choice) {
        case 1:
                HuffmanEncode("input.txt", "out.bin", "dic.txt");
                break;
        case 2:
                HuffmanDecode("out.bin", "res.txt", "dic.txt");
                break;
        default:
                break;
        }
}

void HuffmanEncode(char *iName, char *oName, char *dictName){
        map<char, int> ch;
        vector<int> code;
        map <char, vector<int> > voc;

        getText(ch, iName);

        if (isOver)
                return;

        list<Node*> t;
        for (map<char, int>::iterator itr = ch.begin(); itr != ch.end(); ++itr){
                Node *p = new Node;
                p->chr = itr->first;
                p->num = itr->second;
                t.push_back(p);
        }

        while (t.size() != 1){
                t.sort(cmp());

                Node *SonL = t.front();
                t.pop_front();
                Node *SonR = t.front();
                t.pop_front();

                Node *parent = new Node(SonL, SonR);
                t.push_back(parent);

        }

        Node *head = t.front();

        getCode(head, code, voc);
        out(voc, ch, iName, oName, dictName);
}

void HuffmanDecode(char *iName, char *oName, char *dictName){
        int tmp, b;
        map<char, int> ch;
        vector<int> code;

        FILE *dict = fopen(dictName, "rt");
        FILE *out = fopen(oName, "wt");

        if (dict == NULL){
                printf("Error. Cannot open %s.", dictName);
                isOver = true;
                return;
        }
        while (fscanf(dict, "%d %d", &tmp, &b) > 0){
                ch[tmp] = b;
        }

        list<Node*> t;
        for (map<char, int>::iterator itr = ch.begin(); itr != ch.end(); ++itr){
                Node *p = new Node;
                p->chr = itr->first;
                p->num = itr->second;
                t.push_back(p);
        }

        while (t.size() != 1){
                t.sort(cmp());

                Node *SonL = t.front();
                t.pop_front();
                Node *SonR = t.front();
                t.pop_front();

                Node *parent = new Node(SonL, SonR);
                t.push_back(parent);

        }

        Node *head = t.front();

        Node *p = head;
        char byte;
        bool bit;
        int cnt = 0;

        ifstream in(iName, ios::in | ios::binary);

        byte = in.get();
        while (!in.eof()){
                bit = byte & (1 << (7 - cnt));
                if (bit) p = p->right; else p = p->left;
                if (p->left == NULL && p->right == NULL){
                        fprintf(out, "%c", p->chr);
                        p = head;
                }
                cnt++;
                if (cnt == 8) {
                        cnt = 0;
                        byte = in.get();
                }
        }

        in.close();
        fclose(dict);
        fclose(out);
}

void getText(map <char, int> &ch, char *iName){
        FILE *input = fopen(iName, "rt");

        if (input == NULL){
                printf("Error. Cannot open %s.", iName);
                isOver = true;
                return;
        }

        while (!feof(input)){
                int c = fgetc(input);
                if (c == -1)
                        break;
                ch[c]++;
        }
        fclose(input);
}

void out(map <char, vector <int> > voc, map <char, int> &ch, char *iName, char *oName, char *dictName){
        char c;
        double average;
        double lenCode = 0, num = 0;
        int cnt = 0;
        char buf = 0;

        vector <int> tmp;
        FILE *input = fopen(iName, "rt");
        FILE *dict = fopen(dictName, "w");

        if (input == NULL){
                printf("Error. Cannot open %s.", iName);
                isOver = true;
                return;
        }

        map <char, int>::iterator i;
        map <char, vector <int> >::iterator itr;
        for (i = ch.begin(), itr = voc.begin(); i != ch.end(); i++, itr++){
                fprintf(dict, "%d %d\n", i->first, i->second);
                tmp = itr->second;
                num += i->second;
                lenCode += (i->second * tmp.size());
        }

        ofstream out(oName, ios::out | ios::binary);

        while (!feof(input)){
                c = fgetc(input);
                vector<int> x = voc[c];
                for (int i = 0; i < x.size(); i++){
                        buf = buf | x[i] << (7 - cnt);
                        cnt++;
                        if (cnt == 8) { cnt = 0;   out << buf; buf = 0; }
                }
        }

        average = lenCode / num;
//        printf("Average length of code word: %0.2f", average);
        printf("Done \n");

        fclose(input);
        fclose(dict);
        out.close();
}

void getCode(Node *Head, vector <int> &code, map <char, vector <int> > &voc){

        if (Head->left != NULL) {
                code.push_back(0);
                getCode(Head->left, code, voc);
        }

        if (Head->right != NULL) {
                code.push_back(1);
                getCode(Head->right, code, voc);
        }

        if (Head->chr)
                voc[Head->chr] = code;

        if (!code.empty())
                code.pop_back();
}
