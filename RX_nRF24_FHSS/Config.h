#ifndef __Config_h__
#define __Config_h__

#include <Arduino.h>
#include <stdint.h>

/*
**********************************************************************************************************
Receiver configuration description (see examples below)
**********************************************************************************************************
#define PIN_LED         2   Define a pin other than A5
#define PIN_BATTERY_A1  A7  Define an analog pin other than A6 (only A5 or A7 available)
#define PIN_BATTERY_A2  A6  Define an analog pin other than A7 (only A5 or A6 available)

#define BATT_VOLTAGE_A1       4.2   Set the maximum nominal voltage for battery A1
#define MONITORED_VOLTAGE_A1  3.45  Set minimum alarm voltage for battery A1

#define BATTERY_A2                 Option to add a second battery
#define BATT_VOLTAGE_A2       8.4  Set the maximum nominal voltage for battery A2
#define MONITORED_VOLTAGE_A2  6.9  Set minimum alarm voltage for battery A2

#define SERVO                              Define servo outputs (up to 14 servos, ATmega328PB only)
const uint8_t servo_pins[] = {2, 3, ...};  Select pins for servos

#define MIX_TANK  Option to add a Tank / Arcade mix for two motors

#define NUM_MOTORS  1 to 2  Define the number of motors

#define TIMER..30HZ to 31250HZ  Frequency setting according to motor type

Setting motor limits in % (0 to 100)
#define REACTION_MOTOR    15   Motor starting response. Prevents initial magnetic resistance of the rotor
#define MAX_FWD_MOTOR_T2  100  Maximum permitted power for the forward direction
#define MAX_REV_MOTOR_T2  50   Maximum permitted power for the reverse direction (e.g., safer reversing)
#define BRAKE_MOTOR_T2    0    Braking force during stopping (0 = idling, 100 = maximum brake)
**********************************************************************************************************
*/

//******************************************************************
// Custom receiver configuration
//******************************************************************
//#define BLANIK_L13      // Glider Let L-13 Blaník (4 servos)
#define FERARI_F40      // Ferari F-40 (1 motor + 1 servo)
//#define BUGGY_1_32      // Buggy 1:32 (2 motors)
//#define EACHINE_MONSTER // Eachine Monster (2 motors with mix)

//***********************************
// Glider Let L-13 Blaník (4 servos)
//***********************************
#if defined(BLANIK_L13)
  #define BATT_VOLTAGE_A1  4.2
  #define MONITORED_VOLTAGE_A1  3.45
  #define SERVO
  const uint8_t servo_pins[] = {2, 3, 4, 5};
#endif

//*********************************
// Ferari F-40 (1 motor + 1 servo)
//*********************************
#if defined(FERARI_F40)
  #define PIN_LED  13
  #define PIN_BATTERY_A1  A7
  #define BATT_VOLTAGE_A1  4.2
  #define MONITORED_VOLTAGE_A1  3.45
  #define SERVO
  const uint8_t servo_pins[] = {9}; // Except for pins 3, 11
  #define NUM_MOTORS  1
  
  #define MOTOR_T2
  #define TIMER2_122HZ
  #define REACTION_MOTOR_T2  0
  #define MAX_FWD_MOTOR_T2   100
  #define MAX_REV_MOTOR_T2   100
  #define BRAKE_MOTOR_T2     0
#endif

//***********************
// Buggy 1:32 (2 motors)
//***********************
#if defined(BUGGY_1_32)
  #define PIN_LED  2
  #define PIN_BATTERY_A1  A7
  #define BATT_VOLTAGE_A1  4.2
  #define MONITORED_VOLTAGE_A1  3.45
  #define NUM_MOTORS  2
  
  #define MOTOR_T2
  #define TIMER2_31250HZ
  #define REACTION_MOTOR_T2  0
  #define MAX_FWD_MOTOR_T2   100
  #define MAX_REV_MOTOR_T2   100
  #define BRAKE_MOTOR_T2     0
  
  #define MOTOR_T1
  #define TIMER1_31250HZ
  #define REACTION_MOTOR_T1  0
  #define MAX_FWD_MOTOR_T1   100
  #define MAX_REV_MOTOR_T1   100
  #define BRAKE_MOTOR_T1     0
#endif

//*************************************
// Eachine Monster (2 motors with mix)
//*************************************
#if defined(EACHINE_MONSTER)
  #define PIN_LED  2
  #define BATT_VOLTAGE_A1  4.2
  #define MONITORED_VOLTAGE_A1  3.45
  #define MIX_TANK
  #define NUM_MOTORS  2
  
  #define MOTOR_T2
  #define TIMER2_31250HZ
  #define REACTION_MOTOR_T2  0
  #define MAX_FWD_MOTOR_T2   100
  #define MAX_REV_MOTOR_T2   100
  #define BRAKE_MOTOR_T2     100
  
  #define MOTOR_T1
  #define TIMER1_31250HZ
  #define REACTION_MOTOR_T1  0
  #define MAX_FWD_MOTOR_T1   100
  #define MAX_REV_MOTOR_T1   100
  #define BRAKE_MOTOR_T1     100
#endif

//********************************************************************
// Receiver maximum outputs configuration examples for the ATmega328P
//********************************************************************
//#define SERVO_P        // Up to 12 servos
//#define MOTOR_T2_T1    // 2 motors (option to mixing)
//#define MOTOR_T2_SERVO // 1 motor + up to 12 servos

//*******************
// Up to 12 servos
//*******************
#if defined(SERVO_P)
  #define BATT_VOLTAGE_A1  4.2
  #define MONITORED_VOLTAGE_A1  3.45
  #define SERVO
  const uint8_t servo_pins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
#endif

//*****************************
// 2 motors (option to mixing)
//*****************************
#if defined(MOTOR_T2_T1)
  #define BATT_VOLTAGE_A1  4.2
  #define MONITORED_VOLTAGE_A1  3.45
  //#define MIX_TANK
  #define NUM_MOTORS  2
  
  #define MOTOR_T2
  #define TIMER2_31250HZ
  #define REACTION_MOTOR_T2  0
  #define MAX_FWD_MOTOR_T2   100
  #define MAX_REV_MOTOR_T2   100
  #define BRAKE_MOTOR_T2     0
  
  #define MOTOR_T1
  #define TIMER1_31250HZ
  #define REACTION_MOTOR_T1  0
  #define MAX_FWD_MOTOR_T1   100
  #define MAX_REV_MOTOR_T1   100
  #define BRAKE_MOTOR_T1     0
#endif

//***************************
// 1 motor + up to 12 servos
//***************************
#if defined(MOTOR_T2_SERVO)
  #define BATT_VOLTAGE_A1  4.2
  #define MONITORED_VOLTAGE_A1  3.45
  #define SERVO
  const uint8_t servo_pins[] = {0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 12, 13}; // Except for pins 3, 11
  #define NUM_MOTORS  1
  
  #define MOTOR_T2
  #define TIMER2_31250HZ
  #define REACTION_MOTOR_T2  0
  #define MAX_FWD_MOTOR_T2   100
  #define MAX_REV_MOTOR_T2   100
  #define BRAKE_MOTOR_T2     0
#endif

//**********************************************************************
// Receiver maximum outputs configuration examples for ATmega328PB only
//**********************************************************************
//#define SERVO_PB          // Up to 14 servos (In the Servo library, in the file "Servo.h", change the line SERVOS_PER_TIMER from 12 to 14)
//#define MOTOR_T1_T4       // 2 motors (option to mixing)
//#define MOTOR_T2_T4       // 2 motors (option to mixing)
//#define MOTOR_T2_T4_SERVO // 2 motors (option to mixing) + up to 10 servos

//********************
// Up to 14 servos
//********************
#if defined(SERVO_PB)
  #define BATT_VOLTAGE_A1  4.2
  #define MONITORED_VOLTAGE_A1  3.45
  #define SERVO
  const uint8_t servo_pins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
#endif

//*****************************
// 2 motors (option to mixing)
//*****************************
#if defined(MOTOR_T1_T4)
  #define BATT_VOLTAGE_A1  4.2
  #define MONITORED_VOLTAGE_A1  3.45
  //#define MIX_TANK
  #define NUM_MOTORS  2
  
  #define MOTOR_T1
  #define TIMER1_31250HZ
  #define REACTION_MOTOR_T1  0
  #define MAX_FWD_MOTOR_T1   100
  #define MAX_REV_MOTOR_T1   100
  #define BRAKE_MOTOR_T1     0
  
  #define MOTOR_T4
  #define TIMER4_31250HZ
  #define REACTION_MOTOR_T4  0
  #define MAX_FWD_MOTOR_T4   100
  #define MAX_REV_MOTOR_T4   100
  #define BRAKE_MOTOR_T4     0
#endif

//*****************************
// 2 motors (option to mixing)
//*****************************
#if defined(MOTOR_T2_T4)
  #define BATT_VOLTAGE_A1  4.2
  #define MONITORED_VOLTAGE_A1  3.45
  //#define MIX_TANK
  #define NUM_MOTORS  2
  
  #define MOTOR_T2
  #define TIMER2_31250HZ
  #define REACTION_MOTOR_T2  0
  #define MAX_FWD_MOTOR_T2   100
  #define MAX_REV_MOTOR_T2   100
  #define BRAKE_MOTOR_T2     0
  
  #define MOTOR_T4
  #define TIMER4_31250HZ
  #define REACTION_MOTOR_T4  0
  #define MAX_FWD_MOTOR_T4   100
  #define MAX_REV_MOTOR_T4   100
  #define BRAKE_MOTOR_T4     0
#endif

//***********************************************
// 2 motors (option to mixing) + up to 10 servos
//***********************************************
#if defined(MOTOR_T2_T4_SERVO)
  #define BATT_VOLTAGE_A1  4.2
  #define MONITORED_VOLTAGE_A1  3.45
  #define SERVO
  const uint8_t servo_pins[] = {0, 4, 5, 6, 7, 8, 9, 10, 12, 13}; // Except for pins 1, 2, 3, 11
  //#define MIX_TANK
  #define NUM_MOTORS  2
  
  #define MOTOR_T2
  #define TIMER2_31250HZ
  #define REACTION_MOTOR_T2  0
  #define MAX_FWD_MOTOR_T2   100
  #define MAX_REV_MOTOR_T2   100
  #define BRAKE_MOTOR_T2     0
  
  #define MOTOR_T4
  #define TIMER4_31250HZ
  #define REACTION_MOTOR_T4  0
  #define MAX_FWD_MOTOR_T4   100
  #define MAX_REV_MOTOR_T4   100
  #define BRAKE_MOTOR_T4     0
#endif
// End of receiver configuration ***************************************************************

// Pokud nepotřebujeme motory, nastavíme výchozí hodnotu na 0
#ifndef NUM_MOTORS
  #define NUM_MOTORS  0
#endif

// Pokud nepotřebujeme serva, nastavíme výchozí hodnotu na 0
#ifndef SERVO
  const uint8_t NUM_SERVOS = 0;
// Pokud potřebujeme serva, spočítáme je z pole a vytvoříme objekty
#else
  const uint8_t NUM_SERVOS = sizeof(servo_pins);
  Servo servo[NUM_SERVOS];
#endif

// Finální matematický součet kanálů pro rádio struktury
const uint8_t num_rc_channels = NUM_MOTORS + NUM_SERVOS;

// Nastavení mrtvého pásma nekvalitních potenciometrů RC vysílače pro řízení motoru
#define DEAD_ZONE  15

// Nastavení hodnoty rozsahu regulace
#define MIN_CONTROL_VAL  1000
#define MID_CONTROL_VAL  1500
#define MAX_CONTROL_VAL  2000

#endif // Konec __Config_h__
