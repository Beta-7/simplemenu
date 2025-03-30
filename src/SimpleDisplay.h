// SimpleDisplay.h
#ifndef SIMPLEDISPLAY_H
#define SIMPLEDISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "MenuItem.h"

class SimpleDisplay {
  public:
    SimpleDisplay(Adafruit_ST7735& tft, MenuItem menuItems[], int menuItemCount);
    
    void renderText();
    void renderTextWithScroll(int i);
    void handleButtonPresses();
    void renderTick();
    int clampIndex(int index);

    // Navigation functions
    void navigateLeft();
    void navigateRight();
    void navigateUp();
    void navigateDown();

  private:
    Adafruit_ST7735& tft;
    MenuItem* menuItems;
    int menuItemCount;
    int selectedIndex;
    int displayWindowIndex;
    uint16_t textColor;
    uint16_t textBackground;
    uint16_t selectedTextColor;
    uint16_t selectedTextBackground;
    const int WINDOW_SIZE = 5;
    const int ITEM_OFFSET = 10;
    unsigned long lastScrollTime;
    int scrollOffset;
    bool scrolling;
};

#endif
