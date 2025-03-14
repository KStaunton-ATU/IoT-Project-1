Arduino Yún Alarm System

This project is an alarm system built using the Arduino Yún. It detects changes in light intensity and triggers a buzzer and LED while sending notifications via PushingBox and Google App Script.

Requirements:
Make sure your Aduino Yun is connected to wi-fi first
To use this software you will need the following:
Arduino IDE with the Bridge and HttpClient libraries
Google spreadsheet URL ID string with the Google App script deployed
A PushingBox URL service 

Hardware Requirements:
Arduino Yun
Grove Shield
One light sensor connected to the Analog 0 pin
One buzzer sensor connected to pin 3
One LED sensor connected to pin 4

Simple Guide:
Open Light_Sensor_Alarm.cpp in your Arduino IDE
Ensure your google spreadsheet is created and the ID is assigned to ‘var id’ in the Google App script before deploying it.
Ensure your PushingBlock service is setup and assign your device ID to ‘string deviceID’ on line 12 in your IDE.
