/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    MemoryManager.h
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef MemoryManager_h
#define MemoryManager_h

#include <EEPROM.h>
#include "utility.h"

#define DEBUG 1

template <class T> int EEPROM_writeAnything(int ee, const T &value)
{
    Serial.print("Saving Value: ");
    Serial.print(value, 5);
    Serial.print(" to EEPROM ADDRESS ");
    Serial.println(ee);
    const byte *p = (const byte *)(const void *)&value;
    int i;
    for (i = 0; i < sizeof(value); i++)
        EEPROM.update(ee++, *p++);
    return i;
}

template <class T> int EEPROM_readAnything(int ee, T &value)
{
    byte *p = (byte *)(void *)&value;
    int i;
    for (i = 0; i < sizeof(value); i++)
        *p++ = EEPROM.read(ee++);
    return i;
}


class MemoryManager
{

public:
    MemoryManager();
    registerParameter(char *name, float *location);
    registerParameter(char *name, int *location);

    //TODO
    //registerParameter(const __FlashStringHelper *name, float *location);
    //registerParameter(const __FlashStringHelper *name, int *location);

    int printParameter(char *name);

    printParameterFloat(int paramIdx);
    printParameterInt(int paramIdx);

    int changeParameter(char *name, float value);

    // int recallParameter(char *name);
    // int saveParameter(char *name);

    printAllParameters(void);

    reset(void);
    saveAll(void);
    recallAll(void);
    setup(void);

    enablePlasticMode(void);
    disablePlasticMode(void);
    enableRecallAtBootup(void);
    disableRecallAtBootup(void);

#if DEBUG == 1
    debugPrint()
    {
        Serial.println(_paramNamesFloat);
    }

#endif


private:
    float *_paramPtrsFloat[40];
    int *_paramPtrsInt[40];

    byte _parameterIdxFloat;
    byte _parameterIdxInt;

    char _paramNamesFloat[1200];
    char _paramNamesInt[1200];



    bool _plasticMemory, _recallAtStartup, _reservedSetting1, _reservedSetting2, _registrationPeriodOpen, _validData;

    byte _sizeOfFloat, _sizeOfInt;
    int  _AdrsMemSettings, _AdrsMemValidity, _AdrsFirstCrc, _AdrsNumFloatsExpected, _AdrsNumIntsExpected,
         _AdrsFloatParamNameCrc, _AdrsIntParamNameCrc, _AdrsFirstFloat, _AdrsFirstInt, _AdrsCRCLast, _AdrsCRCFirst, _NumCrcCoppies,
         _MaxFloats, _MaxInts;

    byte _NumFloats, _NumInts;

    const unsigned long _crc_table[16] =
    {
        0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
        0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
        0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
        0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
    };

    unsigned long eepromCrc(void);
    unsigned long nameCrc(char *name);

};


#endif
