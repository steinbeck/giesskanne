#include <SPI.h>
#include <LoRa.h>
#include <U8x8lib.h>
#include "Ticker.h" // This is https://github.com/sstaub/Ticker


#define ss 18
#define rst 14
#define dio0 26

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(15, 4, 16);

//# Unusable Pins on this device due to LoRa and OLED modules
//# Used by LoRa:
//# SCK  5
//# MISO  19
//# MOSI  27
//# CS  18
//# RST 14
//# DIO0  26
//# Used by OLED Display > ESP32 (I2C)
//# SCL 15
//# SDA 4
//# RST 16

// Set number of relays
#define NUM_ZONES  6

int  num_minutes=1;

// Assign each GPIO to a relay
int relayGPIOs[NUM_ZONES] = {33, 22, 23, 2, 32, 17};
const int plusbutton = 36;
const int minusbutton = 37;
const int triggerbutton = 39;

int minusbuttonState = 0;
int plusbuttonState = 0;
int triggerbuttonState = 0;

boolean one_run_done = false;
boolean paused = true;
boolean start = false;
int zone = 0;
long lastmillis = 0;
long currentmillis = 0;
long irrigationtime = num_minutes * 60000; //this should be 60000 for minutes. 

void updateTimeLeft();
Ticker updateTimer(updateTimeLeft, 1000, 0, MILLIS);

void setup() {
  Serial.begin(115200);

   /* 
   * Initialize OLED display and print App name plus version
   */
  u8x8.begin(); 
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setFlipMode(1);
  u8x8.drawString(0, 0, "Giesskanne 1.0");
  /* 
   * Set all GPIO triggering relays to OUTPUT  
   */

  pinMode(plusbutton, INPUT);
  pinMode(minusbutton, INPUT);
  pinMode(triggerbutton, INPUT);
  /* 
   * Set all GPIO triggering relays to OUTPUT  
   */
  for(int i = 0; i < NUM_ZONES; i++){
    pinMode(relayGPIOs[i], OUTPUT);
    digitalWrite(relayGPIOs[i], HIGH);
  }
  u8x8.setCursor(0, 3);
  u8x8.print("IrrigT = ");
  u8x8.print(num_minutes);
  u8x8.print(" min");
  start = true;  
  updateTimer.start();
}

void loop() {
  irrigationtime = num_minutes * 60000; //this should be 60000 for minutes. 
  updateTimer.update();

  processButtons();

  if (paused) 
  {
    u8x8.setCursor(0, 1);
    u8x8.print("Paused     ");
  }
  if (!paused)
  {
    u8x8.setCursor(0, 1);
    u8x8.print("Running    ");
  }
  
  if (!one_run_done && !paused)
  {
    currentmillis = millis();
    
    if ((currentmillis - lastmillis) > irrigationtime || start)
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
      start = false;
      if (zone == NUM_ZONES + 1) 
      {
        one_run_done = true;
        paused = true;
        u8x8.setCursor(0, 5);
        u8x8.print("Finished!");
        Serial.println("Irrigation schedule finished");
      }
    }
  }
}  


  void processButtons()
  {
    plusbuttonState = digitalRead(plusbutton);
    minusbuttonState = digitalRead(minusbutton);
    triggerbuttonState = digitalRead(triggerbutton);
    if (plusbuttonState == 1) 
    {
      num_minutes ++;
      irrigationTimeChanged();
      delay(200);
      return;
    }
    if (minusbuttonState == 1  && num_minutes > 1){
      num_minutes --; 
      irrigationTimeChanged();  
      delay(200);
      return;
    }
    if (triggerbuttonState == 1) 
    {
      if (paused) paused = false;
      else paused = true;
      delay(200);
      return;
    }
  }

  void updateTimeLeft()
  {
    if (paused) return;
    u8x8.setCursor(0, 4);
    u8x8.print(((currentmillis - lastmillis))/60000);
    u8x8.print("/");
    u8x8.print(num_minutes);
    u8x8.print("min in Z");
    u8x8.print(zone);
  }    

  void irrigationTimeChanged()
  {
    u8x8.setCursor(0, 3);
    u8x8.print("IrrigT = ");
    u8x8.print(num_minutes);
    u8x8.print(" min");
  }

  
