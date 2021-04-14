#ifndef __AVL_CLASS__H__
#define __AVL_CLASS__H__

#include <iomanip>
#include <iostream>
#include <sys/stat.h>
#include <cstring>
#include <cmath>
#include <fstream>
#include "customErrorClass.h"

using namespace std;

struct S_NODE {
    int value;
    int LHeight;
    int RHeight;
    S_NODE * parent;
    S_NODE * LChild;
    S_NODE * RChild;
};

class C_AVLTree {
    public:
        C_AVLTree();
        ~C_AVLTree();
        void processCommands(int, char **);
        void insertValue(int);
        void deleteValue(int);
        void printTree();
        void deleteTree();
        void exitProgram();
        void testRotate(int);

    private:
        S_NODE * root;

        S_NODE * p_createNode(int);
        S_NODE * p_insertValue(S_NODE *, S_NODE *);
        S_NODE * p_findValue(S_NODE *, int);
        void p_printTree(S_NODE *, int);
        void p_deleteTree(S_NODE *);
        void p_deleteValue(S_NODE *);
        void p_rotateLeft(S_NODE *);
        void p_rotateRight(S_NODE *);
        bool p_validateInput(int, char **);
        S_NODE * p_balance(S_NODE *, S_NODE *);
        S_NODE * p_scenarioLL(S_NODE *);
        S_NODE * p_scenarioLR(S_NODE *);
        S_NODE * p_scenarioRL(S_NODE *);
        S_NODE * p_scenarioRR(S_NODE *);
        int p_findHeight(S_NODE *);
};

#endif