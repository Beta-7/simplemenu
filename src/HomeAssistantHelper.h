#ifndef HOMEASSISTANTHELPER_H
#define HOMEASSISTANTHELPER_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <utility>

class HomeAssistantHelper {
  public:

  HomeAssistantHelper(const char* homeAssistantUrl, const char* accessToken);

    std::pair<String, String> callService(const char* service, const char* entityId);
    std::pair<String, String> readSensorValue(const char* sensorEntityId);

  private:
    const char* _homeAssistantUrl;
    const char* _accessToken;

    String sendHttpRequest(const char* url, const char* method, const String& payload = "");
};

#endif
