#ifndef HOMEASSISTANTHELPER_H
#define HOMEASSISTANTHELPER_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class HomeAssistantHelper {
  public:

  HomeAssistantHelper(const char* homeAssistantUrl, const char* accessToken);

    // Functions to interact with Home Assistant
    void callService(const char* service, const char* entityId);
    String readSensorValue(const char* sensorEntityId);

  private:
    const char* _homeAssistantUrl;
    const char* _accessToken;

    // Helper function to send HTTP requests
    String sendHttpRequest(const char* url, const char* method, const String& payload = "");
};

#endif
