/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    HomeRoutine.cpp
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "HomeRoutine.h"

/*
** Executed each time the routine is changed to this one
*/
HomeRoutine::setup()
{

    /*
    ** Start with motor enabled.  Otherwise, serial cmd will
    ** be required to start device.
    */
    _ptrState->setMotorEnabled(true);

    /*
    ** Do not monitor for x out of bounds.
    */
    _ptrDiagnostic->DisableAll();

    /*
    ** Monitor for motor stall
    */
    _ptrDiagnostic->EnableStallDetection();

    _phaseNumber = 0;

    Serial.println(F("HOME ROUTINE: Setup Complete"));

}

/*
** Executed every computer cycle.  Frequency variable, dependant on other tasks.
*/
HomeRoutine::doStuff()
{
    /*
    ** Check if desired dt has passed between executions
    */
    if(0 < _timeKeeper.check())
    {
        switch(_phaseNumber)
        {
        case START_HOME:
            _ptrState->setCmd(-_coarseAlignSpeed);
            _moveTime = millis();
            _phaseNumber = DETECT_COLLISION;
            break;
        // Move towards limit switch
        case DETECT_COLLISION:
            if(_ptrHardware->xLimitSwitch())
            {
                _ptrState->setCmd( _coarseAlignSpeed);
                _moveTime = millis();
                _phaseNumber =REVERSE;
            }
            else if((millis() - _moveTime) > _coarseAlignTimeout)
            {
                _ptrState->setCmd(0);
                _phaseNumber = ABORT_HOME_ROUTINE;
            }
            break;
        case REVERSE:
            if((millis() - _moveTime) >= _reverseTime)
            {
                _ptrState->setCmd(-_fineAlignSpeed);
                _moveTime = millis();
                _phaseNumber++;
            }
            break;
        case FINE_ALIGN:
            if(_ptrHardware->xLimitSwitch())
            {
                _ptrHardware->stop();
                _ptrState->zeroX();
                _ptrState->setCmd(_midpointMoveSpeed);
                _phaseNumber++;
            }
            else if((millis() - _moveTime) > _FineAlignTimeout)
            {
                _ptrState->setCmd(0);
                _phaseNumber = ABORT_HOME_ROUTINE;
            }
            break;
        case HOMEING_COMPLETE:
            if(_ptrState->getX() >= _postHomePosition )
            {
                _ptrState->setCmd(0);
                _ptrState->setGoodX();
            }
            break;
        case ABORT_HOME_ROUTINE:
            _ptrState->setCmd(0);
            _ptrState->reportError(DIAGNOSTIC_STOP, "Homeing Timeout");
            break;
        default:
            _ptrState->setCmd(0);
            _ptrState->reportError(CODE_ERROR, "Homeing Unkown Switch Error");
            break;
        }

    }

}
