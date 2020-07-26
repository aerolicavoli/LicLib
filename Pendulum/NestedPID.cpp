/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    NestedPID.cpp
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
*
*
*
*          +              kp_outer +          Theta REf                 kp_inner   +
* X_Ref ---->O--------------|>------O-----[filt]-----O---------------------|>----O---CMD
*           -|   |                  |+\+           + |-                         +|    |
*            |   ---[int]---|>-------  \             |    ---[dtheta/dt]---|>----     |
*            |            ki_outer      ^kd_outer    |    |           kd_inner      [plant]
*            |                           |           |    |                          |  |
*            |                         [div]         |    |                          |  |
*            |                           |           ---------------------------------  |
*            ----------------------------------------------------------------------------
*
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "NestedPID.h"

/*
** Executed each time the routine is changed to this one
*/
NestedPID::setup()
{

    /*
    ** Start with motor dissabled. Serial cmd will
    ** required to start device.
    */
    _ptrState->setMotorEnabled(false);

    /*
    ** Clear all diagnostics
    */
    _ptrDiagnostic->DisableAll();

    _ptrXInt->reset();

    /*
    ** Stop at x limits
    ** Stop at theta limits
    */
    _ptrDiagnostic->EnableCollisionDetection();
    _ptrDiagnostic->EnablePredictiveCollisionDetection();
    _ptrDiagnostic->EnableThetaLimit();
    _ptrDiagnostic->EnableStallDetection();

    Serial.println(F("NESTED PID ROUTINE: Setup Complete"));

}

/*
** Executed every computer cycle.  Frequency variable, dependant on other tasks.
*/
NestedPID::doStuff(unsigned long &time)
{
    /*
    ** Limit gains. Gains can be updated on the fly.  If a sign error occurs
    ** it could cause damage to the machine
    */
    _kp_outer = limit(_kp_outer,-0.2,0.0);
    _ki_outer = limit(_ki_outer,-0.2,0.0);
    _kd_outer = limit(_kd_outer, 0.0,0.2);
 
    _kp_inner = limit(_kp_inner, 0.0,200.0);
    _kd_inner = limit(_kd_inner, 0.0,0.0);

    _XRef = limit(_XRef, 2.0, 110.0);

    /*
    ** Check if desired dt has passed between executions
    */
    if(0 < _timeKeeper.check(time))
    {
      float xError = _XRef - _ptrState->X;
      float thetaRef = xError*_kp_outer;
      bool integrationEnabled = (NEUTRAL_ERROR >= _ptrState->getError());
      // thetaRef += _ptrXInt->calcIntegral( xError*_ki_outer, _integratorLimit, time);
      float intValue = _ptrXInt->calcIntegral( xError*_ki_outer, _integratorLimit, time,integrationEnabled);
      
      thetaRef += intValue;
      
      thetaRef += _ptrState->Xdot*_kd_outer;

      _ptrState->cmd =  (_ptrOuterCmdFilt->calcFilter(thetaRef) - _ptrState->Theta)*_kp_inner + _ptrState->Thetadot*_kd_inner;

      /*
      ** Update loop time
      */
      time = millis();
    }

}
