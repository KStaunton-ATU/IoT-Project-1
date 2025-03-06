//Written by Kevin Staunton
const int lightPin = A0; // Analog pin A0
const int buzzerPin = 3; // Buzzer pin
const int ledPin = 4; //LED Pin
const int baseline = 500;

// Add your PushingBox Scenario DeviceID here:
String deviceID = "INSERT DEVICE ID HERE";
//we want to eventually make a HTTP request to Pushing Box
String baseURL = "api.pushingbox.com/pushingbox?devid=";
//and the columns we want to update. Lets assume its 1 column for now. The lightsensor value when the alarm triggers.
String spreadsheetValues ="&LightValue=";
#include <Bridge.h>
#include <HttpClient.h>
// #include <Process.h> //to get the timestamp when the alarm triggers
// String timestamp;

void setup() { 
  //initialise
  pinMode(lightPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  //Bridge needs time to get started. So wait 3 seconds
  Bridge.begin();//enables microcontrolller network functions 
  delay(3000);
}

void loop() {
    // Initialize the client library
    HttpClient client; //this object will let us make HTTP requests to the pushingbox service

    int lightValue = analogRead(lightPin);
  if (lightValue < baseline) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
    Serial.print("Light Intensity: "+lightValue);

    // timestamp = getTimestamp();
    // Serial.println(timestamp);

    //wait 3 seconds and turn off the alarm
    delay(3000);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);

    //recreate the steps from the latest lab to set up a google spreadsheet and pushingbox service
    //build URL string and then make a HTTP request to that PB service
    String requestURL = baseURL + deviceID + spreadsheetValues + lightValue;
    client.get(requestURL);
    client.get(api.pushingbox.com/pushingbox?devid=v42FDA5675FD7DA8)
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }
  delay(1000);
}

//The Google App Script already inserts a timestamp, so all we need is the light sensor value
//uncomment lines 15,16,39,40 to make this function work
// String getTimestamp() {
//     Process date;
  
//     // Run the date command on the Linux processor
//     date.begin("date");
//     date.addParameter("+%d-%m-%Y %H:%M:%S"); //day/month/year hour/minute/second format
//     date.run();
  
//     // Read the output of the date command
//     String result = "";
//     while (date.available() > 0) {
//       char c = date.read();//read each character and append it to the string result
//       result += c;
//     }
  
//     return result;
//   }
