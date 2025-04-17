int Led=D7;
int SENSOR=D2;
int val;
void setup()
{  
  pinMode(Led,OUTPUT);
  pinMode(SENSOR,INPUT);
}
void loop()
{
  val=digitalRead(SENSOR);
  if(val==HIGH)
    digitalWrite(Led, HIGH);
  else
    digitalWrite(Led, LOW);
}
