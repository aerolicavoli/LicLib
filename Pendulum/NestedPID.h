/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    NestedPID.h
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef NestedPID_h
#define NestedPID_h

#include "Controller.h"
#include "Integrator.h"
#include "DigitalFilter.h"
#include "PenEnums.h"

class NestedPID : public Controller
{

public:
    NestedPID(unsigned long UpdateInterval, PassiveDiagnostic *ptrDiagnostic, HardwareInterface *ptrHardware, SystemState *ptrState, MemoryManager *ptrMemoryManager) :
        Controller(UpdateInterval, ptrDiagnostic, ptrHardware, ptrState, ptrMemoryManager)
    {

        /*
        ** Start with initial gains
        */
        _kp_outer = -0.033; //Must Be Negative
        _ki_outer = -0.0035; // Must Be Negative
        _kd_outer = 0.055; // Must Be Positive 0.053
        
        _kp_inner = 60.0; // Must Be Positive
        _kd_inner = 0.0; //  

        _XRef     = 55.0;
        
        _integratorLimit = 10.0;

        /*
        ** Register gains for cmd line tuning and eeprom saveing
        */
        _ptrMemoryManager->registerParameter("KPX", &_kp_outer);
        _ptrMemoryManager->registerParameter("KIX", &_ki_outer);
        _ptrMemoryManager->registerParameter("KDX", &_kd_outer);
        _ptrMemoryManager->registerParameter("KPT", &_kp_inner);
        _ptrMemoryManager->registerParameter("KDT", &_kd_inner);
        _ptrMemoryManager->registerParameter("INTLIM", &_integratorLimit);
        _ptrMemoryManager->registerParameter("XREF", &_XRef);
        
        _ptrXInt = new Integrator();

        /*
        ** Setup outerloop cmd moving average filter
        */
        _ptrOuterCmdFilt = new DigitalFilter(14);
        _ptrOuterCmdFilt->_taps[0] = 0.071428;
        _ptrOuterCmdFilt->_taps[1] = 0.071428;
        _ptrOuterCmdFilt->_taps[2] = 0.071428;
        _ptrOuterCmdFilt->_taps[3] = 0.071428;
        _ptrOuterCmdFilt->_taps[4] = 0.071428;
        _ptrOuterCmdFilt->_taps[5] = 0.071428;
        _ptrOuterCmdFilt->_taps[6] = 0.071428;
        _ptrOuterCmdFilt->_taps[7] = 0.071428;
        _ptrOuterCmdFilt->_taps[8] = 0.071428;
        _ptrOuterCmdFilt->_taps[9] = 0.071428;
        _ptrOuterCmdFilt->_taps[10] = 0.071428;
        _ptrOuterCmdFilt->_taps[11] = 0.071428;
        _ptrOuterCmdFilt->_taps[12] = 0.071428;
        _ptrOuterCmdFilt->_taps[13] = 0.071428;

    }

    doStuff(unsigned long &time);

    setup(void);

    /*
    ** Gains given as class variables for UI tuning
    */
    float _kp_outer, _ki_outer, _kp_inner, _kd_inner, _integratorLimit, _XRef, _kd_outer;
    Integrator *_ptrXInt;
    DigitalFilter *_ptrOuterCmdFilt;

};

#endif
