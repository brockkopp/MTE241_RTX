#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include "data_structures/PCB.h"
using namespace std;

//Project wide variables
#define EXIT_SUCCESS 	0
#define EXIT_ERROR	1
#define DEBUG_MODE	1

void debugMsg(string message);
void debugMsg(string message, int newLinesB, int newLinesA);
int assure(bool condition, string message, string fileName, int lineNum, bool isFatal);
