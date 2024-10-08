#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "sound.h"
#include "SpedenSpelit.h"

// Use these 2 volatile variables for communicating between
// loop() function and interrupt handlers
volatile int buttonNumber = -1;           // for buttons interrupt handler
volatile bool newTimerInterrupt = false;  // for timer interrupt handler
bool gameStarted = false;                 // Muuttuja pelin tilalle
int score = 0;
extern byte ledNumber = 0;
int testeri = 0;
int countteri = 0;
volatile int totalInterrupts = 0;

// Arvotut numerot talletetaan 100 alkion mittaiseen taulukkoon (randomNumbers)
int randomNumbers[100];
// Kun käyttäjä painaa kytkimiä 0,1,2,3 nämä näppäinten painallukset talletetaan taulukkoon (userNumbers)
int userNumbers[100];
// Indeksi, joka seuraa missä kohtaa taulukkoa mennään eli monestikko käyttäjä on painanut nappeja
extern byte nbrOfButtonPush = 0;  //byte-tyyppi, koska arvo ei ylitä 255 ja käyttää vähemmän muistia. oli käytetty muualla koodissa myös.

bool newRandomNumberReady = false;  // Flag to indicate that a new random number is ready to be used
int globalRandomNumber = 0;         // Global variable to store the random number

bool timeToCheckGame = false;

void setup() {
  Serial.begin(9600);
  initializeDisplay();
  initializeLeds();
  initializeSound();
  initializeTimer();
  initButtonsAndButtonInterrupts();
  showResult(score);  // after test, show score (default 0)
}

void loop() {
  if (buttonNumber >= 0) {
    if (buttonNumber == 5 && !gameStarted) {  // start the game if buttonNumber == 5 (näppäin 4) ja jos peliä ei ole jo aloitettu
      startTheGame();
      gameStarted = true;            // Merkitään peli käynnistyneeksi
    } else if (buttonNumber == 6) {  // stop the game if buttonNumber == 6 (reset button)
      stopTheGame();
    } else if (gameStarted && buttonNumber >= 1 && buttonNumber <= 4) {  // if the game is started and the buttonNumber is between 2 and 5 (game buttons)
      userNumbers[nbrOfButtonPush] = buttonNumber;
      Serial.print(nbrOfButtonPush);
      Serial.print(" usr-> ");
      Serial.println(userNumbers[nbrOfButtonPush]);
      nbrOfButtonPush++;
                                                        // Tallentaa käyttäjän painaman numeron taulukkoon
                                                       // Kasvattaa painallusten määrää
      
      timeToCheckGame = true;  // kutsutaan checkGame funktiota myöhemmin
    }
    buttonNumber = -1;  // Resetoi buttonNumber jotta seuraava painallus otetaan oikein
  }

  if (newTimerInterrupt && gameStarted) {  // Jos on aika luoda uusi numero ja peli on käynnissä
    ledNumber = globalRandomNumber;        // asettaa ledin numeron timerin mukaan
    if (ledNumber == 1) {
      playButtonOneSelected();
    } else if (ledNumber == 2) {
      playButtonTwoSelected();
    } else if (ledNumber == 3) {
      playButtonThreeSelected();
    } else if (ledNumber == 4) {
      playButtonFourSelected();
    }
    setLed(ledNumber);                                      // asettaa ledin
    if (nbrOfButtonPush < 100) {                            // Varmistetaan, ettei taulukko ylity ja peli kaadu
      randomNumbers[countteri] = globalRandomNumber;
      Serial.print(countteri);
      Serial.print(" rnd-> ");
      Serial.println(randomNumbers[countteri]);
      //Serial.println("toimii tämä"); //POISTA
      //Serial.println(randomNumbers[nbrOfButtonPush]);
      //Serial.println(randomNumbers[countteri]);  //laittaa oikeita numeroita POISTA
      countteri++;
       
    }
    newTimerInterrupt = false;  // resetoi interruptin
  }
  if (timeToCheckGame) {
    testeri = checkGame(randomNumbers, userNumbers, nbrOfButtonPush); //KAIKKI ARRAYN KOHDAT -1!! TARKISTA
  if (testeri == 0){
    stopTheGame();
  }
  if (testeri == 1){
    incrementDisplay();
  }
    timeToCheckGame = false;
  }
}

void initializeTimer(void) {
  cli();
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;  // set entire TCCR1A register to 0
  TCCR1B = 0;  // same for TCCR1B
  TCNT1 = 0;   //initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;                        // = (16*10^6) / (1*1024) - 1 (must be <65536)
  TCCR1B |= (1 << WGM12);               // CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10);  // 1024 prescaler
  sei();
}


/* 
Timerin keskeytysten käsittelemiseksi.
Communicate to loop() that it's time to make new random number.
Increase timer interrupt rate after 10 interrupts.
*/
ISR(TIMER1_COMPA_vect) {
  static int interruptCount = 0;  // Static variable to count interrupts
  interruptCount++;               // Increment the interrupt count
  totalInterrupts++;              // keep track of total timer runs

  if (interruptCount >= 10) {  // Increase the timer interrupt rate
    OCR1A = OCR1A / 1.1;       // Change the compare match value to increase the interrupt rate
    interruptCount = 0;        // Reset the interrupt count
  }

  if (totalInterrupts > 110){ // stop the game at 110 leds so the arduino doesnt implode
    stopTheGame();
  }
  
  // Communicate to loop() that a new random number is ready
  globalRandomNumber = random(1, 5);  // Generate a new random number
  newRandomNumberReady = true;        // set a flag
  newTimerInterrupt = true;
}


int checkGame(int* randomNumbers, int* userNumbers, int nbrOfButtonPush ) {
    //Serial.println(randomNumbers[nbrOfButtonPush -1]);
    //Serial.println(randomNumbers[nbrOfButtonPush -1]);
    //Serial.println(randomNumbers[nbrOfButtonPush +1]);
    //Serial.println(userNumbers[nbrOfButtonPush]);
    //Serial.println(nbrOfButtonPush);
    for (int i=0; i<nbrOfButtonPush; i++){
      if (randomNumbers[i] != userNumbers[i]) {
        Serial.println(randomNumbers[i]);
        Serial.println(randomNumbers[i -1]);
        Serial.println("-----");
        Serial.println(userNumbers[i]);
        Serial.println(userNumbers[i -1]);

        Serial.println("----");
        return 0;  // Palauta 0, jos virhe löydetään.
      }
    }
    return 1;
    
  }


void incrementDisplay() {
  score++;            // Increment the score
  showResult(score);  // Update the display with the new score
}

void initializeGame() {
  /*
  initializeGame() subroutine is used to initialize all variables
  needed to store random numbers and player button push data.
  This function is called from startTheGame() function.
  */
  for (int i = 0; i < 100; i++) {  // Nollataan taulukot
    randomNumbers[i] = -1;
    userNumbers[i] = -1;
  }
  score = 0;               // Nollataan pisteet
  globalRandomNumber = 0;  // Nollataan globatRandomNuber
  showResult(0);           // Nollataan pisteet-display
  nbrOfButtonPush = 0;     // Nollataan nappejen painallukset
  initializeTimer();       // Asettaa ajastimen uudelleen
  
  globalRandomNumber = random(1, 5); // luo väkisin ensimmäinen numeron timerin ulkopuolella, koska timeri ei tee ekaa numeroa
  newRandomNumberReady = true; // asettaa flagin uudelle numerolle valmiiksi 
  newTimerInterrupt = true; // asettaa timerin keskeytyksen valmiiksi 
}

void startTheGame() {  // void startTheGame() kutsuu initializeGame() funktiota ja enabloi Timer1 keskeytykset käynnistääkseen pelin
  // tulosta serial monitoriin, kun funktio alkaa
  TIMSK1 &= ~(1 << OCIE1A);  // Disabloi Timer1 keskeytys (jos sattumalta vielä päällä)
  TIMSK1 = 0;

  playGameStartSound();
  Serial.println("startTheGame function starting!");

  // Kutsutaan initializeGame()-funktiota
  initializeGame();

  // enabloi Timer1 compare keskeytykset
  TIMSK1 |= (1 << OCIE1A);
}

void stopTheGame() {

  // tulosta serial monitoriin, kun funktio alkaa

  Serial.println("stopTheGame function starting!");

  // disabloi Timer1 compare keskeytykset

  TIMSK1 &= ~(1 << OCIE1A);

  gameStarted = false;  // Pelin tila ei-aloitetuksi
  ledEndGame();
  playGameEndSound();
  countteri = 0;
  nbrOfButtonPush = 0;
}