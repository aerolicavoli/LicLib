/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    SystemTest.cpp
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "SystemTest.h"

SystemTest::SystemTest(MemoryManager *ptrMemoryManager)
{
    _TestPin = -1;
    _ptrMemoryManager = ptrMemoryManager;
    _ptrMemoryManager->registerParameter("TestPin", &_TestPin);
	
}

void SystemTest::doStuff(void)
{
    switch(_TestPin)
    {
    case -1:
        Serial.print(F("Starting System Test\n"
                       "Unplug all items from data ports\n"
                       "Type \"TestPin #\"\n"));
        _TestPin = 0;
        break;
    case 0:
        //Do Nothing
        break;
    case A7:
    case A6:
    case 34:
    case A11:
    case A10:
    case A9:
    case A8:
    case 1:
    case A14:
    case A13:
    case A12:
    case 26:
    case 28:
    case 30:
    case 32:
    case 19:
    case 20:
    case 21:
    case 6:
    case 3:
    case 2:
    case 24:
    case 22:
        // Do Nothing
        break;
    default:
        Serial.print(F("Specified pin is not available\n"
                       "Available pins\n"
                       "Plug A (Left): A7, A6, 34\n"
                       "Plug B: A11, A10, A9, A8\n"
                       "Plug C: 1, A14, A13, A12\n"
                       "Plug D: 26, 28, 30, 32\n"
                       "Plug E: 3, 2, 24, 22"));
        _TestPin = 0;

    }

    if(0 < _TestPin)
    {
        pinMode(_TestPin, OUTPUT);
        digitalWrite(_TestPin, HIGH);
        delay(5000);
        Serial.println(F("Test Complete"));
        digitalWrite(_TestPin, LOW);
    }

}

