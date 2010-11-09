#ifndef H_PQ
#define H_PQ

/*The PQ class provides Priority Queue functionality*/
#include "PCB.h"
#include "Queue.h"
#include "../debug.h"

typedef PCB* pItemType;

class PQ {
  public:
  	PQ( int nPriority );
  	~PQ( );
	  bool pq_enqueue ( pItemType newData, int priorityLevel );
	  pItemType pq_dequeue( );       
  
  private:
	//Queue* _master[];
	Queue* _master[4];
};

#endif
