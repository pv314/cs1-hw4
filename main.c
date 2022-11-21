/* COP 3502C Assignment 4
This program is written by: Heng Hong */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"
#define MAXLEN 50

typedef struct itemNode
{
    char name[MAXLEN];
    int count;
    struct itemNode *left, *right;
}itemNode;

typedef struct treeNameNode
{
    char treeName[MAXLEN];
    struct treeNameNode *left, *right;
    itemNode *theTree;
}treeNameNode;

treeNameNode* createTreeNameNode(char* treeName){
    treeNameNode* nameNode = malloc(sizeof (treeNameNode));
    strcpy(nameNode->treeName,treeName);
    nameNode->left =NULL;
    nameNode->right = NULL;
    nameNode->theTree = NULL;
    return  nameNode;
}

itemNode *createItemNode(char *name, int count){
    itemNode *node = malloc(sizeof(itemNode));
    strcpy(node->name, name);
    node->count = count;
    node->left = NULL;
    node->right = NULL;
    return node;
}

treeNameNode* insertTreeNameNode(treeNameNode *root, treeNameNode *element) {

    if (root == NULL)
        return element;

    //new coming is bigger than root,go right
    if (strcmp(element->treeName,root->treeName) > 0)
        root->right = insertTreeNameNode(root->right, element);
    else
        root->left = insertTreeNameNode(root->left, element);

    return root;
}

itemNode* insertItemNode(itemNode* root, itemNode* element) {
    if (root == NULL)
        return element;

    //new coming is bigger than root ,go right
    if (strcmp(element->name,root->name) > 0)
        root->right = insertItemNode(root->right, element);
    else
        root->left = insertItemNode(root->left, element);

    return root;
}

treeNameNode* buildNameTree(int n){

    treeNameNode*  nameNode = NULL;
    char name[MAXLEN];

    for(int i =0;i < n; i++){
        //read in tree name
        scanf("%s",name);

        //create a temp box to receive the super node info
        treeNameNode* tempNameNode = createTreeNameNode(name);

        //get the node ready and insert the information received before
        nameNode = insertTreeNameNode(nameNode,tempNameNode);
    }

    return nameNode;
}

treeNameNode* searchNameNode(treeNameNode * root, char treeName[MAXLEN]){

    if(root == NULL )
        return NULL;

    treeNameNode* nameNode;

    //determine which side need to go to.
    if(strcmp(treeName,root->treeName) == 0)
        return root;
    else if(strcmp(treeName,root->treeName) < 0){
        nameNode = searchNameNode(root->left,treeName);
    }else {
        nameNode = searchNameNode(root->right,treeName);
    }
    return nameNode;
}

void buildItemTree(treeNameNode* root, int I){

    char treeName[MAXLEN],itemName[MAXLEN];
    int cnt;
    for(int i = 0;i < I; i++){

        //read in the item in the list info
        scanf("%s %s %d",treeName,itemName,&cnt);

        //create a temp box to receive the sub node info
        itemNode* tempItemName = createItemNode(itemName,cnt);

        //find the sub root properly and prepare insert;
        treeNameNode* treeNode = searchNameNode(root, treeName);

        //get the right position and insert the sub node info received before;
        treeNode->theTree = insertItemNode(treeNode->theTree,tempItemName);

    }
}

int searchItemTree(itemNode* root, char* itemName){

    if (root == NULL)
        return 0;

    //determine which side need go for beside the root is null
    if (strcmp(root->name, itemName) == 0)
        return root->count;
    else if (strcmp(root->name, itemName) > 0)
        return searchItemTree(root->left, itemName);
    else
        return searchItemTree(root->right, itemName);
}

void showTreeName(treeNameNode* root,FILE *fPtr) {

    if(root != NULL) {

        //inorder traverse for tree name
        showTreeName(root->left,fPtr);
        printf("%s ", root->treeName);
        fprintf(fPtr,"%s ", root->treeName);
        showTreeName(root->right,fPtr);
    }
}

void traverseItemTree(itemNode * root,FILE *fPtr) {

    if (root != NULL) {

        //traverse sub tree item
        traverseItemTree(root->left, fPtr);
        printf("%s ", root->name);
        fprintf(fPtr, "%s ", root->name);
        traverseItemTree(root->right, fPtr);
    }

}

void traverse_in_traverse(treeNameNode* root,FILE *fPtr) {

    if(root != NULL) {

        //go through top tree by indoor traverse
        traverse_in_traverse(root->left,fPtr);
        printf("\n===%s===\n", root->treeName);
        fprintf(fPtr, "\n===%s===\n", root->treeName);
        traverseItemTree(root->theTree,fPtr);
        traverse_in_traverse(root->right,fPtr);
    }
}


void search(treeNameNode *root, char *treeName, char *nodeName,FILE *fPtr){
    treeNameNode *tree = searchNameNode(root, treeName);

    //if tree is not exist
    if (tree == NULL) {
        printf("\n%s does not exist", treeName);
        fprintf(fPtr, "\n%s does not exist", treeName);

    }else{
        //create a box to receive the count of item from sub tree
        int result = searchItemTree(tree->theTree, nodeName);
        if (result == 0){
            printf("\n%s not found in %s", nodeName, treeName);
            fprintf(fPtr, "\n%s not found in %s", nodeName, treeName);

        }else{
            printf("\n%d %s found in %s", result, nodeName, treeName);
            fprintf(fPtr, "\n%d %s found in %s", result, nodeName, treeName);

        }
    }
}

void freeNode(itemNode *root){
    if (root != NULL){

        //free node for both side
        freeNode(root->left);
        freeNode(root->right);
        free(root);
    }
}

void freeTree(treeNameNode *root){
    if (root != NULL){

        //free both side top tree

        freeTree(root->left);
        freeTree(root->right);
        freeNode(root->theTree);
        free(root);
    }
}

int countNode (itemNode* root, char* nodeName) {

    if(root == NULL)
        return 0;
    //if there is only one node
    if(strcmp(root->name,nodeName) == 0)
        return 1;
        //recursively go deep count the number for either side
    else if(strcmp(root->name,nodeName) > 0)
        return 1 + countNode (root->left, nodeName);
    else
        return 1 + countNode(root->right,nodeName);
}

void item_before(treeNameNode* root, char* treeName, char* itemName,FILE *fPtr){

    //create the box to access the top tree
    treeNameNode *topTree = searchNameNode(root,treeName);

    // receive the box for the count and printf
    int cunt = countNode(topTree->theTree,itemName);

    printf("\nitem before %s: %d", itemName, 1 + cunt);
    fprintf(fPtr, "\nitem before %s: %d", itemName, 1+ cunt);


}

int calcLeft(itemNode *root){
    int left = 0;
    //go to left side deepest and keep count
    while (root->left != NULL){
        left++;
        root = root->left;
    }
    //and check the right side to keep it add up
    if (root->right != NULL){
        while (root->right != NULL){
            left++;
            root = root->right;
        }
    }
    return left;
}

int calcRight(itemNode *root){
    int right = 0;

    //same idea for calcLeft function
    while (root->right != NULL){
        right++;
        root = root->right;
    }
    if (root->left != NULL){
        while (root->left != NULL){
            right++;
            root = root->left;
        }
    }
    return right;
}

void height_balance(treeNameNode *root, char *treeName,FILE *fPtr){

    treeNameNode *tree = searchNameNode(root, treeName);

    int left,right,height;

    //if left node or right node is empty
    if (tree->theTree->left == NULL)
        left = -1;
    if (tree->theTree->right == NULL)
        right = -1;

    //create a box to receive the cunt from side if there are no empty
    if (tree->theTree->left != NULL){
        left = calcLeft(tree->theTree->left);
    }
    if (tree->theTree->right != NULL){
        right = calcRight(tree->theTree->right);
    }

    //absolute value
    height = abs(right - left);

    printf("\n%s: left height %d, right height %d, difference %d, %s",
           treeName, left, right, height, height == 0 ? "balanced" : "not balanced");
    fprintf(fPtr, "\n%s: left height %d, right height %d, difference %d, %s",
            treeName, left, right, height, height == 0 ? "balanced" : "not balanced");
}

int addAll(itemNode *root){
    if (root == NULL)
        return 0;
    else //Summing the values of nodes in a tree
        return root->count + addAll(root->left) + addAll(root->right);
}

void count (treeNameNode* root,char* treeName,FILE *fPtr) {
    treeNameNode *tree = searchNameNode(root, treeName);

    //create the box to receive the cunt of node and printf
    if (tree == NULL){
        printf("\n%s does not exist", treeName);
        fprintf(fPtr,"\n%s does not exist", treeName);

    }else{
        int  countTemp = addAll(tree->theTree);
        printf("\n%s count %d", treeName, countTemp);
        fprintf(fPtr,"\n%s count %d", treeName, countTemp);

    }

}

itemNode* parent(itemNode *root, itemNode *node) {
    // Take care of NULL cases.
    if (root == NULL || root == node)
        return NULL;
    // The root is the direct parent of node.
    if (root->left == node || root->right == node)
        return root;
    // Look for node's parent in the left side of the tree.
    if (strcmp(node->name, root->name) < 0)
        return parent(root->left, node);
        // Look for node's parent in the right side of the tree.
    else if (strcmp(node->name,root->name) > 0)
        return parent(root->right, node);
    return NULL; // Catch any other extraneous cases.
}

itemNode *minVal(itemNode *root) {
    // Root stores the minimal value.
    if (root->left == NULL)
        return root;
        // The left subtree of the root stores the minimal value.
    else
        return minVal(root->left);
}

itemNode *maxVal(itemNode *root) {
    // Root stores the maximal value.
    if (root->right == NULL)
        return root;
        // The right subtree of the root stores the maximal value.
    else
        return maxVal(root->right);
}

int isLeaf(itemNode *node) {
    return (node->left == NULL && node->right == NULL);
}

int hasOnlyLeftChild(itemNode *node) {
    // Returns 1 iff node has a left child and no right child.
    return (node->left != NULL && node->right == NULL);
}

int hasOnlyRightChild(itemNode *node) {
    // Returns 1 iff node has a right child and no left child.
    return (node->left == NULL && node->right != NULL);
}

itemNode* findNode(itemNode *current_ptr, char* itemValue) {
    // Check if there are nodes in the tree.
    if (current_ptr != NULL) {
        // Found the value at the root.
        if (strcmp(current_ptr->name,itemValue) == 0)
            return current_ptr;
        // Search to the left.
        if (strcmp(itemValue,current_ptr->name) < 0)
            return findNode(current_ptr->left, itemValue);
            // Or...search to the right.
        else
            return findNode(current_ptr->right, itemValue);
    } else
        return NULL; // No node found.
}

itemNode* deleteItem(itemNode *root, char *nodeName){

    if(!root) return NULL;

    itemNode *delnode, *new_del_node, *save_node;
    itemNode *par;
    char save_val[MAXLEN];

    delnode = findNode(root, nodeName);// Get a pointer to the node to delete.
    //printf("\ntotal cat before delete is %d---------\n",delnode->count);
    //printf("dfdsf");

    par = parent(root, delnode); // Get the parent of this node.

    // Take care of the case where the node to delete is a leaf node.
    if (isLeaf(delnode)){ // case 1

        // Deleting the only node in the tree.
        if (par == NULL){
            free(root); // free the memory for the node.
            return NULL;
        }

        // Deletes the node if it's a left child.
        if (strcmp(nodeName, par->name) > 0){
            free(par->left); // Free the memory for the node.
            par->left = NULL;
        }

            // Deletes the node if it's a right child.
        else{
            free(par->right); // Free the memory for the node.
            par->right = NULL;
        }
        strcpy(delnode->name, "");
        return root; // Return the root of the new tree.
    }

    // Take care of the case where the node to be deleted only has a left
    // child.
    if (hasOnlyLeftChild(delnode)){

        // Deleting the root node of the tree.
        if (par == NULL){
            save_node = delnode->left;
            free(delnode);    // Free the node to delete.
            return save_node; // Return the new root node of the resulting tree.
        }

        // Deletes the node if it's a left child.
        if (strcmp(nodeName, par->name) > 0){
            save_node = par->left;       // Save the node to delete.
            par->left = par->left->left; // Readjust the parent pointer.
            free(save_node);             // Free the memory for the deleted node.
        }

            // Deletes the node if it's a right child.
        else{
            save_node = par->right;        // Save the node to delete.
            par->right = par->right->left; // Readjust the parent pointer.
            free(save_node);               // Free the memory for the deleted node.
        }
        strcpy(delnode->name, "");
        return root; // Return the root of the tree after the deletion.
    }

    // Takes care of the case where the deleted node only has a right child.
    if (hasOnlyRightChild(delnode)){

        // Node to delete is the root node.
        if (par == NULL){
            save_node = delnode->right;
            free(delnode);
            return save_node;
        }

        // Delete's the node if it is a left child.
        if (strcmp(nodeName, par->name) > 0){
            save_node = par->left;
            par->left = par->left->right;
            free(save_node);
        }

            // Delete's the node if it is a right child.
        else{
            save_node = par->right;
            par->right = par->right->right;
            free(save_node);
        }
        return root;
    }
    //if your code reaches hear it means delnode has two children
    // Find the new physical node to delete.
    new_del_node = minVal(delnode->right);

    int save_num = new_del_node->count;
    strcpy(save_val, new_del_node->name);

    root =  deleteItem(root, save_val); // Now, delete the proper value.

    // Restore the data to the original node to be deleted.

    strcpy(delnode->name,save_val);
    delnode->count = 0;


    return root;
}

void delete(treeNameNode *root, char *treeName, char *nodeName, FILE *fPtr){

    treeNameNode *tree = searchNameNode(root, treeName);

    if (tree == NULL){
        printf("\n%s does not exist", treeName);
        fprintf(fPtr,"\n%s does not exist", treeName);

    }
    // getting the info from the delete and printf
    else{
        tree->theTree = deleteItem(tree->theTree, nodeName);
        printf("\n%s deleted from %s", nodeName, treeName);
        fprintf(fPtr,"\n%s deleted from %s", nodeName, treeName);

    }
}

void delete_name(treeNameNode *root, char *treeName,FILE *fPtr){

    treeNameNode *tree = searchNameNode(root, treeName);

    strcpy(tree->treeName, " ");
    tree = NULL;
    freeTree(tree);
    printf("\n%s deleted", treeName);
    fprintf(fPtr,"\n%s deleted", treeName);

}

void reduce(treeNameNode *root, char *treeName, char *nodeName, int count, FILE *fPtr){

        treeNameNode *tree = searchNameNode(root, treeName);

        //getting the right node
        itemNode *temp = findNode(tree->theTree, nodeName);

        //calc the count
        temp->count = temp->count - count;
        printf("\n%s reduced", nodeName);
        fprintf(fPtr,"\n%s reduced", nodeName);

        //if negative delete the node
        if (temp->count <= 0)
            tree->theTree = deleteItem(tree->theTree, nodeName);
}

void queries(int Q, treeNameNode *root, FILE *fPtr) {

    for (int i = 0; i < Q; i++) {

        char action[MAXLEN], tree[MAXLEN], item[MAXLEN];
        int cnt;

        scanf("%s", action);

        //using the read in action to determine the following scan needed
        if (strcmp(action, "search") == 0 || strcmp(action, "item_before") == 0 || strcmp(action, "delete") == 0) {
            scanf("%s %s", tree, item);
        }
        else if (strcmp(action, "height_balance") == 0 || strcmp(action, "count") == 0 ||
                 strcmp(action, "delete_name") == 0) {
            scanf("%s",tree);
        }
        else if (strcmp(action, "reduce") == 0) {
            scanf("%s %s %d", tree,item, &cnt);
        }

        //use the key word to call properly function
        if (strcmp(action, "search") == 0)
            search(root, tree, item,fPtr);
        else if (strcmp(action, "item_before") == 0)
            item_before(root, tree, item,fPtr);
        else if (strcmp(action, "height_balance") == 0)
            height_balance(root, tree,fPtr);
            //printf("\n execute height_balance function-----------here------------");
        else if (strcmp(action, "count") == 0)
            count(root,tree,fPtr);
            //printf("\n execute count function-----------here------------");
        else if (strcmp(action, "reduce") == 0)
            reduce(root, tree, item, cnt,fPtr);
            //printf("\n execute reduce function-----------here------------");
        else if (strcmp(action, "delete") == 0)
            delete(root, tree,item,fPtr);
        else if (strcmp(action, "delete_name") == 0)
            delete_name(root, tree,fPtr);

    }
}

int main() {

    atexit(report_mem_leak);
    int N, I, Q;

    FILE *fPtr = fopen("out.txt", "w");

    scanf("%d %d %d", &N, &I, &Q);

    treeNameNode *myNameRoot = buildNameTree(N);
    buildItemTree(myNameRoot, I);
    showTreeName(myNameRoot,fPtr);
    traverse_in_traverse(myNameRoot,fPtr);
    queries(Q, myNameRoot,fPtr);

    printf("\n");
    fprintf(fPtr,"\n");

    freeTree(myNameRoot);

    return 0;
}
