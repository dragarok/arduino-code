// Code for Ultrasonic Distance Module US-015
unsigned int EchoPin = 2;
unsigned int TrigPin = 3;
unsigned long Time_Echo_us = 0;
//Len_mm_X100 = length*100
unsigned long Len_cm_X100 = 0;
unsigned long Len_Integer = 0; //
unsigned int Len_Fraction = 0;
void setup()
{
Serial.begin(9600);
pinMode(EchoPin, INPUT);
pinMode(TrigPin, OUTPUT);
}
void loop()
{
digitalWrite(TrigPin, HIGH);
delayMicroseconds(50);
digitalWrite(TrigPin, LOW);
Time_Echo_us = pulseIn(EchoPin, HIGH);
if((Time_Echo_us < 60000) && (Time_Echo_us > 1))
{
Len_cm_X100 = (Time_Echo_us*34)/2;
Len_Integer = Len_cm_X100/1000;
Len_Fraction = Len_cm_X100%1000;
Serial.print("Present Length is: ");
Serial.print(Len_Integer, DEC);
Serial.print(".");
if(Len_Fraction < 10)
Serial.print("0");
Serial.print(Len_Fraction, DEC);
Serial.println("cm");
delay(1000);
}
}
