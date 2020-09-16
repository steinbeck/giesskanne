#include <SPI.h>
#include <LoRa.h>
#include <U8x8lib.h>

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

int irrigationTime = 30;

// Set number of relays
#define NUM_ZONES  6
#define NUM_MINUTES 3

// Assign each GPIO to a relay
int relayGPIOs[NUM_ZONES] = {33, 22, 23, 2, 32, 17};
const int plusbutton = 36;
const int minusbutton = 37;
const int triggerbutton = 38;

int minusbuttonState = 0;
int plusbuttonState = 0;
int triggerbuttonState = 0;

boolean one_run_done = false;
boolean irrigationTimeChanged = false;
boolean paused = true;
int zone = 0;
long lastmillis = 0;
long currentmillis = 0;
long irrigationtime = NUM_MINUTES * 1000; //this should be 60000 for minutes. 

void setup() {
  Serial.begin(115200);
  u8x8.begin(); 
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setFlipMode(1);
  u8x8.drawString(0, 1, "Giesskanne 1.0");
  pinMode(36, INPUT);
  pinMode(37, INPUT);
 
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

//  Serial.print("Plus Botton: ");
//  Serial.println(plusbuttonState);
//  Serial.print("Minus Botton: ");
//  Serial.println(minusbuttonState);

  processButtons();
  if (irrigationTimeChanged) 
  {
    u8x8.setCursor(0, 4);
    u8x8.print("T = ");
    u8x8.print(irrigationTime);
    u8x8.print(" min");
    irrigationTimeChanged = false;
    
  }

  if (paused) 
  {
    u8x8.setCursor(0, 16);
    u8x8.print("Paused");
  }
  else
  {
    u8x8.setCursor(0, 16);
    u8x8.print("Running");
    
  }
  
  if (!one_run_done && !paused)
  {
    currentmillis = millis();
    if (currentmillis%1000 == 0)
    { 
      //Serial.println(currentmillis - lastmillis);
      //Serial.println(irrigationtime);
    }
    if ((currentmillis - lastmillis)%60000 == 0)
    { 
          u8x8.setCursor(0, 50);
          u8x8.print((currentmillis - lastmillis)/60000);
          u8x8.print(" m left in z ");
          u8x8.print(zone);
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


  void processButtons()
  {
    plusbuttonState = digitalRead(plusbutton);
    //Serial.println(plusbuttonState);
    minusbuttonState = digitalRead(minusbutton);
    //Serial.println(minusbuttonState);
    triggerbuttonState = digitalRead(triggerbutton);
    Serial.println(triggerbuttonState);
    if (plusbuttonState == 1) 
    {
      irrigationTime ++;
      irrigationTimeChanged = true;
      delay(200);
      return;
    }
    if (minusbuttonState == 1  && irrigationTime > 1){
      irrigationTime --; 
      irrigationTimeChanged = true;  
      return;
    }
    if (triggerbuttonState == 1) 
    {
      
      if (paused) paused = false;
      if (!paused) paused = true;
      delay(200);
      return;
    }
    
    
  }
