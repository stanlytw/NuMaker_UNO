int Led=D7;
int Shock=D2;
int val;
void setup()
{
    pinMode(Led,OUTPUT);
    pinMode(Shock,INPUT);
}
void loop()
{
    val=digitalRead(Shock);
    if(val==HIGH)
        digitalWrite(Led,LOW);
    else
        digitalWrite(Led,HIGH);
}


