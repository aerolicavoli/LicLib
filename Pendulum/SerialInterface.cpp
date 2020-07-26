/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    SerialInterface.cpp
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "SerialInterface.h"


SerialInterface::SerialInterface(unsigned long updateInterval, SystemState *state, int *majorMode, MemoryManager *ptrMemoryManager)
{
    _timeKeeperRead = Metro(updateInterval);
    _timeKeeperWrite = Metro(updateInterval);
    _baudRate = 19200;
    _state = state;
    _Token = 32; //44;
    _majorMode = majorMode;
    _ptrMemoryManager = ptrMemoryManager;

    char _allCmds[115] = "STOP,START,XZ,TZ,RST,"
                         "IDLE,HOME,SIN,CNC,UISIN,EERST,"
                         "PRT,SAVE,RCL,ENPLASTIC,"
                         "DSPLASTIC,ENRECALL,"
                         "DISRECALL,TST,w,s,PID";
}

SerialInterface::setup()
{
    Serial.begin(_baudRate);

    while (!Serial)
    {
        // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println(F("------ OOPEN SYSTEM IN STARTUP ------"));
    Serial.println(F("SERIAL INTERFACE: Setup Starting"));
    Serial.println(F("SERIAL INTERFACE: Setup Complete"));

}

SerialInterface::write(unsigned long &time)
{
    if(0 < _timeKeeperWrite.check(time))
    {
        /* 		byte intialize = 36; //$

         		byte* byteX = (byte)(_state->getX());
         		byte* byteXdot = (byte)(_state->getXdot());
         		byte* byteTheta = (byte)(_state->getTheta());
         		byte* byteThetadot = (byte)(_state->getThetadot());
         		byte* byteCmd = (byte)(_state->getCmd());

         		byte buf[16] = {byteX[0], byteX[1], byteX[2], byteX[3],
        		            byteXdot[0], byteXdot[1], byteXdot[2], byteXdot[3],
        		            byteTheta[0], byteTheta[1], byteTheta[2], byteTheta[3],
        		            byteThetadot[0], byteThetadot[1], byteThetadot[2], byteThetadot[3]};
        		            //byteCmd[0], byteCmd[1]}

        	Serial.write(buf,16);
        */
        Serial.print(_state->getX(), 4);
        Serial.print(",");
        Serial.print(_state->getTheta(), 4);
        Serial.print(",");
        Serial.print(digitalRead(22));
        Serial.print(",");
        // 		Serial.print(_state->getThetadot(), 4);
        //  		Serial.print(",");
        Serial.print(_state->getCmd());
        Serial.print(",");
        Serial.print(_state->_printThis);
        Serial.print(",");
        Serial.print(_state->getMotorEnabled());
        Serial.print(",");
        Serial.println(_state->getErrorString());
        time = millis();
    }

}

SerialInterface::read(unsigned long &time)
{
    if(0 < _timeKeeperRead.check(time) && (Serial.available() > 0))
    {
        /*
        ** Clear Buffers
        */
        memset(CommandBuffer, 0, BufferSize + 1);
        memset(cmdStr1, 0, BufferSize);
        memset(cmdStr2, 0, BufferSize);
        memset(cmdStr3, 0, BufferSize);
        memset(cmdStr4, 0, BufferSize);
        memset(cmdStr5, 0, BufferSize);

        Arg1 = 0.0;
        Arg2 = 0.0;
        Arg3 = 0.0;
        Arg4 = 0.0;

        /*
        ** Reset Indexes
        */
        int wordsParsed;
        _WordIndex = 0;
        int BufferIndex = 0;

        while (Serial.available() > 0)
        {
            // read the incoming byte:
            byte incomingByte = Serial.read();

            if(_Token == incomingByte)
            {
                _WordIndex++;
            }
            CommandBuffer[BufferIndex] = incomingByte;

            if(BufferSize <= BufferIndex)
            {
                _state->reportError(UI_ERROR, "Command Too Long");
            }
            else
            {
                BufferIndex++;
            }
        }

        /*
        ** If a command was read in, parse it
        */
        if(0 < BufferIndex)
        {
            //CommandBuffer[BufferIndex] = 32;
            switch(_WordIndex)
            {
            case 0:
                wordsParsed = sscanf(CommandBuffer, "%s", cmdStr1);
                break;

            case 1:
                wordsParsed = sscanf(CommandBuffer, "%s %s", cmdStr1, cmdStr2);
                Arg1 = atof(cmdStr2);
                break;

            case 2:
                wordsParsed = sscanf(CommandBuffer, "%s %s %s", cmdStr1, cmdStr2, cmdStr3);
                Arg1 = atof(cmdStr2);
                Arg2 = atof(cmdStr3);
                break;

            case 3:
                wordsParsed = sscanf(CommandBuffer, "%s %s %s %s", cmdStr1, cmdStr2, cmdStr3, cmdStr4);
                Arg1 = atof(cmdStr2);
                Arg2 = atof(cmdStr3);
                Arg3 = atof(cmdStr4);

            case 4:
                wordsParsed = sscanf(CommandBuffer, "%s %s %s %s %s", cmdStr1, cmdStr2, cmdStr3, cmdStr4, cmdStr5);
                Arg1 = atof(cmdStr2);
                Arg2 = atof(cmdStr3);
                Arg3 = atof(cmdStr4);
                Arg4 = atof(cmdStr5);
                break;

            default:
                _state->reportError(UI_ERROR, "Too many words");
            }

            if(wordsParsed != (_WordIndex + 1))
            {
                _state->reportError(UI_ERROR, "Parseing Error");
            }

            Serial.print("Command: ");
            Serial.print(cmdStr1);
            Serial.print(" Argument: ");
            Serial.print(Arg1,8);
            Serial.print(", ");
            Serial.print(Arg2,8);
            Serial.print(", ");
            Serial.print(Arg3,8);
            Serial.print(", ");
            Serial.println(Arg4,8);

            executeCmd();

            /*
            ** Update loop time
            */
            time = millis();
        }
    }

}

SerialInterface::executeCmd()
{
    int cmdNum;
    cmdNum = parseKeyword(cmdStr1,
                          ("STOP,START,XZ,TZ,RST,"
                           "IDLE,HOME,SIN,CNC,UISIN,EERST,"
                           "PRT,SAVE,RCL,ENPLASTIC,"
                           "DSPLASTIC,ENRECALL,"
                           "DISRECALL,TST,w,s,PID" ));



    switch(cmdNum)
    {
    case cmdSTOP:
        _state->setMotorEnabled(false);
        _state->setCmd(0);
        break;
    case cmdSTART:
        _state->setMotorEnabled(true);
     //   *_majorMode = CNC_ROUTINE;
        break;
    case cmdZEROX:
        _state->zeroX( Arg1 );
        _ptrMemoryManager->changeParameter("MOVE", 0.0);
        break;
    case cmdZEROTHETA:
        _state->zeroTheta( Arg1 );
        break;

    case cmdRESET:
        _state->reportError(NO_ERROR, "NO ERROR");
        break;

    case cmdIDLE:
        *_majorMode = IDLE;
        _state->setCmd(0);
        break;
    case cmdHome:
        *_majorMode = HOME_ROUTINE;
		_state->setCmd(0);
		break;
    case cmdSINEWAVE:
        *_majorMode = SINE_WAVE;
        break;
    case cmdCNC:
        *_majorMode = CNC_ROUTINE;
        break;

    case cmdUISINEWAVE:
        *_majorMode = UI_SINE_WAVE;
        break;

    case cmdEEPROMRESET:
        _ptrMemoryManager->reset();
        break;

    case cmdEEPROMPRINTALL:
        _ptrMemoryManager->printAllParameters();
        break;

    case cmdEEPROMSAVEALL:
        _ptrMemoryManager->saveAll();
        break;

    case cmdEEPROMRECALLALL:
        _ptrMemoryManager->recallAll();
        break;

    case cmdEnablePlasticMode:
        _ptrMemoryManager->enablePlasticMode();
        break;

    case cmdDisablePlasticMode:
        _ptrMemoryManager->disablePlasticMode();
        break;

    case cmdEnableRecallAtBootup:
        _ptrMemoryManager->enableRecallAtBootup();
        break;

    case cmdDisableRecallAtBootup:
        _ptrMemoryManager->disableRecallAtBootup();
        break;

    case cmdSYSTEMTEST:
        *_majorMode = SYSTEM_TEST;
        break;

    case cmdUP:
        *_majorMode = CNC_ROUTINE;
        _ptrMemoryManager->changeParameter("MOVE_TIME", 1);
        break;
    case cmdDOWN:
        *_majorMode = CNC_ROUTINE;
        _ptrMemoryManager->changeParameter("MOVE_TIME", -1);
        break;
    case cmdNESTED_PID:
        *_majorMode = NESTEDPID_ROUTINE;
        break;
    default:

        /*
        ** Check for registered variables
        */
        int paramIdx;
        if(_WordIndex >= 1)
        {
            paramIdx = _ptrMemoryManager->changeParameter(cmdStr1, Arg1);
        }
        else
        {
            paramIdx = _ptrMemoryManager->printParameter(cmdStr1);

        }

        /*
        ** Unkown Cmd
        */
        if (paramIdx < 0)
        {
            Serial.println(F(
                               "OOPEN Version 0.1\n"
                               "Command Options are:\n"
                               "STOP         -   Stop motor\n"
                               "SART         -   Start motor\n"
                               "XZ           -   Zero X\n"
                               "TZ           -   Zero Theta\n"
                               "RST          -   Clear Error Codes\n"
                               "IDLE         -   Do nothing\n"
                               "HOME         -   Change Routine: Home device\n"
                               "SIN          -   Change Routine: Open Loop Sin Wave\n"
                               "CNC          -   Change Routine: CNC Mode\n"
                               "UISIN        -   Change Routine: Open Loop Sin Wave With Encoder UI\n"
                               "EERST        -   Reformat EEPROM Memory, will lose all presets\n"
                               "PRT          -   Print all registered parameters and values\n"
                               "SAVE         -   Store all parameters to EEPROM\n"
                               "RCL          -   Read in all parameter values from EEPROM\n"
                               "ENPLASTIC    - EEPROM save parameters as they are changed\n"
                               "DSPLASTIC    - ^\n"
                               "ENRECALL     - Read in eeprom values at bootup\n"
                               "DSRECALL     - ^\n"
                               "TST          -   Light Up Specific Pin Numbers\n"
                               "w            -   +X one step\n"
                               "s            -   -X one step\n"
                               "PID          -   Change Routine: Nested PID\n"));
        }
        break;
    }

}