/* Jarmo : Mun mielestä nyt riittää starttiääni, satunnaisgeneraattorilla valitun painettavan
napin "käynnistysääni", ja game over.*/

#include "sound.h"
#include "pitches.h"

const int buzzerPin = 7; // jos rakennetaan projektisuunnitelman mukaan, pinni 7 pitäisi olla vapaa
int startsound[] = {
  NOTE_F4, NOTE_A4, NOTE_F5, NOTE_E5, NOTE_F5, NOTE_F6, NOTE_F7
};


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
    for (int i = 0; i < 8; i++){
        tone(buzzerpin, startsound[i], 200)
        delay(300);
        noTone(buzzerpin);
    }
    
}
void playButtonOneSelected(void)
{
    tone(buzzerpin, NOTE_F5, 150)
    delay(150);
    noTone(buzzerpin);
}

void playButtonTwoSelected(void)
{
    tone(buzzerpin, NOTE_D5, 150)
    delay(150);
    noTone(buzzerpin);
}

void playButtonThreeSelected(void)
{
    tone(buzzerpin, NOTE_B4, 150)
    delay(150);
    noTone(buzzerpin);
}

void playButtonFourSelected(void)
{
    tone(buzzerpin, NOTE_G4, 150)
    delay(150);
    noTone(buzzerpin);
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


