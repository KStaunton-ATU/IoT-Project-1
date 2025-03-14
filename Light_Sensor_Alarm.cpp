//Written by Kevin Staunton
const int lightPin = A0; // Analog pin A0
const int buzzerPin = 3; // Buzzer pin
const int ledPin = 4; //LED Pin
const int baseline = 300;

#include <Bridge.h>
#include <HttpClient.h>

//we want to eventually make a HTTP request to Pushing Box
HttpClient client; //this object will let us make HTTP requests to the pushingbox service
String baseURL = "api.pushingbox.com/pushingbox?devID=";
String deviceID = "v0CC38418000E8B6";
String spreadsheetValue ="&LightValue=";

void setup() 
{ 
  //initialise
  pinMode(lightPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  //Bridge needs time to get started. So wait 3 seconds
  Bridge.begin();
  delay(3000);
}

void loop() 
{
  int lightValue = analogRead(lightPin);

  if (lightValue < baseline) 
  {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
    Serial.print("Light Intensity: ");
    Serial.println(lightValue);

    //wait 3 seconds and turn off the alarm
    delay(3000);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);

    //build URL string and then make a HTTP request to that PB service
    String requestURL = baseURL + deviceID + spreadsheetValue + lightValue;
    client.get(requestURL);
    client.get("api.pushingbox.com/pushingbox?devid=v42FDA5675FD7DA8");
  } 
  else 
  {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }
  delay(1000);
}
