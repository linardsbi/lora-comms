#ifndef _HOME_HVS_DOCUMENTS_PLATFORMIO_PROJECTS_ESP_LORA_COMMS_SRC_CONTROLLERS_USERCONTROLLER_HPP
#define _HOME_HVS_DOCUMENTS_PLATFORMIO_PROJECTS_ESP_LORA_COMMS_SRC_CONTROLLERS_USERCONTROLLER_HPP
#include <ESPAsyncWebServer.h>

class UserController {
public:
    static void index(AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", {"comms/create-partner"});
    }
    static void create(AsyncWebServerRequest *request) {
        // TODO: create response
        request->redirect("/home");
    }
};
#endif
