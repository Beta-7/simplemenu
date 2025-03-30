// MenuItem.h
#ifndef MENUITEM_H
#define MENUITEM_H

#include <Arduino.h>

class MenuItem {
  public:
    String label; // The label of the menu item

    // Constructor to initialize label
    MenuItem(String label);
};

#endif