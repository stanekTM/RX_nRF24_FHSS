#ifndef __PWM_h__
#define __PWM_h__

#include <Arduino.h>
#include <stdint.h>

//**********************************************
// PWM předdělička pro ATmega328P a ATmega328PB
//**********************************************

//***************************************************************************************
// Pin PD5(5) a PD6(6) (8-bit Timer 0, funkce delay, millis, micros a delayMicroseconds)
//***************************************************************************************
//#define TIMER0_61HZ
//#define TIMER0_244HZ
//#define TIMER0_976HZ_DEFAULT
//#define TIMER0_7812HZ
//#define TIMER0_62500HZ

#if defined(TIMER0_61HZ)
  #define PRESCALER_TIMER0_5_6  1024
#endif
#if defined(TIMER0_244HZ)
  #define PRESCALER_TIMER0_5_6  256
#endif
#if defined(TIMER0_976HZ_DEFAULT)
  #define PRESCALER_TIMER0_5_6  64
#endif
#if defined(TIMER0_7812HZ)
  #define PRESCALER_TIMER0_5_6  8
#endif
#if defined(TIMER0_62500HZ)
  #define PRESCALER_TIMER0_5_6  1
#endif

//********************************************************************
// Pin PB1(9) a PB2(10) (16-bit Timer 1, knihovna Servo do 12 kanálů)
//********************************************************************
//#define TIMER1_30HZ
//#define TIMER1_122HZ
//#define TIMER1_488HZ_DEFAULT
//#define TIMER1_3906HZ
//#define TIMER1_31250HZ

#if defined(TIMER1_30HZ)
  #define PRESCALER_TIMER1_9_10  1024
#endif
#if defined(TIMER1_122HZ)
  #define PRESCALER_TIMER1_9_10  256
#endif
#if defined(TIMER1_488HZ_DEFAULT)
  #define PRESCALER_TIMER1_9_10  64
#endif
#if defined(TIMER1_3906HZ)
  #define PRESCALER_TIMER1_9_10  8
#endif
#if defined(TIMER1_31250HZ)
  #define PRESCALER_TIMER1_9_10  1
#endif

//**************************************
// Pin PD3(3) a PB3(11) (8-bit Timer 2)
//**************************************
//#define TIMER2_30HZ
//#define TIMER2_122HZ
//#define TIMER2_244HZ
//#define TIMER2_488HZ_DEFAULT
//#define TIMER2_976HZ
//#define TIMER2_3906HZ
//#define TIMER2_31250HZ

#if defined(TIMER2_30HZ)
  #define PRESCALER_TIMER2_3_11  1024
#endif
#if defined(TIMER2_122HZ)
  #define PRESCALER_TIMER2_3_11  256
#endif
#if defined(TIMER2_244HZ)
  #define PRESCALER_TIMER2_3_11  128
#endif
#if defined(TIMER2_488HZ_DEFAULT)
  #define PRESCALER_TIMER2_3_11  64
#endif
#if defined(TIMER2_976HZ)
  #define PRESCALER_TIMER2_3_11  32
#endif
#if defined(TIMER2_3906HZ)
  #define PRESCALER_TIMER2_3_11  8
#endif
#if defined(TIMER2_31250HZ)
  #define PRESCALER_TIMER2_3_11  1
#endif

//**********************************************************************************************************
// Pin PD0(0)RX sériový a sdílený pin PD2(2) (ATmega328PB 16-bit Timer 3, knihovna Servo dalších 12 kanálů)
//**********************************************************************************************************
//#define TIMER3_30HZ
//#define TIMER3_122HZ
//#define TIMER3_488HZ_DEFAULT
//#define TIMER3_3906HZ
//#define TIMER3_31250HZ

#if defined(TIMER3_30HZ)
  #define PRESCALER_TIMER3_0  1024
#endif
#if defined(TIMER3_122HZ)
  #define PRESCALER_TIMER3_0  256
#endif
#if defined(TIMER3_488HZ_DEFAULT)
  #define PRESCALER_TIMER3_0  64
#endif
#if defined(TIMER3_3906HZ)
  #define PRESCALER_TIMER3_0  8
#endif
#if defined(TIMER3_31250HZ)
  #define PRESCALER_TIMER3_0  1
#endif

//************************************************************************
// Pin PD1(1)TX sériový a sdílený pin PD2(2) (ATmega328PB 16-bit Timer 4)
//************************************************************************
//#define TIMER4_30HZ
//#define TIMER4_122HZ
//#define TIMER4_488HZ_DEFAULT
//#define TIMER4_3906HZ
//#define TIMER4_31250HZ

#if defined(TIMER4_30HZ)
  #define PRESCALER_TIMER4_1_2  1024
#endif
#if defined(TIMER4_122HZ)
  #define PRESCALER_TIMER4_1_2  256
#endif
#if defined(TIMER4_488HZ_DEFAULT)
  #define PRESCALER_TIMER4_1_2  64
#endif
#if defined(TIMER4_3906HZ)
  #define PRESCALER_TIMER4_1_2  8
#endif
#if defined(TIMER4_31250HZ)
  #define PRESCALER_TIMER4_1_2  1
#endif

//******************************************************************
// Nastavení PWM předděličky
//******************************************************************
inline void set_PWM_prescaler(const uint8_t pin, uint16_t prescale)
{
  uint8_t mode = 0;
  
  // Vstupní kontrola pro 8-bit Timer 0, 16-bit Timer 1 a ATmega328PB 16-bit Timer 3, 16-bit Timer 4
  if (pin == 0 || pin == 1 || pin == 2 || pin == 5 || pin == 6 || pin == 9 || pin == 10)
  {
    if (prescale == 1)         mode = 0b00000001;
    else if (prescale == 8)    mode = 0b00000010;
    else if (prescale == 64)   mode = 0b00000011;
    else if (prescale == 256)  mode = 0b00000100;
    else if (prescale == 1024) mode = 0b00000101;
    else return;
  }
  // Vstupní kontrola pro 8-bit Timer 2 (obsahuje navíc prescalery 32 a 128)
  else if (pin == 3 || pin == 11)
  {
    if (prescale == 1)         mode = 0b00000001;
    else if (prescale == 8)    mode = 0b00000010;
    else if (prescale == 32)   mode = 0b00000011;
    else if (prescale == 64)   mode = 0b00000100;
    else if (prescale == 128)  mode = 0b00000101;
    else if (prescale == 256)  mode = 0b00000110;
    else if (prescale == 1024) mode = 0b00000111;
    else return;
  }
  else return; // Neplatný pin
  
  // Přímý zápis do registrů podle pinu.
  // Pin PD5(5) and PD6(6) (8-bit Timer 0, funkce delay, millis, micros a delayMicroseconds)
  if (pin == 5 || pin == 6)
  {
    TCCR0A = (TCCR0A & 0b11111100) | 0b00000001; // Mode 1: 8-bit Phase Correct PWM (strop 255)
    TCCR0B = (TCCR0B & 0b11111000) | mode; // Nastavíme prescaler z mode
  }
  // Pin PB1(9) and PB2(10) (16-bit Timer 1, knihovna Servo do 12 kanálů)
  else if (pin == 9 || pin == 10)
  {
    TCCR1A = (TCCR1A & 0b11111100) | 0b00000001; // Mode 1: 8-bit Phase Correct PWM (strop 255)
    TCCR1B = (TCCR1B & 0b11111000) | mode; // Nastavíme prescaler z mode
  }
  // Pin PD3(3) and PB3(11) (8-bit Timer 2)
  else if (pin == 3 || pin == 11)
  {
    TCCR2A = (TCCR2A & 0b11111100) | 0b00000001; // Mode 1: 8-bit Phase Correct PWM (strop 255)
    TCCR2B = (TCCR2B & 0b11111000) | mode; // Nastavíme prescaler z mode
  }
  // Specifické registry pouze pro rozšířený čip ATmega328PB
  #if defined(__AVR_ATmega328PB__) || defined(__AVR_ATmega328pb__)
    // Pin PD1(1)TX sériový a sdílený pin PD2(2) (16-bit Timer 4)
    else if (pin == 1 || pin == 2)
    {
      TCCR4A = (TCCR4A & 0b11111100) | 0b00000001; // Mode 1: 8-bit Phase Correct PWM (strop 255)
      TCCR4B = (TCCR4B & 0b11111000) | mode; // Nastavíme prescaler z mode
    }
    // Pin PD0(0)RX sériový (16-bit Timer 3, knihovna Servo dalších 12 kanálů)
    else if (pin == 0) // Ošetřeno bez pinu PD2(2) kvůli eliminaci kolize s Timerem 4
    {
      TCCR3A = (TCCR3A & 0b11111100) | 0b00000001; // Mode 1: 8-bit Phase Correct PWM (strop 255)
      TCCR3B = (TCCR3B & 0b11111000) | mode; // Nastavíme prescaler z mode
    }
  #endif
}

#endif // Konec __PWM_h__
