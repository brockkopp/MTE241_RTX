/*This class provides the Linked List functionality*/
#include <stdio.h>
#include <stdlib.h>

#include "List.h"

using namespace std;





/* Functions Definitions (Implementation)
 * **************************************/

List list_create() { 
/*Creates a List item, initializes head and tail to null, and sets size = 0

Function Running Time: O(1)
*/
  struct listTag *L;
  
  L=(struct listTag*)malloc(sizeof(struct listTag));
  if( L!=NULL )  
  {
      L->head = NULL;
      L->tail = NULL;
      L->size=0;  
      return L;
  }
  
  return NULL;
}

//void list_destroy( List L ) { 
///*Frees the memory allocated to each individual node in the list, also frees L
//Function Running Time: O(n)
//*/
//  nodeType *currentNode;
//  currentNode = (nodeType*)malloc(sizeof(nodeType));
//  
//  currentNode = (L->head);
//  int i;
//  for(i=0;i<(L->size);i++) {
//    if (currentNode->next != NULL){    
//      currentNode = (currentNode->next);//Step to next node
//      free(currentNode->previous);//Free current node (the next node's prev node)
//    }
//  }
//  
//  L->size = 0;
//  free(L);
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

//void list_print( const List L ) {
///*Prints the list passed to the function. If the list is empty, prints "List
//is empty"

//Function Running Time: O(n)
//*/  
//  printf("_Printing list:\n");
//  
//  if(list_isEmpty(L)) {
//    printf("List is empty\n");
//    return;
//    }
//  
//  nodeType *currentNode;
//  
//  //printf("\n\nin fxn -> %p\n%p\n\n\nsize = %i", currentNode->next, currentNode, L->size);

//  currentNode = (L->head);
//  int i;
//  
//  for(i=0;i<(L->size);i++) {
//    printf("Element %i = %i\n", i, currentNode->data);
//    currentNode = (currentNode->next);
//  }
//}

//bool list_insertFront( itemType value, List L ) { 
///*Inserts a new element into position 0 in the list. If it is the only element
//in the list, it will change L's values to reflect that

//Function Running Time: O(n)
//*/
//  nodeType *newNode;
//  newNode = (nodeType*)malloc(sizeof(nodeType));
//  
//  newNode->next = L->head;
//  newNode->previous = NULL;
//  newNode->data = value;
//  
//  if ((L->head)!=NULL) 
//    (L->head)->previous = newNode;
//    
//  else //else if L->head==NULL, then this is the only element in the list
//    L->tail = newNode;
//    
//  L->head = newNode;
//  L->size++;
//  
//  return true;
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

//bool list_deleteBack( List L ) {
///*Deletes the element at the last position. Frees the memory allocated to that
//node.

//Function Running Time: O(1)
//*/    
//  nodeType *tempNode;
//  tempNode = L->tail;  
//    
//  L->tail = (L->tail)->previous;
//  free(tempNode);
//  (L->size)--;
//  return true;
//}

//bool list_insert( unsigned int position, itemType value, List L ) {
///*Inserts a node into an indicated position. Note: Indexing starts at zero.

//Function Running Time O(n)
//*/
//  nodeType *currentNode;
//  currentNode = (nodeType*)malloc(sizeof(nodeType));
//  
//  currentNode=L->head; //Move to first node
//  
//  int i;
//  for(i=0; i<(L->size); i++) {
//        if (i==position) { //Then insert value
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

//bool list_delete( unsigned int position, List L ) {
///*Deletes a node from the list, reorganizes the pointers in the surrounding
//nodes, and frees the memory allocated to the deleted node

//Function Running Time: O(n)
//*/    
//    
//  nodeType *currentNode;
//  
//  currentNode=L->head; //Move to first node
//  
//  int i;
//  for(i=0; i<(L->size); i++) {
//        if (i==position) { //make previous jump over current to next
//            (currentNode->previous)->next = (currentNode->next);
//            L->size--;
//            
//            free(currentNode);
//            return true;
//        }
//        
//        else { //Move to next node
//            currentNode = (currentNode->next);
//        }
//  }

//  free(currentNode);
//  return false;
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
