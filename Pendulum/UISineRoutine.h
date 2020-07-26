/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    UISineRoutine.h
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef UISineRoutine_h
#define UISineRoutine_h

#include "Metro.h"
#include "SystemState.h"
#include "PassiveDiagnostic.h"
#include "MemoryManager.h"

class UISineRoutine
{

public:
    UISineRoutine(unsigned long updateInterval, SystemState *state, PassiveDiagnostic *Diagnostic);
    doStuff(void);
    setup(void);

private:
    Metro _timeKeeper;
    SystemState *_state;
    PassiveDiagnostic *_Diagnostic;
    MemoryManager *_ptrMemoryManager;

    double _xMapMin, _xMapMax, _periodMin, _periodMax;

    double _thetaMapMin, _thetaMapMax, _ampMin, _ampMax;
};


#endif
