/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    SystemState.h
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SystemState_h
#define SystemState_h

#include "PenEnums.h"
#include "HardwareInterface.h"
#include "Metro.h"
#include "String.h"
#include "utility.h"
#include "MemoryManager.h"
#include "DigitalFilter.h"

//class string
class SystemState
{

public:

    SystemState( unsigned long updateInterval, HardwareInterface *hardware, MemoryManager *ptrMemoryManager);

    doStuff(unsigned long &time);

    /*
    ** Mutator Methods
    */
    void setX(float newX);
    void setTheta(float newTheta);
    void setCmd(int newCmd)
    {
        cmd = newCmd;
    }
    void reportXCollision(void)
    {
        goodX = false;
    }
    void disableErrorReporting(void)
    {
        _errorReportingEnabled = false;
        ErrorState = NO_ERROR;
        ErrorString = "";
    }
    void enableErrorReporting(void)
    {
        _errorReportingEnabled = true;
    }
    void reportError(int errorType, String errorString)
    {
      //TODO fix order of things
        //if(_errorReportingEnabled && ((NEUTRAL_ERROR > ErrorState > errorType) && (NEUTRAL_ERROR > errorType)))
        //{
            ErrorState = errorType;
            ErrorString = errorString;
        //}
    }
    void zeroX(void)
    {
      zeroX(0.0);
    }

    void zeroTheta(void)
    {
      zeroTheta(0.0);
    }
    void zeroX( float userOffset )
    {
        XZeroingOffset = X + XZeroingOffset + userOffset;
    }

    void zeroTheta( float userOffset )
    {
        ThetaZeroingOffset = Theta + ThetaZeroingOffset + userOffset;
    }

    void setEncX(int32_t newEncX)
    {
        encX = newEncX;
    }
    void setEncTheta(int32_t newEncTheta)
    {
        encTheta = newEncTheta;
    }

    void setMotorEnabled(bool newMotorEnabled)
    {
        motorEnabled = newMotorEnabled;
    }

    void setSoftStart(bool newSoftStart)
    {
        softStart = newSoftStart;
    }
    

    void setGoodX(void)
    {
      goodX = true;
    }

    /*
    ** Accessor Methods
    */
    bool getGoodX(void)
    {
      return(goodX);
    }

    bool getMotorEnabled() const
    {
        return(motorEnabled);
    }
    bool getSoftStart() const
    {
        return(softStart);
    }
    
    float getTimeNow() const
    {
        return(TimeNow);
    }

    float getX() const
    {
        return(X);
    }
    float getXdot() const
    {
        return(Xdot);
    }
    float getXdotFiltered() const
    {
        return(XdotFiltered);
    }
    float getXdotdot() const
    {
        return(Xdotdot);
    }
    float getXFuture() const
    {
        return(Xfuture);
    }
    bool  getXValidity() const
    {
        return(goodX);
    }

    float getTheta() const
    {
        return(Theta);
    }
    float getThetaLimited() const
    {
        return(ThetaLimited);
    }
    float getThetadot() const
    {
        return(Thetadot);
    }
    float getThetadotdot() const
    {
        return(Thetadotdot);
    }
    float getThetaFuture() const
    {
        return(Thetafuture);
    }
    bool  getThetaValidity() const
    {
        return(goodTheta);
    }

    int    getCmd() const
    {
        return(cmd);
    }
    int    getError() const
    {
        return(ErrorState);
    }
    String getErrorString() const
    {
        return(ErrorString);
    }

    float X, Xdot, Xdotdot;
    float Theta, Thetadot, Thetadotdot, ThetaLimited;
    int cmd;

    float _filtXdot;
    float _filtCmd; 

    Metro _timeKeeper;
    float _printThis;

private:

    bool motorEnabled, _errorReportingEnabled, softStart;

    int ErrorState;
    bool goodX, goodTheta;
    unsigned long TimeNow, TimePast, TimeFuture;

    float XZeroingOffset, ThetaZeroingOffset;

    float Xpast, Xdotpast;
    
    float XdotFiltered;
    float Xfuture;
    float _pulse2CM, _pulse2Deg;
    float Thetapast, Thetadotpast;
    
    float Thetafuture;

    

    int32_t encTheta, encX;
    String ErrorString;
    
    HardwareInterface *_hardWare;
    MemoryManager *_ptrMemoryManager;
    DigitalFilter *_dxFilt;
    DigitalFilter *_cmdFilt;
};

#endif
