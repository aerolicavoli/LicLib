/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    Motor.cpp
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
#include "Motor.h"

Motor::Motor(unsigned long updateInterval, HardwareInterface *hardWare, SystemState *state, MemoryManager *ptrMemoryManager)
{
    _timeKeeper = Metro(updateInterval);
    _hardWare = hardWare;
    _ptrMemoryManager = ptrMemoryManager;
    _state = state;
    _lastCmd = 0;
    
    /*
    ** 255 is the max, 12V applied to the motor
    ** This is hard on the machine, limit to avoid
    ** damage
    */
    _maxCmd = 94; 
    _minCmd = 7;
    /*
    ** Soft start scales cmds starting from zero over a short
    ** period after restarting from a shutdown event.
    */    
    _timeSinceLastStop = 0;
    _softStartDuration = 3000; //ms

    /*
    ** Register Parameters with eeprom memory manager
    */
    _ptrMemoryManager->registerParameter("SYS_MAXCMD", &_maxCmd);
}

Motor::doStuff(unsigned long &time)
{
    if(0 < _timeKeeper.check(time))
    {
        int cmd = _state->getCmd();

        if( ( ( NEUTRAL_ERROR >= _state->getError() ) ||
                ((DIAGNOSTIC_STOP_NEG_CORRECT == _state->getError()) && (cmd < 0)) ||
                ((DIAGNOSTIC_STOP_POS_CORRECT == _state->getError()) && (cmd > 0)) )
                && ( true == _state->getMotorEnabled()) )
        {
          
          // Cmd Conditioning
          if( _maxCmd < abs(cmd) )
          {
            if(cmd > 0)
            {
                cmd = _maxCmd;
            }
            else
            {
                cmd = -_maxCmd;
            }
          } else if( _minCmd >= abs(cmd) )
          {
            cmd = 0;
          }

          
          // Scale Cmds for soft start
          if(_state->getSoftStart() && ((time - _timeSinceLastStop) < _softStartDuration))
          {
            cmd = (float)cmd*(time - _timeSinceLastStop)/_softStartDuration;
          }

          if( _lastCmd != cmd )
          {
              _hardWare->moveMotor(cmd);
              _lastCmd = cmd;
          }
        }
        else
        {
          _timeSinceLastStop = time;
          _hardWare->stop();
        }

        /*
        ** Update Loop Time
        */
        time = millis();
    }

}
