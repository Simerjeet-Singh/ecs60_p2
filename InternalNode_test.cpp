#include <iostream>
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
                           InternalNode *p, BTreeNode *left, BTreeNode *right) :
BTreeNode(LSize, p, left, right), internalSize(ISize)
{
    keys = new int[internalSize]; // keys[i] is the minimum of children[i]
    children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()


int InternalNode::getMinimum()const
{
    if(count > 0)   // should always be the case
        return children[0]->getMinimum();
    else
        return 0;
} // InternalNode::getMinimum()
//check if new leaf node is required
BTreeNode* InternalNode::newLeafNodeOrNot(int value){
    BTreeNode* ptr;
    for(int i=count-1;i>=0;i--){
        if(value>keys[i]){
            ptr = children[i]->insert(value);
            return ptr;
        }
        else if(i==0&&value<keys[0])
            ptr = children[i]->insert(value);
        else
            ptr=0;
    }
    return ptr;
}
//end of newLeafNodeOrNot
void InternalNode::updateKeys(){
    for(int i = 0;i<count;i++){
        keys[i] = children[i]->getMinimum();
    }
}
InternalNode* InternalNode::insert(int value)
{
    BTreeNode* ptr;
    //if(count == internalSize){
    //    ptr = newLeafNodeOrNot(value);
    //    if(!ptr){//new leafnode was not created
    //        updateKeys();
    //    }
    //}	
    if(count<internalSize){
        ptr = newLeafNodeOrNot(value);
        if(!ptr){//new leafnode was not created
            updateKeys();
//	    if(rightSibling)
//		    ((InternalNode*)rightSibling)->keys[0] = ((InternalNode*)rightSibling)->children[0]->getMinimum();

        }
        else{
            //new leaf node was created but has to incorporated in same Internal node
            this->insert(ptr);
        }
    }
    else if(count==internalSize){
        ptr = newLeafNodeOrNot(value);
        if(!ptr){//new leafnode was not created
            updateKeys();
            if(rightSibling)
                    ((InternalNode*)rightSibling)->keys[0] = ((InternalNode*)rightSibling)->children[0]->getMinimum();

        }
        //else{
        //    //new leaf node was created but has to incorporated in same Internal node
        //    this->insert(ptr);
        //}
    }

    else if(count==internalSize){//internal node is full
        //so look at left or right or new internal node
	cout<<"Creation of new internal node"<<endl<<endl;
        ptr = newLeafNodeOrNot(value);
	cout<<"after creation of newLeafNodeOrnot"<<endl;
	//if(ptr){
	//	cout<<"checking condition"<<endl;
	//	updateKeys();
        //    //if(rightSibling)
        //     //       ((InternalNode*)rightSibling)->keys[0] = ((InternalNode*)rightSibling)->children[0]->getMinimum();

        //}
	if(leftSibling != NULL && leftSibling->getCount() < internalSize)
        {
            if(ptr->getMinimum()>children[0]->getMinimum())
            {
                ((InternalNode*)leftSibling)->insert(children[0]);
                leftShift();
                insert(ptr);
            }
            else
            {
                ((InternalNode*) leftSibling)->insert(ptr);
            }
        }
        else if((InternalNode*)rightSibling != NULL && rightSibling->getCount() < internalSize)
        {
            if(ptr->getMinimum()<children[count-1]->getMinimum())
            {
                ((InternalNode*)rightSibling)->insert(children[count-1]);
                count--;
                insert(ptr);
            }
            else
            {
                ((InternalNode*)rightSibling)->insert(ptr);
            }
        }
        else{
            InternalNode *newInternalNode = new InternalNode(internalSize, leafSize, parent, this, (InternalNode*)rightSibling);
            if(((InternalNode*)rightSibling) == NULL) { //rightsibling is not there
                rightSibling = newInternalNode;
                newInternalNode->setLeftSibling(this);
            }
            else{
                newInternalNode->setLeftSibling(this);
                newInternalNode->setRightSibling((InternalNode*)rightSibling);
                this->getRightSibling()->setLeftSibling(newInternalNode);//not sure about ->
                this->setRightSibling(newInternalNode);
            }
            if(internalSize%2==1){
		    cout<<"Odd internal node size " <<endl<<endl;
                int position = position_odd(ptr);
                if(position == internalSize/2){
                    for(int i = position; i<internalSize; i++){ //made change here maybe position+1 but no good
                        ((InternalNode*)rightSibling)->insert(children[i]);    // changed count to LeafSize for i<leafSize
                        count--;
                    }
                    insert(ptr);
                }
                else{
                    for(int i=position;i<internalSize;i++){
                        ((InternalNode*)rightSibling)->insert(children[i]);
                        count--;
                    }
                    ((InternalNode*)rightSibling)->insert(ptr);
                }
            }
            else {//even internal node size
                int position_even = position_evenf(ptr);
                if(position_even == leafSize/2){
                    for(int i = position_even;i<internalSize;i++){
                        ((InternalNode*)rightSibling)->insert(children[i]);
                        count--;
                    }
                    ((InternalNode*)rightSibling)->insert(ptr);
                }
                else{
                    for(int i=position_even;i<internalSize;i++){
                        ((InternalNode*)rightSibling)->insert(children[i]);
                        count--;
                    }
                    this->insert(ptr);
                }
            }
            //return newLeafNode;
            return ((InternalNode*)rightSibling);
        }
        
    }//internal node full else getting done
    // students must write this
    return NULL; // to avoid warnings for now.
} // InternalNode::insert()

int InternalNode::position_odd(BTreeNode *ptr){
    int pos = internalSize/2;
    if(ptr->getMinimum() > children[pos]->getMinimum())//changed back
        return pos;
    else
        return pos+1;
}//done with position_odd

int InternalNode::position_evenf(BTreeNode *ptr){
    int pos = internalSize/2;
    if(children[pos-1]->getMinimum() > ptr->getMinimum())
        return pos-1;
    else
        return pos;
}//done with position evenf

void InternalNode::leftShift()
{
    for(int i = 0;i<count-1;i++){
        children[i] = children[i+1];
    }
//    children[count-1] = 0;
    count--;
}// leftShift()

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
    children[0] = oldRoot;
    children[1] = node2;
    keys[0] = children[0]->getMinimum();
    count++;
    keys[1] = children[1]->getMinimum();
    count++;
    children[0]->setParent(this);
    children[1]->setParent(this);
    children[0]->setRightSibling(children[1]);
    children[1]->setRightSibling(children[0]);
    // students must write this
} // InternalNode::insert()


void InternalNode::insert(BTreeNode *newNode) // from a sibling
//new node here is actally new leafnode created and count
//was less than internal size
{
   // children[count] = newNode;
    int pos = insert_pos(newNode);
    children[pos]=newNode;
    //cout<<"position from insert_pos in internal node is: "<<pos<<endl;
    //cout<<"count now is : " <<count<<endl;
    count++;
    //cout<<"count now after count++ : " <<count<<endl;
    updateKeys();
    // students may write this
} // InternalNode::insert()
//determining position of where to insert new leaf node
int InternalNode::insert_pos(BTreeNode *newNode)
    {
        if(count == 0)
        {
            return 0;
        }
        else if(children[count-1]->getMinimum()<newNode->getMinimum())
            return count;
        else
        {
            for (int i = count-1; i>=0; i--)
            {
                if (children[i]->getMinimum()>newNode->getMinimum())
                {
                    children[i+1] = children[i];
                }
                else
                {
                    return i+1; //made correction
                }
                
            }
        }
        return 0;  // check this element inserting is smallest element
    }//done with insert_pos
    
void InternalNode::print(Queue <BTreeNode*> &queue)
    {
        int i;
        cout << "Internal: ";
        for (i = 0; i < count; i++)
            cout << keys[i] << ' ';
        cout << endl;
        for(i = 0; i < count; i++)
            queue.enqueue(children[i]);
        
    } // InternalNode::print()
    
