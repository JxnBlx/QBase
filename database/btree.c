#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "btree.h"
// #include <climits>

BPT* bptInit(int _t){
    BPT* tree = (BPT*)malloc(sizeof(BPT));
    tree->t = _t;
    tree->root = NULL;
    return tree;
}

void bptnInit(BPTNode* node, int _t){
    node->keys = (int*)malloc(sizeof(int)* (2*_t));
    for (int i = 0; i<2*_t;i++) node->keys[i] = INT_MAX;
    node->t = _t;
    node->n = 0;
    node->children = NULL;
    node->leaf = 0;
    node->next = NULL;
    node->prev = NULL;
    node->parent = NULL;
}

void insert(BPT* tree, int key){
    if (tree->root == NULL){
        BPTNode* root = (BPTNode*)malloc(sizeof(BPTNode));
        bptnInit(root, tree->t);
        root->leaf = 1;
        tree->root = root;
    }
    if (search(tree, key)) return;
    nodeInsert(tree->root, key);
}

void nodeInsert(BPTNode* root, int key){
    if (root == NULL){
        return;
    }
    if (root->leaf == 1){
        // If root is empty, insert key
        if (root->n == 0){
            root->keys[0] = key;
            root->n++;
        } else if (root->n <= 2*root->t) {  // else insert key in the correct position
            int i = 0;
            while(i < root->n && key > root->keys[i]){
                i++;
            }
            if (i < root->n){ // Shift keys to the right
                for (int j = root->n; j > i; j--){
                    root->keys[j] = root->keys[j-1];
                }
                root->keys[i] = key;
                root->n++;

            } else { // Insert key at the end
                root->keys[i] = key;
                root->n++;
            }
        }
        if (root->n == 2*root->t){
            nodeSplit(root);
        }
    } else {
        // Find the child to insert the key
        int i = 0;
        while(i <= root->n-1 && key > root->keys[i]){
            i++;
        }
        nodeInsert(root->children[i], key);
    }

}

void nodeSplit(BPTNode* root){
    // Create two new nodes
    BPTNode* leftNode = (BPTNode*)malloc(sizeof(BPTNode));
    BPTNode* rightNode = (BPTNode*)malloc(sizeof(BPTNode));
    // Initialize the nodes
    bptnInit(leftNode, root->t);
    bptnInit(rightNode, root->t);
    if (root->leaf){
        leftNode->next = rightNode;
        rightNode->prev = leftNode;
    }
    // Get the middle key
    int mid = root->keys[root->t-1];
    int midAdd = 0;
    if (root->leaf == 1){
        leftNode->leaf = 1;
        rightNode->leaf = 1;
        midAdd = 1;
    }
    // Copy the keys to the new nodes
    for (int i = 0; i < root->t-1+midAdd; i++){
        leftNode->keys[i] = root->keys[i];
        leftNode->n++;
    }
    for (int i = root->t; i < 2*root->t; i++){
        rightNode->keys[i-root->t] = root->keys[i];
        rightNode->n++;
    }
    // Copy the children if the node is not a leaf
    if (root->leaf == 0){
        leftNode->leaf = 0;
        rightNode->leaf = 0;
        leftNode->children = (BPTNode**)malloc(sizeof(BPTNode*)*(2*root->t+1));
        rightNode->children = (BPTNode**)malloc(sizeof(BPTNode*)*(2*root->t+1));
        for (int i = 0; i < root->t; i++){
            leftNode->children[i] = root->children[i];
            leftNode->children[i]->parent = leftNode;
        }
        for (int i = 0; i < root->t+1; i++){
            rightNode->children[i] = root->children[i+root->t];
            rightNode->children[i]->parent = rightNode;
        }
    }
    // Update the parent of the new nodes
    if (root->parent == NULL){
        // clear the root and set the new nodes as the children of the root
        for (int i = 0; i<root->n; i++){
            root->keys[i] = INT_MAX;
        }
        if (root->children == NULL){
            BPTNode** children = (BPTNode**)malloc(sizeof(BPTNode*)*(2*root->t+1));
            root->children = children;
        }
        for (int i = 0; i<root->n; i++){
            root->children[i] = NULL;
        }
        root->keys[0] = mid;
        root->children[0] = leftNode;
        root->children[1] = rightNode;
        root->n = 1;
        root->leaf = 0;
        leftNode->parent = root;
        rightNode->parent = root;
        // update linked list
        
    } else {
        // update parent
        BPTNode* parent = root->parent;
        int i = 0;
        while (i < parent->n && parent->keys[i] < mid) {
            i++;
        }
        for (int j = parent->n; j > i; j--) {
            parent->keys[j] = parent->keys[j - 1];
            parent->children[j + 1] = parent->children[j];
        }
        parent->keys[i] = mid;
        parent->children[i] = leftNode;
        parent->children[i + 1] = rightNode;
        parent->n++;
        leftNode->parent = parent;
        rightNode->parent = parent;
        //update linked list
        if (root->leaf){
            if (root->prev != NULL){
                root->prev->next = leftNode;
                leftNode->prev = root->prev;
            }
            if (root->next != NULL){
                root->next->prev = rightNode;
                rightNode->next = root->next;
            }
        }
        if (parent->n == 2 * parent->t) {
            nodeSplit(parent);
        }
    }

}

int search(BPT* tree, int key){
    if (tree->root == NULL || tree->root->n == 0){
        return 0;
    } else {
        return nodeSearch(tree->root, key);
    }
}

int nodeSearch(BPTNode* root, int key){
    if (root->keys[0] == INT_MAX){
        return 0;
    }

    int i = 0;
    while (i<root->n && key > root->keys[i]){
        i++;
    }
    if (root->leaf == 1) return root->keys[i] == key;
    else return nodeSearch(root->children[i], key);
}

int del(BPT* tree, int key){
    if (tree->root == NULL || tree->root->keys[0] == INT_MAX){
        return 0;
    }
    return nodeDel(tree->root, key);
}

int nodeDel(BPTNode* root, int key){
    int i = 0;
    while(key > root->keys[i] && i < root->n) i++;
    if (root->leaf){
        if (root->keys[i] != key) return 0; //key doesnt exist
        else{ //delete the key
            root->n--;
            if (root->n != 0){ //if node isn't empty
                for(i; i<root->n;i++){
                    root->keys[i] = root->keys[i+1];
                }
                root->keys[i] = INT_MAX;
            } else { //if node is now empty
                root->keys[0] = INT_MAX;
                clearEmptyNode(root);
            }
            return 1;
        }
    } else {
        return nodeDel(root->children[i], key);
    }
}

void clearEmptyNode(BPTNode* root){
    if (root->keys[0] != INT_MAX) return;
    else if (root->parent != NULL){
        BPTNode* parent = root->parent;
        int i = 0;
        while (i<parent->n && parent->children[i] != root) i++;
            parent->n;
            parent->children[i+1];
        if (root->leaf){
            if (root->prev != NULL){
                root->prev->next = root->next;
            }
            if (root->next != NULL){
                root->next->prev = root->prev;
            }
        }
        parent->n--;
        for (int j = i; j<parent->n; j++){
            parent->keys[j] = parent->keys[j+1];
        }
        for (i; i<=parent->n; i++){
            parent->children[i] = parent->children[i+1];
        }
        parent->keys[i] = INT_MAX;
        parent->children[i] = NULL;
        if (parent->keys[0] == INT_MAX) clearEmptyNode(parent);
    }
}

BPTNode* getClosestNode(BPTNode* node, int key){
    if (node->leaf) return node;
    int i = 0;
    while(i<node->n && key>node->keys[i]) i++;
    return getClosestNode(node->children[i], key);
}

int* getRange(BPT* tree, int start, int end){
    if (tree->root == NULL) return NULL;
    if (tree->root->keys[0] == INT_MAX) return NULL;
    BPTNode* closestStart = getClosestNode(tree->root, start);
    BPTNode* closestEnd = getClosestNode(tree->root, end);
    int len = 0; 
    int size = 4;
    int* nodes = (int*)malloc(sizeof(int)*4);
    while (closestStart != NULL && closestStart->prev != closestEnd){
        // printf("Closest start: %d\n", closestStart->keys[0]);
        int i = 0; 
        while (closestStart->keys[i] < start) i++;
        while(i<closestStart->n && closestStart->keys[i] <= end){
            // printf("node key: %d\n", closestStart->keys[i]);
            nodes[len] = closestStart->keys[i];
            len++;
            i++;
            
            if (len==size){
                int* temp = (int*)malloc(sizeof(int)*size*2);
                if (temp==NULL) printf("Memory allocation failed at size %d\n", size*2);
                for (int i = 0; i< size; i++){
                    temp[i] = nodes[i];
                }
                size *= 2;
            }
        }
        closestStart = closestStart->next;
    }
    if (len!=size) nodes[len] = -1;
    return nodes;
}

void freeTree(BPT* tree) {
    if (tree == NULL || tree->root == NULL) {
        free(tree);
        return;
    }
    freeNode(tree->root);
    free(tree);
}

void freeNode(BPTNode* root) {
    if (root == NULL) {
        return;
    }

    if (!root->leaf){
        for (int i = 0; i<root->n; i++){
            freeNode(root->children[i]);
        }
    }

    free(root->keys);

    if(root->children != NULL) free(root->children);

    free(root);

}