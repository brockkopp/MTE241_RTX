#include "rtx.h"

using namespace std;

/*The PQ class provides Priority Queue functionality*/

class PQ {
  public:
    void penqueue (PCB new_data, priority_level);
    PCB * pdequeue ();       
  
  private:
    Queue master[];
};
