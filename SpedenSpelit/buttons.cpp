#include "buttons.h" //lisätään headeri, jossa definet buttonsiin liittyen
unsigned long merkattuaika = 0; //aikakytkin, myöhemmin esillä

void initButtonsAndButtonInterrupts(void)
{
  PCICR |= 0b00000100; //enabloi portti D interr (PCINT16-23(D0-7)) "or" bitwisellä kun haluan testata
  PCMSK2 |= 0b01111100; //pinnit PCINT18-22 (eli D2-6) enabloitu interruptille
  for (int i = 2; i <= 6; i++)
  {
    pinMode(i, INPUT_PULLUP); //pinmodet säätää digitalpinnille vastuksen 
  }
}

ISR(PCINT2_vect) //funktio pineille D0-D7, jossa on aikaraja painalluksille
 {

  const unsigned long kynnysaika = 100; //debounssille kynnysaika jolloin painallusta ei lueta
  unsigned long aika = millis(); //varastoi Arduinon laskema aika, uns. long pakollinen(manuaali)

  if (aika > merkattuaika + kynnysaika) //aikarajoite painalluksille
  {
    uint8_t pinState = PIND & 0b01111100; //selite alimmaisena
    if ((pinState & (1 << PIND2)) == 0) 
    {
      buttonNumber = 2; //TARKISTA NÄÄ INOSTA KU VALMIS
    }
    else if ((pinState & (1 << PIND3)) == 0) 
    {
      buttonNumber = 3;
    }
    else if ((pinState & (1 << PIND4)) == 0) 
    {
      buttonNumber = 4;
    }
    else if ((pinState & (1 << PIND5)) == 0) 
    {
      buttonNumber = 5;
    }
    else if ((pinState & (1 << PIND6)) == 0)
    {
      buttonNumber = 6;
    }
    }
    merkattuaika = aika;
  }

/* toistuva DigitalRead ifien alla on hitaampi tapa käydä funktiossa katsomassa pinnien aktiivisuutta
  joten siirsin haettavat luvut variaabeleihin suoraan. PIND on D-portin rekisteri, joka muuttuu 
  pinnien tilan muuttuessa. pinstateen erotetaan &:lla 0b01111100 (käytetyt portit eli) ne bitit 
  joita halutaan verrata PINDi paikkaa bitwisellä siirrettyyn binääriarvoon. Haetaan 0. Binääreillä 
  tapahtumat ovat havainnollistavia.

ESIMERKKI PINSTATESTA!  
pind6 tuottaa 01000000 kun pin6=6 ja 1<< tekee siitä 0100000
	pindstate   00111100 & = 0, tulostetaan
  (pind       00111101)
	
	
pin5 tuottaa  00100000
	pindstate 	01011100 & = 0, tulostetaan
  (pind 		  01011101)
	

MUTTA: pind 4 tuottaa 00010000       
        jos pindstate 01011100 & != 0, ei tulosteta
        (jos pind     01011101)
  */