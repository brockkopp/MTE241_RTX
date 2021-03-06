#ifndef H_DEBUG
#define H_DEBUG

#include <iostream>
#include <sstream>
#include <string>
using namespace std;

//Project wide variables
#define EXIT_SUCCESS 	0
#define EXIT_ERROR	-1

#define DEBUG_MODE	0
#define TESTS_MODE	0

void die(int sigNum);
void debugMsg(string message);
void debugMsg(int message);
void debugMsg(string message, int newLinesB, int newLinesA);
int assure(bool condition, string message, string fileName, int lineNum, string func, bool isFatal);
#endif
