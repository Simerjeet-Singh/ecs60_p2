#ifndef LeafNodeH
#define LeafNodeH

#include "BTreeNode.h"

class LeafNode:public BTreeNode
{
    int *values;
public:
    LeafNode(int LSize, InternalNode *p, BTreeNode *left,
             BTreeNode *right);
    int getMinimum() const;
    LeafNode* insert(int value); // returns pointer to new Leaf if splits
    // else NULL
    void print(Queue <BTreeNode*> &queue);
    //int max_sort(int value);
    int insert_pos(int value); // returns insert position
    // LeafNode * split(); // return a pointer with new()
    void leftShift(void);
    int position_odd(int value);  // determines position if leafSize odd
    int position_evenf(int value); // determines position if leafSize even
}; //LeafNode class

#endif

