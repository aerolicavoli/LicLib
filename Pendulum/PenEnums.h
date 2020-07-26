/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    PenEnums.h
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef PenEnums_h
#define PenEnums_h

/*
** Errors enumerated by severity. Anything worse than a UI_Error
** will cause a shutdown until reset is called.
*/

// Error Name                     Motor Output    Clearing Behavior
// STARTUP                        Allowed         Default At Startup
// NO_ERROR                       Allowed         Manual Clear
// DIAGNOSTIC_WARN                Allowed         Manual Clear
// MOTOR_WARN                     Allowed         Manual Clear
// UI_ERROR                       Allowed         Manual Clear
// NEUTRAL_ERROR                  Allowed         Manual Clear
// CODE_ERROR                     Blocked         Manual Clear
// DIAGNOSTIC_STOP_TEMP           Blocked         Cleared Automatically Every Diagnostic Cycle
// DIAGNOSTIC_STOP_NEG_CORRECT    NEG cmd Allowed Cleared Automatically Every Diagnostic Cycle
// DIAGNOSTIC_STOP_POS_CORRECT    POS cmd Allowed Cleared Automatically Every Diagnostic Cycle
// DIAGNOSTIC_STOP_THETA          Blocked         Cleared Automatically When Within Theta Start Bounds
// DIAGNOSTIC_STOP                Blocked         Manual Clear

enum errorStates { 	STARTUP = 0,
                    NO_ERROR,
                    DIAGNOSTIC_WARN,
                    UI_ERROR,
                    MOTOR_WARN,
                    NEUTRAL_ERROR,
                    CODE_ERROR,
                    DIAGNOSTIC_STOP_TEMP,
                    DIAGNOSTIC_STOP_NEG_CORRECT,
                    DIAGNOSTIC_STOP_POS_CORRECT,
                    DIAGNOSTIC_STOP_THETA,
                    DIAGNOSTIC_STOP,
                    BAD_GAIN_STOP
                 };



//enum sisoDigPins { PLUG1_PIN1 = 0,
				   //PLUG1_PIN1 = 0,
	               //PLUG1_PIN1 = 0,
	               //PLUG1_PIN1 = 0,
	               //PLUG1_PIN1 = 0,
//}
enum routines {	IDLE = 0,
                SINE_WAVE,
                UI_SINE_WAVE,
                SYSTEM_TEST,
                CNC_ROUTINE,
                HOME,
                NESTEDPID_ROUTINE,
                HOME_ROUTINE
              };

enum cmdTypes {cmdSTOP = 0,
               cmdSTART,
               cmdZEROX,
               cmdZEROTHETA,
               cmdRESET,
               cmdIDLE,
               cmdHome,
               cmdSINEWAVE,
               cmdCNC,
               cmdUISINEWAVE,
               cmdEEPROMRESET,
               cmdEEPROMPRINTALL,
               cmdEEPROMSAVEALL,
               cmdEEPROMRECALLALL,
               cmdEnablePlasticMode,
               cmdDisablePlasticMode,
               cmdEnableRecallAtBootup,
               cmdDisableRecallAtBootup,
               cmdSYSTEMTEST,
               cmdUP,
               cmdDOWN,
               cmdNESTED_PID
              };

#endif
