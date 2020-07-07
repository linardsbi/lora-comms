#ifndef _HOME_HVS_DOCUMENTS_PLATFORMIO_PROJECTS_ESP_LORA_COMMS_SRC_COMMSSERVER_HPP
#define _HOME_HVS_DOCUMENTS_PLATFORMIO_PROJECTS_ESP_LORA_COMMS_SRC_COMMSSERVER_HPP
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Hash.h>

class CommsServer {
public:
  CommsServer(const uint16_t port) : server(new AsyncWebServer(port)) {

    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/html", PSTR(R"raw(<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="./css/style.css" type="text/css">
  <script type="text/javascript" src="./js/main.js"></script>
</head>
<body>
  <h2>ESP8266 DHT Server</h2>
  <p>
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">%</sup>
  </p>
</body>
</html>)raw"),
                      [request](const String &vars) {
                        Serial.println(vars);
                        return String("whatevs");
                      });
    });

    server->on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/plain", PSTR(R"raw(<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h2>Temperature</h2>
</body>
</html>)raw"));
    });
    server->on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/plain", PSTR(R"raw(<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h2>Humidity</h2>
</body>
</html>)raw"));
    });

    server->begin();
  }

  ~CommsServer() {
      if (server) delete server;
  }

private:
  AsyncWebServer *server{nullptr};
};
#endif
