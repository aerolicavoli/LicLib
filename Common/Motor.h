/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    Motor.h
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef Motor_h
#define Motor_h

#include "Metro.h"
#include "HardwareInterface.h"
#include "SystemState.h"
#include "PenEnums.h"
#include "MemoryManager.h"


class Motor
{

public:
    Motor(unsigned long updateInterval, HardwareInterface *hardWare, SystemState *astate, MemoryManager *ptrMemoryManager);
    doStuff(unsigned long &time);

private:
    Metro _timeKeeper;
    MemoryManager *_ptrMemoryManager;
    int _lastCmd, _maxCmd, _minCmd;
    SystemState *_state;
    HardwareInterface *_hardWare;
    float _softStartDuration;
    unsigned long _timeSinceLastStop;
};

#endif
