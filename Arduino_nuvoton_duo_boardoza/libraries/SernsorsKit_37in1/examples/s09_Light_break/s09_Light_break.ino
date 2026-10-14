int Led=D7;
int SensorOutputPin=D2;
int val;
void setup()
{
    pinMode(Led,OUTPUT);
    pinMode(SensorOutputPin,INPUT);
}
void loop()
{
    val=digitalRead(SensorOutputPin);
    if(val==HIGH)
    {
        digitalWrite(Led,HIGH);
    }
    else
    {
        digitalWrite(Led,LOW);
    }
}
