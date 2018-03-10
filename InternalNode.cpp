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
BTreeNode* InternalNode::getChildren(int i){
	return children[i]->getLeftSibling();
}
//check if new leaf node is required
BTreeNode* InternalNode::newLeafNodeOrNot(int value){
    cout<<"Inside NewLeafNodeOrNot " <<endl;
    BTreeNode* ptr;
    for(int i=count-1;i>=0;i--){
	if(i==count-1  && value>keys[count-1]){
	   cout<<"value of i in internal node  "<<i<<endl<<endl;
           ptr = children[i]->insert(value);//changed it to i+1 from i
	    return ptr;
	}	
	else if(value>keys[i]){
    	    ptr = children[i]->insert(value);//changed it to i+1 from i
            cout<<"value of ptr is " <<ptr<<endl;
	    return ptr;
        }
        else if(i==0&&value<keys[0])
	{
	    ptr = children[i]->insert(value);
	    return ptr;
	}
	else
            ptr=0;
    }
    cout<<"value of ptr is " <<ptr<<endl;
    return ptr;
}
//end of newLeafNodeOrNot
void InternalNode::updateKeys(){
    cout<<"Inside updateKeys " <<endl;
    for(int i = 0;i<count;i++){
        keys[i] = children[i]->getMinimum();
    }
}
InternalNode* InternalNode::insert(int value)
{
    
    cout<<"left sibling of children is: " <<children[0]->getLeftSibling();
    cout<<"Inside insert(value) InternalNode " <<endl;
    BTreeNode* ptr;
    cout<<"valueof count and internalSize are: "<< count << " " <<internalSize<<endl;	
    if(count<internalSize){
        ptr = newLeafNodeOrNot(value);
        if(!ptr){//new leafnode was not created
            updateKeys();
	    cout<<"count is less than internal size and we need no new leaf "<<endl;
	    if(rightSibling)
		    ((InternalNode*)rightSibling)->keys[0] = ((InternalNode*)rightSibling)->children[0]->getMinimum();

        }
        else{// if(count!=internalSize){
            //new leaf node was created but has to incorporated in same Internal node
	    cout<<"count is less than internal size and we need new leaf node"<<endl;
            this->insert(ptr);
        }
    }
    else{//internal node is full
        //so look at left or right or new internal node
	//cout<<"entered else"<<endl<<endl;
        ptr = newLeafNodeOrNot(value);
	if(!ptr){
		cout<<"entered else of internal node insert"<<endl<<endl;
		updateKeys();
           // if(rightSibling)
           //         ((InternalNode*)rightSibling)->keys[0] = ((InternalNode*)rightSibling)->children[0]->getMinimum();

        }
	else if(leftSibling != NULL && leftSibling->getCount() < internalSize)
        {
	    cout<<"checking leftsiblingof internalnode"<<endl;	
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
	    cout<<"checking rightsiblingof internalnode"<<endl;	
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
	    cout<<"creating new internal node"<<endl<<endl;
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
		    //cout<<"Odd internal node size " <<endl<<endl;
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
                if(position_even == internalSize/2){
                    for(int i = position_even;i<internalSize;i++){
                        ((InternalNode*)rightSibling)->insert(children[i]);
                        count--;
                    }
		    //cout<<"value of count is after removal and insertion in right sibling: "<<count<<endl<<endl;
                    ((InternalNode*)rightSibling)->insert(ptr);
		    //cout<<"value being inserted is greater and in even sizse internlal node"<<endl<<endl;
                }
                else{
                    for(int i=position_even;i<internalSize;i++){
                        ((InternalNode*)rightSibling)->insert(children[i]);
                        count--;
                    }
		    //cout<<"value being inserted is less in even sizse internlal node"<<endl<<endl;
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
    cout<<"Inside position_odd(BTreeNode *ptr) InternalNode " <<endl;
    int pos = internalSize/2;
    if(ptr->getMinimum() > children[pos]->getMinimum())//changed back
        return pos+1;
    else
        return pos;
}//done with position_odd

int InternalNode::position_evenf(BTreeNode *ptr){
    cout<<"Inside position_even(BTreeNode *ptr) InternalNode " <<endl;
    int pos = internalSize/2;
    if(children[pos-1]->getMinimum() > ptr->getMinimum())
        return pos-1;
    else
        return pos;
}//done with position evenf

void InternalNode::leftShift()
{
    cout<<"Inside leftshift) InternalNode " <<endl;
    for(int i = 0;i<count-1;i++){
        children[i] = children[i+1];
    }
//    children[count-1] = 0;
    count--;
}// leftShift()

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
    cout<<"Inside insert(BTreeNode *oldRoot, BTreeNode *node2) InternalNode " <<endl;
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
    cout<<"Inside insert(BTreeNode *newNode" <<endl;
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
    cout<<"Inside insert_pos(BTreeNode *newNode" <<endl;
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
    
