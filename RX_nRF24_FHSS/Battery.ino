//***********************
// Měření napětí baterií
//***********************
void read_batt()
{
  // Měření napětí: Každých 1000 ms (1 vteřina) funguje jako přirozený filtr proti proudovým špičkám
  if (millis() - batt_timer >= 1000)
  {
    batt_timer = millis(); // Reset časovače baterií
    
    // Měření baterie A1
    tx_telemetry_data.batt_A1 = analogRead(PIN_BATTERY_A1) >> 2; // Převede 10-bit ADC rozsah (0 až 1023) na 8-bit rozsah telemetrie (0 až 255)
    
    // Pokud napětí A1 klesne, nebo byl alarm aktivní už dříve, uzamkneme stav vybité baterie
    if ((tx_telemetry_data.batt_A1 <= (255.0 / BATT_VOLTAGE_A1) * MONITORED_VOLTAGE_A1) || low_batt_flag_A1)
    {
      low_batt_flag_A1 = true;
    }
    
    // Měření baterie A2
    #if defined(BATTERY_A2)
      tx_telemetry_data.batt_A2 = analogRead(PIN_BATTERY_A2) >> 2; // Převede 10-bit ADC rozsah (0 až 1023) na 8-bit rozsah telemetrie (0 až 255)
      
      // Pokud napětí A2 klesne, nebo byl alarm aktivní už dříve, uzamkneme stav vybité baterie
      if ((tx_telemetry_data.batt_A2 <= (255.0 / BATT_VOLTAGE_A2) * MONITORED_VOLTAGE_A2) || low_batt_flag_A2)
      {
        low_batt_flag_A2 = true;
      }
    #endif
  }
}
