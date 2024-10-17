typedef struct BPTNode {
    int *keys;          // Array of keys
    int t;              // Minimum degree (defines the range for number of keys)
    struct BPTNode **children; // Array of child pointers
    int n;              // Current number of keys
    int leaf;           // Is true when node is leaf. Otherwise false
    struct BPTNode *next; // Next node in the linked list
    struct BPTNode *prev; // Prev node in the linked list
    struct BPTNode *parent; // Parent node
} BPTNode;

typedef struct {
    BPTNode* root;
    int t;
} BPT;

BPT* bptInit(int _t);
void bptnInit(BPTNode* node, int _t);
void insert(BPT* tree, int key);
void nodeInsert(BPTNode* root, int key);
void nodeSplit(BPTNode* root); 
int search(BPT* tree, int key);
int nodeSearch(BPTNode* root, int key);
int del(BPT* tree, int key);
int nodeDel(BPTNode* root, int key);
void clearEmptyNode(BPTNode* node);
BPTNode* getClosestNode(BPTNode* node, int key);
int* getRange(BPT* tree, int start, int end);
void freeTree(BPT* tree);
void freeNode(BPTNode* root);