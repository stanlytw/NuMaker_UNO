/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/
#define UNO_PIN2 2    //Also D2, PC.9
#define UNO_PIN6 6    //Also D6, PE.4
#define UNO_PIN16 16  //Also A0, PB.6
#define UNO_A0 0
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  //Test GPIO PWM Functionality and AnalogRead 2022-10-14
  //Short UNO_PIN6 and UNO_PIN6 
  pinMode(UNO_PIN6, OUTPUT);
  pinMode(UNO_PIN16, INPUT);

  //Test Serial1 Functionality 2022-10-14
  Serial1.begin(9600);
  
  //Test GPIO Interrupt Functionality 2022-10-14
  pinMode(UNO_PIN2, INPUT);
  attachInterrupt(UNO_PIN2, blink, CHANGE);

}

// the loop function runs over and over again forever
void loop() {
  int i, val, set_v;

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                       // wait for a second

  //Test GPIO PWM Functionality 2022-10-14
  for(i=0;i<255;i++)
  {
      analogWrite(UNO_PIN6, i);//i=255 mapping to Vdd, i=0 mapping to Vss
                               //full duty = 2ms
      delay(50);

  }
  val = analogRead(UNO_A0);  
  Serial1.print("10-bit ADC result:"); 
  Serial1.println(val); 
  //Test Serial1 Functionality 2022-10-14
  Serial1.println("Hello");
}


void blink() {
  //Test GPIO Interrupt Functionality 2022-10-14
  Serial1.println("Interrupt");
}
