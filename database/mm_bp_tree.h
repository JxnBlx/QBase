#include <stdint.h>
#define _T 3
#define MAX_KEYS _T*2

typedef struct {
    size_t filesize; // size_t bytes
    size_t root_offset; // size_t bytes
    uint32_t node_size; // 4 bytes
} Header;

typedef struct {
    int *keys[MAX_KEYS];    // 24 bytes
    size_t *children[MAX_KEYS]; // size*6 bytes
    int n;              // 4 bytes
    size_t next; // size_t bytes
    size_t prev; // size_t bytes
    size_t parent; // size_t bytes
    bool leaf;           // 1 byte //bit?
} BPTNode;

typedef struct {
    size_t root_offset;
    int t = _T;
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