
// Set number of relays
#define NUM_ZONES  6
#define NUM_MINUTES 1

// Assign each GPIO to a relay
int relayGPIOs[NUM_ZONES] = {16, 17, 18, 19, 21, 22};

boolean one_run_done = false;
int zone = 0;
long lastmillis = 0;
long currentmillis = 0;
long irrigationtime = NUM_MINUTES * 60000; //this should be 60000 for minutes. 

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
