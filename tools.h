#include <string>
#include <sstream>
using namespace std;

string intToStr(int num);
string intToStr(int num, unsigned int length);
int strToInt(string input, int* output);
int parseString( string input, string output[], char token, int maxCount);
string getSigDesc(int sigNum);
