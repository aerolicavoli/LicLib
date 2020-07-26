/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    SerialInterface.h
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SerialInterface_h
#define SerialInterface_h

#include "Metro.h"
#include "SystemState.h"
#include <string.h>

#include "utility.h"
#include "PenEnums.h"
#include "MemoryManager.h"

class SerialInterface
{

public:
    SerialInterface(unsigned long updateInterval, SystemState *state, int *majorMode, MemoryManager *ptrMemoryManager);
    setup(void);
    read(unsigned long &time);
    write(unsigned long &time);

    char _allCmds[115];

private:

    executeCmd(void);
    int _baudRate;
    Metro _timeKeeperRead, _timeKeeperWrite;
    SystemState *_state;
    MemoryManager *_ptrMemoryManager;

    int _Token;
    int BufferSize = 50;
    int _WordIndex;
    char *CommandBuffer = new char[BufferSize + 1];
    char *cmdStr1 = new char[BufferSize];
    char *cmdStr2 = new char[BufferSize];
    char *cmdStr3 = new char[BufferSize];
    char *cmdStr4 = new char[BufferSize];
    char *cmdStr5 = new char[BufferSize];
    int *_majorMode;
    float Arg1, Arg2, Arg3, Arg4;


};

#endif
