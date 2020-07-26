/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    MemoryManager.cpp
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
*
* Memory Configuration:
*
*      | EEPROM  |
* Type | ADDRESS | Usage
* -----|---------|-------
*   ~  |0-4      | Reserved
* byte |5        | Memory Settings Values
* byte |6        | Memory Settings Validity
* long |7,8,9,10 | Mem CRC Copy 1
* long |11,...,14| Mem CRC Copy 2
* long |15,...,18| Mem CRC Copy 3
* byte |19       | Number Of Floats
* byte |20       | Number Of Ints
* long |21,...,24| Float Param Name CRC
* long |25,...,28| Int Param Name CRC
*   ~  |29-99    | Reserved
* Float|100-499  | 100 Floats
* Int  |500-699  | 100 Ints
*   ~  |700-4095 | Reserved
*
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "MemoryManager.h"


MemoryManager::MemoryManager()
{
    /*
    ** Initialize Dynamic Parameter Registry
    */
    _parameterIdxFloat = 0;
    _parameterIdxInt = 0;

    for(int k = 0; k < sizeof(_parameterIdxFloat); k++)
    {
        _paramPtrsFloat[k] = NULL;
    }
    for(int k = 0; k < sizeof(_parameterIdxInt); k++)
    {
        _paramPtrsInt[k] = NULL;
    }
    for(int k = 0; k < sizeof(_paramNamesFloat); k++)
    {
        _paramNamesFloat[k] = NULL;
    }
    for(int k = 0; k < sizeof(_paramNamesInt); k++)
    {
        _paramNamesInt[k] = NULL;
    }

    /*
    ** Once true, parameter registration must stop
    */
    _registrationPeriodOpen = true;

    _validData = true;

    /*
    ** Memory Configuration Known At Startup, const
    */
    _AdrsMemSettings        = 5;
    _AdrsMemValidity        = 6;
    _AdrsFirstCrc           = 7;
    _AdrsNumFloatsExpected  = 19;
    _AdrsNumIntsExpected    = 20;
    _AdrsFloatParamNameCrc  = 21;
    _AdrsIntParamNameCrc    = 25;
    _AdrsFirstFloat         = 100; //100 + 4*100 -1 = 499
    _AdrsFirstInt           = 500; //500 + 2*100 -1 = 699

    _AdrsCRCFirst           = 19; //Do not include EEPROM CRC in CRC calc
    _AdrsCRCLast            = 699;

    _NumCrcCoppies          = 3;
    _MaxFloats              = 100;
    _MaxInts                = 100;

    _sizeOfFloat            = 4;
    _sizeOfInt              = 2;

    /*
    ** Memory Configuration Known At Startup, dynamic
    */
    _plasticMemory = 0;
    _recallAtStartup = 0;

    /*
    ** Memory Configuration Known After Setup
    */
    _NumFloats = 0;
    _NumInts = 0;

}

MemoryManager::setup()
{
    Serial.println("MEMORY MANAGER: Setup Starting");

    /*
    ** Start with _validData true assumption
    */
    _validData = true;

    /*
    ** Prevent further parameter registraton
    */
    _registrationPeriodOpen = false;

    /*
    ** Check if parameter name list trunicated by
    ** Store parameter number matches memory.
    ** If it doesn't, invalidate all memory, rewrite data.
    ** If it does, write new data
    **
    */
    unsigned long startupEepromCrc = eepromCrc();
    unsigned long eepromCrcReadBack1, eepromCrcReadBack2, eepromCrcReadBack3;
    EEPROM_readAnything(_AdrsFirstCrc, eepromCrcReadBack1);
    EEPROM_readAnything(_AdrsFirstCrc + _sizeOfFloat, eepromCrcReadBack2);
    EEPROM_readAnything(_AdrsFirstCrc + _sizeOfFloat * 2, eepromCrcReadBack3);

    if((eepromCrcReadBack1 != eepromCrcReadBack2) || (eepromCrcReadBack2 != eepromCrcReadBack3) || (eepromCrcReadBack3 != startupEepromCrc ))
    {
        _validData = false;
        Serial.println(F("MEMORY MANAGER: Invalid EEPROM Data due to failed ciclical redundancy check."));
        Serial.print(F("EEPROM Stored CRC 1: "));
        Serial.println(eepromCrcReadBack1);
        Serial.print(F("EEPROM Stored CRC 2: "));
        Serial.println(eepromCrcReadBack2);
        Serial.print(F("EEPROM Stored CRC 3: "));
        Serial.println(eepromCrcReadBack3);
        Serial.print(F("Startup Calced CRC:  "));
        Serial.println(startupEepromCrc);
    }

    /*
    ** Check for parameter number changes
    */
    _NumFloats = EEPROM.read(_AdrsNumFloatsExpected);
    _NumInts = EEPROM.read(_AdrsNumIntsExpected);
    if((_NumFloats != _parameterIdxFloat) || (_parameterIdxInt != _NumInts))
    {
        _validData = false;
        Serial.println(F("MEMORY MANAGER: Invalid EEPROM Data due incorrect number of stored parameters."));
        Serial.print(F("Number of Stored Ints: "));
        Serial.println(_NumInts);
        Serial.print(F("Number of registered Ints: "));
        Serial.println(_parameterIdxInt);
        Serial.print(F("Number of Stored Floats: "));
        Serial.println(_NumFloats);
        Serial.print(F("Number of registered Floats: "));
        Serial.println(_parameterIdxFloat);
    }

    /*
    ** Check for parameter name changes
    */
    unsigned long floatNamesCRC = nameCrc(_paramNamesFloat);
    unsigned long intNamesCRC = nameCrc(_paramNamesInt);
    unsigned long expectedFloatNameCRC;
    unsigned long expectedIntNameCRC;
    EEPROM_readAnything(_AdrsFloatParamNameCrc, expectedFloatNameCRC);
    EEPROM_readAnything(_AdrsIntParamNameCrc, expectedIntNameCRC);

    if((expectedFloatNameCRC != floatNamesCRC))
    {
        _validData = false;
        Serial.println(F("MEMORY MANAGER: Invalid EEPROM Data due to ciclical redundancy check difference."));
        Serial.print(F("Float Name Registry CRC: "));
        Serial.println(floatNamesCRC);
        Serial.print(F("Expected Value: "));
        Serial.println(expectedFloatNameCRC);
    }
    if((expectedIntNameCRC != intNamesCRC))
    {
        _validData = false;
        Serial.println(F("MEMORY MANAGER: Invalid EEPROM Data due to ciclical redundancy check difference."));
        Serial.print(F("Int Name Registry CRC: "));
        Serial.println(intNamesCRC);
        Serial.print(F("Expected Value: "));
        Serial.println(expectedIntNameCRC);
    }


    /*
    ** Recall and Process Memory Settings
    **
    ** Settings Byte
    ** Bit  | Useage
    ** 0    | Plastic Memory
    ** 1    | Recall at Startup
    ** 2    | Reserved
    ** 3    | Reserved
    ** 4    | Not Bit 0
    ** 5    | Not Bit 1
    ** 6    | Not Bit 2
    ** 7    | Not Bit 3
    */

    byte settingsByte  = EEPROM.read(_AdrsMemSettings);

    /*
    ** Validate Settings
    */
    if((bitRead(settingsByte, 0) != bitRead(settingsByte, 0 + 4)) &&
            (bitRead(settingsByte, 1) != bitRead(settingsByte, 1 + 4)) &&
            (bitRead(settingsByte, 2) != bitRead(settingsByte, 2 + 4)) &&
            (bitRead(settingsByte, 3) != bitRead(settingsByte, 3 + 4)))
    {
        _plasticMemory    = bitRead(settingsByte, 0);
        _recallAtStartup  = bitRead(settingsByte, 1);
        _reservedSetting1 = bitRead(settingsByte, 2);
        _reservedSetting2 = bitRead(settingsByte, 3);
    }
    else
    {
        Serial.println(F("MEMORY MANAGER: Invalid Settings, Useing Default."));
        for (byte i = 0; i < 8; ++i)
        {
            Serial.print(bitRead(settingsByte, i) );
        }
        Serial.println(" ");
    }

    if(_plasticMemory) Serial.println(F("MEMORY MANAGER: Plastic Memory Mode Enabled"));
    if(_recallAtStartup) Serial.println(F("MEMORY MANAGER: Recall At Startup Enabled"));

    if(_validData)
    {
        Serial.println(F("MEMORY MANAGER: Valid EEPROM Data, EEPROM Functionality Enabled"));
        if(_recallAtStartup)
        {
            recallAll();
        }
        else
        {
            printAllParameters();
        }

    }
    else
    {
        Serial.println(F("MEMORY MANAGER: Invalid EEPROM Data. EEPROM Functionality Dissabled. EEPROM Reset Required"));
        //reset(); //CARFUL!!! This could cause infinite recursion if reset fails.  Need a way to prevent it.
        //Manually reset only for now serial interface "EEPROMRESET"
    }

    Serial.println(F("MEMORY MANAGER: Setup Complete"));
}

MemoryManager::enablePlasticMode()
{
    _plasticMemory = true;
    byte settingsByte  = EEPROM.read(_AdrsMemSettings);
    bitSet(settingsByte, 0);
    bitClear(settingsByte, 4);
    EEPROM.update(_AdrsMemSettings, settingsByte);
}

MemoryManager::disablePlasticMode()
{
    _plasticMemory = false;
    byte settingsByte  = EEPROM.read(_AdrsMemSettings);
    bitClear(settingsByte, 0);
    bitSet(settingsByte, 4);
    EEPROM.update(_AdrsMemSettings, settingsByte);
}

MemoryManager::enableRecallAtBootup()
{
    _recallAtStartup = true;
    byte settingsByte  = EEPROM.read(_AdrsMemSettings);
    bitSet(settingsByte, 1);
    bitClear(settingsByte, 5);
    EEPROM.update(_AdrsMemSettings, settingsByte);
}

MemoryManager::disableRecallAtBootup()
{
    _recallAtStartup = false;
    byte settingsByte  = EEPROM.read(_AdrsMemSettings);
    bitClear(settingsByte, 1);
    bitSet(settingsByte, 5);
    EEPROM.update(_AdrsMemSettings, settingsByte);
}

MemoryManager::printAllParameters()
{
    /*
    ** Print All Float Values
    */
    char *ptrFloatName = &_paramNamesFloat[0];
    Serial.println(F("Registered Paramete rs: "));
    for (int k = 0; k < _parameterIdxFloat; k++)
    {

        do
        {
            Serial.print(*ptrFloatName);
            ptrFloatName++;
        }
        while(*ptrFloatName > ',');
        ptrFloatName++;
        Serial.print(" = ");
        Serial.println(*_paramPtrsFloat[k], 8);
    }

    /*
    ** Print All Int Values
    */
    char *ptrIntName = &_paramNamesInt[0];
    for (int k = 0; k < _parameterIdxInt; k++)
    {
        do
        {
            Serial.print(*ptrIntName);
            ptrIntName++;
        }
        while(*ptrIntName > ',');
        ptrIntName++;
        Serial.print(" = ");
        Serial.println(*_paramPtrsInt[k]);
    }

}

MemoryManager::reset()
{
    /*
    ** Clear EEPROM
    */
    Serial.println(F("MEMORY MANAGER: EEPROM Reset Starting"));
    Serial.println(F("MEMORY MANAGER: EEPROM Clearing All Addresses"));
    for (int i = 0 ; i < _AdrsCRCLast ; i++)
    {
        EEPROM.update(i, 0);
    }

    /*
    ** Write Valid Default Settings
    */
    Serial.println(F("MEMORY MANAGER: EEPROM Writing Default Settings"));
    EEPROM.update(_AdrsMemSettings, 240); // 1 1 1 1 0 0 0 0

    /*
    ** Write Registered Parameters to memory
    */
    Serial.println(F("MEMORY MANAGER: EEPROM Saving all registered parameters"));
    saveAll();

    /*
    ** Reperform data validity checks and settings parseing.
    */
    Serial.println(F("MEMORY MANAGER: EEPROM Revalidating Data via Setup"));
    setup();

    Serial.println(F("MEMORY MANAGER: EEPROM Reset Complete"));
}

MemoryManager::saveAll()
{
    /*
    ** Float Values
    */
    for (int k = 0; k < _parameterIdxFloat; k++)
    {
        EEPROM_writeAnything(k * _sizeOfFloat + _AdrsFirstFloat, *_paramPtrsFloat[k]);
    }

    /*
    ** Float Values
    */
    for (int k = 0; k < _parameterIdxInt; k++)
    {
        EEPROM_writeAnything(k * _sizeOfInt + _AdrsFirstInt, *_paramPtrsInt[k]);
    }

    /*
    ** Count Verification
    */
    _NumFloats = _parameterIdxFloat;
    _NumInts   = _parameterIdxInt;

    EEPROM_writeAnything(_AdrsNumFloatsExpected, _NumFloats);
    EEPROM_writeAnything(_AdrsNumIntsExpected, _NumInts);

    /*
    ** Name CRC Verification
    */
    unsigned long floatNamesCRC = nameCrc(_paramNamesFloat);
    unsigned long intNamesCRC = nameCrc(_paramNamesInt);

    EEPROM_writeAnything(_AdrsFloatParamNameCrc, floatNamesCRC);
    EEPROM_writeAnything(_AdrsIntParamNameCrc, intNamesCRC);

    /*
    ** Memory CRC
    */
    unsigned long newEepromCrc = eepromCrc();

    EEPROM_writeAnything(_AdrsFirstCrc, newEepromCrc);
    EEPROM_writeAnything(_AdrsFirstCrc + _sizeOfFloat, newEepromCrc);
    EEPROM_writeAnything(_AdrsFirstCrc + _sizeOfFloat * 2, newEepromCrc);
}

MemoryManager::recallAll(void)
{
    Serial.println(F("MEMORY MANAGER: Recalling All Stored Parameters"));
    if(_validData)
    {
        for (int k = 0; k < _parameterIdxFloat; k++)
        {
            EEPROM_readAnything(k * _sizeOfFloat + _AdrsFirstFloat, *_paramPtrsFloat[k]);
        }

        /*
        ** Float Values
        */
        for (int k = 0; k < _parameterIdxInt; k++)
        {
            EEPROM_readAnything(k * _sizeOfInt + _AdrsFirstInt, *_paramPtrsInt[k]);
        }

        printAllParameters();
    }
}

// TODO alow for parameter names to be stored in flash memeroy
//MemoryManager::registerParameter(const __FlashStringHelper *name, float *location);
//MemoryManager::registerParameter(const __FlashStringHelper *name, int *location);

MemoryManager::registerParameter(char *name, float *location)
{
    if(_registrationPeriodOpen && (_MaxFloats > _parameterIdxFloat))
    {
        _paramPtrsFloat[_parameterIdxFloat] = location;
        if(_parameterIdxFloat)
        {
            strncat(_paramNamesFloat, name, sizeof(_paramNamesFloat) - strlen(name) - 1);
        }
        else
        {
            strcpy (_paramNamesFloat, name);
        }

        strncat(_paramNamesFloat, ",", sizeof(_paramNamesFloat) - strlen(name) - 1);
        _parameterIdxFloat++;
    }
    else
    {
        _validData = false;
    }

}

MemoryManager::registerParameter(char *name, int *location)
{
    if(_registrationPeriodOpen && (_MaxInts > _parameterIdxInt))
    {
        _paramPtrsInt[_parameterIdxInt] = location;
        if(_parameterIdxInt)
        {
            strncat(_paramNamesInt, name, sizeof(_paramNamesInt) - strlen(name) - 1);
        }
        else
        {
            strcpy (_paramNamesInt, name);
        }

        strncat(_paramNamesInt, ",", sizeof(_paramNamesInt) - strlen(name) - 1);
        _parameterIdxInt++;
    }
    else
    {
        _validData = false;
    }
}


int MemoryManager::printParameter(char *name)
{
    Serial.print(name);
    int paramIdx = parseKeyword(name, _paramNamesFloat);
    if(0 > paramIdx)
    {
        int paramIdx = parseKeyword(name, _paramNamesInt);
        printParameterInt(paramIdx);
    }
    else
    {
        printParameterFloat(paramIdx);
    }

    return(paramIdx);
}

MemoryManager::printParameterInt(int paramIdx)
{
    if(0 <= paramIdx )
    {
        Serial.print(" = ");
        Serial.println(*_paramPtrsInt[paramIdx]);
    }
    else
    {
        Serial.println(F(" Does Not Exist"));
    }
}

MemoryManager::printParameterFloat(int paramIdx)
{
    if(0 <= paramIdx )
    {
        Serial.print(" = ");
        Serial.println(*_paramPtrsFloat[paramIdx],8);
    }
    else
    {
        Serial.println(F(" Does Not Exist"));
    }
}

int MemoryManager::changeParameter(char *name, float value)
{
    Serial.print("Attempting To Change: ");
    Serial.print(name);
    int paramIdx = parseKeyword(name, _paramNamesFloat);
    if(0 <= paramIdx)
    {
        printParameterFloat(paramIdx);
        *_paramPtrsFloat[paramIdx] = value;
        Serial.print(F("Value Changed to "));
        printParameterFloat(paramIdx);
    }
    else
    {

        paramIdx = parseKeyword(name, _paramNamesInt);
        if(0 <= paramIdx)
        {
            printParameterInt(paramIdx);
            *_paramPtrsInt[paramIdx] = (int)value;
            Serial.print(F("Value Changed to "));
            printParameterInt(paramIdx);
        }
        else
        {
            Serial.println(F("Parameter DNE"));
        }
    }

    if(_plasticMemory)
    {
        saveAll();
    }

    return(paramIdx);
}

unsigned long MemoryManager::eepromCrc(void)
{

    unsigned long crc = ~0L;

    for (int index = _AdrsCRCFirst ; index < _AdrsCRCLast  ; ++index)
    {
        crc = _crc_table[(crc ^ EEPROM[index]) & 0x0f] ^ (crc >> 4);
        crc = _crc_table[(crc ^ (EEPROM[index] >> 4)) & 0x0f] ^ (crc >> 4);
        crc = ~crc;
    }
    return(crc);
}

unsigned long MemoryManager::nameCrc(char *name)
{

    unsigned long crc = ~0L;

    for (int index = 0 ; index < strlen(name)  ; ++index)
    {
        crc = _crc_table[(crc ^ name[index]) & 0x0f] ^ (crc >> 4);
        crc = _crc_table[(crc ^ (name[index] >> 4)) & 0x0f] ^ (crc >> 4);
        crc = ~crc;
    }
    return(crc);
}

// MemoryManager::recallParameter(char *name)
// {
//   if(_validData)
//   {
//     int paramIdx = parseKeyword(name,_paramNamesFloat);
//     if(0>paramIdx)
//     {
//       paramIdx = parseKeyword(name,_paramNamesInt);
//       printParameterInt(paramIdx);
//       Serial.print("Recalled Int from EEPROM as ");
//       EEPROM_readAnything(paramIdx*_sizeOfInt + _AdrsFirstInt, *_paramPtrsInt[paramIdx]);
//     } else
//     {
//       printParameterFloat(paramIdx);
//       Serial.print("Recalled Float from EEPROM as ");
//       EEPROM_readAnything(paramIdx*_sizeOfFloat + _AdrsFirstFloat, *_paramPtrsFloat[paramIdx]);
//     }
//     paramIdx = printParameter(name);
//   }
// }

// MemoryManager::saveParameter(char *name)
// {
//   if(_validData)
//   {
//     int paramIdx = parseKeyword(name,_paramNamesFloat);
//     if(0>paramIdx)
//     {
//       paramIdx = parseKeyword(name,_paramNamesInt);
//       printParameterInt(paramIdx);
//       Serial.print("Saved Int to EEPROM as ");
//       EEPROM_writeAnything(paramIdx*_sizeOfInt + _AdrsFirstInt, *_paramPtrsInt[paramIdx]);

//     } else {
//       printParameterFloat(paramIdx);
//       Serial.print("Saved Float to EEPROM as ");
//       EEPROM_writeAnything(paramIdx*_sizeOfFloat + _AdrsFirstFloat, *_paramPtrsFloat[paramIdx]);

//     }
//     paramIdx = printParameter(name);

//     unsigned long newEepromCrc = eepromCrc();

//     EEPROM_writeAnything(_AdrsFirstCrc                 , newEepromCrc);
//     EEPROM_writeAnything(_AdrsFirstCrc + _sizeOfFloat  , newEepromCrc);
//     EEPROM_writeAnything(_AdrsFirstCrc + _sizeOfFloat*2, newEepromCrc);

//     unsigned long eepromCrcReadBack1, eepromCrcReadBack2, eepromCrcReadBack3;
//     EEPROM_readAnything(_AdrsFirstCrc                 , eepromCrcReadBack1);
//     EEPROM_readAnything(_AdrsFirstCrc + _sizeOfFloat  , eepromCrcReadBack2);
//     EEPROM_readAnything(_AdrsFirstCrc + _sizeOfFloat*2, eepromCrcReadBack3);

//     if((eepromCrcReadBack1 != eepromCrcReadBack2) || (eepromCrcReadBack2 != eepromCrcReadBack3) || (eepromCrcReadBack3 != newEepromCrc) )
//     {
//       _validData = false;
//     }

//   }
// }