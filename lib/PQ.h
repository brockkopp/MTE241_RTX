#ifndef H_PQ
#define H_PQ

/*The PQ class provides Priority Queue functionality*/
#include "PCB.h"
#include "Queue.h"
#include "../debug.h"

#define PRIORITY_LEVELS 4

class PCB;
class Queue;

typedef PCB* pItemType;

class PQ {
  public:
  	PQ( int nPriority );
  	~PQ( );
		bool pq_enqueue ( pItemType newData, int priorityLevel );
		pItemType pq_dequeue( );
		pItemType pq_pluck( pItemType target); 
		string toString();  
  	int length();
  
  private:
		Queue* _master[ PRIORITY_LEVELS ]; //<-- I still believe this should not be hardcoded (!) --Karl
};

#endif
