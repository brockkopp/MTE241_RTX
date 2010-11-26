#include <string>
#include <sstream>
#include "debug.h"
#include "RTX.h"

using namespace std;

class RTX;
class MsgEnv;

MsgEnv* getMessage(int msgType, RTX* rtx);
string intToStr(int num);
string intToStr(int num, unsigned int length);
int strToInt(string input, int* output);
int parseString( string input, string output[], char token, int maxCount);
string getSigDesc(int sigNum);
int countChars(string haystack, char needle);
