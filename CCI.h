#ifndef H_CCI
#define H_CCI

#include "debug.h"
#define SIGINT 2
class CCI
{
	public:
		CCI();
		static int parseString( string input, string output[], char token, int maxCount);
		void processCCI();
};

#endif
