int redpin = D10;    // select LED output pin for red LED
int bluepin = D12;   // select LED output pin for blue LED
int greenpin = D13;  // select LED output pin for green LED
int val;

void setup() {
  pinMode(redpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  for (val = 255; val > 0; val--) {
    analogWrite(11, val);
    analogWrite(10, 255 - val);
    analogWrite(9, 128 - val);
    delay(1);
  }
  for (val = 0; val < 255; val++) {
    analogWrite(11, val);
    analogWrite(10, 255 - val);
    analogWrite(9, 128 - val);
    delay(1);
  }
  Serial.println(val, DEC);
}
