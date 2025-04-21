// SimpleDisplay.h
#ifndef SIMPLEDISPLAY_H
#define SIMPLEDISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "MenuItem.h"

class SimpleDisplay {
  public:
  SimpleDisplay(Adafruit_ST7735& tft, MenuItem rootItem);
    
    void renderText();
    void renderTextWithScroll(int i);
    void handleButtonPresses();
    void renderTick();

    // Navigation functions
    void navigateLeft();
    void navigateRight();
    void navigateUp();
    void navigateDown();

  private:
    Adafruit_ST7735& tft;
    MenuItem menu;
    MenuItem *shownMenu;
    std::vector<MenuItem*> menuPath;
    uint16_t textColor;
    uint16_t textBackground;
    uint16_t selectedTextColor;
    uint16_t selectedTextBackground;
    const int WINDOW_SIZE = 5;
    const int ITEM_OFFSET = 10;
    const int DISPLAY_WIDTH = 118;  
    unsigned long lastScrollTime;
    int selectedIndex;
    int displayWindowIndex;
    int scrollOffset;
    bool scrolling;

    int handleWindow(int index);
    int clampIndex(int index, int change);
    void renderTextRow(int y, String text, uint16_t textColor, uint16_t textBackground);
    void markAllDirty();
    void markItemDirty(int index);
};

#endif
