#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "sound.h"
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

  // Arvotut numerot talletetaan 100 alkion mittaiseen taulukkoon (randomNumbers)
  int randomNumbers[100]; 
  // Kun käyttäjä painaa kytkimiä 0,1,2,3 nämä näppäinten painallukset talletetaan.
  int userNumbers[100];
  // Indeksi, joka seuraa missä kohtaa taulukkoa mennään
  byte nbrOfButtonPush = 0; //byte-tyyppi, koska arvo ei ylitä 255 ja käyttää vähemmän muistia
}

void loop() {
  if(buttonNumber>=0) {
     if(buttonNumber==4) { // start the game if buttonNumber == 4
      startTheGame();
     }

     if(buttonNumber >= 0 && buttonNumber < 4) { // check the game if 0<=buttonNumber<4
      userNumbers[nbrOfButtonPush] = buttonNumber; // Tallentaa käyttäjän painaman numeron taulukkoon
      nbrOfButtonPush++;
      checkGame();
     }
  }

  if(newTimerInterrupt == true) {
     // new random number must be generated
    int myRand = random(1,5);
    // Printing the output
    Serial.println(myRand);
     // and corresponding led must be activated
     ledmuuttuja = myRand; // ledmuuttuja = random(1,5); oli tässä aluksi, mutta se arpoisi vain 
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
  /*
  checkGame() subroutine is used to check the status
  of the Game after each player button press.
  
  If the latest player button press is wrong, the game stops
  and if the latest press was right, game display is incremented
  by 1.
  
  Parameters
  byte lastButtonPress of the player 0 or 1 or 2 or 3
*/
}


void initializeGame() {
	/*
  initializeGame() subroutine is used to initialize all variables
  needed to store random numbers and player button push data.
  This function is called from startTheGame() function.
*/
}

void startTheGame() { // void startTheGame() kutsuu initializeGame() funktiota ja enabloi Timer1 keskeytykset käynnistääkseen pelin
  initializeGame(); // Kutsutaan initializeGame()-funktiota
  
   // enabloi Timer1 keskeytykset käynnistääkseen pelin
}

void stopTheGame() {
  // StopTheGame funktio disabloi Timer1 keskeytykset 
}