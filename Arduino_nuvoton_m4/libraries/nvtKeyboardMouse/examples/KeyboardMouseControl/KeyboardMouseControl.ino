/*
  KeyboardMouseControl

  Simply demo how to control mouse and keyboard without buttons.

  
*/
#ifndef __NVTKB__
#error Select Numaker-UNO-M4KB board for KeyboardMouse demo!
#endif

#include "USBAPI.h"

int range = 2;           // output range of X or Y movement; affects movement speed
int responseDelay = 100;  // response delay of the mouse, in ms


void setup() {
 
  // initialize mouse control:
  Mouse.begin();
}

int upState = 0;// = digitalRead(upButton);
int downState = 0;// = digitalRead(downButton);
int rightState = 0;// = digitalRead(rightButton);
int leftState = -10;// = digitalRead(leftButton);
int clickState = 0;// = digitalRead(mouseButton);


void loop() {

  if(leftState<10) leftState+=1;
  else leftState = -10;


  // calculate the movement distance based on the button states:
  int xDistance = (leftState - rightState) * range;
  int yDistance = (leftState - downState) * range;

  // if X or Y is non-zero, move:
  if ((xDistance != 0) || (yDistance != 0)) {
    Mouse.move(xDistance, yDistance, 0);
  }
  Keyboard.press('l');
  delay(20);
  Keyboard.releaseAll();
  delay(500);

  // a delay so the mouse doesn't move too fast:
  delay(responseDelay);
}
