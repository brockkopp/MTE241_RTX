/*This class provides the Linked List functionality,
it is based off of code writted for teh 1B MTE 140 - Data Structures
course.*/

#include <stdbool.h>

using namespace std;

typedef void* itemType;
typedef struct node {
  itemType data;
  struct node *next;
  struct node *previous;
} nodeType;

class List {


  public:
  	List list_create( );

		void list_destroy( List L );

		bool list_isEmpty( const List L );

		int list_length( const List L );

		void list_print( const List L );

		bool list_insertFront( itemType value, List L );

		bool list_insertBack( itemType value, List L );

		bool list_deleteFront( List L );

		bool list_deleteBack( List L );

		bool list_insert( unsigned int position, itemType value, List L );

		bool list_delete( unsigned int position, List L );

		int list_search( itemType Value, List L );

		itemType *list_select( unsigned int position, List L );

		bool list_replace( unsigned int position, itemType value, List L );
  
  private:

		nodeType *head;
  	nodeType *tail;
  	int size;
};


