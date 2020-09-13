# giesskanne

## Introduction
This is ESP32 code for a garden watering/irrigation system. Giesskanne is the German word for watering can or watering pot. This code requires an ESP32 as well as a relay module (mine has 8 relays). Each relay is switching one valve responsible for a garden zone to be watered. I currently have 6 zones. 
Each zone is watered for a configurable time, then the valve is switched off and the next zone is enabled. 

## Future Plans
Move to an ESP32 LoraWAN moduleso that the current state and progress can be conveniently monitored from my home automation system. Since the ESP32 controler and the valves are in the center of a relatively large garden, WLAN turned out not to work. 
