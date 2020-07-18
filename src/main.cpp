#include "../include/CommsServer.hpp"
#include <Arduino.h>
#include <ESP8266WiFi.h>

CommsServer s(80, {
    .RX = D2,
    .TX = D3,
    .AUX = D5,
    .M0 = D7,
    .M1 = D6,
});
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