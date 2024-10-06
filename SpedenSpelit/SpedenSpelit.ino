#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "sound.h"
#include "SpedenSpelit.h"

// Use these 2 volatile variables for communicating between
// loop() function and interrupt handlers
volatile int buttonNumber = -1;           // for buttons interrupt handler
volatile bool newTimerInterrupt = false;  // for timer interrupt handler
bool gameStarted = false; // Muuttuja pelin tilalle
int score = 0;
extern byte ledNumber = 0;

// Arvotut numerot talletetaan 100 alkion mittaiseen taulukkoon (randomNumbers)
int randomNumbers[100]; 
// Kun käyttäjä painaa kytkimiä 0,1,2,3 nämä näppäinten painallukset talletetaan taulukkoon (userNumbers)
int userNumbers[100];
// Indeksi, joka seuraa missä kohtaa taulukkoa mennään eli monestikko käyttäjä on painanut nappeja
byte nbrOfButtonPush = 0; //byte-tyyppi, koska arvo ei ylitä 255 ja käyttää vähemmän muistia. oli käytetty muualla koodissa myös.

bool newRandomNumberReady = false; // Flag to indicate that a new random number is ready to be used 
int globalRandomNumber = 0; // Global variable to store the random number

bool timeToCheckGame = false;

void setup() {
  Serial.begin(9600);
  initializeDisplay();
  initializeLeds();
  initializeSound();
  initializeTimer();
  initButtonsAndButtonInterrupts();
  // testDisplay(); // display numbers from 0 to 99
  showResult(score); // after test, show score (default 0)
}

void loop() {
  if(buttonNumber>=0) {
     if(buttonNumber==5 && !gameStarted) { // start the game if buttonNumber == 5 (näppäin 4) ja jos peliä ei ole jo aloitettu
      startTheGame();
      gameStarted = true; // Merkitään peli käynnistyneeksi
     }
     else if (buttonNumber == 6) { // stop the game if buttonNumber == 6 (reset button)
      stopTheGame();
     }
     else if (gameStarted && buttonNumber >= 1 && buttonNumber <= 4) { // if the game is started and the buttonNumber is between 2 and 5 (game buttons)
      userNumbers[nbrOfButtonPush] = buttonNumber; // Tallentaa käyttäjän painaman numeron taulukkoon
      nbrOfButtonPush++; // Kasvattaa painallusten määrää
      // checkGame(nbrOfButtonPush); // kutsutaan checkGame funktiota (ei tarvi?)
      timeToCheckGame = true; // kutsutaan checkGame funktiota myöhemmin
     }
     buttonNumber = -1; // Resetoi buttonNumber jotta seuraava painallus otetaan oikein 
  Serial.print("globalRandomNumber : "); // POISTA
  Serial.println(globalRandomNumber); //POISTA
  }

  if(newTimerInterrupt && gameStarted) {  // Jos on aika luoda uusi numero ja peli on käynnissä 
    ledNumber = globalRandomNumber; // asettaa ledin numeron timerin mukaan
    Serial.print("ledNumber : ");      //POISTA
    Serial.println(ledNumber);           //POISTA
    //Serial.println(globalRandomNumber); //POISTA
    //Serial.println(buttonNumber);       //POISTA
    setLed(ledNumber); // asettaa ledin
    if (nbrOfButtonPush < 100) { // Varmistetaan, ettei taulukko ylity ja peli kaadu
      randomNumbers[nbrOfButtonPush] = globalRandomNumber; // tallentaa numeron taulukkoon
    }
    newTimerInterrupt = false; // resetoi interruptin
    // new random number must be generated
    // int myRand = random(1,5);
    // Printing the output
    // Serial.println(myRand);
     // and corresponding led must be activated
    //  ledNumber = myRand;
  }
  if(timeToCheckGame) {
    Serial.println("timeToCheckGame");
    checkGame(nbrOfButtonPush, randomNumbers, userNumbers); // poita randomnumber, usernumber
    timeToCheckGame = false;
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
  TCCR1B |= (1 << WGM12);  // CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10);  // 1024 prescaler
  sei();
	// void initializeTimer(void) 
}


/* 
Timerin keskeytysten käsittelemiseksi.
Communicate to loop() that it's time to make new random number.
Increase timer interrupt rate after 10 interrupts.
*/
ISR(TIMER1_COMPA_vect) {
  static int interruptCount = 0; // Static variable to count interrupts
  interruptCount++; // Increment the interrupt count

  if (interruptCount >= 10) {    // Increase the timer interrupt rate
    OCR1A = OCR1A / 1.1; // Change the compare match value to increase the interrupt rate 
    interruptCount = 0; // Reset the interrupt count
  }

  // Communicate to loop() that a new random number is ready
  globalRandomNumber = random(1, 5); // Generate a new random number
  newRandomNumberReady = true; // set a flag
  newTimerInterrupt = true;
}


void checkGame(byte nbrOfButtonPush, int randomNumbers[], int userNumbers[]) { //poista randomnumber ja usernumber
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
    for (int i = 0; i < 30; i++){ //poista
    Serial.println("randomNumbers");
      Serial.println(randomNumbers[i]); //poista
       Serial.println("userNumbers");
      Serial.println(userNumbers[i]); // poista
      //Serial.println(nbrOfButtonPush[i]); //poista
    } //poista

  if (compareArrays(randomNumbers, userNumbers, nbrOfButtonPush) == -1) { // jos vertailu ei mene läpi, kutsu stopTheGame()-funktiota

    
    stopTheGame(); // Stop the game if the input is wrong
  }
}

int compareArrays(int randomNumbers[], int userNumbers[], int nbrOfButtonPush)
{
    // Loop through the arrays up to the number of button presses.
    for (int i = 0; i < nbrOfButtonPush; ++i)
    {
        // If any element does not match, call stopTheGame().
        if (randomNumbers[i] != userNumbers[i])
        {
            // Tämä funktio kutsutaan, kun input on väärä.
            return -1;  // Palauta -1, jos virhe löydetään.
        }
    }
    // If all elements match, return 0 (indicating success).
    incrementDisplay(); //increment gamedisplay by 1.
    return 0;
}

void incrementDisplay() {
  score++; // Increment the score
  showResult(score); // Update the display with the new score
}

void initializeGame() {
	/*
  initializeGame() subroutine is used to initialize all variables
  needed to store random numbers and player button push data.
  This function is called from startTheGame() function.
  */
   for (int i = 0; i < 100; i++) { // Nollataan taulukot
    randomNumbers[i] = 0;
    userNumbers[i] = 0;
    
  }
  score = 0;
  showResult(0);
  nbrOfButtonPush = 0; // Nollataan nappejen painallukset
  initializeTimer();
  //score = 0;
}
 


void startTheGame() { // void startTheGame() kutsuu initializeGame() funktiota ja enabloi Timer1 keskeytykset käynnistääkseen pelin
  // tulosta serial monitoriin, kun funktio alkaa
  playGameStartSound();
  Serial.println("startTheGame function starting!");
  
  // enabloi Timer1 compare keskeytykset
  TIMSK1 |= (1 << OCIE1A); 

  // Kutsutaan initializeGame()-funktiota
   initializeGame();
}

void stopTheGame() {
  
  // tulosta serial monitoriin, kun funktio alkaa

  Serial.println("stopTheGame function starting!");

  // disabloi Timer1 compare keskeytykset 

  TIMSK1 &= ~(1 << OCIE1A); 

  gameStarted = false; // Pelin tila ei-aloitetuksi
  ledEndGame();
  playGameEndSound();
}