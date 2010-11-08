#ifndef H_PQ
#define H_PQ

/*The PQ class provides Priority Queue functionality*/
#include "PCB.h"
#include "Queue.h"
#include "../debug.h"

typedef int pItemType;

class PQ {
  public:
  	PQ( int nPriority );
  	~PQ( );
	  bool pq_enqueue ( PCB* newData, int priorityLevel );
	  itemType pq_dequeue( );       
  
  private:
    Queue _master[];
};

#endif
