int sensorPin = D10;
int value = 0;

void setup() {
  pinMode(sensorPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  value = analogRead(A5);
  Serial.print("X:");
  Serial.print(value, DEC);

  value = analogRead(A4);
  Serial.print(" | Y:");
  Serial.print(value, DEC);

  value = digitalRead(sensorPin);
  Serial.print(" | Z: ");
  Serial.println(value, DEC);

  delay(100);
}

