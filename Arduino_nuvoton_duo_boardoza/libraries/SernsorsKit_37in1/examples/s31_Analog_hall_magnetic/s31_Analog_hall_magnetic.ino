int Led=D8;
int SENSOR=A5;
int val;
void setup()
{
  pinMode(Led,OUTPUT);
  Serial.begin(115200);
}
void loop()
{
  val=analogRead(SENSOR);
  digitalWrite(Led,HIGH);
  delay(val);
  digitalWrite(Led,LOW);
  delay(val);
  Serial.println(val,DEC);
}
