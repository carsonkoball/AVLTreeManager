#include "avlClass.h"
#include "customErrorClass.h"

/*
**    Author: Carson Koball
**    Function Purpose: 
**        To call the class function processCommands() to create an AVL tree and manage it using commands and values from a given file in the command line.
**    Function Output: 0 if no errors were caught from functions referenced within main(). If errors are thrown, they will be caught within the processCommands() function.
**    Side Effects: Individual functions should be referenced for more details.
*/

int main(int argc, char * argv[]) {

    C_AVLTree myTree;
    myTree.processCommands(argc, argv);

    return 0;
    
}