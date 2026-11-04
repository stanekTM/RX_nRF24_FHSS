#ifndef __Pins_h__
#define __Pins_h__

#include <Arduino.h>
#include <stdint.h>

/*
*************************************************************************************************
Přehled pinů ATmega328P/PB
*************************************************************************************************
PD0 - D0  PWM ATmega328PB (Timer 3 - RX serial, knihovna Servo dalších 12 kanálů)
PD1 - D1  PWM ATmega328PB (Timer 4 - TX serial)
PD2 - D2  PWM ATmega328PB (Timer 3 - knihovna Servo dalších 12 kanálů, sdílený pin na Timeru 4)
PD3 - D3  PWM             (Timer 2)
PD4 - D4
PD5 - D5  PWM             (Timer 0 - funkce delay, millis, micros a delayMicroseconds)
PD6 - D6  PWM             (Timer 0 - funkce delay, millis, micros a delayMicroseconds)
PD7 - D7
PB0 - D8
PB1 - D9  PWM             (Timer 1 - knihovna Servo do 12 kanálů)
PB2 - D10 PWM             (Timer 1 - knihovna Servo do 12 kanálů)
PB3 - D11 PWM MOSI        (Timer 2)
PB4 - D12     MISO
PB5 - D13     SCK
PC0 - D14/A0
PC1 - D15/A1
PC2 - D16/A2
PC3 - D17/A3
PC4 - D18/A4  SDA
PC5 - D19/A5  SCL
PB6 - D20     XTAL1
PB7 - D21     XTAL2
PC6 - D22     RESET
PE0 - D23     ATmega328PB
PE1 - D24     ATmega328PB
PE2 - D25/A6  ATmega328PB
PE3 - D26/A7  ATmega328PB
ADC6   -  A6
ADC7   -  A7
*************************************************************************************************
*/

// Motor / Timer 2
#if defined(MOTOR_T2)
  #define PIN_T2_FWD  3
  #define PIN_T2_REW  11
#endif
// Motor / Timer 1
#if defined(MOTOR_T1)
  #define PIN_T1_FWD  9
  #define PIN_T1_REW  10
#endif
// Motor / Timer 4 (pouze ATmega328PB)
#if defined(MOTOR_T4)
  #define PIN_T4_FWD  1
  #define PIN_T4_REW  2
#endif

// LED alarm (pokud není definován vlastní pin v profilu modelu)
#ifndef PIN_LED
  #define PIN_LED         A5
#endif

// Vstup 1 měřené baterie (pokud není definován vlastní pin v profilu modelu)
#ifndef PIN_BATTERY_A1
  #define PIN_BATTERY_A1    A6
#endif

// Vstup 2 měřené baterie (pokud je definována v profilu modelu)
#if defined(BATTERY_A2)
  #ifndef PIN_BATTERY_A2 // Pokud není definován vlastní pin v profilu modelu
    #define PIN_BATTERY_A2  A7
  #endif
#endif

// Pins for nRF24L01+
#define PIN_CE       A0
#define PIN_CSN      A1

// Software SPI (Provázáno s knihovnou DigitalIO.h a SOFTSPI v RF24_config.h) https://nrf24.github.io/RF24/md_docs_2arduino.html
//          SCK      A2
//          MOSI     A3
//          MISO     A4

#endif // Konec __Pins_h__
