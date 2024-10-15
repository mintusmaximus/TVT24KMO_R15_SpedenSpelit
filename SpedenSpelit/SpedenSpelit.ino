#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "sound.h"
#include "SpedenSpelit.h"

// Use these 2 volatile variables for communicating between loop() function and interrupt handlers
volatile int buttonNumber = -1;           // for buttons interrupt handler
volatile bool newTimerInterrupt = false;  // for timer interrupt handler

bool gameStarted = false;                 // Muuttuja pelin tilalle
int score = 0;                            // Pisteet-muuttuja
extern byte ledNumber = 0;
int testeri = 0;
int countteri = 0;
volatile int totalInterrupts = 0;
volatile int interruptCount = 0;  

// Arvotut numerot talletetaan 100 alkion mittaiseen taulukkoon (randomNumbers)
int randomNumbers[100];
// Kun käyttäjä painaa kytkimiä 0,1,2,3 nämä näppäinten painallukset talletetaan taulukkoon (userNumbers)
int userNumbers[100];
// Indeksi, joka seuraa missä kohtaa taulukkoa mennään eli monestikko käyttäjä on painanut nappeja
extern byte nbrOfButtonPush = 0;  // Byte-tyyppi, koska arvo ei ylitä 255 ja käyttää vähemmän muistia.

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
    if (buttonNumber == 5 && !gameStarted) {  // Aloittaa pelin jos buttonNumber == 5 ja jos peliä ei ole vielä aloitettu
      startTheGame();
      gameStarted = true;            // Merkitään peli käynnistyneeksi
    } else if (buttonNumber == 5) {  // Pysäyttää pelin jos buttonNumber == 5 (reset button)
      stopTheGame();
    } else if (gameStarted && buttonNumber >= 1 && buttonNumber <= 4) {  // Jos peli on aloitettu ja buttonNumber on 2 and 5 välillä (game buttons)
      userNumbers[nbrOfButtonPush] = buttonNumber;
      Serial.print(nbrOfButtonPush);
      Serial.print(" usr-> ");
      Serial.println(userNumbers[nbrOfButtonPush]);
      nbrOfButtonPush++;        // Kasvattaa painallusten määrää
      timeToCheckGame = true;   // Kutsutaan checkGame funktiota
    }
    buttonNumber = -1;          // Resetoi buttonNumber jotta seuraava painallus otetaan oikein
  }

  if (newTimerInterrupt && gameStarted) {  // Jos on aika luoda uusi numero ja peli on käynnissä...
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
    setLed(ledNumber);            // Asettaa ledin
    if (nbrOfButtonPush < 100) {  // Varmistetaan, ettei taulukko ylity ja peli kaadu
      randomNumbers[countteri] = globalRandomNumber;
      Serial.print(countteri);
      Serial.print(" rnd-> ");
      Serial.println(randomNumbers[countteri]);
      countteri++;
    }
    newTimerInterrupt = false;  // resetoi interruptin
  }
  if (timeToCheckGame) {
    testeri = checkGame(randomNumbers, userNumbers, nbrOfButtonPush);
  if (testeri == 0){
    stopTheGame();
  }
  if (testeri == 1){
    incrementDisplay();
  }
    timeToCheckGame = false;
  }
}


/* initializeTimer() subroutine intializes
Arduino Timer1 module to give interrupts at rate 1Hz */
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


/* Timerin keskeytysten käsittelemiseksi.
Communicate to loop() that it's time to make new random number.
Increase timer interrupt rate after 10 interrupts. */
ISR(TIMER1_COMPA_vect) {
  interruptCount++;               // Increment the interrupt count
  totalInterrupts++;              // keep track of total timer runs

  if (interruptCount >= 10) {  // Increase the timer interrupt rate
    OCR1A = OCR1A / 1.1;       // Change the compare match value to increase the interrupt rate
    ledFlashDuration = ledFlashDuration / 1.1; // Change the led flash duration to increase the led flash rate 
    interruptCount = 0;        // Reset the interrupt count
  }

  if (totalInterrupts > 110) {  // Stop the game at 110 leds so the arduino doesnt implode
    totalInterrupts = 0;        // Reset the counter so the game still works 
    stopTheGame();              // Pysäytä peli
  }
  
  // Communicate to loop() that a new random number is ready
  globalRandomNumber = random(1, 5);  // Generate a new random number
  newRandomNumberReady = true;        // set a flag
  newTimerInterrupt = true;
}


/* checkGame() subroutine is used to check the status of the Game after each player button press.
If the latest player button press is wrong, the game stops and
if the latest press was right, game display is incremented by 1 (in incrementDisplay() subroutine).*/
int checkGame(int* randomNumbers, int* userNumbers, int nbrOfButtonPush ) {
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


/* initializeGame() subroutine is used to initialize all variables needed to
store random numbers and player button push data.
This function is called from startTheGame() function. */
void initializeGame() {
  for (int i = 0; i < 100; i++) {  // Nollataan taulukot
    randomNumbers[i] = -1;
    userNumbers[i] = -1;
  }
  score = 0;               // Nollataan pisteet
  globalRandomNumber = 0;  // Nollataan globatRandomNuber
  showResult(0);           // Nollataan pisteet-display
  nbrOfButtonPush = 0;     // Nollataan nappejen painallukset
  initializeTimer();       // Asettaa ajastimen uudelleen
  interruptCount = 0;      // Nollaa timerin interruptit
  ledFlashDuration = 200;  // Asettaa ledin välähdysajan 
  totalInterrupts = 0;
  
  globalRandomNumber = random(1, 5);  // Luo väkisin ensimmäinen numeron timerin ulkopuolella, koska timeri ei tee ekaa numeroa
  newRandomNumberReady = true;        // Asettaa flagin uudelle numerolle valmiiksi 
  newTimerInterrupt = true;           // Asettaa timerin keskeytyksen valmiiksi 
}


/* void startTheGame() kutsuu initializeGame() funktiota
ja enabloi Timer1 keskeytykset käynnistääkseen pelin */
void startTheGame() {
  TIMSK1 &= ~(1 << OCIE1A);  // Disabloi Timer1 keskeytys (jos sattumalta vielä päällä)
  TIMSK1 = 0;

  playGameStartSound();     // Pelin aloitusääni
  initializeGame();         // Kutsutaan initializeGame()-funktiota
  TIMSK1 |= (1 << OCIE1A);  // Enabloi Timer1 compare keskeytykset
}

void stopTheGame() {
  TIMSK1 &= ~(1 << OCIE1A); // disabloi Timer1 compare keskeytykset
  gameStarted = false;      // Pelin tila ei-aloitetuksi
  ledEndGame();             // Ledishow pelin loppumisen merkiksi
  playGameEndSound();       // Pelin loppumisääni
  countteri = 0;
  nbrOfButtonPush = 0;
}