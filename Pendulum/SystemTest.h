/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    SystemTest.h
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SystemTest_h
#define SystemTest_h

#include "MemoryManager.h"

class SystemTest
{

public:
    SystemTest( MemoryManager *ptrMemoryManager);
    void doStuff(void);

private:
    int _TestPin;
    MemoryManager *_ptrMemoryManager;
};

#endif
