int photoresistorPin = A0;   // select the input pin for the photoresistor
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
 pinMode(ledPin, OUTPUT); 
  Serial.begin(9600); 
}

void loop() {
 
    sensorValue = analogRead(photoresistorPin);    
    digitalWrite(ledPin, HIGH);  
    delay(sensorValue);          
    digitalWrite(ledPin, LOW);   
    delay(sensorValue);
    Serial.println(sensorValue, DEC);  
}