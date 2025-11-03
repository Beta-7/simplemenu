#include "HomeAssistantHelper.h"
#include <WiFi.h>
#include <utility>

HomeAssistantHelper::HomeAssistantHelper(const char *homeAssistantUrl, const char *accessToken)
{
    _homeAssistantUrl = homeAssistantUrl;
    _accessToken = accessToken;
}

std::pair<String, String> HomeAssistantHelper::callService(const char *service, const char *entityId)
{
    String url = String(_homeAssistantUrl) + "/api/services/" + service;
    String payload = "{\"entity_id\": \"" + String(entityId) + "\"}";

    String response = sendHttpRequest(url.c_str(), "POST", payload);

    std::pair<String, String> result = {"", ""};

    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, response);

    if (error)
    {
        Serial.print("Error parsing JSON: ");
        Serial.println(error.c_str());
        return result;
    }

    JsonArray arr = doc.as<JsonArray>();
    for (JsonObject obj : arr)
    {
        if (obj["entity_id"] == entityId)
        {
            result.first = String(obj["attributes"]["friendly_name"].as<String>());
            result.second = String(obj["state"].as<String>());
            break;
        }
    }

    return result;
}

std::pair<String, String> HomeAssistantHelper::readSensorValue(const char *sensorEntityId)
{
    String url = String(_homeAssistantUrl) + "/api/states/" + sensorEntityId;
    std::pair<String, String> result = {"", ""};

    String response = sendHttpRequest(url.c_str(), "GET");
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, response);

    if (error)
    {
        Serial.print("Error parsing JSON: ");
        Serial.println(error.c_str());
        return result;
    }

    JsonObject obj = doc.as<JsonObject>();
    if (obj["entity_id"] == sensorEntityId)
    {
        result.first = String(obj["attributes"]["friendly_name"].as<String>());
        result.second = String(obj["state"].as<String>()) + " " + String(obj["attributes"]["unit_of_measurement"].as<String>());
    }

    return result;
}

String HomeAssistantHelper::sendHttpRequest(const char *url, const char *method, const String &payload)
{
    HTTPClient http;
    String response = "";

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("WiFi connected. Sending HTTP request...");

        http.begin(url);
        http.addHeader("Authorization", "Bearer " + String(_accessToken));
        http.addHeader("Content-Type", "application/json");

        int httpCode = 0;
        if (String(method) == "POST")
        {
            httpCode = http.POST(payload);
        }
        else if (String(method) == "GET")
        {
            httpCode = http.GET();
        }

        if (httpCode > 0)
        {
            response = http.getString();
            Serial.println("Response: ");
            Serial.println(response);
        }
        else
        {
            response = "Error with HTTP request";
            Serial.print("Error: ");
            Serial.println(response);
        }

        http.end();
    }
    else
    {
        response = "WiFi not connected";
        Serial.println(response);
    }

    return response;
}