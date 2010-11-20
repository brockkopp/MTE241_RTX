#ifndef H_CCI
#define H_CCI

#include "debug.h"
#include "tools.h"
#include "RTX.h"
#include "lib/Queue.h"
#include "lib/WallClock.h"
#define SIGINT 2

class CCI
{
	public:
		CCI();
		~CCI();
		int processCCI();

		WallClock* wallClock;
		Queue*  userInputs;
};

#endif
