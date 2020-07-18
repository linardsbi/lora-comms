#ifndef _HOME_HVS_DOCUMENTS_PLATFORMIO_PROJECTS_ESP_LORA_COMMS_SRC_CONTROLLERS_MESSAGECONTROLLER_HPP
#define _HOME_HVS_DOCUMENTS_PLATFORMIO_PROJECTS_ESP_LORA_COMMS_SRC_CONTROLLERS_MESSAGECONTROLLER_HPP

#include <ESPAsyncWebServer.h>
#include <Logger.hpp>
#include <LoraRadio.hpp>
#include <memory>
#include <vector>

enum class ByteType: uint8_t {
    STARTTR = 1,
    ENDTR = 4,
    FIXEDTR = 17,
    CLEARTR = 18,
};

class MessageController {
public:
  static void handleWSEvent(std::shared_ptr<E32_100> radio,
                          AsyncWebSocketClient *client, const AwsEventType type,
                          const uint8_t *data, const size_t size) {
    switch (type)
    {
    case WS_EVT_CONNECT:
        break;
    case WS_EVT_DISCONNECT:
        break;
    case WS_EVT_PONG:
        break;
    case WS_EVT_ERROR:
        break;
    case WS_EVT_DATA:
    // TODO implement uint8_t support
        Logger::print(Serial, "data: ", reinterpret_cast<const char *>(data), '\n');

        if (size > 0 && static_cast<ByteType>(data[0]) == ByteType::STARTTR && static_cast<ByteType>(data[size-1]) == ByteType::ENDTR) {
            switch (static_cast<ByteType>(data[1])) {
                case ByteType::FIXEDTR:
                    //const auto params = radio->getParameters();
                    radio->send(reinterpret_cast<const char *>(data[2]));
                break;
                case ByteType::CLEARTR:
                    radio->send(reinterpret_cast<const char *>(data[2]));
                break;
                default:
                    client->printf_P(PSTR("Invalid transmission type: %d"), data[1]);
            }
        }
        
        break;
    default:
        break;
    }
    
    //client->printf("000304:hello");
  }
};
#endif
