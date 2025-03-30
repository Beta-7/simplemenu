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
    int clampIndex(int index);

    // Navigation functions
    void navigateLeft();
    void navigateRight();
    void navigateUp();
    void navigateDown();

  private:
    Adafruit_ST7735& tft;      // TFT display reference
    MenuItem* menuItems;      // Array of MenuItem objects
    int menuItemCount;        // Total number of menu items
    int selectedIndex;        // Index of the selected menu item
    int displayWindowIndex;   // Index of the first menu item to display
    uint16_t textColor;
    uint16_t textBackground;
    uint16_t selectedTextColor;
    uint16_t selectedTextBackground;
    const int WINDOW_SIZE = 5; // Number of menu items to display at once
    unsigned long lastScrollTime;
    int scrollOffset;
    bool scrolling;
};

#endif
