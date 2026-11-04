/*
*********************************************************************************************
RC receiver with FHSS
*********************************************************************************************
RC receiver from my repository https://github.com/stanekTM/RX_nRF24_Stanek

Optimized for RC transmitters:
DIY-Multiprotocol-TX-Module from my fork https://github.com/stanekTM/TX_Multiprotocol_Stanek
OpenAVRc          https://github.com/Ingwie/OpenAVRc_Dev
TX_nRF24_2ch_OLED https://github.com/stanekTM/TX_nRF24_FHSS_2ch_OLED
TX_nRF24_Xch_LED  https://github.com/stanekTM/TX_nRF24_FHSS_Xch_LED
*********************************************************************************************
*/

#include <RF24.h>      // Ovladač bezdrátového čipu nRF24L01+ (v1.4.11 nebo novější)
#include <DigitalIO.h> // Knihovna pro rychlé softwarové SPI a manipulaci s piny
#include <Servo.h>     // Standardní Arduino knihovna pro generování RC pulzů
#include <EEPROM.h>    // Standardní Arduino knihovna čtení a zápisu permanentní paměti EEPROM
#include <avr/wdt.h>   // Knihovna pro ovládání Watchdog časovače
#include "Config.h"
#include "Pins.h"

RF24 radio(PIN_CE, PIN_CSN); // Inicializace rádiového čipu nRF24L01+

// Unikátní RF adresa max. 5 bajtů (číslo nebo znak). Musí být stejná i ve vysílači
const uint8_t rf_address[] = "jirka";

// 4 silné RF kanály (mimo hlavní WiFi frekvence)
const uint8_t rf_channels[] = {12, 40, 76, 88};

uint8_t fhss_index = 0; // Ukazatel na aktuálně používaný RF kanál z tabulky

// Struktura přijímaných dat
struct rx_packet
{
  uint8_t fail_safe_flag;
  uint16_t rc_data[num_rc_channels];
}
rx_data;

// Struktura odesílaných telemetrických dat (ACK Payload)
struct tx_packet
{
  uint8_t rssi = 0;
  uint8_t batt_A1 = 0;
  uint8_t batt_A2 = 0;
}
tx_telemetry_data;

// Hlídače ztráty synchronizace
const uint32_t SYNC_TIMEOUT = 6000;  // 6 ms bez dat spustí nouzové hledání
const uint32_t EMERGENCY_HOP = 4500; // Nouzové skenování každé 4.5 ms, aby se synchronizoval s krokem vysílače
uint32_t sync_loss_timeout = 0; // Časovač ztráty synchronizace
uint32_t emergency_timer = 0;   // Časovač pro nouzové skenování kanálů

uint32_t signal_loss_timeout = 0; // Hlídač totální ztráty signálu (Fail-Safe)

// RSSI a kvalita spojení
uint32_t rssi_timer = 0;    // Časovač měření RSSI
uint8_t packet_counter = 0; // Počítadlo úspěšně přijatých paketů

// Měření baterií
uint32_t batt_timer = 0; // Časovač měření baterií
bool low_batt_alarm_flag_A1 = false; // Příznak vybití baterie A1
bool low_batt_alarm_flag_A2 = false; // Příznak vybití baterie A2

// Permanentní paměť EEPROM
uint16_t eeprom_fail_safe_data[NUM_SERVOS]; // Pole pro uložení / načtení nouzových poloh serv
const int EEPROM_ADDRESS = 10; // Adresa v paměti EEPROM, kam se konfigurace ukládá

bool fail_safe_written_lock = false; // Zámek chránící procesor před cyklickým přepisováním EEPROM

// Prototypy všech funkcí na začátku, aby je smyčka správně viděla
void get_fail_safe_from_eeprom();
void save_fail_safe_to_eeprom();
void set_fail_safe();
void motor_init();
void motor_control();
void servo_init();
void servo_control();
void radio_init();
void manage_rf_link();
void calculate_rssi_link();
void read_batt();
void blink(const uint8_t pin, uint32_t interval);

//***************************************
// Hlavní konfigurace systému při startu
//***************************************
void setup()
{
  // 1. Okamžité vypnutí Watchdogu po startu (prevence nekonečné smyčky po resetu)
  wdt_disable();
  
  //Serial.begin(9600);
  
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BATTERY_A1, INPUT);
  #if defined(BATTERY_A2)
    pinMode(PIN_BATTERY_A2, INPUT);
  #endif
  
  // Získat nouzové polohy z paměti EEPROM
  get_fail_safe_from_eeprom();
  
  // Inicializace motorů
  motor_init();
  
  // Inicializace serv
  servo_init();
  
  // Inicializace ovladače rádia
  radio_init();
  
  // Inicializace časovačů
  sync_loss_timeout = micros();
  emergency_timer = micros();
  signal_loss_timeout = millis();
  rssi_timer = millis();
  batt_timer = millis();
  
  // 2. Aktivace bezpečnostní pojistky Watchdog na interval 500 ms
  wdt_enable(WDTO_500MS);
}

//**********************************
// Hlavní opakovací smyčka programu
//**********************************
void loop()
{
  // Na začátku každé smyčky musíme "nakrmit psa", jinak se Arduino za 500 ms zresetuje
  wdt_reset();
  
  // Příjem a vysílaní telemetrických dat
  manage_rf_link();
  
  // Měření kvality signálu RSSI
  calculate_rssi_link();
  
  // Uložit nouzové polohy do paměti EEPROM
  save_fail_safe_to_eeprom();
  
  // Měření napětí baterií
  read_batt();
}
