//Written by Kevin Staunton
const int lightPin = A0; // Analog pin A0
const int buzzerPin = 3; // Buzzer pin
const int ledPin = 4; //LED Pin
const int baseline = 300;

#include <Bridge.h>
#include <Process.h>
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

    //attempting to use the PushOver API
    sendPushoverNotification("Dummy Test Message");
    delay(30000); // delay 30 seconds to avoid spamming messages and wasting limited calls
  } 
  else 
  {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }
  delay(3000);
}

void sendPushoverNotification(String message) 
{
  Process process;
  String userKey = "ugh795ezmczcigmwnsdbbnevt3m9gf";
  String apiToken = "abs2yokxv6hvmx9s1e9rr8dzkh2tcb";
  process.begin("curl");
  process.addParameter("-s");
  process.addParameter("--form-string");
  process.addParameter("token=" + apiToken);
  process.addParameter("--form-string");
  process.addParameter("user=" + userKey);
  process.addParameter("--form-string");
  process.addParameter("message=" + message);
  process.addParameter("https://api.pushover.net/1/messages.json");
  process.run();

  //print any response from PushOver, if any
  while (process.available()) {
    char c = process.read();
    Serial.print(c);
  }
  Serial.println("API Call Completed");
}
