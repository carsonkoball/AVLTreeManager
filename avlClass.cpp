#include "avlClass.h"
#include "customErrorClass.h"

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To construct an instance of the C_AVLTree class.
**		Function Output: An instance of the C_AVLTree class with class pointer root initialized.
**		Side Effects: Class pointer root initialized to NULL.
*/

C_AVLTree::C_AVLTree(void) {

    this->root = NULL;

}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To deconstruct an instance of the C_AVLTree class.
**		Function Output: Nothing
**		Side Effects: the p_deleteTree() function is called with class pointer root as a parameter. Class pointer root is then set to NULL.
*/

C_AVLTree::~C_AVLTree(void) {

    this->p_deleteTree(this->root);
    this->root = NULL;

}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To notify the user that the program is exiting after the exit command has been invoked.
**		Function Output: Nothing
**		Side Effects: Nothing.
*/

void C_AVLTree::exitProgram(void) {

    if (this->root != NULL) {
        throw (MyException("ERROR: Exiting program with a non-empty tree..."));
    }

}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To read in commands from an existing, filled command file that is entered in the command line.
**		Function Output: Nothing
**		Side Effects: the p_validateInput() function is called with argc and argv passed as parameters. If the file sent in the command line is valid, the program will begin reading the commands and values in the file and calling the respective functions.
*/

void C_AVLTree::processCommands(int argc, char * argv[]) {

    bool inputStatus = false;

    try {
        inputStatus = this->p_validateInput(argc, argv);
    } catch (MyException &errString) {
        cout<<errString.what()<<endl;
    }

    if (inputStatus == true) {
        ifstream myFile;
        myFile.open(argv[1]);

        int command, value;

        if (myFile.is_open()) {
            do {
                try {
                    myFile >> command;
                    switch(command) {
                        case 0:
                            myFile >> value;
                            this->insertValue(value);
                            break;
                        case 1:
                            myFile >> value;
                            this->deleteValue(value);
                            break;
                        case 2:
                            this->printTree();
                            break;
                        case 8:
                            this->deleteTree();
                            break;
                        case 9:
                            // insert exit function here?
                            this->exitProgram();
                            break;
                    }
                } catch (MyException &errString) {
                    cout<<errString.what()<<endl;
                }
            } while (command != 9);
        }
    }
}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To call p_findValue(), p_createNode(), and p_insertValue() to confirm that the given value is unique, to create a new node with the given value if it is unique, and to insert said node into the tree respectively. If the value is not unique, an error is thrown.
**		Function Output: Nothing.
**		Side Effects: A new S_NODE structure is created with the given value if said value is unique. The class pointer root may also be pointed to the created structure if the tree is already empty.
*/

void C_AVLTree::insertValue(int valueToInsert) {

    if (p_findValue(this->root, valueToInsert) == NULL) {
        // invoke p_createNode() function to create memory for new node
        S_NODE * newNode = this->p_createNode(valueToInsert);

        if (NULL == this->root) {
            this->root = newNode;
        } else {
            this->p_insertValue(this->root, newNode);
        }
    } else {
        throw (MyException("ERROR: Value already exists in tree - cannot insert..."));
    }

}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To call p_printTree() to print a formatted version of the tree. If the tree is empty, an error is thrown.
**		Function Output: Nothing.
**		Side Effects: Nothing.
*/

void C_AVLTree::printTree(void) {
    
    if (this->root != NULL) {
        cout<<"KEY: (\e[0;32mLHeight\e[0m-Value-\e[0;95mRHeight\e[0m)"<<endl<<endl;
        this->p_printTree(this->root, 0);
    } else {
        throw (MyException("ERROR: Tree does not exist - nothing to print..."));
    }

}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To call the p_deleteTree() function if the tree is not empty. If the tree is empty, an error is thrown.
**		Function Output: Nothing.
**		Side Effects: If the tree is not empty, the class pointer root is set to NULL after the p_deleteTree() function is called.
*/

void C_AVLTree::deleteTree(void) {
    
    if (this->root != NULL) {
        this->p_deleteTree(this->root);
        this->root = NULL;
    } else {
        throw (MyException("ERROR: Tree does not exist - nothing to delete..."));
    }

}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To call the p_findValue() and p_deleteValue() functions to see if a value exists in the tree and to delete said value if it exists in the tree. If said value does not exist in the tree, an error is thrown.
**		Function Output: Nothing.
**		Side Effects: A nodeToDelete pointer points to the input value if it is found and points to NULL if it is not found.
*/

void C_AVLTree::deleteValue(int valueToDelete) {
    
    S_NODE * nodeToDelete = this->p_findValue(this->root, valueToDelete);

    if (nodeToDelete != NULL) {
        this->p_deleteValue(nodeToDelete);
    } else {
        throw (MyException("ERROR: Value of " + to_string(valueToDelete) + " not found in tree - nothing to delete..."));
    }

}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To create a new S_NODE structure that contains a given input value.
**		Function Output: The newly created S_NODE structure.
**		Side Effects: The LChild, RChild, and parent pointers of the newly created S_NODE structure are set to NULL. The LHeight and RHeight values of the newly created S_NODE are set to 0.
*/

S_NODE * C_AVLTree::p_createNode(int valueInput) {

    S_NODE * newNode = new S_NODE;

    newNode->value = valueInput;
    newNode->LHeight = 0;
    newNode->RHeight = 0;
    newNode->parent = NULL;
    newNode->LChild = NULL;
    newNode->RChild = NULL;

    return newNode;

}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To recursively insert a given S_NODE structure into the tree. If at any time a S_NODE structures LHeight and RHeight values differ by 2 or more, the p_balance() function will be called to correct this.
**		Function Output: The position of where the given S_NODE structure will be inserted.
**		Side Effects: The LHeight and RHeight values for the S_NODES between the imbalanced S_NODE and the root S_NODE are modified.
*/

S_NODE * C_AVLTree::p_insertValue(S_NODE * parentNode, S_NODE * nodeToInsert) {

    if (NULL == parentNode) {
        return nodeToInsert;
    }

    nodeToInsert->parent = parentNode;
    
    // travel down the left branch of the node
    if (nodeToInsert->value < parentNode->value) {
        parentNode->LChild = this->p_insertValue(parentNode->LChild, nodeToInsert);
    // travel down the right branch of the node
    } else {
        parentNode->RChild = this->p_insertValue(parentNode->RChild, nodeToInsert);
    }

    // as the node works its way down the tree, the height values of the nodes that it passes will be updated on return
    parentNode->RHeight = this->p_findHeight(parentNode->RChild);
    parentNode->LHeight = this->p_findHeight(parentNode->LChild);

    // the p_balance() function will be called if a node's LHeight and RHeight values differ by 2 or more.
    if (abs(parentNode->LHeight - parentNode->RHeight) >= 2) {
        return this->p_balance(parentNode, nodeToInsert);
    }

    return parentNode;

}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To recursively locate the S_NODE structure that contains a given value found within a tree or subtree.
**		Function Output: The S_NODE structure that contains the input given value.
**		Side Effects: Nothing.
*/

S_NODE * C_AVLTree::p_findValue(S_NODE * travelNode, int valueToFind) {

    // node does not exist
    if (travelNode == NULL) {
        return travelNode;
    } else {
        // if value we are looking for is less than current node value, move left
        if (valueToFind < travelNode->value) {
            travelNode = p_findValue(travelNode->LChild, valueToFind);
        // if value we are looking for is greater than current node value, move right
        } else if (valueToFind > travelNode->value) {
            travelNode = p_findValue(travelNode->RChild, valueToFind);
        }
    }

    return travelNode;

}

/*
**		Author: Alex Wollman / Carson Koball
**		Function Purpose: 
**			To recursively print the binary search tree in a graphical manner to the console with a given indent value.
**		Function Output: Nothing.
**		Side Effects: Nothing.
*/

void C_AVLTree::p_printTree(S_NODE * travelNode, int indent) {

    if (travelNode == NULL){
        return;
    } else {
        this->p_printTree(travelNode->LChild, indent+8);
        if( indent ){
            cout<<setw(indent)<<" ";
        }
        cout<<"(\e[0;32m"<<travelNode->LHeight<<"\e[0m-"<<travelNode->value<<"-\e[0;95m"<<travelNode->RHeight<<"\e[0m)"<<endl;
        this->p_printTree(travelNode->RChild, indent+8);
    }

}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To recursively remove all S_NODE structures from the tree.
**		Function Output: Nothing.
**		Side Effects: All S_NODE structures in the tree are removed from memory.
*/

void C_AVLTree::p_deleteTree(S_NODE * travelNode) {

    if (travelNode == NULL) {
        return;
    } else {
        // in order deletion of tree
        this->p_deleteTree(travelNode->LChild);
        this->p_deleteTree(travelNode->RChild);
        travelNode->parent = NULL;
        delete travelNode;
    }

}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To delete a given S_NODE structure from the tree.
**		Function Output: Nothing.
**		Side Effects: The S_NODE structure will be removed from memory.
*/

void C_AVLTree::p_deleteValue(S_NODE * nodeToDelete) {
    
    cout<<"Deleting value of "<<nodeToDelete->value<<" from tree..."<<endl;

}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To rotate a given pivot node's parent to the left of said pivot node.
**		Function Output: Nothing.
**		Side Effects: The given pivot node will become the parent of the old parent node.
*/

void C_AVLTree::p_rotateLeft(S_NODE * pivotNode) {

    S_NODE * parentNode = pivotNode->parent;
    
    if (pivotNode->LChild != NULL) {
        pivotNode->LChild->parent = parentNode;
    }
    parentNode->RChild = pivotNode->LChild;
    pivotNode->LChild = parentNode;

    if (parentNode->parent == NULL) {
        this->root = pivotNode;
    } else {
        if (parentNode->parent->RChild == parentNode) {
            parentNode->parent->RChild = pivotNode;
        } else {
            parentNode->parent->LChild = pivotNode;
        }
    }

    pivotNode->parent = parentNode->parent;
    parentNode->parent = pivotNode;

}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To rotate a given pivot node's parent to the right of said pivot node.
**		Function Output: Nothing.
**		Side Effects: The given pivot node will become the parent of the old parent node.
*/

void C_AVLTree::p_rotateRight(S_NODE * pivotNode) {

    S_NODE * parentNode = pivotNode->parent;
    
    if (pivotNode->RChild != NULL) {
        pivotNode->RChild->parent = parentNode;
    }

    parentNode->LChild = pivotNode->RChild;
    pivotNode->RChild = parentNode;
    
    if (parentNode->parent == NULL) {
        this->root = pivotNode;
    } else {
        if (parentNode->parent->RChild == parentNode) {
            parentNode->parent->RChild = pivotNode;
        } else {
            parentNode->parent->LChild = pivotNode;
        }
    }

    pivotNode->parent = parentNode->parent;
    parentNode->parent = pivotNode;

}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To validate that the input file from the command line is entered, that it exists, and that it is not empty. If any of these three rules are broken, an error is thrown.
**		Function Output: False is returned if any of the above three rules are broken, and true if all three rules above are maintained.
**		Side Effects: Nothing.
*/

bool C_AVLTree::p_validateInput(int argc, char * argv[]) {

    bool returnValue = false;
    struct stat fileInformation;
    string errorString;
    int error;

    if (1 >= argc || 4 <= argc) {
        errorString = "ERROR: Incorrect usage (Correct usage: ";
        errorString += argv[0];
        errorString += " [FILENAME])...";
        throw MyException(errorString);
    } else {
        error = stat(argv[1], &fileInformation);

        if (0 != error) {
            errorString = "ERROR: ";
            errorString += strerror(errno);
            errorString += ": ";
            errorString += argv[1];
            throw MyException(errorString);
        }

        if (0 == fileInformation.st_size) {
            errorString = "ERROR: File is empty: ";
            errorString += argv[1];
            throw MyException(errorString);
        }

        returnValue = true;
    }

    return returnValue;

}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To rebalance the tree if any given unbalanced node's LHeight and RHeight values differ by 2 or more. First, the scenario is determined by searching the subtrees of the unbalanced children. Once the scenario type is identified, the unbalanced node is passed to the function that will handle that scenario.
**		Function Output: The new root of the previously unbalanced subtree.
**		Side Effects: The scenario functions should be referenced for further side effects.
*/

S_NODE * C_AVLTree::p_balance(S_NODE * unbalancedNode, S_NODE * newlyInsertedNode) {

    if (unbalancedNode->LChild != NULL) {
        if (unbalancedNode->LChild->LChild != NULL) {
            if (this->p_findValue(unbalancedNode->LChild->LChild, newlyInsertedNode->value) != NULL) {
                // Left-Left unbalanced scenario
                return this->p_scenarioLL(unbalancedNode);
            }
        }
        if (unbalancedNode->LChild->RChild != NULL) {
            if (this->p_findValue(unbalancedNode->LChild->RChild, newlyInsertedNode->value) != NULL) {
                // Left-Right unbalanced scenario
                return this->p_scenarioLR(unbalancedNode);
            }
        }
    }

    if (unbalancedNode->RChild != NULL) {
        if (unbalancedNode->RChild->LChild != NULL) {
            if (this->p_findValue(unbalancedNode->RChild->LChild, newlyInsertedNode->value) != NULL) {
                // Right-Left unbalanced scenario
                return this->p_scenarioRL(unbalancedNode);
            }
        }
    }

    // None of the above scenarios (Right-Right unbalanced scenario)
    return this->p_scenarioRR(unbalancedNode);

}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To rebalance the unbalanced subtree by rotating right on the left child of the unbalanced node. Additionally, the unbalanced node, the unbalanced node's left child, and the unbalanced node's left left grandchild will have their LHeight and RHeight values updated. 
**		Function Output: The left left grandchild of the given unbalanced node.
**		Side Effects: Nothing.
*/

S_NODE * C_AVLTree::p_scenarioLL(S_NODE * grandParentNode) {

    S_NODE * pivotNode = grandParentNode->LChild;
    S_NODE * childNode = pivotNode->LChild;

    this->p_rotateRight(pivotNode);

    //update heights for localized change
    grandParentNode->RHeight = this->p_findHeight(grandParentNode->RChild);
    grandParentNode->LHeight = this->p_findHeight(grandParentNode->LChild);
    pivotNode->RHeight = this->p_findHeight(pivotNode->RChild);
    pivotNode->LHeight = this->p_findHeight(pivotNode->LChild);
    childNode->RHeight = this->p_findHeight(childNode->RChild);
    childNode->LHeight = this->p_findHeight(childNode->LChild);

    return pivotNode;

}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To rebalance the unbalanced subtree by rotating left on the left right grandchild of the unbalanced node and rotating right on the same node. Additionally, the unbalanced node, the unbalanced node's left child, and the unbalanced node's left right grandchild will have their LHeight and RHeight values updated. 
**		Function Output: The left right grandchild of the given unbalanced node.
**		Side Effects: Nothing.
*/

S_NODE * C_AVLTree::p_scenarioLR(S_NODE * grandParentNode) {

    S_NODE * pivotNode = grandParentNode->LChild->RChild;
    S_NODE * parentNode = grandParentNode->LChild;

    this->p_rotateLeft(pivotNode);
    this->p_rotateRight(pivotNode);

    //update heights for localized change
    grandParentNode->RHeight = this->p_findHeight(grandParentNode->RChild);
    grandParentNode->LHeight = this->p_findHeight(grandParentNode->LChild);
    pivotNode->RHeight= this->p_findHeight(pivotNode->RChild);
    pivotNode->LHeight= this->p_findHeight(pivotNode->LChild);
    parentNode->RHeight= this->p_findHeight(parentNode->RChild);
    parentNode->LHeight= this->p_findHeight(parentNode->LChild);

    return pivotNode;

}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To rebalance the unbalanced subtree by rotating right on the right left grandchild of the unbalanced node and rotating left on the same node. Additionally, the unbalanced node, the unbalanced node's right child, and the unbalanced node's right left grandchild will have their LHeight and RHeight values updated. 
**		Function Output: The left right grandchild of the given unbalanced node.
**		Side Effects: Nothing.
*/

S_NODE * C_AVLTree::p_scenarioRL(S_NODE * grandParentNode) {

    S_NODE * pivotNode = grandParentNode->RChild->LChild;
    S_NODE * parentNode = grandParentNode->RChild;

    this->p_rotateRight(pivotNode);
    this->p_rotateLeft(pivotNode);

    //update heights for localized change
    grandParentNode->RHeight = this->p_findHeight(grandParentNode->RChild);
    grandParentNode->LHeight = this->p_findHeight(grandParentNode->LChild);
    pivotNode->RHeight= this->p_findHeight(pivotNode->RChild);
    pivotNode->LHeight= this->p_findHeight(pivotNode->LChild);
    parentNode->RHeight= this->p_findHeight(parentNode->RChild);
    parentNode->LHeight= this->p_findHeight(parentNode->LChild);

    return pivotNode;

}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To rebalance the unbalanced subtree by rotating left on the right child of the unbalanced node. Additionally, the unbalanced node, the unbalanced node's right child, and the unbalanced node's right right grandchild will have their LHeight and RHeight values updated. 
**		Function Output: The left right grandchild of the given unbalanced node.
**		Side Effects: Nothing.
*/

S_NODE * C_AVLTree::p_scenarioRR(S_NODE * grandParentNode) {

    S_NODE * pivotNode = grandParentNode->RChild;
    S_NODE * childNode = pivotNode->RChild;

    this->p_rotateLeft(pivotNode);

    //update heights for localized change
    grandParentNode->RHeight = this->p_findHeight(grandParentNode->RChild);
    grandParentNode->LHeight = this->p_findHeight(grandParentNode->LChild);
    pivotNode->RHeight= this->p_findHeight(pivotNode->RChild);
    pivotNode->LHeight= this->p_findHeight(pivotNode->LChild);
    childNode->RHeight= this->p_findHeight(childNode->RChild);
    childNode->LHeight= this->p_findHeight(childNode->LChild);

    return pivotNode;

}

/*
**		Author: Carson Koball
**		Function Purpose: 
**			To recursively find the height of the largest subtree of a given node. 
**		Function Output: The maximum height of a given node's subtree.
**		Side Effects: Nothing.
*/

int C_AVLTree::p_findHeight(S_NODE * originNode)
{
    if (originNode == NULL) {
        return 0;
    } else {
        int leftHeight = this->p_findHeight(originNode->LChild);
        int rightHeight = this->p_findHeight(originNode->RChild);
     
        if (leftHeight > rightHeight) {
            return leftHeight + 1;
        } else {
            return rightHeight + 1;
        }
    }
}