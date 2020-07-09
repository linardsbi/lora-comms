#include "../include/CommsServer.hpp"
#include <Arduino.h>
#include <ESP8266WiFi.h>

CommsServer s(80);
void setup(){
  Serial.begin(115200);
 
  WiFi.softAP(MY_SSID, MY_PASS); // defined in plaformio.ini

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  Serial.println(WiFi.localIP());

  s.begin();
}
 
void loop(){  

}