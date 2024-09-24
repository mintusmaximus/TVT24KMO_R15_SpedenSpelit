#include "leds.h"

const byte ledPins[] = {A2, A3, A4, A5}; //Analogi pinnien alustukseen
const int ledCount = 4; // käytössä 4 lediä
// testi yritys
void initializeLeds()
{

for (byte i = 0; i < 4; i++){
    pinMode(ledPins[i], OUTPUT); //Analogi pinnien alustus
    digitalWrite(ledPins[i], LOW); //Ledit pois päältä
}
}

void setLed(byte ledNumber)
{
if (ledNumber < 4) {
    for (byte i = 0; i < 4; i++){
        digitalWrite(ledPins[i], LOW);
    }
}

digitalWrite (ledPins[ledNumber], HIGH);
}


void clearAllLeds()
{
for(int i = 0; i < ledCount; i++){
  digitalWrite(ledPins[i], LOW);
}
 
}

void setAllLeds()
{
for(int i = 0; i < ledCount; i++){
  digitalWrite(ledPins[i], HIGH);
}
 
}


void show1()
{
 for (int number = 0; number <= 15; number++){
    for(int i = 0; i < ledCount; i++){
      if (number & (1 << i)){
        digitalWrite(ledPins[i], HIGH);

      }
      else{
        digitalWrite(ledPins[i], LOW);
      }
    }
    delay(500);
}
}

void show2(int rounds)
{
// see requirements for this function from leds.h  
}