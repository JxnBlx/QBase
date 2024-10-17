#include "btree.c"
#include <assert.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
// #include <climits>
#include <cstring>

void get_memory_usage_kb(long* vmrss_kb, long* vmsize_kb) {
    FILE* file = fopen("/proc/self/status", "r");
    if (!file) {
        perror("Could not open /proc/self/status");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "VmRSS:", 6) == 0) {
            sscanf(line, "VmRSS: %ld kB", vmrss_kb);
        } else if (strncmp(line, "VmSize:", 7) == 0) {
            sscanf(line, "VmSize: %ld kB", vmsize_kb);
        }
    }
    fclose(file);
}

void test_free_tree() {
    long vmrss_before, vmsize_before;
    long vmrss_after, vmsize_after;

    // Get initial memory usage
    get_memory_usage_kb(&vmrss_before, &vmsize_before);

    for (int i = 0; i < 1000; i++) {
        BPT* tree = bptInit(3);
        for (int j = 0; j < 10000; j++) {
            insert(tree, j);
        }
        freeTree(tree);
    }

    // Get memory usage after operations
    get_memory_usage_kb(&vmrss_after, &vmsize_after);

    // Print memory usage details
    // printf("Memory usage before: VmRSS = %ld KB, VmSize = %ld KB\n", vmrss_before, vmsize_before);
    // printf("Memory usage after: VmRSS = %ld KB, VmSize = %ld KB\n", vmrss_after, vmsize_after);
    printf("Memory used: VmRSS = %ld KB, VmSize = %ld KB\n", (vmrss_after - vmrss_before), (vmsize_after - vmsize_before));
    printf("Free tree test passed.\n");
}

void printBTree(BPTNode* root, int level) {
    if (root != NULL) {
        // Print current node's keys
        for (int i = 0; i < level; i++) printf("  "); // Indentation for levels
        for (int i = 0; i < root->n; i++) {
            printf("%d ", root->keys[i]);
        }
        printf("\n");
        // Recursively print children nodes
        if (!root->leaf) {
            for (int i = 0; i <= root->n; i++) {
                printBTree(root->children[i], level + 1);
            }
        }
    }
}

void printBTreeLeaves(BPTNode* root) {
    if (root->leaf){
        for (int i = 0; i < root->n; i++){
            printf("%d ", root->keys[i]);
        }
        printf("\n");
    } else {
        for (int i = 0; i <= root->n; i++){
            printBTreeLeaves(root->children[i]);
        }
    }
}

void test_bplus_tree_initialization() {
    BPT* tree = bptInit(3); // Assume this function initializes the tree
    assert(tree != NULL); // Tree should be initialized
    assert(tree->t == 3); // Order should be set correctly
    assert(tree->root == NULL); // Root should be initialized as NULL
    printf("Initialization test passed.\n");
    freeTree(tree);
}

void test_bplus_tree_insertion_0() {
    BPT* tree = bptInit(3);
    insert(tree, 10);
    assert(tree->root->keys[0] == 10);
    printf("Insertion test 0 passed.\n");
    freeTree(tree);
}

void test_bplus_tree_insertion_1() {
    BPT* tree = bptInit(3);
    insert(tree, 10);
    insert(tree, 20);
    assert(tree->root->keys[0] == 10);
    assert(tree->root->keys[1] == 20);
    printf("Insertion test 1 passed.\n");
    freeTree(tree);
}

void test_bplus_tree_insertion_2() {
    BPT* tree = bptInit(10);
    insert(tree, 90);
    insert(tree, 30);
    insert(tree, 40);
    insert(tree, 60);
    insert(tree, 20);
    insert(tree, 10);
    insert(tree, 100);
    assert(tree->root->keys[0] == 10);
    assert(tree->root->keys[1] == 20);
    printf("Insertion test 2 passed.\n");
    freeTree(tree);
}

void test_bplus_tree_node_split() {
    BPT* tree = bptInit(2);
    insert(tree, 10);
    insert(tree, 20);
    insert(tree, 30);
    insert(tree, 40); // This should cause a split
    assert(tree->root->children[0] != NULL); // Check if split occurred
    printf("Node split test passed.\n");
    freeTree(tree);
}

void test_bplus_tree_propagate_splits_upward() {
    BPT* tree = bptInit(2);
    insert(tree, 10);
    insert(tree, 20);
    insert(tree, 30);
    insert(tree, 40); // Further inserts to propagate split upward
    assert(tree->root->keys[0] == 20); // Check root key after propagation
    printf("Propagation of splits upward test passed.\n");
    freeTree(tree);
}

void test_bplus_tree_double_split() {
    BPT* tree = bptInit(2);
    insert(tree, 10);
    insert(tree, 20);
    insert(tree, 30);
    insert(tree, 40);
    insert(tree, 50);
    insert(tree, 60);
    insert(tree, 70);
    insert(tree, 80);
    insert(tree, 90);
    insert(tree, 100);
    assert(tree->root->keys[0] == 40);
    printf("Double split test passed.\n");
    freeTree(tree);
}

void test_bplus_tree_extreme_split() {
    BPT* tree = bptInit(3);
    for (int i = 0; i < 10000; i++) {
        insert(tree, i);
    }
    assert(tree->root->keys[0] != INT_MAX);
    assert(tree->root->keys[1] != INT_MAX);
    assert(tree->root->keys[2] != INT_MAX);
    printf("Extreme split test passed.\n");
    freeTree(tree);
}

void test_bplus_tree_random_numbers(){
    BPT* tree = bptInit(3);
    for (int i = 0; i<1000; i++){
        insert(tree, rand());
    }
    assert(tree->root->keys[0] != INT_MAX);
    printf("Random insert test passed. \n");
    freeTree(tree);
}

void test_bplus_tree_search() {
    BPT* tree = bptInit(3);
    insert(tree, 10);
    insert(tree, 20);
    assert(search(tree, 10)); // Key 10 should be found
    assert(!search(tree, 30)); // Key 30 should not be found yet
    printf("Search test passed.\n");
    freeTree(tree);
}

void test_bplus_tree_search_extreme(){
    BPT* tree = bptInit(3);
    int* incl = (int*)malloc(sizeof(int)*1000);
    for (int i = 0; i<1000; i++){
        incl[i] = rand();
        insert(tree, incl[i]);
    }
    for (int i = 0; i<9000; i++){
        insert(tree, rand());
    }
    for (int i = 999; i >= 0; i--){
        assert(search(tree, incl[i]) == 1);
    }
    for (int i = -1; i>=-1000; i--){
        assert(!search(tree, i));
    }
    printf("Extreme search test passed.\n");
    freeTree(tree);
}

void test_bplus_tree_deletion() {
    BPT* tree = bptInit(3);
    insert(tree, 10);
    insert(tree, 20);
    del(tree, 20);
    assert(!search(tree, 20)); // Key 20 should be deleted
    printf("Deletion test passed.\n");
    freeTree(tree);
}

void test_bplus_tree_empty_deletion(){
    BPT* tree = bptInit(3);
    assert(!del(tree, 1));
    printf("Empty deletion test passed.\n");
    freeTree(tree);
}

void test_bplus_tree_delete_cascade(){
    BPT* tree = bptInit(2);
    insert(tree, 10);
    insert(tree, 20);
    insert(tree, 30);
    insert(tree, 40);
    insert(tree, 50);
    insert(tree, 60);
    del(tree, 50);
    assert(tree->root->n == 2);
    del(tree, 60);
    assert(tree->root->n == 1);
    printf("Cascade test passed.\n");
    freeTree(tree);
}

void test_bplus_tree_delete_cascade_1(){
    BPT* tree = bptInit(3);
    for (int i = 0; i<100; i++){
        insert(tree, i);
    }
    del(tree, 8);
    del(tree, 7);
    del(tree, 6);
    assert(tree->root->children[0]->children[0]->n == 1);
    printf("Cascade test 1 passed.\n");
    freeTree(tree);
}

void test_bplus_tree_delete_cascade_2(){
    BPT* tree = bptInit(3);
    for (int i = 0; i<100; i++){
        insert(tree, i);
    }
    for (int i=0; i<3;i++){
        del(tree, i);
    }
    assert(tree->root->children[0]->children[0]->n == 1);
    assert(tree->root->children[0]->children[0]->children[0]->keys[0] == 3);
    assert(tree->root->children[0]->children[0]->children[1]->keys[0] == 6);
    printf("Cascade test 2 passed.\n");
    freeTree(tree);
}  

void test_bplus_tree_delete_cascade_extreme(){
    BPT* tree = bptInit(3);
    for (int i = 0; i<1000; i++){
        insert(tree, i);
    }
    for (int i=0; i<300;i++){
        del(tree, i);
    }
    for (int i=0; i<300;i++){
        assert(!search(tree, i));
    }
    for (int i=300; i<1000;i++){
        assert(search(tree, i));
    }
    printf("Extreme cascade test passed.\n");
    freeTree(tree);
}   

void test_bplus_tree_get_closest_node(){
    BPT* tree = bptInit(3);
    for (int i = 0; i<1000; i++){
        insert(tree, i);
    }
    del(tree, 444);
    del(tree, 445);
    del(tree, 446);
    del(tree, 447);
    del(tree, 448);
    del(tree, 449);
    del(tree, 450);
    BPTNode* cl = getClosestNode(tree->root, 443);
    BPTNode* cly = getClosestNode(tree->root, 449);
    BPTNode* cln = getClosestNode(tree->root, 451);
    assert(cl->keys[0]==443||cl->keys[1]==443||cl->keys[2]==443);
    assert(cl==cly);
    assert(cl!=cln);
    assert(cl->leaf);
    assert(cln->leaf);
    assert(cly->leaf);
    printf("Get closest node test passed.\n");
    freeTree(tree);
}

void test_bplus_tree_get_closest_node_mass_delete_0(){
    BPT* tree = bptInit(3);
    for (int i = 0; i<1000; i++){
        insert(tree, i);
    }
    for (int i = 0; i<300; i++){
        del(tree, i);
    }
    BPTNode* cl = getClosestNode(tree->root, 1);
    BPTNode* cly = getClosestNode(tree->root, 300);
    assert(cl != NULL);
    assert(cl == cly);
    assert(cl->keys[0]==300 || cl->keys[2]==300 || cl->keys[3]==300);
    assert(cly->leaf);
    printf("Get closest node mass delete test 0 passed.\n");
    freeTree(tree);
}
void test_bplus_tree_get_closest_node_mass_delete_1(){
    BPT* tree = bptInit(3);
    for (int i = 0; i<1000; i++){
        insert(tree, i);
    }
    for (int i = 0; i<750; i++){
        del(tree, i);
    }
    BPTNode* cl = getClosestNode(tree->root, 1);
    BPTNode* cly = getClosestNode(tree->root, 750);
    assert(cl != NULL);
    assert(cl == cly);
    assert(cl->keys[0]==750 || cl->keys[2]==750 || cl->keys[3]==750);
    assert(cly->leaf);
    printf("Get closest node mass delete test 1 passed.\n");
    freeTree(tree);
}

void test_bplus_tree_get_closest_node_max_int(){
    BPT* tree = bptInit(3);
    for (int i = 0; i<1000; i++){
        insert(tree, i);
    }
    BPTNode* cl = getClosestNode(tree->root, INT_MAX);
    assert(cl != NULL);
    assert(cl->keys[0]==999 || cl->keys[2]==999 || cl->keys[3]==999);
    assert(cl->leaf);
    printf("Get closest node max int test passed.\n");
    freeTree(tree);
}

void test_bplus_tree_range_0(){
    BPT* tree = bptInit(3);
    for (int i = 0; i<10; i++){
        insert(tree, i);
    }
    int* range = getRange(tree, 0, 100);
    for (int i = 0; i<9; i++){
        assert(range[i] == i);
    }
    assert(!(range[11]==21));
    printf("Range test 0 passed.\n");
    freeTree(tree);
}

void test_bplus_tree_range_1(){
    BPT* tree = bptInit(3);
    for (int i = 0; i<20; i++){
        insert(tree, i);
    }
    int* range = getRange(tree, 0, 999);
    for (int i = 0; i<20; i++){
        assert(range[i] == i);
    }
    assert((range[20]==-1));
    printf("Range test 1 passed.\n");
    freeTree(tree);
}

void test_bplus_tree_range_random_order(){
    BPT* tree = bptInit(3);
    for (int i = 0; i<1000; i++){
        int r = rand();
        insert(tree, r);
    }
    int* range = getRange(tree, 500, 5000);
    int* range1 = getRange(tree, 500, 500);
    assert(range1[0] == 500 || range1[0] == -1);
    for (int i = 0; range[i]!= -1; i++){
        assert(search(tree, range[i]));
    }
    printf("Range random order test passed.\n");
    freeTree(tree);
}

void test_bplus_tree_range_with_deletes_0(){
    BPT* tree = bptInit(3);
    for (int i = 0; i<20; i++){
        insert(tree, i);
    }
    del(tree, 12);
    del(tree, 13);
    del(tree, 14);
    del(tree, 15);
    del(tree, 16);
    del(tree, 17);
    int* range = getRange(tree, -1, INT_MAX);
    for (int i = 0; range[i]!= -1; i++){
        assert(search(tree, range[i]));
    }
    printf("Range with deletes test 0 passed.\n");
    freeTree(tree);
}

void test_bplus_tree_range_with_deletes_1(){
    BPT* tree = bptInit(3);
    for (int i = 0; i<1000; i++){
        insert(tree, i);
    }
    for (int i = 250; i<500; i++){
        del(tree, i);
    }
    int* range = getRange(tree, 0, 10000);
    for (int i = 0; range[i]!= -1; i++){
        printf("%d, %d\n", i, range[i]);
        assert(search(tree, range[i]));
    }
    printf("Range with deletes test 1 passed.\n");
    freeTree(tree);
}

void test_bplus_tree_extreme_full_10000(){
    BPT* tree = bptInit(3);
    for (int i = 0; i<10000; i++){
        insert(tree, rand());
    }
    for (int i = 0; i<5000; i++){
        del(tree, i);
    }
    int* range = getRange(tree, -INT_MAX, INT_MAX);

    for (int i = 0; range[i]!= -1; i++){
        assert(search(tree, range[i]));
    }
    for (int i = 0; i<10000; i++){
        insert(tree, rand());
    }
    int* range1 = getRange(tree, 7000, INT_MAX);
    for (int i = 0; range1[i]!= -1; i++){
        assert(search(tree, range1[i]));
    }
    printf("Extreme full test 10,000 passed.\n");
}

void test_bplus_tree_extreme_full_1000000(){
    BPT* tree = bptInit(3);
    printf("Inserting\n");
    for (int i = 0; i<1000000; i++){
        insert(tree, rand());
    }
    for (int i = 0; i<50000; i++){
        del(tree, rand());
    }
    int* range = getRange(tree, -INT_MAX, INT_MAX);

    for (int i = 0; range[i]!= -1; i++){
        assert(search(tree, range[i]));
    }
    for (int i = 0; i<1000000; i++){
        insert(tree, rand());
    }
    int* range1 = getRange(tree, 700000, INT_MAX);
    for (int i = 0; range1[i]!= -1; i++){
        assert(search(tree, range1[i]));
    }
    printf("Extreme full test 1,000,000 passed.\n");
}

void test_bplus_tree_extreme_full_10000000(){
    BPT* tree = bptInit(3);
    printf("Inserting\n");
    for (int i = 0; i<10000000; i++){
        insert(tree, rand());
    }
    for (int i = 0; i<500000; i++){
        del(tree, rand());
    }
    
    int* range = getRange(tree, -INT_MAX, INT_MAX);
    if (range == NULL) printf("Range is null\n");
    printf("%d\n", range[0]);
    for (int i = 0; range[i]!= -1; i++){
        printf("%d, %d\n", i, range[i]);
        assert(search(tree, range[i]));
    }
    for (int i = 0; i<10000000; i++){
        insert(tree, rand());
    }
    int* range1 = getRange(tree, 7000000, INT_MAX);
    for (int i = 0; range1[i]!= -1; i++){
        assert(search(tree, range1[i]));
    }
    printf("Extreme full test 10,000,000 passed.\n");
}
void test_bplus_tree_extreme_full_100000000(){
    BPT* tree = bptInit(3);
    printf("Inserting\n");
    for (int i = 0; i<100000000; i++){
        insert(tree, i);
    }
    for (int i = 12891873; i<62456273; i++){
        del(tree, i);
    }
    printf("Getting range\n");
    int* range = getRange(tree, -INT_MAX, INT_MAX);
    printf("Searching\n");
    for (int i = 0; range[i]!= -1; i++){
        assert(search(tree, range[i]));
    }
    printf("Inserting\n");
    for (int i = 0; i<100000000; i++){
        insert(tree, rand());
    }
    printf("Getting range\n");
    int* range1 = getRange(tree, 70000000, INT_MAX);
    for (int i = 0; range1[i]!= -1; i++){
        assert(search(tree, range1[i]));
    }
    printf("Extreme full test 100,000,000 passed.\n");
}


int main() {
    printf("\n");
    // test_free_tree();
    // test_bplus_tree_initialization();
    // test_bplus_tree_insertion_0();
    // test_bplus_tree_insertion_1();
    // test_bplus_tree_insertion_2();
    // test_bplus_tree_node_split();
    // test_bplus_tree_propagate_splits_upward();
    // test_bplus_tree_double_split();
    // test_bplus_tree_extreme_split();
    // test_bplus_tree_random_numbers();
    // test_bplus_tree_search();
    // test_bplus_tree_search_extreme();
    // test_bplus_tree_deletion();
    // test_bplus_tree_delete_cascade();
    // test_bplus_tree_delete_cascade_1();
    // test_bplus_tree_delete_cascade_2();
    // test_bplus_tree_delete_cascade_extreme();
    // test_bplus_tree_get_closest_node();
    // test_bplus_tree_get_closest_node_mass_delete_0();
    // test_bplus_tree_get_closest_node_mass_delete_1();
    // test_bplus_tree_get_closest_node_max_int();
    // test_bplus_tree_range_0();
    // test_bplus_tree_range_1();
    // test_bplus_tree_range_random_order();
    // test_bplus_tree_range_with_deletes_0();
    // test_bplus_tree_range_with_deletes_1();
    // test_bplus_tree_extreme_full_10000();
    // test_bplus_tree_extreme_full_1000000();
    test_bplus_tree_extreme_full_10000000();
    return 0;
}