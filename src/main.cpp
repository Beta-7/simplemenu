#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "MenuItem.h"
#include "SimpleDisplay.h"
#include "HomeAssistantHelper.h"
#include <WiFi.h>
#include <Config.h>

HomeAssistantHelper homeAssistant(HOME_ASSISTANT_URL, ACCESS_TOKEN);

void showValue(Adafruit_ST7735 &tft, char *entityId)
{
  std::pair<String, String> result = homeAssistant.readSensorValue(entityId);
  Serial.println("showValue");

  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  int16_t x, y;
  uint16_t labelWidth, labelHeight;
  tft.getTextBounds(result.first, 0, 0, &x, &y, &labelWidth, &labelHeight);
  int labelX = (128 - labelWidth) / 2;
  tft.setCursor(labelX, 40);
  tft.println(result.first);
  tft.setTextSize(2);
  tft.getTextBounds(result.second, 0, 0, &x, &y, &labelWidth, &labelHeight);
  int valueX = (128 - labelWidth) / 2;
  tft.setCursor(valueX, 80);
  tft.println(result.second);
}

void callServiceScreen(Adafruit_ST7735 &tft, char *service, char *entity)
{
  std::pair<String, String> result = homeAssistant.callService(service, entity);
  Serial.println("callServiceScreen");
  tft.fillScreen(ST7735_BLACK);
  tft.setTextWrap(true);
  tft.setTextColor(ST7735_GREEN);

  int16_t x, y;
  uint16_t textWidth, textHeight;

  if (result.first == "" && result.second == "")
  {
    String noChangeMessage = "No change was made.";
    tft.setTextSize(2);
    tft.getTextBounds(noChangeMessage, 0, 0, &x, &y, &textWidth, &textHeight);

    int noChangeX = (128 - textWidth) / 2 + 5;
    int noChangeY = (160 - textHeight) / 2;
    tft.setCursor(noChangeX, noChangeY);
    tft.println(noChangeMessage);
    return;
  }

  int textSize = 3;
  String friendlyName = result.first;

  tft.setTextSize(textSize);
  tft.getTextBounds(friendlyName, 0, 0, &x, &y, &textWidth, &textHeight);

  while (textWidth > 128 && textSize > 1)
  {
    textSize--;
    tft.setTextSize(textSize);
    tft.getTextBounds(friendlyName, 0, 0, &x, &y, &textWidth, &textHeight);
  }

  int friendlyNameX = (128 - textWidth) / 2;
  int friendlyNameY = (160 - textHeight) / 3;
  tft.setCursor(friendlyNameX, friendlyNameY);
  tft.println(friendlyName);

  String stateMessage = "State: " + result.second;
  tft.setTextSize(2);
  tft.getTextBounds(stateMessage, 0, 0, &x, &y, &textWidth, &textHeight);

  while (textWidth > 128 && textSize > 1)
  {
    textSize--;
    tft.setTextSize(textSize);
    tft.getTextBounds(stateMessage, 0, 0, &x, &y, &textWidth, &textHeight);
  }

  int stateMessageX = (128 - textWidth) / 2;
  int stateMessageY = friendlyNameY + textHeight + 50;
  tft.setCursor(stateMessageX, stateMessageY);
  tft.println(stateMessage);
}

MenuItem mainMenu = MenuItem("Smart Home", {MenuItem("Kitchen", {MenuItem("Sensors", {MenuItem("Humidity", [](Adafruit_ST7735 &tft) { showValue(tft, "sensor.kitchen_humidity"); }),
MenuItem("Temperature", [](Adafruit_ST7735 &tft) { showValue(tft, "sensor.kitchen_temperature"); }),}),
MenuItem("Lights", {MenuItem("Turn all off", [](Adafruit_ST7735 &tft) { callServiceScreen(tft, "light/turn_off", "light.kitchen_light"); }),}),
MenuItem("History"),
MenuItem("Submenu #4"),}),
MenuItem("Living room", {MenuItem("TV"),
MenuItem("Lights"),
MenuItem("Scenes"),
MenuItem("Sensors"),}),
MenuItem("Bedroom"),
MenuItem("Balcony"),
MenuItem("Bathroom"),
MenuItem("Neso drugo"),
MenuItem("Neso treto"),
});

Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
SimpleDisplay simpleDisplay(tft, mainMenu);

void setup(void)
{
  Serial.begin(115200);

  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_UP, INPUT_PULLUP);

  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(2);

  Serial.println(F("Initialized"));

  Serial.print("Connecting to WiFi...");

  tft.setCursor(10, 10);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.println("Connecting to WiFi...");

#ifdef ENABLE_WIFI
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");

    tft.setCursor(10, 30);
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(1);
    tft.print("Connecting...");
  }

  Serial.println();
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
#else
  tft.setCursor(10, 30);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.print("Skiping wifi setup");
#endif

  delay(1000);
  tft.fillRect(10, 30, 120, 20, ST7735_BLACK);
  tft.setCursor(10, 30);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(1);
  tft.println("Connected!");
  delay(1000);

  tft.fillScreen(ST7735_BLACK);
  simpleDisplay.renderText();
  delay(1000);
}

void loop()
{
  if (digitalRead(BUTTON_UP) == LOW)
  {
    simpleDisplay.navigateUp();
  }
  if (digitalRead(BUTTON_DOWN) == LOW)
  {
    simpleDisplay.navigateDown();
  }
  if (digitalRead(BUTTON_LEFT) == LOW)
  {
    simpleDisplay.navigateLeft();
  }
  if (digitalRead(BUTTON_RIGHT) == LOW)
  {
    simpleDisplay.navigateRight();
  }

  simpleDisplay.renderTick();
  delay(100);
}
