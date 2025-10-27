#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "MenuItem.h"
#include "SimpleDisplay.h"
#include "HomeAssistantHelper.h"
#include <WiFi.h>

#define TFT_CS 5
#define TFT_RST 4
#define TFT_DC 2
#define TFT_MOSI 21 // Data out
#define TFT_SCLK 22 // Clock out

#define DISPLAY_WIDTH = 128;

// Button pins
#define BUTTON_RIGHT 13
#define BUTTON_DOWN 12
#define BUTTON_LEFT 14
#define BUTTON_UP 27


const char* homeAssistantUrl = "http://<your_home_assistant_ip>:<port>/api";
const char* accessToken = "<your_long_lived_access_token>";
const char* ssid = "DAVID";
const char* password = "daviddavid2606";

HomeAssistantHelper homeAssistant(homeAssistantUrl, accessToken);

void showValue(Adafruit_ST7735& tft) {
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  int16_t x, y;
  uint16_t labelWidth, labelHeight;
  tft.getTextBounds("Temperature:", 0, 0, &x, &y, &labelWidth, &labelHeight);
  int labelX = (128 - labelWidth) / 2;  
  tft.setCursor(labelX, 40);
  tft.println("Temperature:");
  tft.setTextSize(2);
  String temp = "25.5C";
  tft.getTextBounds(temp, 0, 0, &x, &y, &labelWidth, &labelHeight);
  int tempX = (128 - labelWidth) / 2;  
  tft.setCursor(tempX, 80);
  tft.println(temp);
}

void showSuccessScreen(Adafruit_ST7735& tft) {
  tft.fillScreen(ST7735_BLACK);
  tft.setTextWrap(false);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(4);
  int16_t x, y;
  uint16_t okWidth, okHeight;
  tft.getTextBounds("OK", 0, 0, &x, &y, &okWidth, &okHeight);

  int okX = (128 - okWidth) / 2;  
  int okY = (160 - okHeight) / 3;
  tft.setCursor(okX, okY);
  tft.println("OK");
  tft.setTextSize(2);
  String successText = "Success";

  tft.getTextBounds(successText, 0, 0, &x, &y, &okWidth, &okHeight);

  int successX = (128 - okWidth) / 2;
  int successY = okY + okHeight + 10;
  tft.setCursor(successX, successY);
  tft.println(successText);
}


MenuItem mainMenu("Smart Home", {
  MenuItem("Kitchen", {
    MenuItem("Sensors", {
      MenuItem("Temperature", showValue)
    }),
    MenuItem("Lights", {
      MenuItem("Turn all off", showSuccessScreen)
    }),
    MenuItem("History"),
    MenuItem("Submenu #4"),
  }),
  MenuItem("Living room", {
    MenuItem("TV"),
    MenuItem("Lights"),
    MenuItem("Scenes"),
    MenuItem("Sensors"),

  }),
  MenuItem("Bedroom"),
  MenuItem("Balcony"),
  MenuItem("Bathroom"),
  MenuItem("Neso drugo"),
  MenuItem("Neso treto")
});

Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
SimpleDisplay simpleDisplay(tft, mainMenu);

void setup(void) {
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
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
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

void loop() {
  if(digitalRead(BUTTON_UP) == LOW){
    simpleDisplay.navigateUp();
  }
  if(digitalRead(BUTTON_DOWN) == LOW){
    simpleDisplay.navigateDown();
  }
  if(digitalRead(BUTTON_LEFT) == LOW){
    simpleDisplay.navigateLeft();
  }
  if(digitalRead(BUTTON_RIGHT) == LOW){
    simpleDisplay.navigateRight();
  }

  simpleDisplay.renderTick();
  delay(100);
}
