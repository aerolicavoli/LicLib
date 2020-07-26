/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    Controller.h
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef Controller_h
#define Controller_h

#include "Metro.h"
#include "HardwareInterface.h"
#include "SystemState.h"
#include "PenEnums.h"
#include "PassiveDiagnostic.h"
#include "MemoryManager.h"


class Controller
{
public:
    Controller(unsigned long UpdateInterval, PassiveDiagnostic *ptrDiagnostic, HardwareInterface *ptrHardware, SystemState *ptrState, MemoryManager *ptrMemoryManager);

    PassiveDiagnostic *_ptrDiagnostic;
    Metro              _timeKeeper;
    MemoryManager     *_ptrMemoryManager;
    SystemState       *_ptrState;
    HardwareInterface *_ptrHardware;
};

#endif