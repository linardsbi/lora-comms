#ifndef _HOME_HVS_DOCUMENTS_PLATFORMIO_PROJECTS_ESP_LORA_COMMS_SRC_COMMSSERVER_HPP
#define _HOME_HVS_DOCUMENTS_PLATFORMIO_PROJECTS_ESP_LORA_COMMS_SRC_COMMSSERVER_HPP

#include "../src/controllers/HomeController.hpp"
#include "../src/controllers/MessageController.hpp"
#include "../src/controllers/TransmitController.hpp"
#include "../src/controllers/UserController.hpp"

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Hash.h>
#include <Logger.hpp>
#include <LoraRadio.hpp>
#include <memory>

using LoraRadioPtr = std::shared_ptr<E32_100>;

// needed for lora radio message reception callback
namespace radioCB {
LoraRadioPtr radioPtr{nullptr};
std::shared_ptr<AsyncWebSocket> wsPtr{nullptr};

ICACHE_RAM_ATTR void recv() {
  if (radioPtr && radioPtr->getCurrentMode() == 0 && radioPtr->available()) {
    std::vector<char> buf(radioPtr->available());
    radioPtr->readIntoBuffer(buf);
    // TODO: create a method that gets the module who sent the message
    wsPtr->printfAll("x:%s", buf.data());
    for (const auto item : buf) {
      Serial.print(item, HEX);
      Serial.print(' ');
    }
    Serial.println();
  }
}
} // namespace radioCB

class CommsServer {
public:
  CommsServer(const uint16_t webPort, const ModulePorts &ports)
      : server(new AsyncWebServer(webPort)),
        ws(new AsyncWebSocket("/messages")), radio(new E32_100(ports)) {}

  void begin() {

    radio->begin();

    radioCB::radioPtr = radio;
    radio->onReceive(radioCB::recv);

    ws->onEvent([radio = this->radio](
                    AsyncWebSocket *, AsyncWebSocketClient *client,
                    AwsEventType type, void *, uint8_t *data, size_t len) {
      MessageController::handleWSEvent(radio, client, type, data, len);
    });

    radioCB::wsPtr = ws;

    server->addHandler(ws.get());

    server->on("/", HTTP_GET, HomeController::index);

    server->on("/login", HTTP_POST, HomeController::login);

    server->on("/home", HTTP_GET,
               [radio = this->radio](AsyncWebServerRequest *request) {
                 HomeController::home(radio, request);
               });

    server->on("/new-partner", HTTP_GET, UserController::index);

    server->on("/new-partner/create", HTTP_POST, UserController::create);

    server->on("/broadcast", HTTP_GET, TransmitController::broadcast);

    server->on("/fixed", HTTP_GET,
               [radio = this->radio](AsyncWebServerRequest *request) {
                 TransmitController::show(radio, request);
               });

    server->begin();
  }

private:
  std::unique_ptr<AsyncWebServer> server{nullptr};
  std::shared_ptr<AsyncWebSocket> ws{nullptr};
  LoraRadioPtr radio{nullptr};
};
#endif
