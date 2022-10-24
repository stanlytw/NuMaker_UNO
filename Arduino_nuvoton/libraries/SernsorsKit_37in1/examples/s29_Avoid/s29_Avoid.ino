int Led=13;
int obstacleDetectionPin=10;
int val;
void setup()
{
    pinMode(Led,OUTPUT);
    pinMode(obstacleDetectionPin,INPUT);
}
void loop()
{
    val=digitalRead(obstacleDetectionPin);
    if(val==HIGH)
    {
      digitalWrite(Led,LOW);
    }
    else
    {
      digitalWrite(Led,HIGH);
    }
}
