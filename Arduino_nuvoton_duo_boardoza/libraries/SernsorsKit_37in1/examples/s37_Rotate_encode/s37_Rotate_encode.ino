const int interruptA = D2;       // Set Interrupt 0 on pin 2

int clk = D2;     // PIN2
int DAT = D12;     // PIN3
int SW = D13;      // PIN4
int LED1 = D7;    // PIN5
int LED2 = D8;    // PIN6
int COUNT = 0;

void setup()
{
    attachInterrupt(interruptA, RoteStateChanged, FALLING);

    pinMode(clk, INPUT_PULLUP);
    pinMode(DAT, INPUT);
    pinMode(SW, INPUT);

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    Serial.begin(115200);
}

void loop()
{
    //[2025-03-26]Since our encoder module SW pin signal is quite small.
    //if  (!(digitalRead(SW)))
    //{
    //    COUNT = 0;
    //    Serial.println("STOP COUNT = 0");
    //    digitalWrite(LED1, LOW);
    //    digitalWrite(LED2, LOW);
    //    delay (200);
    //}
    delay (200);
    Serial.print("Current position: ");
    Serial.println(COUNT);
    
}

//-------------------------------------------
void RoteStateChanged() //When CLK  FALLING READ DAT
{
    if  (digitalRead(DAT)) // When DAT = HIGH IS FORWARD
    {
        COUNT++;
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
    }
    else                   // When DAT = LOW IS BackRotate
    {
        COUNT--;
        digitalWrite(LED2, HIGH);
        digitalWrite(LED1, LOW);
    }
	
}
