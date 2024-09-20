#include "leds.h"

const byte ledPins[] = {A2, A3, A4, A5};
const int ledCount = 4;
// testi yritys
void initializeLeds()
{

for (byte i = 0; i < 4; i++){
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
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
// see requirements for this function from leds.h
}

void show2(int rounds)
{
// see requirements for this function from leds.h  
}