#include "HomeAssistantHelper.h"
#include <WiFi.h>

HomeAssistantHelper::HomeAssistantHelper(const char* homeAssistantUrl, const char* accessToken) {
  _homeAssistantUrl = homeAssistantUrl;
  _accessToken = accessToken;
}

void HomeAssistantHelper::callService(const char* service, const char* entityId) {
  String url = String(_homeAssistantUrl) + "/api/services/" + service;
  String payload = "{\"entity_id\": \"" + String(entityId) + "\"}";
  
  sendHttpRequest(url.c_str(), "POST", payload);
}

String HomeAssistantHelper::readSensorValue(const char* sensorEntityId) {
  String url = String(_homeAssistantUrl) + "/api/states/" + sensorEntityId;

  String response = sendHttpRequest(url.c_str(), "GET");

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, response);

  if (error) {
    return "Error parsing sensor value";
  } else {
    return doc["state"].as<String>();
  }
}

String HomeAssistantHelper::sendHttpRequest(const char* url, const char* method, const String& payload) {
  HTTPClient http;
  String response = "";

  if (WiFi.status() == WL_CONNECTED) {
    http.begin(url);
    http.addHeader("Authorization", "Bearer " + String(_accessToken));
    http.addHeader("Content-Type", "application/json");

    int httpCode = 0;
    if (String(method) == "POST") {
      httpCode = http.POST(payload);
    } else if (String(method) == "GET") {
      httpCode = http.GET();
    }

    if (httpCode > 0) {
      response = http.getString();
    } else {
      response = "Error with HTTP request";
    }

    http.end();
  } else {
    response = "WiFi not connected";
  }

  return response;
}
