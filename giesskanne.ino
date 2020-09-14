#include <SPI.h>
#include <LoRa.h>

#define ss 18
#define rst 14
#define dio0 26


# Unusable Pins on this device due to LoRa and OLED modules
# Used by LoRa:
# SCK  5
# MISO  19
# MOSI  27
# CS  18
# RST 14
# DIO0  26
# Used by OLED Display > ESP32 (I2C)
# SCL 15
# SDA 4
# RST 16



// Set number of relays
#define NUM_ZONES  6
#define NUM_MINUTES 3

// Assign each GPIO to a relay
int relayGPIOs[NUM_ZONES] = {0, 22, 23, 18, 15, 17};

boolean one_run_done = false;
int zone = 0;
long lastmillis = 0;
long currentmillis = 0;
long irrigationtime = NUM_MINUTES * 1000; //this should be 60000 for minutes. 

void setup() {
  Serial.begin(115200);
  for(int i = 0; i < NUM_ZONES; i++){
    pinMode(relayGPIOs[i], OUTPUT);
    digitalWrite(relayGPIOs[i], HIGH);
  }

  Serial.println("Starting irrigation schedule ....");
  Serial.print("Irrigation time set to ");
  Serial.print(irrigationtime);
  Serial.println(" milliseconds.");

  
  digitalWrite(relayGPIOs[zone], LOW);
  Serial.print("Switched on zone ");
  Serial.println(zone+1);
  zone++;
  lastmillis = millis();
  
}

void loop() {
  
  if (!one_run_done)
  {
    currentmillis = millis();
    if (currentmillis%1000 == 0)
    { 
      //Serial.println(currentmillis - lastmillis);
      //Serial.println(irrigationtime);
    }
    
    if ((currentmillis - lastmillis) > irrigationtime)
    {
      if (zone < NUM_ZONES)
      {
        Serial.println("Timer triggered");
        digitalWrite(relayGPIOs[zone], LOW);
        Serial.print("Switched on zone ");
        Serial.println(zone + 1);
      }
      if (zone > 0)
      {
        digitalWrite(relayGPIOs[zone-1], HIGH);
        Serial.print("Switched off zone ");
        Serial.println(zone); 
      }
      zone++;
      lastmillis = millis();
      if (zone == NUM_ZONES + 1) 
      {
        one_run_done = true;
        Serial.println("Irrigation schedule finished");
      }
    }
  }
}  
