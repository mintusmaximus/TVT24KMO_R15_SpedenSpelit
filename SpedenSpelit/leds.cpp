#include "leds.h"

const byte ledPins[] = {A2, A3, A4, A5}; //Analogi pinnien alustukseen
const int ledCount = 4; // käytössä 4 lediä
const int ledFlashDuration = 350; // ledin välähdysaika

void initializeLeds()
{
  for (byte i = 0; i < 4; i++){
    pinMode(ledPins[i], OUTPUT); //Analogi pinnien alustus
    digitalWrite(ledPins[i], LOW); //Ledit pois päältä
}
}

void setLed(byte ledNumber)
{
  if (ledNumber < 5) {
    for (byte i = 0; i < 4; i++){
        digitalWrite(ledPins[i], LOW);
    }
}
  digitalWrite (ledPins[(ledNumber -1)], HIGH);
  delay(ledFlashDuration);
  digitalWrite (ledPins[(ledNumber -1)], LOW);
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
  int maxNopeus = 100;
  int minNopeus = 800;
  int nopeudenMuutos = 50;

  int nopeus = minNopeus;

 for(int k = -1; k < rounds; k++){
  for(int i = -1; i < 4; i++){
    for (int j = 0; j <= i; j++) {
        digitalWrite(ledPins[j], HIGH);
    }
    delay(nopeus);

    for (int j = -1; j <= i; j++) {
        digitalWrite(ledPins[j], LOW);
  }
  if(nopeus > maxNopeus){
    nopeus -= nopeudenMuutos;
  }
 }
}
}

void ledEndGame()
{
  setAllLeds();
  delay(500);
  clearAllLeds();
  delay(500);

}