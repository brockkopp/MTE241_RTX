#ifndef H_DEBUG
#define H_DEBUG

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
//#include "lib/PCB.h" //<--- Was causing looping dependancy and redefinition issues during build. Uncomment it and give it a try -Karl
using namespace std;

//Project wide variables
#define EXIT_SUCCESS 	0
#define EXIT_ERROR	1
#define DEBUG_MODE	1

void die(int sigNum);
void debugMsg(string message);
void debugMsg(string message, int newLinesB, int newLinesA);
int assure(bool condition, string message, string fileName, int lineNum, string func, bool isFatal);

#endif
