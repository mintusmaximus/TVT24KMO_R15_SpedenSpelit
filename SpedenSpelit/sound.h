#ifndef SOUND_H
#define SOUND_H
#include <Arduino.h>



void initializeSound(void);



void playGameStartSound(void);



void playButtonOneSelected(void);



void playButtonTwoSelected(void);



void playButtonThreeSelected(void);



void playButtonFourSelected(void);



void playGameEndSound(void);



void playCorrectSound(void);



void playWrongSound(void);

#endif
