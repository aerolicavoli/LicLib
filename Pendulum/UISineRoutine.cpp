/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    UISineRoutine.cpp
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "UISineRoutine.h"


UISineRoutine::UISineRoutine(unsigned long updateInterval, SystemState *state, PassiveDiagnostic *Diagnostic)
{
    _timeKeeper = Metro(updateInterval);
    _state = state;
    _Diagnostic = Diagnostic;

    _xMapMin = -10.0;
    _xMapMax = 10.0;
    _periodMin =  2500.0;
    _periodMax = 25000.0;

    _thetaMapMin = -700.0;
    _thetaMapMax = 700.0;
    _ampMin =  40.0;
    _ampMax = 254.0;
}

UISineRoutine::setup()
{
    /*
    ** Start with motor turned on.  No waiting, just go.
    */
    _state->setMotorEnabled(true);

    /*
    ** Do not monitor for faults.
    ** I have this dissabled just for motor testing off the device
    ** At least xlimit protections should be used for control schemes.
    */
    _Diagnostic->DisableAll();
}

UISineRoutine::doStuff()
{
    if(0 < _timeKeeper.check())
    {
        double period, amp;

        /*
        ** Get user input
        */
        if(_xMapMin > _state->getX())
        {
            period = _periodMin;
        }
        else if(_xMapMax < _state->getX())
        {
            period = _periodMax;
        }
        else
        {
            period = (_state->getX() - _xMapMin) * (_periodMax - _periodMin)
                     / (_xMapMax - _xMapMin) + _periodMin;
        }


        if(_thetaMapMin > _state->getX())
        {
            amp = _ampMin;
        }
        else if(_thetaMapMax < _state->getX())
        {
            amp = _ampMax;
        }
        else
        {
            period = (_state->getX() - _thetaMapMin) * (_ampMax - _ampMin) / (_thetaMapMax - _thetaMapMin) + _ampMin;
        }

        int cmd;

        if ( abs(period) < 2505.0)
        {
            cmd = 0;
        }
        else
        {
            cmd = sin(2 * 3.14 * _state->getTimeNow() / period) * amp;
        }

        _state->setCmd(cmd);

    }

}
