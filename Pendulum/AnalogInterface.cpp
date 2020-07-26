/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    AnalogInterface.cpp
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
#include "AnalogInterface.h"


AnalogInterface::AnalogInterface(unsigned long updateInterval, SystemState *state, int *majorMode, MemoryManager *ptrMemoryManager, HardwareInterface* ptrHardware)
{
    _timeKeeperRead = Metro(updateInterval);
    _timeKeeperWrite = Metro(updateInterval);
    _state = state;
    _majorMode = majorMode;
    _ptrMemoryManager = ptrMemoryManager;
    _ptrHardware = ptrHardware;
    _switchState = 0;
    _oldrightPot = 0;

}

AnalogInterface::setup()
{

    FastLED.addLeds<WS2811, A11, GRB>(_leds, 2);
 	FastLED.setBrightness(  64 );

 	pinMode(A10,INPUT);
 	pinMode(A9,INPUT);
 	pinMode(A8,INPUT);
    
    _leds[0] = CRGB::Blue;
    _leds[1] = CRGB::Blue;

    FastLED.show();

    Serial.println(F("ANALOG INTERFACE: Setup Starting"));
    Serial.println(F("ANALOG INTERFACE: Setup Complete"));

}

AnalogInterface::write(unsigned long &time)
{
    if(0 < _timeKeeperWrite.check(time))
    {
    	/*
    	** Do Stuff
    	*/ 
    	if(_state->getMotorEnabled())
    	{
    		_leds[1] = CRGB::Green;

    	} else {
    		_leds[1] = CRGB::Red;
    	}

    	if(NEUTRAL_ERROR < _state->getError() )
    	{
    		_leds[0] = CRGB::Red;
    	} else if (NO_ERROR < _state->getError())
    	{
    		_leds[0] = CRGB::Yellow;
    		
    	} else
    	{
    		_leds[0] = CRGB::Green;
    	}

    	FastLED.show();

    	/*
        ** Update loop time
        */
        time = millis();
    }

}

AnalogInterface::read(unsigned long &time)
{
    if(0 < _timeKeeperRead.check(time) )
    {
    	/*
    	** SISO Digital Pins
    	*/
    	// for(int i = 0; i < _numSisoDigPins; i++)
    	// {
    	// 	bool pinState = _ptrHardware->readDigitalPin(ListOfPins[i])
    	// 	if(PIN_ERROR == pinState)
    	// 	{
    	// 		_state->reportError(UI_ERROR, F("Invalid Pin Setup"));
    	// 	}

    	// 	switch(ListOfPins[i])
    	// 	{

    	// 	}

  //  	}

    	/*
    	** SISO Analog Pins
    	*/
    	//Pot Right
    	int rightPot = analogRead(A10);

    	if(rightPot != _oldrightPot)
    	{
    		_oldrightPot = rightPot;
    		float newXRef = 60.0*rightPot/1023.0 + 55.0 - 30.0;
    		//_ptrMemoryManager->changeParameter("XREF", newXRef);
    		_state->_printThis = newXRef; 

    	}		
    	
    	//Pot Left
    	int leftPot = analogRead(A9);
    	
    	//Switch
    	int switchVal = analogRead(A8);

    	//Left State
    	if(switchVal < 10)
    	{
    		_state->setMotorEnabled(true);
    		_switchState = 1;

    	} else if(switchVal > 900) // Middle State
    	{
    		_state->setMotorEnabled(false);
    		_switchState = 2;

    	} else //Right State 
    	{	
    		if(_switchState != 3)
    		{
    			_switchState = 3;
    			_state->setMotorEnabled(false);
    			_state->zeroTheta();
    		}
    		

    	}

    	/*
    	** Multiplexed Pins
    	*/
        

        /*
        ** Update loop time
        */
        time = millis();
        
    }

}