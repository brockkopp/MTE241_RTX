#include "WallClock.h"

WallClock::WallClock(unsigned int tickLength)
{
	_miliseconds = 0;
	_seconds = 0;
	_minutes = 0;
	_hours = 0;
	_isDisplayed = false;
	_isNewTime = false;
	_tickLength = tickLength;
}

void WallClock::setDisplayed(bool isDisplayed)
{
	_isDisplayed = isDisplayed;
	_isNewTime = isDisplayed;
	if (isDisplayed)			//TESTING ONLY
		ualarm(_tickLength,_tickLength);
}

void WallClock::increment()
{
	_miliseconds += 100;

	if(_miliseconds >= 1000)
	{
		_miliseconds -= 1000;
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

int WallClock::setTime(string time[])
{
	int ret = EXIT_ERROR;
	int hours = 0, minutes = 0, seconds = 0;

	if (strToInt(time[0],&hours) == EXIT_SUCCESS && strToInt(time[1],&minutes) == EXIT_SUCCESS && strToInt(time[2],&seconds) == EXIT_SUCCESS)
		ret = setTime(hours, minutes, seconds);

	return ret;
}

string WallClock::toString()
{
	string ret;

	if(_isDisplayed && _isNewTime)
	{
		_isNewTime = false;
		ret = intToStr(_hours,2) + ":" + intToStr(_minutes,2) + ":" + intToStr(_seconds,2);
	}
	else
		ret = "";

	return ret;
}
