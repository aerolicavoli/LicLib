/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    HardwareInterface.h
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef HardwareInterface_h
#define HardwareInterface_h

//#define XENCPIN1 2
//#define XENCPIN2 3

//#define THETAENCPIN1 20
//#define THETAENCPIN2 21

#include "MemoryManager.h"

//int xEncPin1        = 2;
//int xEncPin2        = 3;

//int ThetaEncPin1    = 20;
//int ThetaEncPin2    = 21;

const unsigned int NUM_PINS = 24;

enum pinStates {	PIN_LOW = 0,
              	    PIN_HIGH,
                	PIN_ERROR
                };

enum specPinModes {NOT_FOR_UI = 0,
				   NOT_CONNECTED,
				   AVAILABLE,
				   DIGITAL_SI,
				   DIGITAL_SO,
				   ANALOG_SI,
				   ANALOG_SO
				};

enum pinNames {
	PLUGA_PIN3 = 0,
	PLUGA_PIN4,
	PLUGA_PIN6,
	PLUGA_PIN2,

	PLUGB_PIN3,
	PLUGB_PIN4,
	PLUGB_PIN6,
	PLUGB_PIN2,

	PLUGC_PIN3,
	PLUGC_PIN4,
	PLUGC_PIN6,
	PLUGC_PIN2,

	PLUGD_PIN3,
	PLUGD_PIN4,
	PLUGD_PIN6,
	PLUGD_PIN2,

	PLUGE_PIN3,
	PLUGE_PIN4,
	PLUGE_PIN6,
	PLUGE_PIN2,

	PLUGF_PIN3,
	PLUGF_PIN4,
	PLUGF_PIN6,
	PLUGF_PIN2 };


struct pinStructure
{
	int pinNum;
	int specPinMode;
};

extern pinStructure ListOfPins[NUM_PINS];


class HardwareInterface
{

public:


    /*
    ** Setup Encoders
    */
    //static Encoder xEnc;// xEnc(2, 3);
    //static Encoder thetaEnc;// thetaEnc(20, 21);

    HardwareInterface(MemoryManager *ptrMemoryManager);
    setup(void);
    stop(void);

    double readX(void);
    void writeX(double Xcm);
    double readTheta(void);
    void writeTheta(double Thetadeg);
    void moveMotor(int power);

    bool xLimitSwitch(void);

	int readDigitalPin(unsigned int pin);


    /*
    ** Pins
    */
    int motorDirPin     = 48;
    int motorPowerPin   =  4;

    int xLimitSwitchPin = 22;



private:
    MemoryManager *_ptrMemoryManager;

    float pulse2Xcm, pulse2Thetadeg;

};

#endif
