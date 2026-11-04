//***************************************
// Získat nouzové polohy z paměti EEPROM
//***************************************
void get_fail_safe_from_eeprom()
{
  EEPROM.get(EEPROM_ADDRESS, eeprom_fail_safe_data);
  
  #if defined(SERVO)
    for (uint8_t i = 0; i < NUM_SERVOS; i++)
    {
      // Pokud jsou data v EEPROM prázdná (nový procesor), poškozená nebo mimo standardní RC rozsah 1000 až 2000 us,
      // nastavíme výchozí hodnotu na 1500 us
      if (eeprom_fail_safe_data[i] == 65535 || eeprom_fail_safe_data[i] < MIN_CONTROL_VAL || eeprom_fail_safe_data[i] > MAX_CONTROL_VAL)
      {
        eeprom_fail_safe_data[i] = MID_CONTROL_VAL;
      }
    }
  #endif
}

//*****************************************
// Nastavit nouzové polohy z paměti EEPROM
//*****************************************
void set_fail_safe()
{
  // Motory vždy nastavíme na bezpečnou střední hodnotu 1500 us (volnoběh / brzda)
  #if NUM_MOTORS > 0
    for (uint8_t i = 0; i < NUM_MOTORS; i++)
    {
      rx_data.rc_data[i] = MID_CONTROL_VAL;
      
      //Serial.printf("%d-%d\n", i, rx_data.rc_data[i]);
    }
    
    // Okamžitá aktualizace motorů
    motor_control();
  #endif
  
  #if defined(SERVO)
    for (uint8_t i = 0; i < NUM_SERVOS; i++)
    {
      // Nastavíme nouzové polohy serv z paměti EEPROM
      servo[i].writeMicroseconds(eeprom_fail_safe_data[i]);
      
      //Serial.printf("%d-%d\n", i, eeprom_fail_safe_data[i]);
    }
  #endif
}

//****************************************
// Uložit nouzové polohy do paměti EEPROM
//****************************************
void save_fail_safe_to_eeprom()
{
  #if defined(SERVO)
    // Detekce příznaku Fail-Safe: Uloží aktuální hodnoty rc_data do paměti EEPROM pouze jednou
    if (rx_data.fail_safe_flag == 1)
    {
      if (!fail_safe_written_lock)
      {
        for (uint8_t i = 0; i < NUM_SERVOS; i++)
        {
          // Index [NUM_MOTORS + i] posouvá zápis dat až za kanály obsazené motory
          eeprom_fail_safe_data[i] = rx_data.rc_data[NUM_MOTORS + i];
          
          //Serial.printf("%d-%d\n", i, eeprom_fail_safe_data[i]);
        }
        
        // Zapsat do paměti EEPROM na adresu 10
        EEPROM.put(EEPROM_ADDRESS, eeprom_fail_safe_data);
        
        // Okamžitý reset hlídačů, aby rádio po 13ms spánku neztratilo FHSS krok
        sync_loss_timeout = micros(); 
        signal_loss_timeout = millis();
        
        fail_safe_written_lock = true; // Uzamknout proti opakovanému zápisu!
      }
    }
    else fail_safe_written_lock = false; // Jakmile tlačítko pustíte, zámek se uvolní pro příští použití
  #endif
}
