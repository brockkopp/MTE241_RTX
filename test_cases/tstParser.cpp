#include "../debug.h"
#include "../CCI.h"

int parseString( string input, string output[], char token, int maxCount);

int main()
{
	string tokens[3];
	cout << CCI::parseString("test parsing function ", tokens, ' ', 3);
	cout << tokens[0] << "\n";
	cout << tokens[1] << "\n";
	cout << tokens[2] << "\n";
}


