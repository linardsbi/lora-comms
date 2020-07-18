#ifndef _HOME_HVS_DOCUMENTS_PLATFORMIO_PROJECTS_ESP_LORA_COMMS_SRC_CONTROLLERS_TRANSMITCONTROLLER_HPP
#define _HOME_HVS_DOCUMENTS_PLATFORMIO_PROJECTS_ESP_LORA_COMMS_SRC_CONTROLLERS_TRANSMITCONTROLLER_HPP
#include <ESPAsyncWebServer.h>
#include <LoraRadio.hpp>
#include <memory>

class TransmitController {
public:
  static auto processFixed(const String &arg) {
    if (arg == "ADDRESSEE") {
      return String("0 3 4");
    }
    return arg;
  }

  static void show(std::shared_ptr<E32_100> radio,
                   AsyncWebServerRequest *request) {

    request->send_P(
        200, "text/html", {"comms/fixed-transmit"}, [radio](const String &arg) {
          const auto p = radio->readParameters();
          return (p.ADDH == 0 && p.ADDL == 3 && p.CHAN == 4) ? String("0 1 4")
                                                             : String("0 3 4");
        });
  }

  static void broadcast(AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", {"comms/broadcast"});
  }
};
#endif