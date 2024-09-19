#include "leds.h"

// testi yritys
void initializeLeds()
{
const byte ledPins[] = {A2, A3, A4, A5};

for (byte i = 0; i < 4; i++){
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
}
}

void setLed(byte ledNumber)
{
// see requirements for this function from leds.h

}


void clearAllLeds()
{
// see requirements for this function from leds.h
 
}

void setAllLeds()
{
// see requirements for this function from leds.h test commit
}


void show1()
{
// see requirements for this function from leds.h
}

void show2(int rounds)
{
// see requirements for this function from leds.h  
}