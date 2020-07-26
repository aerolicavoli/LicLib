/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    PassiveDiagnostic.h
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef PassiveDiagnostic_h
#define PassiveDiagnostic_h

#include "Metro.h"
#include "HardwareInterface.h"
#include "SystemState.h"
#include "PenEnums.h"
#include "MemoryManager.h"



class PassiveDiagnostic
{

public:
    PassiveDiagnostic(unsigned long updateInterval, HardwareInterface *hardWare, SystemState *state, MemoryManager *ptrMemoryManager);
    doStuff(unsigned long &time);

    /*
    ** Mutator Methods
    */
    setThetaLimit(float a, float b)
    {
        _thetaLimit[0] = a;
        _thetaLimit[1] = b;
    }
    setXLimit(float a, float b)
    {
        _xLimit[0] = a;
        _xLimit[1] = b;
    }

    setStallFailTime(unsigned long stallFailTime)
    {
        _stallFailTime = stallFailTime;
    }
    setWrongEncFailTime(unsigned long wrongEncFailTime)
    {
        _wrongEncFailTime = wrongEncFailTime;
    }

    EnableThetaLimit(void)
    {
        _thetaLimitEnabled = true;
    }
    EnableWrongPlugDummyDetection(void)
    {
        _wrongPlugDummyDetectionEnabled = true;
    }
    EnableCollisionDetection(void)
    {
        _collisionDetectionEnabled = true;
    }
    EnablePredictiveCollisionDetection(void)
    {
        _predictiveCollisionDetectionEnabled = true;
    }
    EnableStallDetection(void)
    {
        _stallDetectionEnabled = true;
    }
    EnableThetaDriftDetection(void)
    {
        _thetaDriftDetectionEnabled = true;
    }
    EnableXDriftDetection(void)
    {
        _xDriftDetectionEnabled = true;
    }
    EnableAll(void);

    DisableThetaLimit(void)
    {
        _thetaLimitEnabled = false;
    }
    DisableWrongPlugDummyDetection(void)
    {
        _wrongPlugDummyDetectionEnabled = false;
    }
    DisableCollisionDetection(void)
    {
        _collisionDetectionEnabled = false;
    }
    DisablePredictiveCollisionDetection(void)
    {
        _predictiveCollisionDetectionEnabled = false;
    }
    DisableStallDetection(void)
    {
        _stallDetectionEnabled = false;
    }
    DisableThetaDriftDetection(void)
    {
        _thetaDriftDetectionEnabled = false;
    }
    DisableXDriftDetection(void)
    {
        _xDriftDetectionEnabled = false;
    }
    DisableAll(void);

    float _thetaLimit[2], _xLimit[2], _thetaEntranceLimit[2];

private:
    Metro _timeKeeper;
    SystemState *_state;
    MemoryManager *_ptrMemoryManager;
    HardwareInterface *_hardWare;
    bool _thetaLimitEnabled, _wrongPlugDummyDetectionEnabled, _collisionDetectionEnabled;
    bool _predictiveCollisionDetectionEnabled, _stallDetectionEnabled;
    bool _xDriftDetectionEnabled, _thetaDriftDetectionEnabled;



    int _stallFailTime, _wrongEncFailTime, _stallTime;

    float _velCmdStallRatio, _vel_cmd_wrongEnc, _noStallCmdThresh;

};

#endif
