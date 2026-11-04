//*******************
// Inicializace serv
//*******************
void servo_init()
{
  #if defined(SERVO)
    for (uint8_t i = 0; i < NUM_SERVOS; i++)
    {
      servo[i].attach(pins_servo[i]);
      servo[i].writeMicroseconds(eeprom_fail_safe_data[i]);
    }
  #endif
}

//*******************
// Řízení serv
//*******************
void servo_control()
{
  #if defined(SERVO)
    // Index [NUM_MOTORS + i] posouvá čtení dat až za kanály obsazené motory
    for (uint8_t i = 0; i < NUM_SERVOS; i++)
    {
      servo[i].writeMicroseconds(rx_data.rc_data[NUM_MOTORS + i]);
      
      //Serial.printf("%d-%d\n", i, rx_data.rc_data[NUM_MOTORS + i]);
    }
  #endif
}
