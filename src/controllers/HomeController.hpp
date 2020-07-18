#ifndef _HOME_HVS_DOCUMENTS_PLATFORMIO_PROJECTS_ESP_LORA_COMMS_SRC_CONTROLLERS_HOMECONTROLLER_HPP
#define _HOME_HVS_DOCUMENTS_PLATFORMIO_PROJECTS_ESP_LORA_COMMS_SRC_CONTROLLERS_HOMECONTROLLER_HPP
#include <ESPAsyncWebServer.h>
#include <Logger.hpp>
#include <LoraRadio.hpp>
#include <memory>

class HomeController {
public:
  static auto processIndex(const String &arg) { return String("aa"); }
  static void index(AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", {"index"}, processIndex);
  }

  static void home(std::shared_ptr<E32_100> radio,
                   AsyncWebServerRequest *request) {

    request->send_P(
        200, "text/html", {"comms/home"}, [radio](const String &arg) {
          const auto p = radio->readParameters();
          return (p.ADDH == 0 && p.ADDL == 3 && p.CHAN == 4) ? String("first:0 1 4")
                                                             : String("first:0 3 4");
        });
  }

  static void login(AsyncWebServerRequest *request) {
    // TODO: create response

    Logger::print(Serial, F("User entered password: "),
                  request->arg("password"), '\n');

    if (!request->authenticate("me", "secret"))
      return request->requestAuthentication();
    else
      request->redirect("/home");
  }
};
#endif