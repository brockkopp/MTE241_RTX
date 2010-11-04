#include "PCB.h"
#include "Queue.h"

using namespace std;

/*The PQ class provides Priority Queue functionality*/

typedef int itemType;

class PQ {
  public:
  	PQ( itemType target, int nPriority );
    void penqueue (PCB newData, int priorityLevel);
    itemType * pdequeue ();       
  
  private:
    Queue master[];
};
