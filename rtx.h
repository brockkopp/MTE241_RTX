//#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
using namespace std;

//Global Defines
#define DEBUG_MODE	1
#define EXIT_SUCCESS 	0
#define EXIT_ERROR	1

//debug.cpp
void debugMsg(string message);
int assure(bool condition, string message, string fileName, int lineNum, bool isFatal);

//timingServices.cpp
void test();
