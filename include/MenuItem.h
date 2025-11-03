#pragma once
#include <Arduino.h>
#include <vector>
#include <functional>      
#include <Adafruit_ST7735.h>
class MenuItem {
  public:
    String label;
    std::vector<MenuItem> subMenuItems;
    std::function<void(Adafruit_ST7735& tft)> onSelect;
    bool dirty = true;
    int menuItemCount;

    MenuItem(String label);

    MenuItem(String label, const std::vector<MenuItem>& subMenuItems);
    MenuItem(String label, std::function<void(Adafruit_ST7735& tft)> onSelect);

    void addSubMenu(const std::vector<MenuItem>& items);
};
