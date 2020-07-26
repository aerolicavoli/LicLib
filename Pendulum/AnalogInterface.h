/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    AnalogInterface.h
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef AnalogInterface_h
#define AnalogInterface_h

#include "Metro.h"
#include "SystemState.h"
#include <string.h>

#include "utility.h"
#include "PenEnums.h"
#include "MemoryManager.h"
#include "HardwareInterface.h"
#include <FastLED.h>

class AnalogInterface
{

public:
    AnalogInterface(unsigned long updateInterval, SystemState *state, int *majorMode, MemoryManager *ptrMemoryManager, HardwareInterface *ptrHardware);
    setup(void);
    read(unsigned long &time);
    write(unsigned long &time);

    
private:

    Metro _timeKeeperRead, _timeKeeperWrite;
    SystemState *_state;
    MemoryManager *_ptrMemoryManager;

    int *_majorMode;
    HardwareInterface *_ptrHardware;
    int _switchState;
    int _oldrightPot;
    CRGB _leds[2];

};

#endif
