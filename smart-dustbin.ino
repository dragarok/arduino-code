#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


const int trigPin = 2;  //D4
const int echoPin = 0;  //D3

long duration;
int distance;

unsigned long oldTime;

char ssid[] = "Light111";  //wifi name
char password[] = "##########"; //wifi password
int status = WL_IDLE_STATUS;

uint32_t rtcData;
uint32_t *ptr = (uint32_t *)&rtcData;


void setup()
{
  Serial.begin(9600);
  delay(10);
  connectWifi();
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

}


void connectWifi() // Attempt to connect to wifi network
{
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  

  StaticJsonBuffer<300> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();
  JSONencoder["latitude"] = 28.92;
  JSONencoder["longitude"] = 35.62;


  char JSONmessageBuffer[300];
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);
  
  HTTPClient http;
  http.begin("http://yoursite.com/route/route");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(JSONmessageBuffer);
  String payload = http.getString();
  Serial.println(httpCode);
  Serial.println(payload);
  http.end();
  
}

void loop()
{
  
    Serial.println("Hello fuckers");
    
    //for deep sleep and retrieving data from RTC
    ESP.rtcUserMemoryRead(65, ptr, sizeof(ptr)); //at offset of 65 
    Serial.println(rtcData);
    if(rtcData!=0xffffffff)
    {
      Serial.println("Hello I am the first loop");
      Serial.println("Hello I am sleeping for 35 minutes");
      rtcData=0xFFFFFFFF;
      ESP.rtcUserMemoryWrite(65, ptr, sizeof(ptr)); //now reset the value offset of 65
      //take this thing to deep sleep and then write status to RTC to find if 60 seconds have passed or not
      ESP.deepSleep(10e6);
    }
    else
    {
      rtcData=0x00000000;
      Serial.println("Hello I am the second loop");
      Serial.println("Hello I am sleeping for 25 minutes");
      Serial.println("Hello I am done sleeping");
      ESP.rtcUserMemoryWrite(65, ptr, sizeof(ptr)); //now reset the value offset of 65
      ESP.deepSleep(20e6);
    }
  
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);

    // Calculating the distance
    distance = duration * 0.034 / 2;
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
    delay(2000);
}
