/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    SystemState.cpp
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
#include "SystemState.h"



SystemState::SystemState(unsigned long updateInterval, HardwareInterface *hardWare, MemoryManager *ptrMemoryManager)
{
    _timeKeeper = Metro(updateInterval);

    _hardWare = hardWare;

    _ptrMemoryManager = ptrMemoryManager;
    /*
    ** Initialize State Variables
    */
    XZeroingOffset = 0.0;
    ThetaZeroingOffset = 0.0;

    Xpast = 0.0;
    Xdotpast = 0.0;
    X = 70.0;
    Xdot = 0.0;
    _filtXdot = 0.0;
    Xdotdot = 0.0;
    Xfuture = 0.0;
    XdotFiltered = 0.0;
    goodX = false;

    Thetapast = 0.0;
    Thetadotpast = 0.0;
    Theta = 0.0;
    ThetaLimited = 0.0;
    Thetadot = 0.0;
    Thetadotdot = 0.0;
    Thetafuture = 0.0;
    goodTheta = false;

    _pulse2CM = -0.0042; // 1.61798*3.14/2400
    _pulse2Deg = 0.05859375; //360.0/(4096.0*1.5)

    TimePast = millis() - updateInterval;
    TimeNow = millis();
    TimeFuture = millis() + updateInterval;

    ErrorState = NO_ERROR;
    //memset(ErrorString,0,100);
    ErrorString = F("NO_ERROR");

    cmd = 0;
    _filtCmd = 0.0;

    encTheta = 0;
    encX = 0;
    motorEnabled = false;
    softStart = true;

    _errorReportingEnabled = true;

    _printThis = 555.5;
    //ptrMemoryManager->registerParameter("SYS_PULSE2CM", &_pulse2CM);

    //Moving Average Filter
    _dxFilt = new DigitalFilter(14);
        _dxFilt->_taps[0] = 0.071428;
        _dxFilt->_taps[1] = 0.071428;
        _dxFilt->_taps[2] = 0.071428;
        _dxFilt->_taps[3] = 0.071428;
        _dxFilt->_taps[4] = 0.071428;
        _dxFilt->_taps[5] = 0.071428;
        _dxFilt->_taps[6] = 0.071428;
        _dxFilt->_taps[7] = 0.071428;
        _dxFilt->_taps[8] = 0.071428;
        _dxFilt->_taps[9] = 0.071428;
        _dxFilt->_taps[10] = 0.071428;
        _dxFilt->_taps[11] = 0.071428;
        _dxFilt->_taps[12] = 0.071428;
        _dxFilt->_taps[13] = 0.071428;

     //Moving Average Filter
    _cmdFilt = new DigitalFilter(14);
        _dxFilt->_taps[0] = 0.071428;
        _dxFilt->_taps[1] = 0.071428;
        _dxFilt->_taps[2] = 0.071428;
        _dxFilt->_taps[3] = 0.071428;
        _dxFilt->_taps[4] = 0.071428;
        _dxFilt->_taps[5] = 0.071428;
        _dxFilt->_taps[6] = 0.071428;
        _dxFilt->_taps[7] = 0.071428;
        _dxFilt->_taps[8] = 0.071428;
        _dxFilt->_taps[9] = 0.071428;
        _dxFilt->_taps[10] = 0.071428;
        _dxFilt->_taps[11] = 0.071428;
        _dxFilt->_taps[12] = 0.071428;
        _dxFilt->_taps[13] = 0.071428;

}

void SystemState::setX(float newX)
{
    /*
    ** cm
    */
    _hardWare->writeX(newX);
    X = _hardWare->readX();
    Xpast = X;
    goodX = true;
}

void SystemState::setTheta(float newTheta)
{
    /*
    ** deg
    */
    _hardWare->writeTheta(newTheta);
    Theta = _hardWare->readTheta();
    Thetapast = Theta;
    goodTheta = true;
}

SystemState::doStuff(unsigned long &time)
{
      Xpast = X;
      //Xdotpast = Xdot;
      Thetapast = Theta;
      //Thetadotpast = Thetadot;
      TimePast = TimeNow;
      TimeNow = time;
      //TimeFuture = 2 * TimeNow - TimePast;
      //float dt = (TimeNow - TimePast) / 1000.0;
      float OneOverdt = 1000.0 / (TimeNow - TimePast); //1.0 / dt;

      /*
      ** Update
      */
      X = encX * _pulse2CM - XZeroingOffset;
      Theta = encTheta * _pulse2Deg - ThetaZeroingOffset; 
      ThetaLimited = constrainTo180(encTheta);
      //X = _hardWare->readX();
      //Theta = _hardWare->readTheta();

      /*
      ** Differentiate
      */
      Xdot = (X - Xpast) * OneOverdt;
      //Xdotdot = (Xdot - Xdotpast) * OneOverdt;
      Thetadot = (Theta - Thetapast) * OneOverdt;
      //Thetadotdot = (Thetadot - Thetadotpast) * OneOverdt;

      /*
      ** Integrate
      */
      Xfuture = X +  X - Xpast;
      Thetafuture = Theta + Theta - Thetapast;

      _filtXdot = _dxFilt->calcFilter(Xdot);
      _filtCmd = _cmdFilt->calcFilter((float)cmd);

      /*
      ** Update Loop Time
      */
      time = millis();

}
