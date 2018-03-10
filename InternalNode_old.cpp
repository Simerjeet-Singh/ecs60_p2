#include <iostream>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()

void InternalNode::leftShift()
{
    for(int i = 0;i<count-1;i++){
        keys[i] = children[i+1]->getMinimum();
    }
   // children[count-1] = 0;
   // cout << "here leftshift" <<endl;
    count--;
}// leftShift()
int InternalNode::position_odd(int value){
    int pos = leafSize/2;
    if(children[pos]->getMinimum() > value)
        return pos;
    else
        return pos+1;
}

int InternalNode::position_evenf(int value){
    int pos = leafSize/2;
    if(children[pos-1]->getMinimum() > value)
        return pos-1;
    else
        return pos;
}
int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()


InternalNode* InternalNode::insert(int value)
{
  // students must write this
  BTreeNode *p;
  for(int i = count-1;i>=0;i--){
  	 	if(value<keys[i])
			p = children[i]->insert(value);
		else if(i==0)
			p = children[i]->insert(value);	
  }
  
  if (count < internalSize)
    {
	for(int i = count-1;i>=0;i--){
        	if(keys[i]<value)
        	{
			if(children[i]->insert(value) == NULL)
				keys[i] = children[i]->getMinimum();
			else //new leaf node was created and need to a new value to internal node
			{	
			//	BTreeNode *p = children[i]->insert(value);

			}

		}

    	}// InternalNode Full, create new LeafNode
    }
    //split() // create new InternalNode
    else //current leaf node is full
    {
        //cout << "current leaf node is full" << endl;
        if(leftSibling != NULL && leftSibling->getCount() < internalSize)
        {
            if(value>children[0]->getMinimum())
            {
                leftSibling->insert(children[0]->getMinimum());
                leftShift();
                insert(value);
            }
            else
            {
                leftSibling->insert(value);
            }
        }
        else if(rightSibling != NULL && rightSibling->getCount() < internalSize)
        {
            if(value<children[count-1]->getMinimum())
            {
                rightSibling->insert(children[count-1]->getMinimum());
                //children[count-1]->getMinimum = 0; 
		count--;
                insert(value);
            }
            else
            {
                rightSibling->insert(value);
            }
        }
        else //need to split
        {
                //cout << "NEED To split" << endl;
            InternalNode *newInternalNode = new InternalNode(internalSize,leafSize, parent, this, rightSibling);
            //cout << "Created new leafnode" << endl;
            if(rightSibling == NULL) { //rightsibling is not there
                //rightSibling = newInternalNode;
                this->setRightSibling(newInternalNode);
                //cout << "newleaf node is now rightSibling" << endl;
                newInternalNode->setLeftSibling(this);
            }
            else{
                newInternalNode->setLeftSibling(this);
                newInternalNode->setRightSibling(rightSibling);
                this->getRightSibling()->setLeftSibling(newInternalNode);//not sure about ->
                this->setRightSibling(newInternalNode);
            }
            if(internalSize%2==1){
                int position = position_odd(value);
                //cout << "Position: " << position << endl;
                if(position == internalSize/2){
                    for(int i = position; i<internalSize; i++){ //made change here maybe position+1 but no good
                        rightSibling->insert(children[i]->getMinimum());    // changed count to LeafSize for i<internalSize
                //cout << "count: " << count << endl;
                        count--;
                    }
                    insert(value);
                }
                else{
                  //cout<< "count for right sibling is: "<<rightSibling->getCount()<<endl;
                  for(int i=position;i<internalSize;i++){
                    rightSibling->insert(children[i]->getMinimum());
                    count--;
                    //cout << "count after insert: " << count << endl;
                  }
                  //cout << "Went into else to insert into rightSibling" << endl;
                  //cout<< "count for right sibling is: "<<rightSibling->getCount()<<endl;
                  rightSibling->insert(value);
                  //cout<< "count for right sibling is: "<<rightSibling->getCount()<<endl;
                }
            }
            else {//even internalSize
              int position_even = position_evenf(value);
              if(position_even == internalSize/2){
                for(int i = position_even;i<internalSize;i++){
                  rightSibling->insert(children[i]->getMinimum());
                  count--;
                }
                rightSibling->insert(value);
              }
              else{
                for(int i=position_even;i<internalSize;i++){
                  rightSibling->insert(children[i]->getMinimum());
                  count--;
                }
                this->insert(value);
              }
            }
            //return newInternalNode;
            //cout<<"right Sibling address is: "<<rightSibling<<endl;
            return (InternalNode*)rightSibling;
        }
    }
    //cout<<"right Sibling address is: "<<rightSibling<<endl;

  return NULL; // to avoid warnings for now.
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
  // students must write this
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

} // InternalNode::insert()

//int InternalNode::insert_pos(BTreeNode *newNode){
//   for(int i = count-1;i>=0;i--){
//	   if(children[i]->getMinimum()>newNode->getMinimum())
//		   children[i+1] = children[i];
//	   else
//		   return i+1;
//   }
//}
int InternalNode::insert_pos(BTreeNode *newNode){
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
            if (children[i]->getMinimum()<newNode->getMinimum()) 
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

void InternalNode::updateKeys(BTreeNode *newNode)
{
	for(int i = 0;i<count;i++){
		keys[i] = children[i]->getMinimum();
	}
}
void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
  int pos = insert_pos(newNode);
  children[pos]=newNode;
  count++;
  updateKeys();
}
 // InternalNode::insert()

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

