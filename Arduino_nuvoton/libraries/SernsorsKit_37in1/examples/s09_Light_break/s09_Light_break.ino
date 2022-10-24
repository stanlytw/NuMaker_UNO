int Led=13;
int SensorOutputPin=3;
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
