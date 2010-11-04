#include "../lib/Queue.h"
#include <iostream>

using namespace std;

int main() {
/*Test the queue class*/

int testdata1 = 1;
 int testdata2 = 2;
  int testdata3 = 3;
   int testdata4 = 4;

Queue test;

test.enqueue( testdata1 );
test.enqueue( testdata2 );
test.enqueue( testdata3 );
test.enqueue( testdata4 );

test.dequeue( );
test.dequeue( );
test.dequeue( );
test.dequeue( );


return 0;
}
