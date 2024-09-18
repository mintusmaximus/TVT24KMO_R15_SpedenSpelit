#include "display.h"


// pin setup for SN54HC595 8-bit serial-in, parallel-out shift register 
int pinMR = 12;
int pinSRCLK = 11;
int pinRCLK = 10;
int pinOE = 9;
int pinSER = 8; 



/*
  initializeDisplay subroutine initializes 5 pins needed for controlling 7-segment
  displays in a cascade as follows:
  Arduino pin 12 = serial to parallel component reset
  Arduino pin 11 = serial to parallel component shiftClock
  Arduino pin 10 = serial to parallel component latchClock
  Arduino pin 9  = serial to parallel component outEnable
  Arduino pin 8  = serial to parallel component serialInput
*/
void initializeDisplay(void)
{
    // set pin modes for the chip pins
    pinMode(pinMR, OUTPUT);
    pinMode(pinSRCLK, OUTPUT);
    pinMode(pinRCLK, OUTPUT);
    pinMode(pinOE, OUTPUT);
    pinMode(pinSER, OUTPUT);

    // set initial states for the chip pins
    digitalWrite(pinOE, LOW);
    digitalWrite(pinMR, HIGH);
    digitalWrite(pinSRCLK, LOW);
    digitalWrite(pinRCLK, LOW);
    digitalWrite(pinSER, LOW);
    digitalWrite(pinMR, LOW);
    digitalWrite(pinMR, HIGH);
}


/*
  WriteByte subroutine writes number 0,1,...,9 to
  7-segment display. If there are many 7-segment
  displays cascaded, last parameter can be used to
  control when number is actually shown at the outputs of
  display element.
  
  Parameters:
  uint8_t number: accepts values 0,1,..,9
  
  bool last: either true or false. True = number is displayed
  at the output, false is used if this function is called
  multiple times to write all cascaded numbers to 7-segment
  displays.
*/
void writeByte(uint8_t bits,bool last)
{

}




/*
  writeHighAndLowNumber subroutine writes a number 0,1,..,99
  to 2 cascaded 7-segment displays. This subroutine uses
  WriteByte subroutine to write 2 numbers to the display.
  
  Parameters:
  
  uint8_t tens: number 0,1,..,9
  uint8_t ones: number 0,1,..,9
*/
void writeHighAndLowNumber(uint8_t tens,uint8_t ones)
{

}






/*
  showResuts(byte result) This function separates tens and ones
  from a result number and calls writeHighAndLowNumber subroutine
  to actually write the number to 7-segment display.
  
  Parameters:
  byte result: A number between 0,1,..,99. This function
*/
void showResult(byte number)
{

}

