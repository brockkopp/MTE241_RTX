/*The PQ class provides Priority Queue functionality*/
#include "PCB.h"
#include "Queue.h"

typedef int pItemType;

class PQ {
  public:
  	PQ( int nPriority );
    void penqueue (PCB newData, int priorityLevel);
    pItemType * pdequeue ();       
  
  private:
    Queue _master[];
};
