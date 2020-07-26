/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    HomeRoutine.h
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef HomeRoutine_h
#define HomeRoutine_h

#include "Controller.h"

enum homingPhases { START_HOME = 0,
                    DETECT_COLLISION,
                    REVERSE,
                    FINE_ALIGN,
                    HOMEING_COMPLETE,
                    ABORT_HOME_ROUTINE
                  };

class HomeRoutine : public Controller
{

public:
    HomeRoutine(unsigned long UpdateInterval, PassiveDiagnostic *ptrDiagnostic, HardwareInterface *ptrHardware, SystemState *ptrState, MemoryManager *ptrMemoryManager) :
        Controller(UpdateInterval, ptrDiagnostic, ptrHardware, ptrState, ptrMemoryManager)
    {

        _phaseNumber = 0;
        _moveTime = 0;

        _coarseAlignTimeout = 30000; //ms
        _coarseAlignSpeed = 22; //1/255 max power
        _reverseTime = 500; //ms
        _fineAlignSpeed = 17;
        _postHomePosition =  55.0; //cm
        _FineAlignTimeout = 12000; //ms
        _midpointMoveSpeed = 50;


    }

    doStuff(void);

    setup(void);

    /*
    ** Gains given as class variables for UI tuning
    */
    int _phaseNumber;
    unsigned long _moveTime, _coarseAlignTimeout, _reverseTime, _FineAlignTimeout;
    int _coarseAlignSpeed, _fineAlignSpeed, _midpointMoveSpeed;
    float _postHomePosition;

};

#endif
