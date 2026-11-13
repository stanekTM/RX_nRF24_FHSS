//*****************************
// Inicializace ovladače rádia
//*****************************
void radio_init()
{
  radio.begin();
  radio.setPALevel(RF24_PA_MIN); // Pro testy na stole stačí minimální výkon
  radio.setDataRate(RF24_1MBPS);
  
  radio.setAutoAck(true); // Zapnutí automatického hardwarového potvrzování příjmu paketů
  radio.setPayloadSize(sizeof(rx_data)); // Nastavuje základní / maximální délku vaší letové struktury
  
  radio.enableDynamicPayloads(); // Povolení automatického přizpůsobení délky paketů
  radio.enableAckPayload(); // Zapnutí funkce přibalování telemetrie do potvrzovacích ACK paketů
  
  radio.setCRCLength(RF24_CRC_16);
  
  radio.openReadingPipe(1, rf_address); // Otevíráme Pipe 1 pro příjem dat (sjednoceno s parametrem writeAckPayload)
  
  radio.setChannel(rf_channels[fhss_index]); // Nastavení startovního RF kanálu z FHSS tabulky
  
  radio.writeAckPayload(1, &tx_telemetry_data, sizeof(tx_telemetry_data)); // Přednabití telemetrie na Pipe 1
  radio.startListening(); // Přepnutí do režimu příjmu
}

//**************************************
// Příjem a vysílaní telemetrických dat
//**************************************
void manage_rf_link()
{
  // SCÉNÁŘ A: Data v pořádku dorazila
  if (radio.available())
  {
    // Stahujeme ze vzduchu přesnou reálnou délku paketu (data se do struktury rx_data uloží bez posunu)
    radio.read(&rx_data, radio.getDynamicPayloadSize());
    
    packet_counter++; // Započítáme úspěšný paket do RSSI
    
    // Reset obou hlídačů ztráty signálu na aktuální čas
    sync_loss_timeout = micros(); 
    signal_loss_timeout = millis();
    
    // Okamžitá aktualizace motorů
    motor_control();
    
    // Okamžitá aktualizace serv
    servo_control();
    
    // Příprava telemetrie pro další paket na Pipe 1 (sjednoceno s openReadingPipe)
    radio.writeAckPayload(1, &tx_telemetry_data, sizeof(tx_telemetry_data));
    
    // Synchronní FHSS krok na další RF kanál v tabulce
    fhss_index = (fhss_index + 1) % 4; // Posun indexu RF kanálu
    radio.setChannel(rf_channels[fhss_index]); // Nastavení RF kanálu na novou frekvenci
    
    radio.startListening(); // Zapne příjem na novém FHSS kanálu čistě a bez parametrů
  }
  // SCÉNÁŘ B: Ztráta synchronizace (uplynulo více než 6 ms od posledního úspěšného paketu)
  else if (micros() - sync_loss_timeout > SYNC_TIMEOUT)
  {
    // Rychlé nouzové asynchronní skenování RF kanálů každé 4.5 ms, aby se synchronizoval s krokem vysílače
    if (micros() - emergency_timer > EMERGENCY_HOP)
    {
      emergency_timer = micros(); // Reset časovače
      
      // Rychle skáčeme na další RF kanál za účelem synchronizace vysílače
      fhss_index = (fhss_index + 1) % 4; // Posun indexu RF kanálu
      radio.setChannel(rf_channels[fhss_index]); // Nastavení RF kanálu na novou frekvenci
      
      radio.startListening(); // Zapne nouzový příjem čistě a bez parametrů
    }
  }
  
  // SCÉNÁŘ C: Aktivace Fail-Safe (uplynulo více než 1000 ms (1 vteřina) bez dat)
  if (millis() - signal_loss_timeout > 1000)
  {
    set_fail_safe(); // Nastavit nouzové polohy z paměti EEPROM
    
    tx_telemetry_data.rssi = 0; // Při totálním výpadku signálu vynutíme nulu
    
    blink(PIN_LED, 100); // Krizový stav: LED bliká rychle (0.1 s)
  }
  // Vybitá baterie A1 nebo A2: LED bliká (0.3 s)
  else if (low_batt_flag_A1 || low_batt_flag_A2) blink(PIN_LED, 300);
  // Ukládání Fail-Safe: LED bliká (0.5 s)
  else if (rx_data.fail_safe_flag == 1) blink(PIN_LED, 500);
  // Normální provoz: LED trvale svítí
  else digitalWrite(PIN_LED, HIGH);
}

//*****************************
// Měření kvality signálu RSSI
//*****************************
void calculate_rssi_link()
{
  // Každých 650 ms (okno pro 100 očekávaných paketů)
  if (millis() - rssi_timer >= 650)
  {
    rssi_timer = millis(); // Reset časovače
    
    // Pojistka proti přetečení
    if (packet_counter > 100) packet_counter = 100;
    
    // Počet paketů (0-100) je přímo výsledné procento
    tx_telemetry_data.rssi = packet_counter; // Zápis RSSI do telemetrie zpětné vazby
    
    packet_counter = 0; // Vynulování počítadla úspěšně přijatých paketů
  }
}

//*****************************************************
// Funkce pro blikání LED diody bez zpoždění procesoru
//*****************************************************
void blink(const uint8_t pin, uint32_t interval)
{
  digitalWrite(pin, (millis() / interval) % 2); // Zbytek po dělení časem (millis) určuje stav zapnuto/vypnuto
}
