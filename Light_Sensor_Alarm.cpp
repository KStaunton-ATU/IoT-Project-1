//Written by Kevin Staunton
#include <Bridge.h>
#include <Process.h>
#include <HttpClient.h>
#include <YunServer.h>
#include <YunClient.h>

const int lightPin = A0; // Analog pin A0
const int buzzerPin = 3; // Buzzer pin
const int ledPin = 4; //LED Pin
const int baseline = 300;

String userKey = "ugh795ezmczcigmwnsdbbnevt3m9gf";
String apiToken = "abs2yokxv6hvmx9s1e9rr8dzkh2tcb";
String webhookURL = "https://hook0.com/webhooks/alarm_off";
String hook0APIKey = "90ad5237-eef3-4f55-9733-26b633e474e7"; 

YunServer server;

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
  server.listenOnPort(5555);  // Open a port to listen for http requests

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
    Serial.println("Alarm triggered");
    Serial.print("Light Intensity: ");
    Serial.println(lightValue);

    //prepare the message for the Pushover notification
    String message = getMessage(lightValue);    
    //attempting to use the PushOver API
    sendPushoverNotification(message);
    delay(30000); // delay 30 seconds to avoid spamming messages and wasting limited calls

    //wait 3 seconds and turn off the alarm
    delay(3000);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);

    //build URL string and then make a HTTP request to that PB service
    String requestURL = baseURL + deviceID + spreadsheetValue + lightValue;
    client.get(requestURL);
    client.get("api.pushingbox.com/pushingbox?devid=v42FDA5675FD7DA8");

    YunClient client = server.accept();//listening for HTTP request
    if (client) 
    {
        handleAlarmOffRequest(client);
        client.stop();
    }
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

String getTimestamp() {
    Process date;  
    // Run the date command on the Linux processor
    date.begin("date");
    date.addParameter("+%d-%m-%Y %H:%M:%S"); //day/month/year hour/minute/second format
    date.run();
  
    // Read the output of the date command
    String result = "";
    while (date.available() > 0) {
      char c = date.read();//read each character and append it to the string result
      result += c;
    }
  
    return result;
  }

String getMessage(int lightValue)
{
  // Webhook URL to disable the alarm
  String webhookURL = "https://hook0.com/webhooks/alarm_off";

  String message = "Alarm Triggered\n";
  message += "Time: " + getTimestamp() + "\n";
  message += "Light Value: " + (string)lightValue +"\n";
  message += "Click Here To Disable Alarm: " + webhookURL;

  return message;
}  

void handleAlarmOffRequest(YunClient client) 
{
    String requestHeader = client.readStringUntil('\r'); //gets request method (GET or POST) and the request endpoint (alarm_off)

    if (requestHeader.indexOf("POST /alarm_off") != -1) 
    {
        String authHeader = client.readStringUntil('\n');  // Read authorization header as Authorisation: Bearer [API Key]
        if (authHeader.indexOf(hook0APIKey) != -1) // Checks if it contains API key 
        {  
            Serial.println("Valid webhook received! Disabling alarm");
            digitalWrite(buzzerPin, LOW);
            digitalWrite(ledPin, LOW);
            client.println("Alarm turned off successfully!");
        } 
        else 
        {
            client.println("Invalid request!");
        }
    }
}
