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
  }
}

void initializeTimer(void) {
	// void initializeTimer(void) 
}
ISR(TIMER1_COMPA_vect) {
  /* Timerin keskeytysten käsittelemiseksi.
  Communicate to loop() that it's time to make new random number.
  Increase timer interrupt rate after 10 interrupts.
  */
  
}


void checkGame(byte nbrOfButtonPush) {
	// käyttäjän inputin tarkistamiseen. See requirements for the function from SpedenSpelit.h
}


void initializeGame() {
	// see requirements for the function from SpedenSpelit.h
}

void startTheGame() {
   // pelin käynnistämiseen. See requirements for the function from SpedenSpelit.h
}

void stopTheGame() {
  //
}