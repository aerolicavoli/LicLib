/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    PassiveDiagnostic.cpp
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
#include "PassiveDiagnostic.h"


PassiveDiagnostic::PassiveDiagnostic(unsigned long updateInterval, HardwareInterface *hardWare, SystemState *state, MemoryManager *ptrMemoryManager)
{
    _timeKeeper = Metro(updateInterval);
    _hardWare = hardWare;
    _state = state;
    _ptrMemoryManager = ptrMemoryManager;

    _thetaLimitEnabled                    = false;
    _wrongPlugDummyDetectionEnabled       = false;
    _collisionDetectionEnabled            = false;
    _predictiveCollisionDetectionEnabled  = false;
    _stallDetectionEnabled                = false;
    _stallDetectionEnabled                = false;
    _thetaDriftDetectionEnabled           = false;

    _thetaLimit[0]    = -10.0;
    _thetaLimit[1]    =  10.0;

    _thetaEntranceLimit[0] = -2;
    _thetaEntranceLimit[1] =  2;

    _xLimit[0]        =  4.0;
    _xLimit[1]        =  106.0;

    _stallTime        =  0;
    _stallFailTime    =  800;  //ms to shutdown
    _velCmdStallRatio =  0.02;  //Ratio to determine stall TODO TUNE
    _noStallCmdThresh =  9.0;  //Cmds below 9 frequently stall.  Note, the motor class
                               // has a minimum cmd it will send to the motor. Anything below this is ignored
                               // this could be misintereted as a stall.

    _wrongEncFailTime = 2000;     //ms to shutdown
    _vel_cmd_wrongEnc =  1.23; //Ratio to determine wrong encoder TODO TUNE

    _ptrMemoryManager->registerParameter("SYS_XMIN", &_xLimit[0]);
    _ptrMemoryManager->registerParameter("SYS_XMAX", &_xLimit[1]);

    _ptrMemoryManager->registerParameter("SYS_THETAMIN", &_thetaLimit[0]);
    _ptrMemoryManager->registerParameter("SYS_THETAMAX", &_thetaLimit[1]);

    _ptrMemoryManager->registerParameter("SYS_STALLTIME", &_stallFailTime);
    _ptrMemoryManager->registerParameter("SYS_VELCMDSTALL", &_velCmdStallRatio);

    _ptrMemoryManager->registerParameter("SYS_WRONGENCTIME", &_wrongEncFailTime);
    _ptrMemoryManager->registerParameter("SYS_VELCMDWRONGENC", &_vel_cmd_wrongEnc);

}

PassiveDiagnostic::EnableAll(void)
{
    _thetaLimitEnabled                    = true;
    _wrongPlugDummyDetectionEnabled       = true;
    _collisionDetectionEnabled            = true;
    _predictiveCollisionDetectionEnabled  = true;
    _stallDetectionEnabled                = true;
    _thetaDriftDetectionEnabled           = true;
}

PassiveDiagnostic::DisableAll(void)
{
    _thetaLimitEnabled                    = false;
    _wrongPlugDummyDetectionEnabled       = false;
    _collisionDetectionEnabled            = false;
    _predictiveCollisionDetectionEnabled  = false;
    _stallDetectionEnabled                = false;
    _thetaDriftDetectionEnabled           = false;
}

PassiveDiagnostic::doStuff(unsigned long &time)
{
    if(0 < _timeKeeper.check(time))
    {
        /*
        ** Shutdown Criteria
        */
        if ((DIAGNOSTIC_STOP_TEMP == _state->getError()) || (DIAGNOSTIC_STOP_NEG_CORRECT == _state->getError()) || (DIAGNOSTIC_STOP_POS_CORRECT == _state->getError()))
        {
            _state->reportError(NO_ERROR, "No Error");
        }

        /*
        ** Shutdown at theta limits
        */
        if(_thetaLimitEnabled )
        {

          //Theta Out Of Bounds
          if( (DIAGNOSTIC_STOP_THETA != _state->getError()) && 
            ((_thetaLimit[0] >= _state->getTheta()) || (_thetaLimit[1] <= _state->getTheta())))
          {
            //Theta Out Of Bounds Error Out
            _state->reportError(DIAGNOSTIC_STOP_THETA, F("Theta Out Of Bounds"));
          } else if ( (DIAGNOSTIC_STOP_THETA == _state->getError()) && 
              ((_thetaEntranceLimit[0] <= _state->getTheta()) && (_thetaEntranceLimit[1] >= _state->getTheta())))
          {
            // Reset the error
            _state->reportError(NO_ERROR, "No Error");
          }         
        }

        /*
        ** Shutdown if opperator switched encoder plugs
        */
//        if(_wrongPlugDummyDetectionEnabled)
//        {
//            // Vel/cmd > threshold
//        }

        /*
        ** Shutdown at X limits
        */
        if( _collisionDetectionEnabled)
        {
            // Upper Limit Collsion
            if((_xLimit[1] <= _state->getX()) ||  _hardWare->xLimitSwitch() )
            {
                _state->reportError(DIAGNOSTIC_STOP_NEG_CORRECT, F("X Too Large"));
            }
            // Lower Limit Collsion
            else if ( _xLimit[0] >= _state->getX() )
            {
                _state->reportError(DIAGNOSTIC_STOP_POS_CORRECT, F("X Too Small"));
            }
        }

        /*
        ** Shutdown if collision predicted in near term
        */
        if(_predictiveCollisionDetectionEnabled)
        {
          if (_xLimit[1] <= _state->getXFuture())
          {
            _state->reportError(DIAGNOSTIC_STOP_NEG_CORRECT, F("X Future Too Large"));
          }
          else if(_xLimit[0] >= _state->getXFuture())
          {
            _state->reportError(DIAGNOSTIC_STOP_POS_CORRECT, F("X Future Too Small"));
          }

        }
                
        /*
        ** Shutdown if stall or excessive drag detected
        */
        if(_stallDetectionEnabled)
        {
          float absFiltCmd = abs(_state->_filtCmd);
          if( absFiltCmd > _noStallCmdThresh ) // Greater than zero check
          {
            if(abs(_state->_filtXdot)/absFiltCmd < _velCmdStallRatio )
            {
              if( -1 == _stallTime)
              {
                _stallTime = millis();  //A stall has started
              } else if((millis() - _stallTime) < _stallFailTime)
              {
                //Allow cart enough time to move
              } else
              {
                //Shut it down, a stall has been detected
                _state->reportError(DIAGNOSTIC_STOP, F("Stall Detected. Vel/Cmd < threshold"));
              }    
            } else
            {
              _stallTime = -1;
            }
          } 
            
            
        }

        /*
        ** Warn Criteria
        */

        /*
        ** Significant X Drift On Single Power Cycle
        ** Observed during home operations
        */
//        if(_xDriftDetectionEnabled)
//        {
//            //Previous X declaration far from new X declaration
//        }

        /*
        ** Significant theta Drift On Single Power Cycle
        ** Observed during volunteer reporting from control logic
        */
//        if(_thetaDriftDetectionEnabled)
//        {
//            //Previous theta declaration far from new theta declaration
//        }


        /*
        ** Count designed to be large enough to ignore UI related delays
        */
        if(Metro::cpuOverloadCount > 100)
        {

            Metro::cpuOverloadCount = 0;
            _state->reportError(DIAGNOSTIC_WARN, F("1202 Alarm"));

        }

        //int meomory = freeMemory();

        /*
        ** Update Loop Time
        */
        time = millis();

    }

}
