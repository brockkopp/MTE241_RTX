#ifndef H_WALL_CLOCK
#define H_WALL_CLOCK

#include "../debug.h"
#include "../tools.h"
#include <sstream>

class WallClock
{
	public:
		WallClock(unsigned int tickLength);
		bool increment();
		int setTime( int hours, int minutes, int seconds );
		int setTime(string time[]);
		void setDisplayed(bool isDisplayed);
		string toString();
		
	private:
		unsigned int _hours;
		unsigned int _minutes;
		unsigned int _seconds;
		unsigned int _miliseconds;
		bool _isDisplayed;
		bool _isNewTime;
		unsigned int _tickLength;
};
#endif
