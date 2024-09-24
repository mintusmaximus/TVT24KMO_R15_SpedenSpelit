#include "buttons.h"
unsigned long merkattuaika = 0; //aikakytkin, myöhemmin esillä

void initButtonsAndButtonInterrupts(void)
{
  
  
  PCICR |= 0b00000100; //enabloi portti D auki (PCINT16-23), "or" bitwisellä kun haluan testata
  PCMSK2 |= 0b01111100; //pinnit PCINT18-22 enabloitu interruptille
  pinMode(2, INPUT_PULLUP); //pinmodet säätää digitalpinnille vastuksen kaa
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  digitalWrite(2, HIGH); //pinni puskee sähköä 
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);

}

ISR(PCINT2_vect) //funktio pineille D0-D7, jossa on aikaraja painalluksille
 {

  const unsigned long kynnysaika = 120;
  unsigned long aika = millis();

  if (aika > merkattuaika + kynnysaika) //aikarajoite painalluksille
  {
    if (digitalRead(2) == LOW)
    {
      Serial.println("2");      //Serialprint kaikki pois ku valmis
      buttonNumber = 2;      // SELVITÄ KUULUUKO NÄÄ TÄNNE. INOSTA KOPIOITU    
    }
    if (digitalRead(3) == LOW)
    {
      Serial.println("3");
      buttonNumber = 3;      // SELVITÄ KUULUUKO NÄÄ TÄNNE
    }
    if (digitalRead(4) == LOW)
    {
      Serial.println("4");
      buttonNumber = 4;      // SELVITÄ KUULUUKO NÄÄ TÄNNE
    }
    if (digitalRead(5) == LOW)
    {
      Serial.println("5");
      buttonNumber = 5;      // SELVITÄ KUULUUKO NÄÄ TÄNNE
    }
    if (digitalRead(6) == LOW) // kutonen käynnistää pelin, tärkeä myöh ehkä
    {
      Serial.println("6");
      buttonNumber = 6;      // SELVITÄ KUULUUKO NÄÄ TÄNNE
    }
    merkattuaika = aika;
  }
}