#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "sound.h"
#include "SpedenSpelit.h"

volatile int buttonNumber = -1;           // for buttons interrupt handler
volatile bool newTimerInterrupt = false;  // for timer interrupt handler
bool gameStarted = false;                 // Muuttuja pelin tilalle
bool timeToCheckGame = false;
int score = 0;                            // Pisteet-muuttuja
extern byte ledNumber = 0;
int testeri = 0;
int countteri = 0;

int randomNumbers[100]; // Taulukko pelin arpomille numeroille
int userNumbers[100]; // Taulukko käyttäjän painalluksille
extern byte nbrOfButtonPush = 0;

bool newRandomNumberReady = false;  // Flag to indicate that a new random number is ready to be used
int globalRandomNumber = 0;         // Global variable to store the random number

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
    if (buttonNumber == 5 && !gameStarted) {  // Aloita peli jos buttonNumber == 5 ja jos peliä ei ole jo aloitettu
      startTheGame();
      gameStarted = true;            // Merkitään peli käynnistyneeksi
    } else if (buttonNumber == 6) {  // Pysäytä peli jos buttonNumber == 6 (reset button)
      stopTheGame();
    } else if (gameStarted && buttonNumber >= 1 && buttonNumber <= 4) {  // Jos peli on aloitettu ja buttonNumber on 2 and 5 välillä (game buttons)
      userNumbers[nbrOfButtonPush] = buttonNumber;                       // Tallentaa käyttäjän painaman numeron taulukkoon
      timeToCheckGame = true;  // Kutsutaan checkGame funktiota
    }
    buttonNumber = -1;  // Resetoi buttonNumber jotta seuraava painallus otetaan oikein
  }

  if (newTimerInterrupt && gameStarted) {  // Jos on aika luoda uusi numero ja peli on käynnissä
    ledNumber = globalRandomNumber;        // Asettaa ledin numeron timerin mukaan
    if (ledNumber == 1) {
      playButtonOneSelected();
    } else if (ledNumber == 2) {
      playButtonTwoSelected();
    } else if (ledNumber == 3) {
      playButtonThreeSelected();
    } else if (ledNumber == 4) {
      playButtonFourSelected();
    }
    setLed(ledNumber); // Asettaa ledin
    if (nbrOfButtonPush < 100) { // Varmistetaan, ettei taulukko ylity ja peli kaadu
      randomNumbers[countteri] = globalRandomNumber;
      countteri++;
       
    }
    newTimerInterrupt = false;  // Resetoi interruptin
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

void initializeTimer(void) {
  cli(); // clear interruptions
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;  // set entire TCCR1A register to 0
  TCCR1B = 0;  // same for TCCR1B
  TCNT1 = 0;   // initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;                        // = (16*10^6) / (1*1024) - 1 (must be <65536)
  TCCR1B |= (1 << WGM12);               // CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10);  // 1024 prescaler
  sei(); // set interruptions
}


/* Timerin keskeytysten käsittelemiseksi.
Communicate to loop() that it's time to make new random number.
Increase timer interrupt rate after 10 interrupts. */
ISR(TIMER1_COMPA_vect) {
  static int interruptCount = 0;  // Static variable to count interrupts
  interruptCount++;               // Increment the interrupt count

  if (interruptCount >= 10) {  // Increase the timer interrupt rate
    OCR1A = OCR1A / 1.1;       // Change the compare match value to increase the interrupt rate
    interruptCount = 0;        // Reset the interrupt count
  }

  // Communicate to loop() that a new random number is ready
  globalRandomNumber = random(1, 5);  // Generate a new random number
  newRandomNumberReady = true;        // set a flag
  newTimerInterrupt = true;
}


/* checkGame() subroutine is used to check the status of the Game after each player button press.
If the latest player button press is wrong, the game stops and if the latest press was right,
game display is incremented by 1 (in the incrementDisplay() subroutine). */
int checkGame(int randomNumbers[], int userNumbers[], int nbrOfButtonPush ) { 
    Serial.println(randomNumbers[nbrOfButtonPush -1]);
    Serial.println(userNumbers[nbrOfButtonPush]);
    Serial.println(nbrOfButtonPush);
    if (randomNumbers[nbrOfButtonPush -1] != userNumbers[nbrOfButtonPush]) {
      return 0;  // Palauta 0, jos virhe löydetään.
    }
    else {
      return 1; // Muuten palauta 1.
  }
}

void incrementDisplay() {
  score++;            // Increment the score
  showResult(score);  // Update the display with the new score
}


/* initializeGame() subroutine is used to initialize all variables
needed to store random numbers and player button push data.
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
  
  globalRandomNumber = random(1, 5);
  newRandomNumberReady = true;
  newTimerInterrupt = true;
}


/* void startTheGame() kutsuu initializeGame() funktiota
ja enabloi Timer1 keskeytykset käynnistääkseen pelin */
void startTheGame() {
  TIMSK1 &= ~(1 << OCIE1A);  // Disabloi Timer1 keskeytykset (jos sattumalta vielä päällä)
  TIMSK1 = 0;
  playGameStartSound(); // Pelinaloitusääni
  initializeGame(); // Kutsutaan initializeGame()-funktiota
  TIMSK1 |= (1 << OCIE1A); // Enabloi Timer1 compare keskeytykset
}

void stopTheGame() {
  TIMSK1 &= ~(1 << OCIE1A); // Disabloi Timer1 compare keskeytykset
  gameStarted = false;  // Pelin tila ei-aloitetuksi
  ledEndGame(); // Ledishow pelin päättymisen merkiksi
  playGameEndSound(); // Pelinlopetusääni
}