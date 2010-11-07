#include "WallClock.h"

WallClock::WallClock()
{
	_seconds = 0;
	_minutes = 0;
	_hours = 0;
	_isDisplayed = false;
	_isNewTime = false;
}

void WallClock::increment()
{
	_seconds ++;
	if (_seconds > 60)
	{
		_seconds = _seconds - 60;
		_minutes++;
		if(_minutes > 60)
		{
			_minutes = _minutes - 60;
			_hours = (++_hours % 24);
		}
		_isNewTime = true;
	}
}

int WallClock::setTime( int hours, int minutes, int seconds )
{
	int ret = EXIT_SUCCESS;

	if(hours >= 24 || minutes >= 60 || seconds >= 60) 
		ret = EXIT_ERROR;
	else
	{
		_hours = hours;
		_minutes = minutes;
		_seconds = seconds;
	}

	return ret;
}

string WallClock::toString()
{
	stringstream ret;
	
	return intToStr(_hours,2) + ":" + intToStr(_minutes,2) + ":" + intToStr(_seconds,2);
}
