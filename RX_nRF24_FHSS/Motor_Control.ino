#include "PWM.h"

// Globální proměnné pro přepočet limitů motoru z %
#if defined(MOTOR_T2)
  uint8_t reaction_motor_t2;
  uint8_t max_fwd_motor_t2;
  uint8_t max_rev_motor_t2;
  uint8_t brake_motor_t2;
#endif
#if defined(MOTOR_T1)
  uint8_t reaction_motor_t1;
  uint8_t max_fwd_motor_t1;
  uint8_t max_rev_motor_t1;
  uint8_t brake_motor_t1;
#endif
#if defined(MOTOR_T4)
  uint8_t reaction_motor_t4;
  uint8_t max_fwd_motor_t4;
  uint8_t max_rev_motor_t4;
  uint8_t brake_motor_t4;
#endif

//*********************
// Inicializace motorů
//*********************
void motor_init()
{
  // Motor / Timer 2
  #if defined(MOTOR_T2)
    // Inicializace - motor stojí
    digitalWrite(PIN_T2_FWD, LOW);
    digitalWrite(PIN_T2_REW, LOW);
    
    // Vynulování registrů plnění (piny 11 a 3)
    OCR2A = 0;
    OCR2B = 0;
    
    // Nastavení předděliče časovače
    set_PWM_prescaler(PIN_T2_FWD, PRESCALER_TIMER2_3_11);
    
    // Propojení pinů s časovačem na 0
    analogWrite(PIN_T2_FWD, 0);
    analogWrite(PIN_T2_REW, 0);
    
    // Nakonec přepneme piny do režimu OUTPUT
    pinMode(PIN_T2_FWD, OUTPUT);
    pinMode(PIN_T2_REW, OUTPUT);
    
    // Automatický přepočet z % na rozsah 0 až 255
    reaction_motor_t2 = (uint8_t)(REACTION_MOTOR_T2 * 2.55);
    max_fwd_motor_t2  = (uint8_t)(MAX_FWD_MOTOR_T2 * 2.55);
    max_rev_motor_t2  = (uint8_t)(MAX_REV_MOTOR_T2 * 2.55);
    brake_motor_t2    = (uint8_t)(BRAKE_MOTOR_T2 * 2.55);
  #endif // Konec MOTOR_T2
  
  // Motor / Timer 1
  #if defined(MOTOR_T1)
    // Inicializace - motor stojí
    digitalWrite(PIN_T1_FWD, LOW);
    digitalWrite(PIN_T1_REW, LOW);
    
    // Vynulování registrů plnění (piny 9 a 10)
    OCR1A = 0;
    OCR1B = 0;
    
    // Nastavení předděliče časovače
    set_PWM_prescaler(PIN_T1_FWD, PRESCALER_TIMER1_9_10);
    
    // Propojení pinů s časovačem na 0
    analogWrite(PIN_T1_FWD, 0);
    analogWrite(PIN_T1_REW, 0);
    
    // Nakonec přepneme piny do režimu OUTPUT
    pinMode(PIN_T1_FWD, OUTPUT);
    pinMode(PIN_T1_REW, OUTPUT);
    
    // Automatický přepočet z % na rozsah 0 až 255
    reaction_motor_t1 = (uint8_t)(REACTION_MOTOR_T1 * 2.55);
    max_fwd_motor_t1  = (uint8_t)(MAX_FWD_MOTOR_T1 * 2.55);
    max_rev_motor_t1  = (uint8_t)(MAX_REV_MOTOR_T1 * 2.55);
    brake_motor_t1    = (uint8_t)(BRAKE_MOTOR_T1 * 2.55);
  #endif // Konec MOTOR_T1
  
  // Motor / Timer 4 (pouze ATmega328PB)
  #if defined(MOTOR_T4)
    // Inicializace - motor stojí
    digitalWrite(PIN_T4_FWD, LOW);
    digitalWrite(PIN_T4_REW, LOW);
    
    // Vynulování registrů plnění (piny 1 a 2)
    OCR4A = 0;
    OCR4B = 0;
    
    // Nastavení předděliče časovače
    set_PWM_prescaler(PIN_T4_FWD, PRESCALER_TIMER4_1_2);
    
    // Propojení pinů s časovačem na 0
    analogWrite(PIN_T4_FWD, 0);
    analogWrite(PIN_T4_REW, 0);
    
    // Nakonec přepneme piny do režimu OUTPUT
    pinMode(PIN_T4_FWD, OUTPUT);
    pinMode(PIN_T4_REW, OUTPUT);
    
    // Automatický přepočet z % na rozsah 0 až 255
    reaction_motor_t4 = (uint8_t)(REACTION_MOTOR_T4 * 2.55);
    max_fwd_motor_t4  = (uint8_t)(MAX_FWD_MOTOR_T4 * 2.55);
    max_rev_motor_t4  = (uint8_t)(MAX_REV_MOTOR_T4 * 2.55);
    brake_motor_t4    = (uint8_t)(BRAKE_MOTOR_T4 * 2.55);
  #endif // Konec MOTOR_T4
}

//*******************
// Řízení motorů
//*******************
void motor_control()
{
  // Deklarace cílových proměnných na začátku, aby byly dostupné všem blokům
  #if defined(MOTOR_T2)
    int16_t target_motor_t2 = MID_CONTROL_VAL;
  #endif
  #if defined(MOTOR_T1)
    int16_t target_motor_t1 = MID_CONTROL_VAL;
  #endif
  #if defined(MOTOR_T4)
    int16_t target_motor_t4 = MID_CONTROL_VAL;
  #endif
  
  //************************
  // Tankový / Arkádový mix
  //************************
  #if defined(MIX_TANK)
    // Převod vstupů na bezpečné odchylky se znaménkem (-500 až +500) proti podtečení RAM.
    // Od hodnoty (1000 až 2000) odečteme střed 1500. Získáme kladné nebo záporné číslo
    int16_t throttle = (int16_t)rx_data.rc_data[0] - MID_CONTROL_VAL;     // Kanál 0 = plyn (vpřed / vzad)
    int16_t steering = (int16_t)rx_data.rc_data[1] - MID_CONTROL_VAL; // Kanál 1 = řízení (doleva / doprava)
    
    // Výpočet arkádového mixu pro levý a pravý motor pásu tanku (-500 až +500).
    // Při zatáčení doprava se levému pásu přidá a pravému ubere, což způsobí otočení modelu
    int16_t left_mix = throttle + steering;
    int16_t right_mix = throttle - steering;
    
    // Převod relativních odchylek zpět na standardní RC rozsah (1000 až 2000 us)
    // Výběr jediné aktivní dvojice motorů pro tankový mix
    #if defined(MOTOR_T2) && defined(MOTOR_T1)
      target_motor_t2 = MID_CONTROL_VAL + left_mix;
      target_motor_t1 = MID_CONTROL_VAL + right_mix;
    #elif defined(MOTOR_T2) && defined(MOTOR_T4)
      target_motor_t2 = MID_CONTROL_VAL + left_mix;
      target_motor_t4 = MID_CONTROL_VAL + right_mix;
    #elif defined(MOTOR_T1) && defined(MOTOR_T4)
      target_motor_t1 = MID_CONTROL_VAL + left_mix;
      target_motor_t4 = MID_CONTROL_VAL + right_mix;
    #endif
  // Normální ovládání bez mixu (pouze jedna dvojice motorů nebo jen jeden motor)
  #else
    #if defined(MOTOR_T2) && defined(MOTOR_T1)
      target_motor_t2 = rx_data.rc_data[0]; // Kanál 0 = plyn (vpřed / vzad)
      target_motor_t1 = rx_data.rc_data[1]; // Kanál 1 = řízení (doleva / doprava)
    #elif defined(MOTOR_T2) && defined(MOTOR_T4)
      target_motor_t2 = rx_data.rc_data[0];
      target_motor_t4 = rx_data.rc_data[1];
    #elif defined(MOTOR_T1) && defined(MOTOR_T4)
      target_motor_t1 = rx_data.rc_data[0];
      target_motor_t4 = rx_data.rc_data[1];
    // Tato sekce se spustí, pouze pokud model nemá žádnou dvojici motorů (jednomotorové modely)
    #else
      #if defined(MOTOR_T2)
        target_motor_t2 = rx_data.rc_data[0]; // Kanál 0 = plyn (vpřed / vzad)
      #endif
      #if defined(MOTOR_T1)
        target_motor_t1 = rx_data.rc_data[0];
      #endif
      #if defined(MOTOR_T4)
        target_motor_t4 = rx_data.rc_data[0];
      #endif
    #endif
  #endif // Konec MIX_TANK
  
  //********************
  // Motor / Timer 2
  //********************
  #if defined(MOTOR_T2)
    int16_t val_motor_t2 = 0;
    
    // Jízda vpřed
    if (target_motor_t2 > MID_CONTROL_VAL + DEAD_ZONE)
    {
      val_motor_t2 = map(target_motor_t2, MID_CONTROL_VAL + DEAD_ZONE, MAX_CONTROL_VAL, reaction_motor_t2, max_fwd_motor_t2);
      val_motor_t2 = constrain(val_motor_t2, reaction_motor_t2, max_fwd_motor_t2);
      analogWrite(PIN_T2_FWD, val_motor_t2);
      digitalWrite(PIN_T2_REW, LOW);
    }
    // Jízda vzad
    else if (target_motor_t2 < MID_CONTROL_VAL - DEAD_ZONE)
    {
      val_motor_t2 = map(target_motor_t2, MID_CONTROL_VAL - DEAD_ZONE, MIN_CONTROL_VAL, reaction_motor_t2, max_rev_motor_t2);
      val_motor_t2 = constrain(val_motor_t2, reaction_motor_t2, max_rev_motor_t2);
      analogWrite(PIN_T2_REW, val_motor_t2);
      digitalWrite(PIN_T2_FWD, LOW);
    }
    // Volnoběh / Brzda
    else
    {
      analogWrite(PIN_T2_FWD, brake_motor_t2);
      analogWrite(PIN_T2_REW, brake_motor_t2);
    }
  #endif // Konec MOTOR_T2
  
  //********************
  // Motor / Timer 1
  //********************
  #if defined(MOTOR_T1)
    int16_t val_motor_t1 = 0;
    
    // Jízda vpřed
    if (target_motor_t1 > MID_CONTROL_VAL + DEAD_ZONE)
    {
      val_motor_t1 = map(target_motor_t1, MID_CONTROL_VAL + DEAD_ZONE, MAX_CONTROL_VAL, reaction_motor_t1, max_fwd_motor_t1);
      val_motor_t1 = constrain(val_motor_t1, reaction_motor_t1, max_fwd_motor_t1);
      analogWrite(PIN_T1_FWD, val_motor_t1);
      digitalWrite(PIN_T1_REW, LOW);
    }
    // Jízda vzad
    else if (target_motor_t1 < MID_CONTROL_VAL - DEAD_ZONE)
    {
      val_motor_t1 = map(target_motor_t1, MID_CONTROL_VAL - DEAD_ZONE, MIN_CONTROL_VAL, reaction_motor_t1, max_rev_motor_t1);
      val_motor_t1 = constrain(val_motor_t1, reaction_motor_t1, max_rev_motor_t1);
      analogWrite(PIN_T1_REW, val_motor_t1);
      digitalWrite(PIN_T1_FWD, LOW);
    }
    // Volnoběh / Brzda
    else
    {
      analogWrite(PIN_T1_FWD, brake_motor_t1);
      analogWrite(PIN_T1_REW, brake_motor_t1);
    }
  #endif // Konec MOTOR_T1
  
  //*************************************
  // Motor / Timer 4 (pouze ATmega328PB)
  //*************************************
  #if defined(MOTOR_T4)
    int16_t val_motor_t4 = 0;
    
    // Jízda vpřed
    if (target_motor_t4 > MID_CONTROL_VAL + DEAD_ZONE)
    {
      val_motor_t4 = map(target_motor_t4, MID_CONTROL_VAL + DEAD_ZONE, MAX_CONTROL_VAL, reaction_motor_t4, max_fwd_motor_t4);
      val_motor_t4 = constrain(val_motor_t4, reaction_motor_t4, max_fwd_motor_t4);
      analogWrite(PIN_T4_FWD, val_motor_t4);
      digitalWrite(PIN_T4_REW, LOW);
    }
    // Jízda vzad
    else if (target_motor_t4 < MID_CONTROL_VAL - DEAD_ZONE)
    {
      val_motor_t4 = map(target_motor_t4, MID_CONTROL_VAL - DEAD_ZONE, MIN_CONTROL_VAL, reaction_motor_t4, max_rev_motor_t4);
      val_motor_t4 = constrain(val_motor_t4, reaction_motor_t4, max_rev_motor_t4);
      analogWrite(PIN_T4_REW, val_motor_t4);
      digitalWrite(PIN_T4_FWD, LOW);
    }
    // Volnoběh / Brzda
    else
    {
      analogWrite(PIN_T4_FWD, brake_motor_t4);
      analogWrite(PIN_T4_REW, brake_motor_t4);
    }
  #endif // Konec MOTOR_T4
}
