#ifndef BUTTONS_H
#define BUTTONS_H
#include <arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>


extern unsigned long merkattuaika; // asettaa ajan, jolloin painallus on tapahtunut
extern const unsigned long kynnysaika; // aikaraja painalluksille
extern volatile int buttonNumber;

const byte firstPin = 2; // First PinChangeInterrupt on D-bus
const byte lastPin =  5; // Last PinChangeInterrupt on D-bus

void initButtonsAndButtonInterrupts(void);

// Intoduce PCINT2_vect Interrupt SeRvice (ISR) function for Pin Change Interrupt.
ISR(PCINT2_vect); 
#endif;