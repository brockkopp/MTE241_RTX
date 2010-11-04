#include "../lib/Queue.h"
#include <iostream>

using namespace std;

int main() {
/*Test the queue class*/


int testdata1 = 1;
 int testdata2 = 2;
  int testdata3 = 3;
   int testdata4 = 4;

cout << "Instantiating queue!\n";
Queue q;

cout << "Enqueue\n";
q.enqueue( testdata1 );

cout << "Test length()\n";
int l = q.length();
cout << "Output of length(). Should = 1: " << l << "\n";

cout << "Dequeue. Also tests the 'edgecase' of dq when length == 1";
int* deq = q.dequeue();

cout << "Value of dequeued node. Should = 1:" << deq* << "\n";


test.enqueue( testdata2 );
test.enqueue( testdata3 );
test.enqueue( testdata4 );

test.dequeue( );
test.dequeue( );
test.dequeue( );
test.dequeue( );


return 0;
}
