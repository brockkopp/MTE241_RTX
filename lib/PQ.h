#ifndef H_PQ
#define H_PQ

/*The PQ class provides Priority Queue functionality*/
#include "PCB.h"
#include "Queue.h"
#include "../debug.h"

#define PRIORITY_LEVELS 4

class PCB;
class Queue;

class PQ {
  public:
  	PQ( int nPriority );
  	~PQ( );
		bool pq_enqueue ( PCB** newData, int priorityLevel );
		PCB* pq_dequeue( );
		PCB* pq_pluck( PCB** target); 
		string toString();  
  	int length();
  
  private:
		Queue* _master[ PRIORITY_LEVELS ]; 
};

#endif
