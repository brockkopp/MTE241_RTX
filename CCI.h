#ifndef H_CCI
#define H_CCI

#include "debug.h"
#include "tools.h"
#include "RTX.h"
#include "lib/WallClock.h"
#define SIGINT 2

class CCI
{
	public:
		CCI();
		WallClock* wallClock;

	private:
		void processCCI();
};

#endif
