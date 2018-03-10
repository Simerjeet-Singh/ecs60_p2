#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
                   BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
    values = new int[LSize];
}  // LeafNode()

void LeafNode::leftShift()
{
    for(int i = 0;i<count-1;i++){
        values[i] = values[i+1];
    }
    values[count-1] = 0;
    cout << "here leftshift" <<endl;
    count--;
}// leftShift()

int LeafNode::getMinimum()const
{
    if(count > 0)  // should always be the case
        return values[0];
    else
        return 0;
    
} // LeafNode::getMinimum()

int i = 0;
LeafNode* LeafNode::insert(int value)
{

	cout <<"Insert int value Leafnode" << endl;
	cout << "Value inserted whether recurssively or not is : " <<value<<endl<<endl;
	    if(value == 69)
	    	cout<<"left sibling of children o is: "<< this->getLeftSibling()->getLeftSibling()<<endl<<endl;
	     if(value == 27){
		cout<<"right sibling of after 27 is in: "<< this->getRightSibling()->getRightSibling()<<endl<<endl;
		}

    // students must write this
    if (count < leafSize)
    {
        int pos = insert_pos(value);
        cout << "count < leafSize" << endl;
        values[pos] = value;
        count++;
        
    }// LeafNode Full, create new LeafNode
    //split() // create new LeafNode
    else //current leaf node is full
    {
	cout << "current leaf node is full" << endl;
        if((leftSibling != NULL) && (leftSibling->getCount() < leafSize))
        {
	    if(leftSibling->getCount() < leafSize){
	    if(value>values[0])
            {
                leftSibling->insert(values[0]);
                leftShift();
                insert(value);
            }
            else
            {
                leftSibling->insert(value);
            }
	    }
        }
        else if(rightSibling != NULL && rightSibling->getCount() < leafSize)
        {
	    cout << "checking right leafnode" << endl;
            if(value<values[count-1])
            {
                rightSibling->insert(values[count-1]);
                values[count-1] = 0; 
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
		   

		cout << "NEED To split" << endl;
            LeafNode *newLeafNode = new LeafNode(leafSize, parent, this, NULL);
	    cout<<"address of this is: "<<this<<endl;
	    cout<<"address of this is: "<<this->getLeftSibling()<<endl;
	    cout<<"address of this is: "<<this->getRightSibling()<<endl;
	 if(value == 69)
		cout<<"left sibling of children before moving is: "<< this->getLeftSibling()->getLeftSibling()<<endl<<endl;

	    //cout<<parent->getChildren(0)<<endl;
	   if(rightSibling == NULL) { //rightsibling is not there
                //rightSibling = newLeafNode;
		rightSibling = newLeafNode;
		//this->setRightSibling(newLeafNode);

		//cout << "newleaf node is now rightSibling" << endl;
                //newLeafNode->setLeftSibling(this);
            }
            else{
		    
		cout << "SETTING PTRS" << endl;
                //newLeafNode->setLeftSibling(this);
                //newLeafNode->setRightSibling(rightSibling);
 		if(value == 69){
		cout<<"left sibling of while pointing in else before this "<< this->getLeftSibling()->getLeftSibling()<<endl<<endl;
		}

		this->getRightSibling()->setLeftSibling(newLeafNode);//not sure about ->
                //this->setRightSibling(newLeafNode);
		if(value == 69){
		cout<<"left sibling of while pointing in else after this "<< this->getLeftSibling()->getLeftSibling()<<endl<<endl;
		}

		rightSibling = newLeafNode;
		if(value == 69){
		cout<<"left sibling of while pointing in else: "<< this->getLeftSibling()->getLeftSibling()<<endl<<endl;
		}

            }
	   if(value == 69){
		cout<<"left sibling of after pointing stuff is: "<< this->getLeftSibling()->getLeftSibling()<<endl<<endl;
		}
	    if(value == 27){
		cout<<"left sibling of after pointing stuff is: "<< this->getRightSibling()<<endl<<endl;
		}

	        if(leafSize%2==1){
                int position = position_odd(value);
		//cout << "Position: " << position << endl;
                if(position == leafSize/2){
                    for(int i = position; i<leafSize; i++){ //made change here maybe position+1 but no good
                        rightSibling->insert(values[i]);    // changed count to LeafSize for i<leafSize
		//cout << "count: " << count << endl;
                        count--;
                    }
                    insert(value);
                }
                else{
		
		  //cout<< "count for right sibling is: "<<rightSibling->getCount()<<endl;
                  for(int i=position;i<leafSize;i++){
                    rightSibling->insert(values[i]);
                    count--;
		    //cout << "count after insert: " << count << endl;
                  }        
		  //cout << "Went into else to insert into rightSibling" << endl;
		  //cout<< "count for right sibling is: "<<rightSibling->getCount()<<endl;
                  rightSibling->insert(value);
		  //cout<< "count for right sibling is: "<<rightSibling->getCount()<<endl;
                }
            }
            else {//even leafSize
              int position_even = position_evenf(value);
              if(position_even == leafSize/2){
                for(int i = position_even;i<leafSize;i++){
                  rightSibling->insert(values[i]);
                  count--;
                }
                rightSibling->insert(value);
              }
              else{
                for(int i=position_even;i<leafSize;i++){
                  rightSibling->insert(values[i]);
                  count--;
                }
                this->insert(value);
              }
            }
            //return newLeafNode;
	    return (LeafNode*)rightSibling;
        }   
    }
    //cout<<"right Sibling address is: "<<rightSibling<<endl;
    return NULL; // to avoid warnings for now.
}
// LeafNode::insert()

int LeafNode::position_odd(int value){
	cout << "position_odd Leafnode" << endl;
    int pos = leafSize/2;
    if(values[pos] > value)
        return pos;
    else
        return pos+1;
}

int LeafNode::position_evenf(int value){
	cout << "position_even Leafnode" << endl;
    int pos = leafSize/2;
    if(values[pos-1] > value)
        return pos-1;
    else
        return pos;
}

void LeafNode::print(Queue <BTreeNode*> &queue)
{
    cout << "Leaf: ";
    for (int i = 0; i < count; i++)
        cout << values[i] << ' ';
    cout << endl;
} // LeafNode::print()

/*int LeafNode::max_sort(int value)
{
    int tmp;
    for (int i = count - 1; i>=0; i--) {
        if values[i] > value {
            values[i+1] = values[i];
        }
        
    }
    return values[count-1];
    
}//sort function
*/
//sorting and giving the position where to insert
int LeafNode::insert_pos(int value)//finding the position where we want to insert
{
	cout << "insert_pos Leafnode" << endl;
    if(count == 0)
    {
        return 0;
    }
    else if(values[count-1]<value)
	    return count;
    else
    {
        for (int i = count-1; i>=0; i--)
        {
	    if (values[i] > value) {
                values[i+1] = values[i];
            }
            else
            {
                return i+1; //made correction
            }
            
        }
    }
    return 0;  // check this element inserting is smallest element
}//done with insert_pos

