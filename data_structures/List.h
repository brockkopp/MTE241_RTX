/*This class provides the Linked List functionality*/

#include <stdbool.h>

/* Type Definitions
 * --------------- */
typedef int itemType;

typedef struct listTag *List;


/* Function Declarations (prototypes)
 * --------------------------------- */

using namespace std;

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

};


