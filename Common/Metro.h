/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    Metro.h
*  Version:      2
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef Metro_h
#define Metro_h

#include <inttypes.h>


class Metro
{

public:
    Metro();
    Metro(unsigned long interval_millis);
    Metro(unsigned long interval_millis, unsigned long startOffset_millis);
    void interval(unsigned long interval_millis);
    unsigned long getInterval();
    uint8_t check();
    uint8_t check(unsigned long &now);
    void reset();
    static int cpuOverloadCount;
    static bool clockCatchUp;

private:
    unsigned long  _previous_millis, _interval_millis, _startOffset_millis;
    bool _firstCall;

};

#endif
