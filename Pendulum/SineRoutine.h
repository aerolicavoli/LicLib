/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    SineRoutine.h
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SineRoutine_h
#define SineRoutine_h

#include "Controller.h"

class SineRoutine : public Controller
{

public:
    SineRoutine(unsigned long UpdateInterval, PassiveDiagnostic *ptrDiagnostic, HardwareInterface *ptrHardware, SystemState *ptrState, MemoryManager *ptrMemoryManager) :
        Controller(UpdateInterval, ptrDiagnostic, ptrHardware, ptrState, ptrMemoryManager)
    {

        /*
        ** Start with initial gains
        */
        _period = 4000;
        _amp = 80;

        /*
        ** Register gains for cmd line tuning and eeprom saveing
        */
        _ptrMemoryManager->registerParameter("SIN_PERIOD", &_period);
        _ptrMemoryManager->registerParameter("SIN_AMP", &_amp);

    }

    doStuff(void);

    setup(void);

    /*
    ** Gains given as class variables for UI tuning
    */
    float _period, _amp;

};

#endif
