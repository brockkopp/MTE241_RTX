/*The PQ class provides Priority Queue functionality*/
#include "PCB.h"
#include "Queue.h"

typedef int itemTypePq;		//Why doesnt this use the Queue data type? (brock)

class PQ {
  public:
  	PQ( itemType target, int nPriority );
	    void penqueue (PCB newData, int priorityLevel);
	    itemTypePq * pdequeue ();       
  
  private:
    Queue _master[];
};
