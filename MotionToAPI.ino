/*
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>

int LED = 13;
int otherled = 12;
int sensor = 14;
int val = 0;
int ledState = LOW;
int counter = 0;

void setup() {
  Serial.begin(19200); //The baudrate set at Tools -> Serial Monitor to print lines (handy for debugging)
  WiFiManager wifimanager; //A wifimanager prevents a hardcoded wifi password in code
  wifimanager.autoConnect("Arduino"); //connect to the wifi with the name "Arduino" and go to the following ip addres in the browser: 192.168.4.1

  pinMode(sensor,INPUT);
  pinMode(LED,OUTPUT); 
  pinMode(otherled,OUTPUT);
  
}

// the loop function runs over and over again forever
void loop() {
  while (WiFi.status() == WL_CONNECTED) {  
    if(counter == 60)sendAliveCall();
    sendWakeyCall();
    addCounter();    
  }
}

void addCounter(){
  delay(1000);
  counter++;  
  Serial.println(counter);
}

void sendWakeyCall(){
  val = digitalRead(sensor);
  if(val == HIGH){
    digitalWrite(LED,HIGH);    
    doApiCall("6042jp15Rmd", "WakeyWakey");
  }
  else {
  digitalWrite(LED,LOW);    
    }  
}

void sendAliveCall()
{
  resetCounter();
  ledState = !ledState;        
  digitalWrite(otherled,ledState);
  doApiCall("6042jp15Rmd", "Alive");
}

void resetCounter(){
  counter = 0;
}

void doApiCall(String IoT, String endpoint) {
  HTTPClient http;

  BearSSL::WiFiClientSecure client;
  client.setInsecure();               // Don't check fingerprint
  String url = "https://rondjewelzijnapi.azurewebsites.net/api/" + endpoint + "/" + IoT + "";
  http.begin(client, url);

  int httpCode = http.GET();
  String payload = http.getString();

  if (httpCode == HTTP_CODE_OK) {
    Serial.println("Calling: " + endpoint);
  } else {
    Serial.printf("HTTP Error: %s\n", http.errorToString(httpCode).c_str());
    return;
  }

  http.end();
}
