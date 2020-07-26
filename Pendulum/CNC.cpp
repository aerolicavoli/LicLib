/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    CNC.cpp
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
#include "CNC.h"


CNC::setup()
{
    /*
    ** Start with motor turned on.  No waiting, just go.
    */
    _ptrState->setCmd(0);
    _ptrState->setMotorEnabled(true);
    _gotoXPos = _ptrState->getX();

    /*
    ** Do not monitor for faults.
    ** I have this dissabled just for motor testing off the device
    ** At least xlimit protections should be used for control schemes.
    */
    _ptrDiagnostic->DisableAll();
    _ptrDiagnostic->EnableCollisionDetection();

    _startTime = 0;
}

CNC::doStuff()
{
    if(0 < _timeKeeper.check())
    {
        /*
        ** Distance Command Recieved, closed loop move distance;
        */
        if(0.001 < abs(_ptrState->getX() - _gotoXPos))
        {

            //BANG BANG
            // Are we there yet?
            if( _walkingMovePrecision > abs(_ptrState->getX() - _gotoXPos) )
            {
                _moving = false;
                _ptrState->setCmd(0);
            }
            // Have we even started
            else if(!_moving)
            {
                if(0 < (_ptrState->getX() - _gotoXPos) )
                {
                    _ptrState->setCmd(-_walkSpeed);
                }
                else
                {
                    _ptrState->setCmd(_walkSpeed);
                }
                _ptrState->setMotorEnabled(true);
                _moving = true;
            }
            // Wait
            else
            {
                if(0 < (_ptrState->getX() - _gotoXPos) )
                {
                    _ptrState->setCmd(-_walkSpeed);
                }
                else
                {
                    _ptrState->setCmd(_walkSpeed);
                }
            }

            //Parabolic
            // float positionError = _ptrState->getX() - _gotoXPos;
            
            // // Position Dead Band
            // if( _walkingMovePrecision < abs(positionError) )
            // {
            //   float targetRate = sign(positionError)*_kParabolicCNCRateGain1*sqrt(abs(positionError));
            //   float rateError = _ptrState->getXdot()-targetRate;
            //   _ptrState->setCmd((int)(rateError*_kParabolicCNCRateGain2));
            // }
            // // Wait
            // else
            // {
            //     _ptrState->setCmd(0);
            // }


        }
        /*
        ** Time Command Recieved
        */
        else if(0.001 < abs(_moveXTime))
        {
            /*
            ** Start the timer
            */
            if(_startTime == 0)
            {
                _startTime = millis();
            }
            /*
            ** Move while its not time yet.
            */
            if((millis() - _startTime) < abs(_moveXTime))
            {
                // Positive X for positive time
                if(0 < _moveXTime )
                {
                    _ptrState->setCmd(_walkSpeed);
                }
                else
                {
                    _ptrState->setCmd(-_walkSpeed);
                }
            }
            // Timer has expired
            else
            {
                _startTime =  0;
                _moveXTime =  0;
                _ptrState->setCmd(0);
            }
        }

        /*
        ** Check for illegal Cmds
        */
        if(_gotoXPos < _ptrDiagnostic->_xLimit[0])
        {
            _gotoXPos = _ptrDiagnostic->_xLimit[0];
        }
        else if(_gotoXPos > _ptrDiagnostic->_xLimit[1])
        {
            _gotoXPos = _ptrDiagnostic->_xLimit[1];
        }
    }
}
