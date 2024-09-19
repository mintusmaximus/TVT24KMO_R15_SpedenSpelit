#include "buttons.h"

void initButtonsAndButtonInterrupts(void)
{
  PCICR |= 0b00000100; //enabloi portti D auki (PCINT16-23), "or" bitwisellä kun haluan testata
  PCMSK2 |= 0b01111100; //pinnit PCINT18-22 enabloitu interruptille
  pinMode(2, INPUT_PULLUP); //pinmodet säätää digitalpinnille XXX
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  digitalWrite(2, HIGH); //pinni puskee sähköä 
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);

  
  // See requirements for this function from buttons.h
}

ISR(PCINT2_vect) //funktio pineille D0-D7
 {
   unsigned long = millis();


  if (digitalRead(2) == LOW && aika > merkattuaika + kynnysaika)
  {   
   Serial.println("2");  //poista
   merkattuaika = aika;  
  }
  if (digitalRead(3) == LOW && aika > merkattuaika + kynnysaika)
  {
   merkattuaika = aika;
   Serial.println("3");  //poista 
  }
  if (digitalRead(4) == LOW && aika > merkattuaika + kynnysaika)
  {
   merkattuaika = aika;
   Serial.println("4");  //poista      
  } 
  if (digitalRead(5) == LOW && aika > merkattuaika + kynnysaika)
  {   
   merkattuaika = aika;
   Serial.println("5");  //poista      
  }
  if (digitalRead(6) == LOW && aika > merkattuaika + kynnysaika)
  {  
   merkattuaika = aika;
   Serial.println("6");  //poista   
  }
  /*
     Here you implement logic for handling
	 interrupts from 2,3,4,5 pins for Game push buttons
	 and for pin 6 for start Game push button.
   */
}