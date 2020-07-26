/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    HardwareInterface.cpp
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
#include "HardwareInterface.h"


// pinStructure ListOfPins[24];

// ListOfPins[PLUGA_PIN3] = {A7,   AVAILABLE}; 
// ListOfPins[PLUGA_PIN4] = {A6,   AVAILABLE};  // PLUGA4
// ListOfPins[PLUGA_PIN6] = {34,   AVAILABLE};  // PLUGA6
// ListOfPins[PLUGA_PIN2] = {0 ,   NOT_FOR_UI}; // PLUGA2 Arduino Reset Button

// ListOfPins[PLUGB_PIN3] = {A11,  AVAILABLE}; // PLUBB3
// ListOfPins[PLUGB_PIN4] = {A10,  AVAILABLE}; // PLUGB4
// ListOfPins[PLUGB_PIN6] = {A9,   AVAILABLE}; // PLUGB6
// ListOfPins[PLUGB_PIN2] = {A8,   AVAILABLE}; // PLUGB2
                 
// ListOfPins[PLUGC_PIN3] = {1,    AVAILABLE}; // PLUGC3
// ListOfPins[PLUGC_PIN4] = {A14,  AVAILABLE}; // PLUGC4
// ListOfPins[PLUGC_PIN6] = {A13,  AVAILABLE}; // PLUGC6
// ListOfPins[PLUGC_PIN2] = {A12,  AVAILABLE}; // PLUGC2
                           
// ListOfPins[PLUGD_PIN3] = {26,   AVAILABLE}; // PLUGD3
// ListOfPins[PLUGD_PIN4] = {28,   AVAILABLE}; // PLUGD4
// ListOfPins[PLUGD_PIN6] = {30,   AVAILABLE}; // PLUGD6
// ListOfPins[PLUGD_PIN2] = {32,   AVAILABLE}; // PLUGD2

// ListOfPins[PLUGE_PIN3] = {22,   NOT_FOR_UI}; // PLUGE3 X Limit Switch
// ListOfPins[PLUGE_PIN4] = {3,    NOT_FOR_UI}; // PLUGE4 X Encoder
// ListOfPins[PLUGE_PIN6] = {2,    NOT_FOR_UI}; // PLUGE6 X Enoder
// ListOfPins[PLUGE_PIN2] = {24,   AVAILABLE};  // PLUGE2

// ListOfPins[PLUGF_PIN3] = {19,   AVAILABLE};  // PLUGF3
// ListOfPins[PLUGF_PIN4] = {20,   NOT_FOR_UI}; // PLUGF4 Theta Encoder
// ListOfPins[PLUGF_PIN6] = {21,   NOT_FOR_UI}; // PLUGF6 Theta Enoder
// ListOfPins[PLUGF_PIN2] = {6 ,   AVAILABLE};  // PLUGF2

/*
** WARNING do not change the order of this struct without updating enum in header
*/
pinStructure ListOfPins[NUM_PINS] = {
							{A7,   AVAILABLE},  // PLUGA3
                            {A6,   AVAILABLE},  // PLUGA4
                            {34,   AVAILABLE},  // PLUGA6
                            {0 ,   NOT_FOR_UI}, // PLUGA2 Arduino Reset Button

                            {A11,  AVAILABLE}, // PLUBB3
                            {A10,  AVAILABLE}, // PLUGB4
                            {A9,   AVAILABLE}, // PLUGB6
                            {A8,   AVAILABLE}, // PLUGB2
                            
                            {1,    AVAILABLE}, // PLUGC3
                            {A14,  AVAILABLE}, // PLUGC4
                            {A13,  AVAILABLE}, // PLUGC6
                            {A12,  AVAILABLE}, // PLUGC2
                            
                            {26,   AVAILABLE}, // PLUGD3
                            {28,   AVAILABLE}, // PLUGD4
                            {30,   AVAILABLE}, // PLUGD6
                            {32,   AVAILABLE}, // PLUGD2

                            {22,   DIGITAL_SI}, // PLUGE3 X Limit Switch
                            {3,    NOT_FOR_UI}, // PLUGE4 X Encoder
                            {2,    NOT_FOR_UI}, // PLUGE6 X Enoder
                            {24,   AVAILABLE},  // PLUGE2

                            {19,   AVAILABLE},  // PLUGF3
                            {20,   NOT_FOR_UI}, // PLUGF4 Theta Encoder
                            {21,   NOT_FOR_UI}, // PLUGF6 Theta Enoder
                            {6 ,   AVAILABLE}}; // PLUGF2

HardwareInterface::HardwareInterface(MemoryManager *ptrMemoryManager)
{
    _ptrMemoryManager = ptrMemoryManager;
}



HardwareInterface::setup()
{
	/*
	** Setup Pin Modes
	*/
	for(unsigned int i = 0; i < NUM_PINS; i++)
	{
		if((DIGITAL_SI == ListOfPins[i].specPinMode) ||
			(ANALOG_SI == ListOfPins[i].specPinMode))
		{
			pinMode(ListOfPins[i].pinNum, INPUT);
		} else if((DIGITAL_SO == ListOfPins[i].specPinMode) ||
			(ANALOG_SO == ListOfPins[i].specPinMode))
		{
			pinMode(ListOfPins[i].pinNum, OUTPUT);
		}
	}
    pinMode(motorDirPin, OUTPUT);
    pinMode(motorPowerPin, OUTPUT);

    analogWrite(motorPowerPin, 0);
    digitalWrite(motorDirPin, LOW);
}

double HardwareInterface::readX(void)
{
    return(0);
}

double HardwareInterface::readTheta(void)
{
    return(0);
}

void HardwareInterface::writeX(double Xcm)
{
    //xEnc.write(Xcm/pulse2Xcm);
}

void HardwareInterface::writeTheta(double Thetadeg)
{
    //thetaEnc.write(Thetadeg/pulse2Thetadeg);
}

HardwareInterface::stop(void)
{
    analogWrite(motorPowerPin, 0);
    digitalWrite(motorDirPin, LOW);
}

int HardwareInterface::readDigitalPin(unsigned int pin)
{
	if(DIGITAL_SO == ListOfPins[pin].pinNum)
	{
		return(digitalRead(pin));
	} else
	{
		return(PIN_ERROR);
	}

}

void HardwareInterface::moveMotor(int power)
{
    if(0 < power)
    {
        digitalWrite(motorDirPin, LOW);
    }
    else
    {
        digitalWrite(motorDirPin, HIGH);
    }

    analogWrite(motorPowerPin, abs(power));

}

bool HardwareInterface::xLimitSwitch(void)
{
    return(digitalRead(ListOfPins[PLUGE_PIN3].pinNum));
}

