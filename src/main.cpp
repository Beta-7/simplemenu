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


void doSomething(Adafruit_ST7735& tft) {
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 40);
  tft.println("Temperature:");
  tft.setCursor(10, 60);
  tft.println("0C");

  Serial.println("You have selected deeper!");
}

void showSuccessScreen(Adafruit_ST7735& tft) {
  tft.fillScreen(ST7735_BLACK);

  tft.setTextWrap(false);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(4);
  tft.setCursor(30, 30);
  tft.println("OK"); 

  tft.setTextSize(2);
  tft.setCursor(30, 80);
  tft.println("Success");
}


MenuItem mainMenu("Smart Home", {
  MenuItem("Kitchen", {
    MenuItem("Sensors", {
      MenuItem("Temperature", doSomething)
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
