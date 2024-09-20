#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"

// Use these 2 volatile variables for communicating between
// loop() function and interrupt handlers
volatile int buttonNumber = -1;           // for buttons interrupt handler
volatile bool newTimerInterrupt = false;  // for timer interrupt handler


void setup() {
  Serial.begin(9600);
  initializeDisplay();
  testDisplay(); // display numbers from 0 to 99
  showResult(0); // after test, show 0


  /*
    Initialize here all modules
  */
   // . Arvotut numerot talletetaan 100 alkion mittaiseen taulukkoon (randomNumbers)
  int randomNumbers[100]; 
 // Kun käyttäjä painaa kytkimiä 0,1,2,3 nämä näppäinten painallukset talletetaan.
  int userNumbers[100];
}

void loop() {
  if(buttonNumber>=0) {
     if(buttonNumber==4) { // start the game if buttonNumber == 4
      startTheGame;
     }

     if(0<=buttonNumber<4) { // check the game if 0<=buttonNumber<4
      checkGame;
     }
  }

  if(newTimerInterrupt == true) {
     // new random number must be generated
     // and corresponding led must be activated
    
    // ledmuuttuja = random(1,5); 
  }
}

void initializeTimer(void) {
  cli();
//set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
sei();
	// void initializeTimer(void) 
}
ISR(TIMER1_COMPA_vect) {
  /* Timerin keskeytysten käsittelemiseksi.
  Communicate to loop() that it's time to make new random number.
  Increase timer interrupt rate after 10 interrupts.
  */
  
}


void checkGame(byte nbrOfButtonPush) {
	// käyttäjän inputin tarkistamiseen.  
  /* CheckGame aliohjelma vertaa randomNumbers ja userNumbers taulukoita aina siihen 
  taulukon indeksiin asti, kuinka monta kertaa käyttäjä on näppäimiä pelin alusta asti painanut.*/
}


void initializeGame() {
	// see requirements for the function from SpedenSpelit.h
}

void startTheGame() {
   // pelin käynnistämiseen. See requirements for the function from SpedenSpelit.h 
   // startTheGame enabloi Timer1 keskeytykset
}

void stopTheGame() {
  // StopTheGame funktio disabloi Timer1 keskeytykset 
}