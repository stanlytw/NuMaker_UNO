/*
  Button
 
 Note: This sample support UNO-M4.
 
 This example code is in the public domain.
 
 */

// constants won't change. They're used here to 
// set pin numbers:
#define NUM 3
const int buttonPin[NUM] = { D11, D12, D13 };     // the number of the pushbutton pin
const int ledPin[NUM]    = {  D7,  D8, D9 };     // the number of the LED

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  for(int i=0;i<NUM;i++)
   pinMode(ledPin[i], OUTPUT);      
  // initialize the pushbutton pin as an input:
  for(int i=0;i<NUM;i++)
    pinMode(buttonPin[i], INPUT);     
}

void loop(){
  // read the state of the pushbutton value:
   for(int i=0;i<NUM;i++)
   {
      buttonState = digitalRead(buttonPin[i]);
    
      // check if the pushbutton is pressed.
      // if it is, the buttonState is HIGH:
      if (buttonState == HIGH) {     
        // turn LED on:    
        digitalWrite(ledPin[i], HIGH);  
      } 
      else {
        // turn LED off:
        digitalWrite(ledPin[i], LOW); 
      }
  }
}