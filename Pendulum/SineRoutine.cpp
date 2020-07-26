/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    SineRoutine.cpp
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
#include "SineRoutine.h"

/*
** Executed each time the routine is changed to this one
*/
SineRoutine::setup()
{

    /*
    ** Start with motor enabled.  Otherwise, serial cmd will
    ** be required to start device.
    */
    _ptrState->setMotorEnabled(true);

    /*
    ** Do not monitor for faults.
    ** I have this dissabled just for motor testing off the device
    ** At least xlimit protections should be used for control schemes.
    */
    _ptrDiagnostic->DisableAll();
    _ptrDiagnostic->EnableCollisionDetection();

    Serial.println(F("SINE ROUTINE: Setup Complete"));

}

/*
** Executed every computer cycle.  Frequency variable, dependant on other tasks.
*/
SineRoutine::doStuff()
{
    /*
    ** Check if desired dt has passed between executions
    */
    if(0 < _timeKeeper.check())
    {
        /*
        ** Calculate A cmd and give it to _state
        ** See SystemState.h for full list of available states and dstates
        */
        if ( abs(_period) < 100)
        {
            _ptrState->setCmd(0);
        }
        else
        {
            _ptrState->setCmd(sin(2 * 3.14 * _ptrState->getTimeNow() / _period)*_amp);
        }
    }

}
