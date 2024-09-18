#include "display.h"


// pin setup for SN54HC595 8-bit serial-in, parallel-out shift register 
const int serialInput = 8; // DS serial data input
const int outEnable = 9; // OE' output enable (active LOW)
const int latchClock = 10; // STCP storage register clock
const int shiftClock = 11; // SHCP shift register clock
const int masterReset = 12; // MR' master reset (active HIGH)



// 7-segment display codes, array index == number to be displayed
const byte displayNumbers[] = { 
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};





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
  pinMode(masterReset, OUTPUT);
  pinMode(shiftClock, OUTPUT);
  pinMode(latchClock, OUTPUT);
  pinMode(outEnable, OUTPUT);
  pinMode(serialInput, OUTPUT);

  digitalWrite(masterReset, HIGH); // turn off the reset
  digitalWrite(outEnable, LOW); // enable the output (active LOW)

  Serial.println("DISPLAY: Initialized"); // debug message
}



// testDisplay will show numbers from 0 to 99 to 7-segment display for testing purposes
void testDisplay(void){
  Serial.println("DISPLAY: Displaying 0-99"); // debug message

  for(int i = 0; i < 100; i++)
  {
    showResult(i);
    delay(100);
  }

  Serial.println("DISPLAY: Test complete"); // debug message
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


void writeByte(uint8_t bits, bool last)
{
  byte segment = displayNumbers[bits]; // set the segment to be displayed

  shiftOut(serialInput, shiftClock, MSBFIRST, segment); // shift out the segment to the display 


  if (last) // if on last byte
  {
    digitalWrite(latchClock, HIGH); // latch the data
    digitalWrite(latchClock, LOW); // unlatch the data
  }


}




/*
  writeHighAndLowNumber subroutine writes a number 0,1,..,99
  to 2 cascaded 7-segment displays. This subroutine uses
  WriteByte subroutine to write 2 numbers to the display.
  
  Parameters:
  
  uint8_t tens: number 0,1,..,9
  uint8_t ones: number 0,1,..,9
*/
void writeHighAndLowNumber(uint8_t tens, uint8_t ones)
{
  // write tens to the first display
  writeByte(tens,false); // call the writeByte function with tens and false

  // write ones to the second display
  writeByte(ones,true); // call the writeByte function with ones and true
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
  uint8_t tens = number / 10; // get tens by dividing number by 10 (ex. 23 / 10 = 2)
  uint8_t ones = number % 10; // get ones by getting the remainder of number divided by 10 (ex. 23 % 10 = 3), makes 23 with above line of code

  writeHighAndLowNumber(tens,ones); // call writeHighAndLowNumber with tens and ones to display the number on the 7-segment display
}

