#include "sound.h"
#include "pitches.h"

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
    Serial.println("SOUND: Playing wrong answer");
    for (int i = 1000; i >= 500; i -= 500){ // Play a descending tone from 1000 to 500 Hz 
        tone(buzzerPin, i, 200); // Play the tone for 200 ms 
        delay(100); // Wait for 100 ms 
    }
}


