#ifndef _HOME_HVS_DOCUMENTS_PLATFORMIO_PROJECTS_ESP_LORA_COMMS_SRC_COMMSSERVER_HPP
#define _HOME_HVS_DOCUMENTS_PLATFORMIO_PROJECTS_ESP_LORA_COMMS_SRC_COMMSSERVER_HPP
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Hash.h>
#include <Logger.hpp>

class CommsServer {
public:
  CommsServer(const uint16_t port) : server(new AsyncWebServer(port)) {}
  void begin() {
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/html", {"index"},
                      [request](const String &vars) {
                        Serial.println(vars);
                        return String("whatevs");
                      });
    });

    server->on("/login", HTTP_POST, [](AsyncWebServerRequest *request) {
      // TODO: create response

      Logger::print(Serial, F("User entered password: "),
                    request->arg("password"), '\n');

      if (!request->authenticate("me", "secret"))
        return request->requestAuthentication();
      else
        request->redirect("/home");
    });

    server->on("/home", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/html", {"comms/home"},
                      [request](const String &vars) {
                        if (vars == "CONNECTIONS") {
                          return String("first:00 03 04,second:00 01 04");
                        }
                        return String("whatevs");
                      });
              
    });

    server->on("/new-partner", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/html", {"comms/create-partner"});
    });

    server->on("/new-partner/create", HTTP_POST,
               [](AsyncWebServerRequest *request) {
                 // TODO: create response
                 request->redirect("/home");
               });

    server->on("/broadcast", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/html", {"comms/broadcast"});
    });

    server->on("/fixed", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/html", {"comms/fixed-transmit"}, [request](const String &vars) {
                        if (vars == "MSG_TO") {
                          return String("first");
                        }
                        return String("whatevs");
                      });
    });

    server->begin();
  }
  ~CommsServer() {
    if (server)
      delete server;
  }

private:
  AsyncWebServer *server{nullptr};
};
#endif
