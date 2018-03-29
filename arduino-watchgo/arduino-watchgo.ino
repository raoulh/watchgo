// USB watchdog for PC.
// Connected to USB for serial port communication with host tool
// and with a relay pin connected to reset

int relayPin = 3;
int ledPin = 13; //onboard

long prevMillis = 0;
int data = 0;
int notAliveSec = 0;

enum
{
    STATE_ALIVE = 0,
    STATE_60 = 1,
    STATE_120 = 2,
    STATE_180 = 3,
};
int state = STATE_ALIVE;

void setup()
{
    Serial.begin(115200);
    pinMode(relayPin, OUTPUT);
    pinMode(ledPin, OUTPUT);
    digitalWrite(relayPin, HIGH);

    prevMillis = millis();
}

void checkState()
{
    switch (state)
    {
    case STATE_60:
        Serial.println("No ping for 1 min");
        break;
    case STATE_120:
        Serial.println("No ping for 2 mins");
        break;
    case STATE_180:
        Serial.println("No ping for 3 mins, reseting host!");
        
        digitalWrite(relayPin, LOW);
        delay(1000);
        digitalWrite(relayPin, HIGH);
        
        delay(5000);
        
        //reset state to initially alive and restart the procedure
        state = STATE_ALIVE;
        notAliveSec = 0;
        prevMillis = millis();
        
        break;
    }
}

void loop()
{
    digitalWrite(ledPin, LOW);
    delay(100);
    
    unsigned long currMillis = millis();
    
    if (Serial.available() > 0)
    {
        data = Serial.read();
    }
    
    if (data == 'P')
    {
        Serial.println("Received ping from host");
        notAliveSec = 0;
        digitalWrite(ledPin, HIGH);
        state = STATE_ALIVE;
    }
    data = 0;
    
    if (currMillis - prevMillis > 1000)
    {
        prevMillis = currMillis;
        notAliveSec++;
        
        if (notAliveSec == 60) state = STATE_60;
        else if (notAliveSec == 120) state = STATE_120;
        else if (notAliveSec >= 180) state = STATE_180;
        
        checkState();
    }
}
