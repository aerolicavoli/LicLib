/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    Metro.cpp
*  Version:      2
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
*
* TODO List:
*            On startup, before any check method is called on any metro objects
*            adjust timeing offsets to
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Metro.h"

//int Metro::cpuOverloadCount = 0;

Metro::Metro()
{

    _interval_millis = 1000;
    _startOffset_millis = 0;
    _previous_millis = millis();
    _firstCall = true;

}


Metro::Metro(unsigned long interval_millis)
{

    _interval_millis = interval_millis;
    _startOffset_millis = 0;

}

Metro::Metro(unsigned long interval_millis, unsigned long _startOffset_millis)
{

    _interval_millis = interval_millis;
    _startOffset_millis = _startOffset_millis;

}


void Metro::interval(unsigned long interval_millis)
{
    _interval_millis = interval_millis;
}

unsigned long Metro::getInterval(void)
{
    return _interval_millis;
}

uint8_t Metro::check()
{
  unsigned long now = millis();
  return(check(now));
}

uint8_t Metro::check(unsigned long &now)
{

    if ( _interval_millis == 0 )
    {
        _previous_millis = now;
        return 1;
    }

    unsigned long timeDiff;
    if(_firstCall)
    {
        timeDiff = now - _previous_millis - _startOffset_millis;
    }
    else
    {
        timeDiff = now - _previous_millis;
    }

    if ( timeDiff >= _interval_millis)
    {


        if(clockCatchUp)
        {
            _previous_millis = now - _interval_millis + timeDiff;

        }
        else
        {
            _previous_millis = now ;
        }


        /*
        ** If time is nearly double interval, throw cpuOverload warrning
        */
        if (timeDiff > (1.1 * _interval_millis))
        {
            cpuOverloadCount++;
        }

        if (_firstCall)
        {
            _firstCall = false;
        }

        return 1;
    }

    return 0;

}

void Metro::reset()
{

    _previous_millis = millis();
    _firstCall = true;

}
