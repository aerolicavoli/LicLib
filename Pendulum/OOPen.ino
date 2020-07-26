/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    OOPen.ino
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Metro.h"
#include "HardwareInterface.h"
#include "SystemState.h"
#include "PassiveDiagnostic.h"
#include "Motor.h"
#include "SerialInterface.h"
#include "SineRoutine.h"
#include "UISineRoutine.h"
#include "PenEnums.h"
#include "MemoryManager.h"
#include "SystemTest.h"
#include "CNC.h"
#include <Encoder.h>
#include "NestedPID.h"
#include "HomeRoutine.h"
#include "AnalogInterface.h"

Encoder  xEnc(2, 3);
Encoder  thetaEnc(20, 21);
/*
** Reset Cpu Overload Count
*/
int Metro::cpuOverloadCount = 0;

/*
** TRUE: Maintain task schedule, poor scheduling will cause 1202 alarm
** FALSE: Maintain constant dt, schedule will rearrange to fill dead time
*/
bool Metro::clockCatchUp = 0;

/*
** Routine Selection and Tracking
*/
int routineNumber = IDLE;
int previousRoutine = CNC_ROUTINE;

/*
** Declare Update Rates
*/
int stateUpdateTime  =  10;
int serialUpdateTime = 700;
int analogUIUpdateTime = 200;

bool HomeOnce = false;

/*
** New-up Pendulum Objects
*/
MemoryManager *ptrMemoryManager     = new MemoryManager();

//ptrMemoryManager->registerParameter("stateUpdateTime", &stateUpdateTime);
//ptrMemoryManager->registerParameter("serialUpdateTime", &serialUpdateTime);

HardwareInterface *ptrHardware      = new HardwareInterface(ptrMemoryManager);
SystemState *ptrPenState            = new SystemState(stateUpdateTime, ptrHardware, ptrMemoryManager);
SerialInterface *ptrSerialInterface = new SerialInterface(serialUpdateTime, ptrPenState, &routineNumber, ptrMemoryManager);
PassiveDiagnostic *ptrDiagnostic    = new PassiveDiagnostic(stateUpdateTime, ptrHardware, ptrPenState, ptrMemoryManager);
Motor *ptrActuator                  = new Motor(stateUpdateTime, ptrHardware, ptrPenState, ptrMemoryManager);

AnalogInterface *ptrAnalogInterface = new AnalogInterface(analogUIUpdateTime, ptrPenState, &routineNumber, ptrMemoryManager, ptrHardware);
SineRoutine *ptrSinRoutine          = new SineRoutine(stateUpdateTime, ptrDiagnostic, ptrHardware, ptrPenState, ptrMemoryManager);
UISineRoutine *ptrUISinRoutine      = new UISineRoutine(stateUpdateTime, ptrPenState, ptrDiagnostic);
SystemTest *ptrSystemTest           = new SystemTest(ptrMemoryManager);
CNC *ptrCNCOperator                 = new CNC(stateUpdateTime, ptrDiagnostic, ptrHardware, ptrPenState, ptrMemoryManager);
NestedPID *ptrNestedPID             = new NestedPID(stateUpdateTime, ptrDiagnostic, ptrHardware, ptrPenState, ptrMemoryManager);
HomeRoutine *ptrHomeRoutine         = new HomeRoutine(stateUpdateTime, ptrDiagnostic, ptrHardware, ptrPenState, ptrMemoryManager);

Metro encoderUpdateTime             =     Metro(stateUpdateTime);


/*
** Configure Pins, Start Serial Interface (Do not wait for serial, there are kids to teach!)
*/
void setup()
{
    digitalWrite(13, HIGH);
    delay(2000);
    digitalWrite(13, LOW);

    /*
    ** Order matters. ptrMemoryManager setup must be last.
    */
    ptrSerialInterface->setup();
    ptrHardware->setup();

    ptrAnalogInterface->setup();

    /*
    ** Parameter Registration Must Stop
    */
    ptrMemoryManager->setup();

    Serial.println(F("------ OOPEN OPERATIONAL, STARTING LOOP ------"));

}
/*
** Computer Cycle
*/
void loop()
{
    /*
    ** Loop time is updated once per loop if no
    ** subroutines execute. If a do stuff method
    ** executes, it will update the loop time after
    ** completion.
    */

    unsigned long loopTime = millis();

    /*
    ** Convert Encoder Data into position, velocity, and acceleration
    */
    if(0 < ptrPenState->_timeKeeper.check(loopTime))
    {
       ptrPenState->setEncX(xEnc.read());
       ptrPenState->setEncTheta(thetaEnc.read());
       ptrPenState->doStuff(loopTime);
       loopTime = millis();
    }


    /*
    ** Interpret Serial Commands
    */
    ptrSerialInterface->read(loopTime);

    /*
    ** Interpret Knobs and Dials
    */
    ptrAnalogInterface->read(loopTime);

    
    /*
    ** Home at startup
    */
    if(!(ptrPenState->getGoodX()) && (!HomeOnce))
    {
      routineNumber = HOME_ROUTINE;
      HomeOnce = true;
    } 



    /*
    ** Select Movement Routine
    */
    switch(routineNumber)
    {
    case IDLE:
        if(routineNumber != previousRoutine)
        {
            Serial.println("System In Idle");
            previousRoutine = routineNumber;
            ptrPenState->setCmd(0);
            ptrPenState->setMotorEnabled(false);
            ptrPenState->enableErrorReporting();
        }

        // Do Nothing
        break;

    case SINE_WAVE:

        if(routineNumber != previousRoutine)
        {
            Serial.println("Sine Wave Routine");
            ptrSinRoutine->setup();
            previousRoutine = routineNumber;
            ptrPenState->enableErrorReporting();
        }
        ptrSinRoutine->doStuff();
        break;

    case UI_SINE_WAVE:
        if(routineNumber != previousRoutine)
        {
            Serial.println("UI Sine Wave Routine");
            ptrUISinRoutine->setup();
            previousRoutine = routineNumber;
            ptrPenState->enableErrorReporting();
        }
        ptrUISinRoutine->doStuff();
        break;

    case SYSTEM_TEST:
        if(routineNumber != previousRoutine)
        {
            Serial.println("SYSTEM TEST");
            ptrPenState->disableErrorReporting();
            previousRoutine = routineNumber;
        }
        ptrSystemTest->doStuff();
        break;

    case CNC_ROUTINE:
        if(routineNumber != previousRoutine)
        {
            Serial.println("CNC Routine");
            previousRoutine = routineNumber;
            ptrPenState->enableErrorReporting();
            ptrCNCOperator->setup();
        }
        ptrCNCOperator->doStuff();
        break;

    case NESTEDPID_ROUTINE:
        if(routineNumber != previousRoutine)
        {
            Serial.println("Nested PID Routine");
            previousRoutine = routineNumber;
            ptrPenState->enableErrorReporting();
            ptrNestedPID->setup();
        }
        ptrNestedPID->doStuff(loopTime);
        break;
    case HOME_ROUTINE:
        if(routineNumber != previousRoutine)
        {
            Serial.println("Home Routine");
            previousRoutine = routineNumber;
            ptrPenState->enableErrorReporting();
            ptrHomeRoutine->setup();
        }
        ptrHomeRoutine->doStuff();

        /*
        ** Nested PID after homeing
        */
        if(ptrPenState->getGoodX())
        {
          routineNumber = NESTEDPID_ROUTINE;
        } 
        break;

    default:
        ptrPenState->reportError(CODE_ERROR, "Bad Routine Switch");
        break;
    }

    /*
    ** Observe States, Check for issues
    */
    ptrDiagnostic->doStuff(loopTime);

    /*
    ** Move Actuator
    */
    ptrActuator->doStuff(loopTime);

    /*
    ** Write To UIs
    */
    ptrSerialInterface->write(loopTime);
    ptrAnalogInterface->write(loopTime);
}
