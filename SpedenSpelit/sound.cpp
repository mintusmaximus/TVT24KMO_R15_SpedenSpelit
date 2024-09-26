#include "sound.h"
#include "pitches.h"
#include 

const int buzzerPin = 7; // jos rakennetaan projektisuunnitelman mukaan, pinni 7 pitäisi olla vapaa


// Sound setup for the game
void initializeSound(void)
{
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, LOW);
    Serial.println("Sound initialized");
}


// Sound for the start of the game
void playGameStartSound(void)
{

}


// Sound for the end of the game
void playGameEndSound(void)
{

}


// Sound for a correct answer
void playCorrectSound(void)
{

}


// Sound for a wrong answer
void playWrongSound(void)
{

}


