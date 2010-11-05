/*The PQ class provides Priority Queue functionality*/
#include "PCB.h"
#include "Queue.h"

typedef int itemType;

class PQ {
  public:
  	PQ( itemType target, int nPriority );
    void penqueue (PCB newData, int priorityLevel);
    itemType * pdequeue ();       
  
  private:
    Queue _master[];
};
