#include "../libs.h"

class WallClock
{
	private:
		unsigned int _hours;
		unsigned int _minutes;
		unsigned int _seconds;
		bool _isDisplayed;
		bool _isNewTime;

	public:
		unsigned int increment();
		unsigned int setTime( int hours, int mins, int seconds );
		string toString();
};
