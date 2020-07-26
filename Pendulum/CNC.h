/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    CNC.h
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef CNC_h
#define CNC_h

#include "Controller.h"
#include "utility.h"

class CNC : public Controller
{

public:
    CNC(unsigned long UpdateInterval, PassiveDiagnostic *ptrDiagnostic, HardwareInterface *ptrHardware, SystemState *ptrState, MemoryManager *ptrMemoryManager) :
        Controller(UpdateInterval, ptrDiagnostic, ptrHardware, ptrState, ptrMemoryManager)
    {
        _gotoXPos      = 0.0; // cm
        _moveXTime     = 0; // ms
        _zeroX         = 0;
        _jogSpeed      = 90;
        _walkSpeed     = 60;
        _moving        = false;
        _walkingMovePrecision = 1.9; // cm
        _kParabolicCNCRateGain1 = -5;
        _kParabolicCNCRateGain2 = -1;

        _ptrMemoryManager->registerParameter("MOVE",   &_gotoXPos);
        _ptrMemoryManager->registerParameter("MOVE_TIME",  &_moveXTime);
        _ptrMemoryManager->registerParameter("ZERO_X",     &_zeroX);
        _ptrMemoryManager->registerParameter("JOG_SPEED",  &_jogSpeed);
        _ptrMemoryManager->registerParameter("WALK_SPEED", &_walkSpeed);
        _ptrMemoryManager->registerParameter("MOVE_PRECISION", &_walkingMovePrecision);
        _ptrMemoryManager->registerParameter("PARCNC_K1", &_kParabolicCNCRateGain1);
        _ptrMemoryManager->registerParameter("PARCNC_K2", &_kParabolicCNCRateGain2);
    }

    doStuff(void);
    setup(void);

private:
    float _gotoXPos, _walkingMovePrecision;
    int   _zeroX, _walkSpeed, _jogSpeed, _moveXTime;
    unsigned long _startTime;
    bool _moving;
    float _kParabolicCNCRateGain1, _kParabolicCNCRateGain2;
};


#endif
