#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "MenuItem.h"
#include "SimpleDisplay.h"

#define TFT_CS 5
#define TFT_RST 4
#define TFT_DC 2
#define TFT_MOSI 21 // Data out
#define TFT_SCLK 22 // Clock out

// Button pins
#define BUTTON_RIGHT 13
#define BUTTON_DOWN 12
#define BUTTON_LEFT 14
#define BUTTON_UP 27

MenuItem menuItems[] = {
    MenuItem("Welcome"),
    MenuItem("Display Test"),
    MenuItem("Readable"),
    MenuItem("Adafruit ST7735"),
    MenuItem("1234567890987654321"),
    MenuItem("Offscreen option"),
    MenuItem("Offscreen option #2")
};

Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
SimpleDisplay simpleDisplay(tft, menuItems, sizeof(menuItems) / sizeof(menuItems[0]));

void setup(void) {
  Serial.begin(115200);

  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_UP, INPUT_PULLUP);

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(2);

  Serial.println(F("Initialized"));

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

  simpleDisplay.renderText();
  delay(100);
}
