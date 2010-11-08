#ifndef H_WALL_CLOCK
#define H_WALL_CLOCK

#include "../debug.h"
#include "../tools.h"
#include <sstream>

class WallClock
{
	private:
		unsigned int _hours;
		unsigned int _minutes;
		unsigned int _seconds;
		bool _isDisplayed;
		bool _isNewTime;

	public:
		WallClock();
		void increment();
		int setTime( int hours, int minutes, int seconds );
		int setTime(string time[]);
		void setDisplayed(bool isDisplayed);
		string toString();
};
#endif
