/*This class provides the Queue functionality*/
#include "Queue.h"

using namespace std;

Queue::Queue()
{
	_front = NULL;
	_rear = NULL;
	
	_length = 0;
}

void Queue::enqueue( itemType value )
{
}

/* 
Dequeues an object from the front of the queue.

Returns a pointer to the dequeued object.

Returns NULL if there is no node to dequeue.
*/
itemType* Queue::dequeue()
{
	QueueNode* currentNode = _rear;
	
	//Special case: length = 0
	if ( _length == 0 ) {
		
		//Throw error
		return NULL;
	}
	
	//Deal with special case of length = 1
	else if ( _length == 1 ) {
		_front = NULL;
		_rear = NULL;
		_length = 0;
		
		return currentNode;
	} 
	
	//Normal case
	else {
		//Loop through queue until we reach the second last entry
		for( int i = 0; i < _length - 1; i++) {
			currentNode = currentNode->link;
		}
		//Now currentNode = second to last node 
	
		//Set head to equal the second to last node
		_front = currentNode;
	
		_length --;
		
		//Return old front of queue
		return currentNode->link;
	}	
}

int Queue::isEmpty()
{
	return -2;
}

int Queue::contains( itemType value )
{
	QueueNode* currentNode = _rear;

	//Loop through queue until we reach the second last entry
	for( int i = 0; i < _length - 1; i++) {
		currentNode = currentNode->link;
	}
}

itemType* Queue::select( unsigned int position )
{
	return NULL;
}

int Queue::replace( unsigned int position, itemType value)
{
	return -2;
}

int Queue::get_length()
{
	return -2;
}
	 

///* Functions Definitions (Implementation)
// * **************************************/

//List list_create() { 
///*Creates a List item, initializes head and tail to null, and sets size = 0

//Function Running Time: O(1)
//*/
//  struct listTag *L;
//  
//  L=(struct listTag*)malloc(sizeof(struct listTag));
//  if( L!=NULL )  
//  {
//      L->head = NULL;
//      L->tail = NULL;
//      L->size=0;  
//      return L;
//  }
//  
//  return NULL;
//}

//bool list_isEmpty( const List L ) {
///*Checks if list is empty based on the L->size variable

//Function Running Time: O(1)
//*/
//  if (L->size==0)
//    return true;
//  return false;
//}

//int list_length( const List L ) {
///*Returns the list length based on the L->size variable

//Function Running Time: O(1)
//*/
//  return L->size;
//}

//bool list_insertBack( itemType value, List L ) { 
///*inserts new element to last position (position 9 of a 10 member list). If the
//new element is the only element of the list, it changes L's values to reflect
//that

//Function Running Time: O(1)
//*/

//  nodeType *newNode;
//  newNode = (nodeType*)malloc(sizeof(nodeType));
//  
//  newNode->next = NULL;
//  newNode->previous = L->tail;
//  newNode->data = value;
//  
//  if ((L->tail)!=NULL)
//    (L->tail)->next = newNode;
//  
//  else //else if L->tail==NULL, then this is the only element in the list
//    L->head = newNode;
//    
//  L->tail = newNode;
//  L->size++;

//  return true;
//}

//bool list_deleteFront( List L ) {
///*Frees memory allocated to first item in list, and changes the list nodes
//around the deleted element to reflect the deletion.

//running time = O(n)
//*/

//  nodeType *tempNode;
//  tempNode = L->head;
//  
//  L->head = (L->head)->next;
//  
//  free(tempNode);
//  
//  (L->size)--;
//  return true;
//}

//int list_search( itemType Value, List L ) {
///*Searches the list for a particular value. Returns the position of the first
//instance of that value. Returns -1 if the value was not found.

//Function Running Time: O(n)
//*/    
//    
//  nodeType *currentNode;
//  
//  currentNode=L->head; //Move to first node
//  
//  int i;
//  for(i=0; i<(L->size); i++) {
//        if ((currentNode->data) == Value) {
//            return i;
//        }
//        
//        else { //Move to next node
//            currentNode = (currentNode->next);
//        }
//  }
//  return -1;
//}

//itemType *list_select( unsigned int position, List L ) {
///*Returns the value of the data item at the given position. Returns -1 if the
//position is an invalid value.

//Function Running Time: O(n)
//*/
//  nodeType *currentNode;
//  currentNode = (nodeType*)malloc(sizeof(nodeType));

//  currentNode=L->head; //Move to first node
//  
//  int i;
//  for(i=0; i<(L->size); i++) {
//        if (i==position) {
//            return &(currentNode->data);
//        }
//        
//        else { //Move to next node
//            currentNode = (currentNode->next);
//        }
//  }

//  return NULL;
//}

//bool list_replace( unsigned int position, itemType value, List L ) {
///*Replaces the value of the data item at the given position.

//Function Running Time: O(n)
//*/
//  nodeType *currentNode;
//  
//  currentNode=L->head; //Move to first node
//  
//  int i;
//  for(i=0; i<(L->size); i++) {
//        if (i == position) {
//            currentNode->data = value;
//            return true;
//        }
//        
//        else { //Move to next node
//            currentNode = (currentNode->next);
//        }
//  }
//  
//  return false;
//}
